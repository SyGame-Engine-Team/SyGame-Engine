/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITree.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Tree view
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUITree.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUITreeNode implementation
GUITreeNode::GUITreeNode()
{
    m_bIsLastChild = true;
    m_iDepth = 0;

    m_bPopulated = true;
    m_iNodeCount = 0;

    m_bExpanded = true;
    m_bSelected = false;

    m_iParentIndex = INVALID_OFFSET;
    m_pParent = NULL;

    m_arrChildren.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_arrChildren.Create();

    m_bIsLeaf = false;
    m_idIcon = GUISKIN_IMAGE_ICON_UNDEFINED;
    m_iNameLength = 0;
    m_strName[0] = NULLBYTE;
}
GUITreeNode::GUITreeNode( GUITreeNode * pParent, Bool bIsLeaf, const GChar * strName, GUISkinImage idIcon )
{
    Assert( pParent != NULL );

    m_bIsLastChild = true;
    if ( pParent->m_arrChildren.Count() > 0 )
        pParent->m_arrChildren[pParent->m_arrChildren.Count() - 1]->m_bIsLastChild = false;
    m_iDepth = ( pParent->m_iDepth + 1 );

    m_bPopulated = false;
    m_iNodeCount = 1;

    m_bExpanded = false;
    m_bSelected = false;

    m_iParentIndex = pParent->m_arrChildren.Count();
    m_pParent = pParent;
    pParent->m_arrChildren.Push( this );

    m_arrChildren.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_arrChildren.Create();

    m_bIsLeaf = bIsLeaf;
    m_idIcon = idIcon;
    GChar * pEnd = StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH - 1 );
    m_iNameLength = ( pEnd - m_strName );

    // Update NodeCounts
    while( pParent != NULL && (pParent->m_bExpanded) ) {
        ++(pParent->m_iNodeCount);
        pParent = pParent->m_pParent;
    }
}
GUITreeNode::~GUITreeNode()
{
    // Recurse
    for( UInt i = 0; i < m_arrChildren.Count(); ++i )
        Delete( m_arrChildren[i] );

    m_arrChildren.Destroy();
}

GUITreeNode * GUITreeNode::GetNode( UInt iNode ) const
{
    Assert( iNode < m_iNodeCount );

    // Lookup node
    UInt iCurNode = 0;
    if ( !IsRoot() ) {
        // Hit case
        if ( iNode == 0 )
            return (GUITreeNode*)this;
        // Skip case
        ++iCurNode;
    }

    // Lookup children
    UInt iNextNode;
    for( UInt i = 0; i < m_arrChildren.Count(); ++i ) {
        iNextNode = ( iCurNode + m_arrChildren[i]->m_iNodeCount );

        // Hit case
        if ( iNextNode == iNode ) {
            if ( (i+1) < m_arrChildren.Count() )
                return m_arrChildren[i+1];
            return NULL;
        }

        // Recurse case
        if ( iNextNode > iNode )
            return m_arrChildren[i]->GetNode( iNode - iCurNode );

        iCurNode = iNextNode;
    }

    // Should never happen
    Assert( false );
    return NULL;
}
GUITreeNode * GUITreeNode::GetNextNode() const
{
    // Downward case
    if ( !m_bIsLeaf && m_bExpanded && !(m_arrChildren.IsEmpty()) )
        return m_arrChildren[0];

    // Upward case
    UInt iNextIndex = m_iParentIndex + 1;
    GUITreeNode * pParent = m_pParent;
    while( pParent != NULL ) {
        if ( iNextIndex < pParent->m_arrChildren.Count() )
            return pParent->m_arrChildren[iNextIndex];
        iNextIndex = pParent->m_iParentIndex + 1;
        pParent = pParent->m_pParent;
    }
    return NULL;
}

