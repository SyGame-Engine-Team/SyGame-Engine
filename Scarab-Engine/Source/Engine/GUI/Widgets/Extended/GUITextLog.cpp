/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITextLog.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Multiline text view
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
#include "GUITextLog.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUITextLogLine implementation
GUITextLogLine::GUITextLogLine()
{
    m_strText[0] = NULLBYTE;
    m_iLength = 0;
}
GUITextLogLine::GUITextLogLine( const GChar * strText )
{
    GChar * pEnd = StringFn->NCopy( m_strText, strText, GUITEXTLOG_LINE_LENGTH );
    m_iLength = ( pEnd - m_strText );
}
GUITextLogLine::~GUITextLogLine()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextLogModel implementation
GUITextLogModel::GUITextLogModel( UInt iLineHeight ):
    GUIWidgetModel(), m_arrLines()
{
    m_iDisplayTop = 0;
    m_iDisplayTopInner = 0;
    m_iDisplayBottom = 0;
    m_iDisplayBottomInner = 0;
    m_iDisplayLineCount = 0;

    m_iLineHeight = iLineHeight;
    if ( m_iLineHeight < GUISKIN_VSPACING(16) )
        m_iLineHeight = GUISKIN_VSPACING(16);

    // Line log
    m_arrLines.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("LayoutData") );
    m_arrLines.Create();
}
GUITextLogModel::~GUITextLogModel()
{
    // Line log
    m_arrLines.Destroy();
}

Void GUITextLogModel::Log( const GChar * strText )
{
    // Build item
    GUITextLogLine newLine( strText );

    // Split item
    _SplitMultilines( &newLine );

    // Log item
    m_arrLines.Push( newLine );

    // Raise callback
    OnTextLog();

    // Update
    SetScroll( m_arrLines.Count() - 1 );
}
Void GUITextLogModel::Clear()
{
    // Clear log
    m_arrLines.Clear();

    // Update
    m_iDisplayBottom = 0;
    m_iDisplayBottomInner = 0;
    _UpdateDisplayFromBottom();
}

Void GUITextLogModel::SetScroll( UInt iScrollPos )
{
    Assert( iScrollPos < m_arrLines.Count() );

    // Scroll to pos
    m_iDisplayBottom = iScrollPos;
    m_iDisplayBottomInner = m_arrLines[iScrollPos].m_iLineCount - 1;

    // Update
    _UpdateDisplayFromBottom();
}
Void GUITextLogModel::ScrollUp( UInt iAmount )
{
    if ( m_arrLines.Count() == 0 )
        return;

    // Scroll up
    while( iAmount > 0 ) {
        if ( !_Decrease_DisplayTop() )
            break;
        --iAmount;
    }

    // Update
    _UpdateDisplayFromTop();
}
Void GUITextLogModel::ScrollDown( UInt iAmount )
{
    if ( m_arrLines.Count() == 0 )
        return;

    // Scroll down
    while( iAmount > 0 ) {
        if ( !_Increase_DisplayBottom() )
            break;
        --iAmount;
    }

    // Update
    _UpdateDisplayFromBottom();
}

Void GUITextLogModel::OnScroll( const Point2 & /*ptLocalPos*/, Int iWheelDelta, GUIEventFlag iFlags )
{
    // Flags
    Bool bCtrl = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );

    // Deal event
    if ( iWheelDelta > 0 )
        ScrollUp( bCtrl ? 3 : 1 );
    else if ( iWheelDelta < 0 )
        ScrollDown( bCtrl ? 3 : 1 );
}

Void GUITextLogModel::OnResize()
{
    // Widget
    GUITextLog * pWidget = (GUITextLog*)_GetWidget();
    UInt iClientHeight = pWidget->GetClientRect().Height();

    // Deal event
    m_iDisplayLineCount = ( iClientHeight / m_iLineHeight );

    for( UInt i = 0; i < m_arrLines.Count(); ++i )
        _SplitMultilines( &(m_arrLines[i]) );

    // Update
    _UpdateDisplayFromBottom();
}