GUITreeNode * GUITreeNode::GetNode( const GChar * strPath ) const
{
    // Skip leading slash
    if ( *strPath == '/' )
        ++strPath;

    // Trailing name case
    if ( *strPath == NULLBYTE )
        return (GUITreeNode*)this;

    // Node name
    GChar strNodeName[GUI_NAME_LENGTH];
    const GChar * strStart = strPath;
    while( *strPath != '/' && *strPath != NULLBYTE )
        ++strPath;
    StringFn->NCopy( strNodeName, strStart, (strPath - strStart) );

    // Recurse
    UInt iChildCount = m_arrChildren.Count();
    for( UInt i = 0; i < iChildCount; ++i ) {
        if ( StringFn->Cmp(m_arrChildren[i]->m_strName, strNodeName) == 0 )
            return m_arrChildren[i]->GetNode( strPath );
    }

    // Not found
    return NULL;
}
Void GUITreeNode::GetPath( GChar * outPath, UInt * outLength ) const
{
    if ( m_pParent == NULL ) {
        *outPath++ = '/';
        *outPath++ = NULLBYTE;
        *outLength = 1;
        return;
    }

    // Recurse
    m_pParent->GetPath( outPath, outLength );

    // Upward
    GChar * pCh = ( outPath + (*outLength) );
    if ( *(pCh - 1) != '/' )
        *pCh++ = '/';
    *outLength = ( StringFn->NCopy( pCh, m_strName, GUI_PATH_LENGTH - (*outLength) )
                   - outPath );
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITreeNode::_DestroyChild( UInt iChild )
{
    Assert( iChild < m_arrChildren.Count() );

    // Save node count
    UInt iChildNodeCount = m_arrChildren[iChild]->GetNodeCount();

    // Update LastChild flag
    if ( m_arrChildren[iChild]->m_bIsLastChild ) {
        if ( iChild > 0 )
            m_arrChildren[iChild - 1]->m_bIsLastChild = true;
    }

    // Update parent indices
    for( UInt i = iChild + 1; i < m_arrChildren.Count(); ++i )
        --(m_arrChildren[i]->m_iParentIndex);

    // Destroy subtree, remove from children
    GUIFn->SelectMemory( TEXT("LayoutData") );

    Delete( m_arrChildren[iChild] );
    m_arrChildren.Remove( iChild, NULL, 1 );

    GUIFn->UnSelectMemory();

    // Update NodeCounts
    GUITreeNode * pParent = m_pParent;
    while( pParent != NULL && (pParent->m_bExpanded) ) {
        pParent->m_iNodeCount -= iChildNodeCount;
        pParent = pParent->m_pParent;
    }
    if ( m_bExpanded )
        m_iNodeCount -= iChildNodeCount;
}

Void GUITreeNode::_Expand()
{
    if ( m_bIsLeaf || m_bExpanded )
        return;

    // Node must be populated by calling model
    Assert( m_bPopulated );

    // Expand node
    m_bExpanded = true;

    // Update NodeCounts
    UInt iExpandCount = 0;
    for( UInt i = 0; i < m_arrChildren.Count(); ++i )
        iExpandCount += m_arrChildren[i]->m_iNodeCount;

    GUITreeNode * pParent = m_pParent;
    while( pParent != NULL ) {
        Assert( pParent->m_bExpanded );
        pParent->m_iNodeCount += iExpandCount;
        pParent = pParent->m_pParent;
    }
    m_iNodeCount += iExpandCount;
}
Void GUITreeNode::_Collapse()
{
    if ( m_bIsLeaf || !m_bExpanded )
        return;

    // Collapse node
    m_bExpanded = false;

    // Update NodeCounts
    UInt iCollapseCount = (m_iNodeCount - 1);

    GUITreeNode * pParent = m_pParent;
    while( pParent != NULL ) {
        Assert( pParent->m_bExpanded );
        pParent->m_iNodeCount -= iCollapseCount;
        pParent = pParent->m_pParent;
    }
    m_iNodeCount -= iCollapseCount;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITreeModel implementation
GUITreeModel::GUITreeModel( UInt iNodeHeight ):
    GUIWidgetModel()
{
    m_iDisplayTop = 0;
    m_iDisplayCount = 0;

    m_iNodeHeight = iNodeHeight;
    if ( m_iNodeHeight < GUISKIN_VSPACING_ICON )
        m_iNodeHeight = GUISKIN_VSPACING_ICON;

    m_iLastSelected = INVALID_OFFSET;
    m_pLastSelected = NULL;

    // Create Tree
    GUIFn->SelectMemory( TEXT("LayoutData") );
    m_pRoot = New GUITreeNode();
    GUIFn->UnSelectMemory();
}
GUITreeModel::~GUITreeModel()
{
    // Destroy Tree
    GUIFn->SelectMemory( TEXT("LayoutData") );
    Delete( m_pRoot );
    GUIFn->UnSelectMemory();
}

GUITreeNode * GUITreeModel::CreateNode( GUITreeNode * pParent, Bool bIsLeaf, const GChar * strName, GUISkinImage idIcon )
{
    // Auto-select root as parent
    if ( pParent == NULL )
        pParent = m_pRoot;

    // Create new node
    GUIFn->SelectMemory( TEXT("LayoutData") );
    GUITreeNode * pNode = New GUITreeNode( pParent, bIsLeaf, strName, idIcon );
    GUIFn->UnSelectMemory();

    // Return new node
    return pNode;
}
Void GUITreeModel::DestroyNode( GUITreeNode * pNode )
{
    // Deny this ...
    if ( pNode->IsRoot() )
        return;

    // Destroy node
    GUITreeNode * pParent = pNode->GetParent();
    pParent->_DestroyChild( pNode->GetParentIndex() );
}

Void GUITreeModel::Expand( GUITreeNode * pNode )
{
    if ( pNode->IsLeaf() || pNode->IsExpanded() )
        return;

    // Populate node
    if ( !(pNode->m_bPopulated) ) {
        // Raise callback
        OnPopulateNode( pNode );

        pNode->m_bPopulated = true;
    }

    // Expand node
    pNode->_Expand();
}
Void GUITreeModel::Collapse( GUITreeNode * pNode )
{
    // Collapse node
    pNode->_Collapse();
}

Void GUITreeModel::SetScroll( UInt iScrollPos )
{
    // Scroll to
    UInt iMaxTop = ( m_pRoot->GetNodeCount() - m_iDisplayCount );
    if ( iScrollPos > iMaxTop )
        m_iDisplayTop = iMaxTop;
    else
        m_iDisplayTop = iScrollPos;
}
Void GUITreeModel::ScrollUp( UInt iAmount )
{
    // Scroll up
    if ( m_iDisplayTop <= iAmount )
        m_iDisplayTop = 0;
    else
        m_iDisplayTop -= iAmount;
}
Void GUITreeModel::ScrollDown( UInt iAmount )
{
    // Scroll down
    UInt iMaxTop = ( m_pRoot->GetNodeCount() - m_iDisplayCount );
    if ( m_iDisplayTop + iAmount > iMaxTop )
        m_iDisplayTop = iMaxTop;
    else
        m_iDisplayTop += iAmount;
}

Void GUITreeModel::SelectAll()
{
    // Select all
    UInt iCurNode = 0;
    GUITreeNode * pCurNode = GetNode( iCurNode );
    while( pCurNode != NULL ) {
        pCurNode->m_bSelected = true;
        m_iLastSelected = iCurNode;
        m_pLastSelected = pCurNode;

        ++iCurNode;
        pCurNode = pCurNode->GetNextNode();
    }

    // Raise callback
    OnSelectionChange();
}
Void GUITreeModel::UnselectAll()
{
    // Unselect all
    GUITreeNode * pCurNode = GetNode( 0u );
    while( pCurNode != NULL ) {
        pCurNode->m_bSelected = false;

        pCurNode = pCurNode->GetNextNode();
    }
    m_iLastSelected = INVALID_OFFSET;
    m_pLastSelected = NULL;

    // Raise callback
    OnSelectionChange();
}
Void GUITreeModel::Select( UInt iNode, Bool bSelect, GUITreeNode * pNode )
{
    // GetNode
    if ( pNode == NULL )
        pNode = GetNode( iNode );
    if ( pNode->m_bSelected == bSelect )
        return;

    // Select/Unselect
    pNode->m_bSelected = bSelect;
    if ( pNode->m_bSelected ) {
        m_iLastSelected = iNode;
        m_pLastSelected = pNode;
    } else {
        m_iLastSelected = INVALID_OFFSET;
        m_pLastSelected = NULL;
    }

    // Raise callback
    OnSelectionChange();
}
Void GUITreeModel::ToggleSelect( UInt iNode, GUITreeNode * pNode )
{
    // GetNode
    if ( pNode == NULL )
        pNode = GetNode( iNode );

    // Toggle Selection
    pNode->m_bSelected = !(pNode->m_bSelected);
    if ( pNode->m_bSelected ) {
        m_iLastSelected = iNode;
        m_pLastSelected = pNode;
    } else {
        m_iLastSelected = INVALID_OFFSET;
        m_pLastSelected = NULL;
    }

    // Raise callback
    OnSelectionChange();
}

Void GUITreeModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
{
    // Handle context menu
    if ( iKey == KEYCODE_MOUSERIGHT ) {
        GUIWidgetModel::OnClick( ptLocalPos, iKey, iFlags );
        return;
    }

    if ( iKey != KEYCODE_MOUSELEFT )
        return;

    // Flags
    Bool bCapsLock = ( (iFlags & GUIEVENT_FLAG_CAPSLOCK) != 0 );
    Bool bShift    = ( (iFlags & GUIEVENT_FLAG_SHIFT) != 0 );
    Bool bCtrl     = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );

    // Pick node
    UInt iNode = m_iDisplayTop + ( ptLocalPos.Y / m_iNodeHeight );

    // Deal event
    if ( iNode >= GetNodeCount() ) {
        UnselectAll();
        return;
    }

    // Get node
    GUITreeNode * pNode = GetNode( iNode );

    // Expand Box Hit test
    Int iExpandBoxX = ( (pNode->GetDepth() - 1) * GUISKIN_HSPACING_ICON );
    Int iExpandBoxY = ( (iNode - m_iDisplayTop) * m_iNodeHeight );
    iExpandBoxX += ( (GUISKIN_HSPACING_ICON - GUISKIN_SIZE_ICON_TINY) >> 1 );
    iExpandBoxY += ( (m_iNodeHeight - GUISKIN_SIZE_ICON_TINY) >> 1 );
    Bool bHitExpandBox =             ( ptLocalPos.X >= iExpandBoxX && ptLocalPos.X < (iExpandBoxX + GUISKIN_SIZE_ICON_TINY) );
    bHitExpandBox = bHitExpandBox && ( ptLocalPos.Y >= iExpandBoxY && ptLocalPos.Y < (iExpandBoxY + GUISKIN_SIZE_ICON_TINY) );

    // Deal event
    if ( bHitExpandBox ) {
        UnselectAll();
        if ( pNode->IsLeaf() )
            return;
        if ( pNode->IsExpanded() )
            Collapse( pNode );
        else
            Expand( pNode );
        return;
    }
    if ( bShift != bCapsLock ) {
        if ( m_pLastSelected == NULL ) {
            ToggleSelect( iNode, pNode );
        } else {
            UInt iCurNode;
            GUITreeNode *pCurNode, *pLastNode;
            if ( m_iLastSelected <= iNode ) {
                iCurNode = m_iLastSelected;
                pCurNode = m_pLastSelected;
                pLastNode = pNode;
                while( pCurNode != pLastNode ) {
                    Select( iCurNode++, true, pCurNode );
                    pCurNode = pCurNode->GetNextNode();
                }
            } else {
                iCurNode = iNode;
                pCurNode = pNode;
                pLastNode = m_pLastSelected;
                while( pCurNode != pLastNode ) {
                    Select( iCurNode++, true, pCurNode );
                    pCurNode = pCurNode->GetNextNode();
                }
            }
            Select( iCurNode, true, pLastNode );
        }
    } else if ( bCtrl ) {
        ToggleSelect( iNode, pNode );
    } else {
        UnselectAll();
        Select( iNode, true, pNode );
    }
}

Void GUITreeModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
{
    if ( m_iDisplayCount >= m_pRoot->GetNodeCount() ) {
        if ( m_iDisplayTop != 0 )
            SetScroll( 0 );
        return;
    }

    // Flags
    Bool bCtrl = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );

    // Deal event
    if ( iWheelDelta > 0 )
        ScrollUp( bCtrl ? 3 : 1 );
    else if ( iWheelDelta < 0 )
        ScrollDown( bCtrl ? 3 : 1 );
}

Void GUITreeModel::OnResize()
{
    // Widget
    GUIList * pWidget = (GUIList*)_GetWidget();
    UInt iClientHeight = pWidget->GetClientRect().Height();

    // Deal event
    m_iDisplayCount = ( iClientHeight / m_iNodeHeight );
}

Void GUITreeModel::OnPopulateNode( GUITreeNode * /*pNode*/ )
{
    // stub method, do nothing
}
Void GUITreeModel::OnSelectionChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUITree implementation
GUITree::GUITree( GUITreeModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    pModel->OnResize();
}
GUITree::~GUITree()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITree::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUITreeModel * pModel = (GUITreeModel*)_GetWidgetModel();

    UInt iNodeHeight = pModel->GetNodeHeight();

    UInt iDisplayTop = pModel->GetDisplayTop();
    UInt iDisplayCount = pModel->GetDisplayCount();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_TREE_BACKGROUND );
    const Rectangle2 & locExpandBox = pSkin->GetLocation( GUISKIN_IMAGE_TREE_EXPANDBOX );
    const Rectangle2 & locCollapseBox = pSkin->GetLocation( GUISKIN_IMAGE_TREE_COLLAPSEBOX );

    GPU2DSolidColorBrush * pColorBrush = pSkin->GetSkinColorBrush();
    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_TREE_TEXT );
    GPU2DColor hSelectedTextColor = pSkin->GetColor( GUISKIN_COLOR_TREE_TEXTSELECTED );
    GPU2DColor hSelectionColor = pSkin->GetColor( GUISKIN_COLOR_TREE_SELECTION );
    GPU2DColor hLinksColor = pSkin->GetColor( GUISKIN_COLOR_TREE_LINKS );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_BUTTON );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Tree rows
    UInt iCellHalfWidth = ( GUISKIN_HSPACING_ICON >> 1 );
    UInt iCellHalfHeight = ( iNodeHeight >> 1 );
    
    GUITreeNode * pCurNode = pModel->GetNode( iDisplayTop );
    GUITreeNode * pParent;

    Point2 ptCurRow = rectClient.TopLeft;
    Point2 ptCurPos;

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    for( UInt iRow = 0; iRow < iDisplayCount; ++iRow ) {
        // Check if we're out of nodes
        if ( pCurNode == NULL )
            break;

        // Begin with links
        pColorBrush->SetColor( &hLinksColor );

        // Draw parent links (right to left)
        ptCurPos = ptCurRow;
        ptCurPos.X += ( (pCurNode->GetDepth() - 2) * GUISKIN_HSPACING_ICON );

        pParent = pCurNode->GetParent();
        for( UInt iDepth = 0; iDepth < (pCurNode->GetDepth() - 1); ++iDepth ) {
            if ( !(pParent->IsLastChild()) ) {
                // Centered vertical line
                hDstRect.fLeft = (Float)( ptCurPos.X + iCellHalfWidth );
                hDstRect.fTop = (Float)( ptCurPos.Y );
                hDstRect.fRight = hDstRect.fLeft + 1.0f;
                hDstRect.fBottom = hDstRect.fTop + (Float)iNodeHeight;

                pGUIContext->FillRectangle( &hDstRect, pColorBrush );
            }

            pParent = pParent->GetParent();
            ptCurPos.X -= GUISKIN_HSPACING_ICON;
        }

        // Draw node links
        ptCurPos = ptCurRow;
        ptCurPos.X += ( (pCurNode->GetDepth() - 1) * GUISKIN_HSPACING_ICON );

            // Centered horizontal half-line
        hDstRect.fLeft = (Float)( ptCurPos.X + iCellHalfWidth );
        hDstRect.fTop = (Float)( ptCurPos.Y + iCellHalfHeight );
        hDstRect.fRight = hDstRect.fLeft + (Float)iCellHalfWidth;
        hDstRect.fBottom = hDstRect.fTop + 1.0f;

        pGUIContext->FillRectangle( &hDstRect, pColorBrush );

            // Centered vertical (half-)line
        hDstRect.fLeft = (Float)( ptCurPos.X + iCellHalfWidth );
        hDstRect.fTop = (Float)( ptCurPos.Y );
        hDstRect.fRight = hDstRect.fLeft + 1.0f;
        hDstRect.fBottom = hDstRect.fTop + (Float)( (pCurNode->IsLastChild()) ? iCellHalfHeight : iNodeHeight );

        pGUIContext->FillRectangle( &hDstRect, pColorBrush );

            // Add Expand/Collapse Box
        if ( !(pCurNode->IsLeaf()) ) {
            hDstRect.fLeft = (Float)( ptCurPos.X + ((GUISKIN_HSPACING_ICON - GUISKIN_SIZE_ICON_TINY) >> 1) );
            hDstRect.fTop = (Float)( ptCurPos.Y + ((iNodeHeight - GUISKIN_SIZE_ICON_TINY) >> 1) );
            hDstRect.fRight = hDstRect.fLeft + (Float)GUISKIN_SIZE_ICON_TINY;
            hDstRect.fBottom = hDstRect.fTop + (Float)GUISKIN_SIZE_ICON_TINY;

            hSrcRect = (pCurNode->IsExpanded()) ? locCollapseBox : locExpandBox;

            pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );
        }

        // Next Column
        ptCurPos.X += GUISKIN_HSPACING_ICON;

        // Draw Icon
        hDstRect.fLeft = (Float)( ptCurPos.X );
        hDstRect.fTop = (Float)( ptCurPos.Y );
        hDstRect.fRight = hDstRect.fLeft + (Float)GUISKIN_HSPACING_ICON;
        hDstRect.fBottom = hDstRect.fTop + (Float)GUISKIN_HSPACING_ICON;

        hSrcRect = pSkin->GetLocation( pCurNode->GetIcon() );

        pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

        // Next Column
        ptCurPos.X += GUISKIN_HSPACING_ICON;

        // Draw Text & Selection
        UInt iNameLength = 0;
        const GChar * strName = pCurNode->GetName( &iNameLength );

        GPU2DTextLayout hTextLayout;
        hTextLayout.Bind( pTextFormat, (Float)( rectClient.DownRight.X + 1 - ptCurPos.X ), (Float)iNodeHeight, strName, iNameLength );

        GPU2DTextMetrics hMetrics;
        hTextLayout.GetMetrics( &hMetrics );

        if ( pCurNode->IsSelected() ) {
            hDstRect.fLeft = (Float)( ptCurPos.X + GUISKIN_MARGINS ) + hMetrics.fLeft;
            hDstRect.fTop = (Float)( ptCurPos.Y ) + hMetrics.fTop;
            hDstRect.fRight = hDstRect.fLeft + hMetrics.fWidth;
            hDstRect.fBottom = hDstRect.fTop + hMetrics.fHeight;

            pColorBrush->SetColor( &hSelectionColor );
            pGUIContext->FillRectangle( &hDstRect, pColorBrush );
        }

        GPU2DPoint hDstPoint = ptCurPos;
        hDstPoint.fX += (Float)GUISKIN_MARGINS;

        pTextBrush->SetColor( (pCurNode->IsSelected()) ? &hSelectedTextColor : &hTextColor );
        pGUIContext->DrawTextLayout( &hDstPoint, &hTextLayout, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        hTextLayout.UnBind();

        // Next row
        ptCurRow.Y += iNodeHeight;
        pCurNode = pCurNode->GetNextNode();
    }
}