Void GUITextLogModel::OnTextLog()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITextLogModel::_SplitMultilines( GUITextLogLine * pLine )
{
    // Log string
    UInt iLineLength = 0;
    const GChar * strLine = pLine->GetText( &iLineLength );

    // Widget
    GUITextLog * pWidget = (GUITextLog*)_GetWidget();
    UInt iDisplayWidth = ( pWidget->GetClientRect().Width() - (GUISKIN_MARGINS << 1) );
    UInt iDisplayHeight = ( pWidget->GetClientRect().Height() - (GUISKIN_MARGINS << 1) );

    // TextLayout
    const GUISkin * pSkin = GUIFn->GetSkin();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_TEXTINPUT );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    GPU2DTextLayout hTextLayout;
    hTextLayout.Bind( pTextFormat, (Float)iDisplayWidth, (Float)iDisplayHeight, strLine, iLineLength ); 

    pLine->m_iLineCount = hTextLayout.GetLineMetricCount();

    GUIFn->SelectMemory( TEXT("Scratch") );
    GPU2DTextLineMetrics * arrLineMetrics = New GPU2DTextLineMetrics[pLine->m_iLineCount];
    GUIFn->UnSelectMemory();

    hTextLayout.GetLineMetrics( arrLineMetrics, pLine->m_iLineCount );

    UInt iOffset = 0;
    for( UInt i = 0; i < pLine->m_iLineCount; ++i ) {
        iOffset += arrLineMetrics[i].iLength;
        pLine->m_arrSplits[i] = iOffset;
    }
    pLine->m_arrSplits[pLine->m_iLineCount - 1] = INVALID_OFFSET;

    GUIFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrLineMetrics );
    arrLineMetrics = NULL;
    GUIFn->UnSelectMemory();

    // Done
    hTextLayout.UnBind();
}
Bool GUITextLogModel::_Decrease_DisplayTop()
{
    if ( m_iDisplayTopInner > 0 )
        --m_iDisplayTopInner;
    else {
        if ( m_iDisplayTop == 0 )
            return false;
        --m_iDisplayTop;
        m_iDisplayTopInner = ( m_arrLines[m_iDisplayTop].m_iLineCount - 1 );
    }
    return true;
}
Bool GUITextLogModel::_Increase_DisplayBottom()
{
    UInt iLast = ( m_arrLines.Count() - 1 );
    UInt iLastInner = ( m_arrLines[m_iDisplayBottom].m_iLineCount - 1 );

    if ( m_iDisplayBottomInner < iLastInner )
        ++m_iDisplayBottomInner;
    else {
        if ( m_iDisplayBottom == iLast )
            return false;
        ++m_iDisplayBottom;
        m_iDisplayBottomInner = 0;
    }
    return true;
}
Void GUITextLogModel::_UpdateDisplayFromBottom()
{
    if ( m_arrLines.Count() == 0 ) {
        m_iDisplayTop = 0;
        m_iDisplayTopInner = 0;
        return;
    }

    // Start at bottom
    m_iDisplayTop = m_iDisplayBottom;
    m_iDisplayTopInner = m_iDisplayBottomInner;
    UInt iLineCount = 1;

    // Seek to top
    while( iLineCount < m_iDisplayLineCount ) {
        if ( !_Decrease_DisplayTop() )
            break;
        ++iLineCount;
    }
}
Void GUITextLogModel::_UpdateDisplayFromTop()
{
    if ( m_arrLines.Count() == 0 ) {
        m_iDisplayBottom = 0;
        m_iDisplayBottomInner = 0;
        return;
    }

    // Start at top
    m_iDisplayBottom = m_iDisplayTop;
    m_iDisplayBottomInner = m_iDisplayTopInner;
    UInt iLineCount = 1;

    // Seek to bottom
    while( iLineCount < m_iDisplayLineCount ) {
        if ( !_Increase_DisplayBottom() )
            break;
        ++iLineCount;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextLog implementation
GUITextLog::GUITextLog( GUITextLogModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    pModel->OnResize();
}
GUITextLog::~GUITextLog()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITextLog::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUITextLogModel * pModel = (GUITextLogModel*)_GetWidgetModel();

    UInt iLineHeight = pModel->GetLineHeight();

    UInt iDisplayLineCount = pModel->GetDisplayLineCount();

    UInt iDisplayBottom = pModel->GetDisplayBottom();
    UInt iDisplayBottomInner = pModel->GetDisplayBottomInner();
    
    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_TEXTLOG_BACKGROUND );

    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_TEXTLOG_TEXT );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_TEXTINPUT );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Log lines
    GPU2DPoint hDstPoint;

    if ( pModel->GetLineCount() == 0 )
        return;

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    UInt iLineLenght;
    GChar strLine[GUITEXTLOG_LINE_LENGTH];

    UInt iLine = iDisplayBottom;
    UInt iInnerLine = iDisplayBottomInner;
    GUITextLogLine * pCurLine = pModel->GetLine( iLine );
    UInt iSplit;

    UInt iCurY = rectClient.DownRight.Y + 1 - iLineHeight;

    for( UInt iRow = 0; iRow < iDisplayLineCount; ++iRow ) {
        // Text
        iSplit = pCurLine->GetSplit( iInnerLine );
        if ( iSplit != INVALID_OFFSET )
            iLineLenght = iSplit;
        else
            iLineLenght = pCurLine->GetLength();
        if ( iInnerLine == 0 )
            StringFn->NCopy( strLine, pCurLine->GetText(), iLineLenght );
        else {
            iSplit = pCurLine->GetSplit( iInnerLine - 1 );
            Assert( iSplit != INVALID_OFFSET );
            iLineLenght -= iSplit;
            StringFn->NCopy( strLine, pCurLine->GetText() + iSplit, iLineLenght );
        }

        hDstRect.fLeft = (Float)( rectClient.TopLeft.X + GUISKIN_MARGINS );
        hDstRect.fTop = (Float)iCurY;
        hDstRect.fRight = hDstRect.fLeft + (Float)( rectClient.Width() - (GUISKIN_MARGINS << 1) );
        hDstRect.fBottom = hDstRect.fTop + (Float)iLineHeight;

        pTextBrush->SetColor( &hTextColor );
        pGUIContext->DrawText( strLine, iLineLenght, pTextFormat, &hDstRect, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

        // Up one line
        if ( iInnerLine > 0 )
            --iInnerLine;
        else {
            if ( iLine == 0 )
                break;
            --iLine;
            pCurLine = pModel->GetLine( iLine );
            iInnerLine = ( pCurLine->GetLineCount() - 1 );
        }

        iCurY -= iLineHeight;
    }
}

