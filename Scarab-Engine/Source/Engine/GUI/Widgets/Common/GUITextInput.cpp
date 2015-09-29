/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUITextInput.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Text Input (monoline)
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
#include "GUITextInput.h"

#include "../../GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUITextInputModel implementation
GUITextInputModel::GUITextInputModel( const GChar * strText ):
    GUIWidgetModel()
{
    m_iDisplayStart = 0;
    m_iDisplayEnd = 0;

    m_iCursor = 0;
    m_iSelectionStart = INVALID_OFFSET;
	m_iSelectionEnd = INVALID_OFFSET;

    m_bInsertMode = true;

    m_iLength = 0;
    m_strText[0] = NULLBYTE;
    if ( strText != NULL )
        m_iLength = ( StringFn->NCopy(m_strText, strText, GUITEXTINPUT_MAX_LENGTH) - m_strText );
}
GUITextInputModel::~GUITextInputModel()
{
    // nothing to do
}

Void GUITextInputModel::CursorIncrease( UInt iAmount )
{
    if ( m_iCursor + iAmount > m_iLength )
        iAmount = (m_iLength - m_iCursor);
    if ( iAmount == 0 )
        return;

    // Increase cursor
	m_iCursor += iAmount;
    if ( m_iCursor > m_iDisplayEnd + 1 ) {
        UInt iInc = ( m_iCursor - m_iDisplayEnd - 1 );
        m_iDisplayStart += iInc;
        if ( m_iDisplayEnd + iInc + 4 < m_iLength )
            m_iDisplayStart += 4;
        _UpdateDisplayFromStart();
    }

    // Raise callback
    OnCursorChange();
}
Void GUITextInputModel::CursorDecrease( UInt iAmount )
{
    if ( iAmount > m_iCursor )
        iAmount = m_iCursor;
    if ( iAmount == 0 )
        return;

    // Decrease cursor
	m_iCursor -= iAmount;
    if ( m_iCursor < m_iDisplayStart ) {
        m_iDisplayStart = m_iCursor;
        if ( m_iDisplayStart >= 4 )
            m_iDisplayStart -= 4;
        _UpdateDisplayFromStart();
    }

    // Raise callback
    OnCursorChange();
}

Void GUITextInputModel::SelectAll()
{
    if ( m_iSelectionStart == 0 && m_iSelectionEnd == (m_iLength - 1) )
        return;

    // Select all
    m_iSelectionStart = 0;
	m_iSelectionEnd = m_iLength - 1;

    // Raise callback
    OnSelectionChange();
}
Void GUITextInputModel::UnselectAll()
{
    if ( m_iSelectionStart == INVALID_OFFSET )
        return;

    // Unselect all
    m_iSelectionStart = INVALID_OFFSET;
	m_iSelectionEnd = INVALID_OFFSET;

    // Raise callback
    OnSelectionChange();
}
Void GUITextInputModel::Select( UInt iStart, UInt iEnd )
{
    Assert( iStart <= iEnd );
    Assert( iEnd < m_iLength );
    if ( m_iSelectionStart == iStart && m_iSelectionEnd == iEnd )
        return;

    // Select
	m_iSelectionStart = iStart;
	m_iSelectionEnd = iEnd;

    // Raise callback
    OnSelectionChange();
}

Void GUITextInputModel::SetInsertMode( Bool bInsertMode )
{
    // Set insert mode
    m_bInsertMode = bInsertMode;
}
Void GUITextInputModel::ToggleInsertMode()
{
    // Toggle insert mode
    m_bInsertMode = !m_bInsertMode;
}

Void GUITextInputModel::SetText( const GChar * strText )
{
    // Reset selection
    if ( m_iSelectionStart != INVALID_OFFSET ) {
        m_iSelectionStart = INVALID_OFFSET;
	    m_iSelectionEnd = INVALID_OFFSET;

        // Raise callback
        OnSelectionChange();
    }

    // Set new text
    m_iLength = ( StringFn->NCopy( m_strText, strText, GUITEXTINPUT_MAX_LENGTH - 1 )
                  - m_strText );

    // Raise callback
    OnTextChange();

    // Update
    m_iDisplayStart = 0;
    _UpdateDisplayFromStart();
    SetCursor( m_iLength );
}
Void GUITextInputModel::ClearText()
{
    // Reset selection
    if ( m_iSelectionStart != INVALID_OFFSET ) {
        m_iSelectionStart = INVALID_OFFSET;
	    m_iSelectionEnd = INVALID_OFFSET;

        // Raise callback
        OnSelectionChange();
    }

    // Clear text
    m_iLength = 0;
    m_strText[0] = NULLBYTE;

    // Raise callback
    OnTextChange();

    // Update
    m_iDisplayStart = 0;
    _UpdateDisplayFromStart();
    SetCursor( 0 );
}
Void GUITextInputModel::PutChar( GChar ch )
{
    // Put character
    if ( m_bInsertMode ) {
        Assert( (m_iLength + 1) < GUITEXTINPUT_MAX_LENGTH );
        for( Int j = (Int)m_iLength; j >= (Int)m_iCursor; --j )
            m_strText[j + 1] = m_strText[j];
        m_strText[m_iCursor] = ch;
        ++m_iLength;
    } else {
        Assert( (m_iCursor + 1) < GUITEXTINPUT_MAX_LENGTH );
        if ( m_iCursor == m_iLength ) {
            m_strText[m_iLength++] = ch;
        } else
			m_strText[m_iCursor] = ch;
	}
    m_strText[m_iLength] = NULLBYTE;

    // Raise callback
    OnTextChange();

    // Update
    _UpdateDisplayFromStart();
	CursorIncrease();
}
Void GUITextInputModel::PutText( const GChar * strText, UInt iLength )
{
    // Put string
    UInt i;
    if ( m_bInsertMode ) {
        Assert( (m_iLength + iLength) < GUITEXTINPUT_MAX_LENGTH );
        for( Int j = (Int)m_iLength; j >= (Int)m_iCursor; --j )
            m_strText[j + iLength] = m_strText[j];
        for( i = 0; i < iLength; ++i )
            m_strText[m_iCursor + i] = strText[i];
        m_iLength += iLength;
    } else {
        Assert( (m_iCursor + iLength) < GUITEXTINPUT_MAX_LENGTH );
        for( i = 0; i < iLength; ++i )
            m_strText[m_iCursor + i] = strText[i];
        if ( m_iCursor + iLength > (Int)m_iLength )
            m_iLength = m_iCursor + iLength;
	}
    m_strText[m_iLength] = NULLBYTE;

    // Raise callback
    OnTextChange();

    // Update
    _UpdateDisplayFromStart();
    CursorIncrease( iLength );
}
Void GUITextInputModel::DeleteText( UInt iLength, Bool bGoLeft )
{
    // Choose delete direction
    UInt iDelete;
    if ( bGoLeft ) {
		if ( m_iCursor == 0 )
			return;
        iDelete = ( m_iCursor - iLength );
    } else {
        if ( m_iCursor == m_iLength )
            return;
        iDelete = m_iCursor;
    }
    if ( iDelete + iLength > m_iLength )
        iLength = (m_iLength - iDelete);

    // Delete string
    UInt iShiftLen = ( (m_iLength + 1) - iDelete - iLength );
    for( UInt i = 0; i < iShiftLen; ++i )
        m_strText[iDelete + i] = m_strText[iDelete + iLength + i];
    m_iLength -= iLength;
    m_strText[m_iLength] = NULLBYTE;

    // Raise callback
    OnTextChange();

    // Update
    _UpdateDisplayFromStart();
    if ( bGoLeft )
        CursorDecrease( iLength );
}

Void GUITextInputModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
    // Flags
    Bool bNumLock  = ( (iFlags & GUIEVENT_FLAG_NUMLOCK) != 0 );
    Bool bCapsLock = ( (iFlags & GUIEVENT_FLAG_CAPSLOCK) != 0 );
    Bool bShift    = ( (iFlags & GUIEVENT_FLAG_SHIFT) != 0 );
    Bool bCtrl     = ( (iFlags & GUIEVENT_FLAG_CTRL) != 0 );
    Bool bAlt      = ( (iFlags & GUIEVENT_FLAG_ALT) != 0 );

    // Deal event
    switch( iKey ) {
        case KEYCODE_LEFT:
            if ( bShift != bCapsLock ) {
				if ( HasSelection() ) {
					if ( m_iCursor == m_iSelectionStart ) {
                        CursorDecrease();
						Select( m_iCursor, m_iSelectionEnd );
                    } else {
                        Assert( m_iCursor == m_iSelectionEnd + 1 );
                        CursorDecrease();
						if ( m_iSelectionStart < m_iSelectionEnd )
                            Select( m_iSelectionStart, m_iCursor - 1 );
                        else
                            UnselectAll();
                    }
				} else {
                    if ( m_iCursor > 0 ) {
                        CursorDecrease();
                        Select( m_iCursor, m_iCursor );
                    }
                }
			} else {
				if ( HasSelection() ) {
					if ( m_iCursor == m_iSelectionStart )
						CursorDecrease();
					else {
                        Assert( m_iCursor == m_iSelectionEnd + 1 );
						SetCursor( m_iSelectionStart );
                    }
					UnselectAll();
				} else
					CursorDecrease();
			}
            break;
        case KEYCODE_RIGHT:
            if ( bShift != bCapsLock ) {
				if ( HasSelection() ) {
					if ( m_iCursor == m_iSelectionStart ) {
                        CursorIncrease();
                        if ( m_iSelectionStart < m_iSelectionEnd )
                            Select( m_iCursor, m_iSelectionEnd );
                        else
                            UnselectAll();
                    } else {
                        Assert( m_iCursor == m_iSelectionEnd + 1 );
                        CursorIncrease();
						Select( m_iSelectionStart, m_iCursor - 1 );
                    }
				} else {
                    if ( m_iCursor < m_iLength ) {
                        CursorIncrease();
                        Select( m_iCursor - 1, m_iCursor - 1 );
                    }   
                }
			} else {
				if ( HasSelection() ) {
					if ( m_iCursor == m_iSelectionStart )
						SetCursor( m_iSelectionEnd + 1 );
					else {
                        Assert( m_iCursor == m_iSelectionEnd + 1 );
						CursorIncrease();
                    }
					UnselectAll();
				} else
					CursorIncrease();
			}
            break;
        case KEYCODE_HOME:
            if ( bShift != bCapsLock ) {
                if ( HasSelection() )
                    Select( 0, m_iSelectionEnd );
                else if ( m_iCursor > 0 )
				    Select( 0, m_iCursor - 1 );
			} else {
				if ( HasSelection() )
					UnselectAll();
			}
            SetCursor( 0 );
            break;
        case KEYCODE_END:
            if ( bShift != bCapsLock ) {
				if ( HasSelection() )
					Select( m_iSelectionStart, m_iLength - 1 );
                else if ( m_iCursor < m_iLength )
				    Select( m_iCursor, m_iLength - 1 );
			} else {
				if ( HasSelection() )
					UnselectAll();
			}
            SetCursor( m_iLength );
            break;

        case KEYCODE_BACKSPACE:
            if ( HasSelection() ) {
                SetCursor( m_iSelectionStart );
                DeleteText( GetSelectionLength(), false );
                UnselectAll();
			} else
                DeleteText( 1, true );
			break;
        case KEYCODE_DELETE:
            if ( HasSelection() ) {
                SetCursor( m_iSelectionStart );
                DeleteText( GetSelectionLength(), false );
                UnselectAll();
			} else
				DeleteText( 1, false );
            break;

        case KEYCODE_INSERT:
            ToggleInsertMode();
            break;

        case KEYCODE_A:
            if ( bCtrl ) {
                SelectAll();
                SetCursor( m_iLength );
                break;
            }

        case KEYCODE_X:
            if ( bCtrl ) {
                // Cut
                if ( HasSelection() ) {
                    Bool bOk = RenderingFn->SetClipboardData( m_strText + m_iSelectionStart, GetSelectionLength() );
                    Assert( bOk );
                    SetCursor( m_iSelectionStart );
                    DeleteText( GetSelectionLength(), false );
                    UnselectAll();
                }
                break;
            }
        case KEYCODE_C:
            if ( bCtrl ) {
                // Copy
                if ( HasSelection() ) {
                    Bool bOk = RenderingFn->SetClipboardData( m_strText + m_iSelectionStart, GetSelectionLength() );
                    Assert( bOk );
                } else {
                    Bool bOk = RenderingFn->SetClipboardData( m_strText, m_iLength );
                    Assert( bOk );
                }
                break;
            }
        case KEYCODE_V:
            if ( bCtrl ) {
                // Paste
                if ( HasSelection() ) {
                    SetCursor( m_iSelectionStart );
                    DeleteText( GetSelectionLength(), false );
                    UnselectAll();
			    }
                GChar strBuffer[GUITEXTINPUT_MAX_LENGTH + 1];
                Bool bOk = RenderingFn->GetClipboardData( strBuffer, GUITEXTINPUT_MAX_LENGTH );
                if ( bOk )
                    PutText( strBuffer, StringFn->Length(strBuffer) );
                break;
            }

        default: {
                GChar ch = StringFn->ConvertKeyCode( iKey, bShift, bCtrl, bAlt, bNumLock, bCapsLock, false );
                if ( ch == NULLBYTE )
                    return; // non-printable

				if ( HasSelection() ) {
					SetCursor( m_iSelectionStart );
					DeleteText( GetSelectionLength(), false );
					UnselectAll();
				}
				PutChar( ch );
            } break;
    }
}

Void GUITextInputModel::OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags )
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

    // Display string
    GChar strDisplay[GUITEXTINPUT_MAX_LENGTH];
    UInt iDisplayLength = GetDisplayText( strDisplay );

    // Widget
    GUITextInput * pWidget = (GUITextInput*)_GetWidget();
    UInt iDisplayWidth = pWidget->GetClientRect().Width();
    UInt iDisplayHeight = pWidget->GetClientRect().Height();

    // Text Layout
    const GUISkin * pSkin = GUIFn->GetSkin();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_TEXTINPUT );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    GPU2DTextLayout hTextLayout;
    hTextLayout.Bind( pTextFormat, (Scalar)iDisplayWidth, (Scalar)iDisplayHeight, strDisplay, iDisplayLength ); 

    GPU2DTextHitTestMetrics hHitTestMetrics;
    Bool bIsInside, bIsTrailingHit;
    hTextLayout.HitTestPoint( &hHitTestMetrics, &bIsInside, &bIsTrailingHit, (Scalar)(ptLocalPos.X), (Scalar)(ptLocalPos.Y) );

    hTextLayout.UnBind();

    // Deal event
    UInt iHit = m_iDisplayStart + hHitTestMetrics.iPosition;
    if ( !bIsInside && bIsTrailingHit )
        ++iHit;

    if ( bShift != bCapsLock ) {
        if ( HasSelection() ) {
            if ( iHit <= m_iSelectionEnd )
                Select( iHit, m_iSelectionEnd );
            else
                Select( m_iSelectionStart, iHit - 1 );
        } else {
            if ( iHit < m_iCursor )
                Select( iHit, m_iCursor - 1 );
            else if ( iHit > m_iCursor )
                Select( m_iCursor, iHit - 1 );
        }
    } else
        UnselectAll();
    SetCursor( iHit );
}

Void GUITextInputModel::OnResize()
{
    // Deal event
    _UpdateDisplayFromStart();
}

Void GUITextInputModel::OnTextChange()
{
    // stub method, do nothing
}
Void GUITextInputModel::OnCursorChange()
{
    // stub method, do nothing
}
Void GUITextInputModel::OnSelectionChange()
{
    // stub method, do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITextInputModel::_UpdateDisplayFromStart()
{
    // Visible string
    const GChar * strVisible = ( m_strText + m_iDisplayStart );

    // Widget
    GUITextInput * pWidget = (GUITextInput*)_GetWidget();
    UInt iDisplayWidth = pWidget->GetClientRect().Width();
    UInt iDisplayHeight = pWidget->GetClientRect().Height();

    // TextLayout
    const GUISkin * pSkin = GUIFn->GetSkin();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_TEXTINPUT );

    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    GPU2DTextLayout hTextLayout;
    hTextLayout.Bind( pTextFormat, (Scalar)iDisplayWidth, (Scalar)iDisplayHeight, strVisible ); 

    GPU2DTextHitTestMetrics hHitTestMetrics;
    Bool bIsInside, bIsTrailingHit;
    hTextLayout.HitTestPoint( &hHitTestMetrics, &bIsInside, &bIsTrailingHit, (Scalar)iDisplayWidth, (Scalar)(iDisplayHeight >> 1) );

    m_iDisplayEnd = m_iDisplayStart + hHitTestMetrics.iPosition;
    if ( bIsInside && hHitTestMetrics.iPosition > 0 )
        --m_iDisplayEnd;

    // Done
    hTextLayout.UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextInput implementation
GUITextInput::GUITextInput( GUITextInputModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( pModel, pParent, guiLayout, strName )
{
    pModel->OnResize();
}
GUITextInput::~GUITextInput()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void GUITextInput::_Draw( const Rectangle2 & rectClient )
{
    GPU2DContext * pGUIContext = GUIFn->GetScreenContext();

    // Model
    GUITextInputModel * pModel = (GUITextInputModel*)_GetWidgetModel();

    UInt iDisplayStart = pModel->GetDisplayStart();

    GChar strDisplay[GUITEXTINPUT_MAX_LENGTH + 1];
    UInt iDisplayLength = pModel->GetDisplayText( strDisplay );

    Bool bHasSelection = pModel->HasSelection();
    UInt iSelectionStart = pModel->GetSelectionStart();
    UInt iSelectionEnd = pModel->GetSelectionEnd();

    UInt iCursor = pModel->GetCursor();
    Bool bInsertMode = pModel->IsInsertMode();

    // Skin
    const GUISkin * pSkin = GUIFn->GetSkin();

    GPU2DBitmap * pSkinBitmap = pSkin->GetSkinBitmap();
    const Rectangle2 & locBackground = pSkin->GetLocation( GUISKIN_IMAGE_TEXTINPUT_BACKGROUND );

    GPU2DSolidColorBrush * pColorBrush = pSkin->GetSkinColorBrush();
    GPU2DColor hTextColor = pSkin->GetColor( GUISKIN_COLOR_TEXTINPUT_TEXT );
    GPU2DColor hSelectedTextColor = pSkin->GetColor( GUISKIN_COLOR_TEXTINPUT_TEXTSELECTED );
    GPU2DColor hSelectionColor = pSkin->GetColor( GUISKIN_COLOR_TEXTINPUT_SELECTION );
    GPU2DColor hContrastColor = pSkin->GetColor( GUISKIN_COLOR_TEXTINPUT_CONTRAST );

    GPU2DSolidColorBrush * pTextBrush = pSkin->GetSkinTextBrush();
    GPU2DTextFormat * pTextFormat = pSkin->GetSkinTextFormat( GUISKIN_FONT_TEXTINPUT );

    // Background
    GPU2DRectangle hDstRect = rectClient;
    GPU2DRectangle hSrcRect = locBackground;

    pGUIContext->DrawBitmap( pSkinBitmap, &hDstRect, &hSrcRect );

    // Text & Selection
    pTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    pTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

        // Get line height
    Scalar fLineHeight = pTextFormat->GetLineHeight( false );

        // Create a text layout
    GPU2DTextLayout hTextLayout;
    hTextLayout.Bind( pTextFormat, (Scalar)(rectClient.Width()), (Scalar)(rectClient.Height()), strDisplay, iDisplayLength );

        // Compute selection position & width
    GPU2DTextRange hSelectionRange;
    if ( pModel->HasSelection() ) {
        Int iSelectionLeft = iSelectionStart - iDisplayStart;
        if ( iSelectionLeft < 0 )
            iSelectionLeft = 0;
        Int iSelectionRight = iSelectionEnd - iDisplayStart;
        if ( iSelectionRight >= (Int)iDisplayLength )
            iSelectionRight = (Int)iDisplayLength - 1;

        hSelectionRange.iPosition = iSelectionLeft;
        hSelectionRange.iLength = iSelectionRight + 1 - iSelectionLeft;

        UInt iCount = hTextLayout.HitTestTextRangeCount( &hSelectionRange );

        GUIFn->SelectMemory( TEXT("Scratch") );
        GPU2DTextHitTestMetrics * arrHitTestMetrics = New GPU2DTextHitTestMetrics[iCount];
        GUIFn->UnSelectMemory();

        hTextLayout.HitTestTextRange( arrHitTestMetrics, iCount, &hSelectionRange );
    
        hDstRect.fLeft = (Scalar)(rectClient.TopLeft.X) + arrHitTestMetrics[0].fLeft;
        hDstRect.fTop = (Scalar)(rectClient.TopLeft.Y) + arrHitTestMetrics[0].fTop;

        Float fSelectionWidth = ( arrHitTestMetrics[iCount - 1].fLeft - arrHitTestMetrics[0].fLeft ) + arrHitTestMetrics[iCount - 1].fWidth;

        GUIFn->SelectMemory( TEXT("Scratch") );
        DeleteA( arrHitTestMetrics );
        arrHitTestMetrics = NULL;
        GUIFn->UnSelectMemory();

        // Draw selection rect
        hDstRect.fRight = hDstRect.fLeft + fSelectionWidth;
        hDstRect.fBottom = hDstRect.fTop + fLineHeight;

        pColorBrush->SetColor( &hSelectionColor );
        pGUIContext->FillRectangle( &hDstRect, pColorBrush );
    }

        // Draw text
    GPU2DPoint hDstPoint = rectClient.TopLeft;

    if ( pModel->HasSelection() ) {
        pColorBrush->SetColor( &hSelectedTextColor );
        hTextLayout.SetForegroundBrush( pColorBrush, &hSelectionRange );
    }

    pTextBrush->SetColor( &hTextColor );
    pGUIContext->DrawTextLayout( &hDstPoint, &hTextLayout, pTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    // Cursor
    if ( HasFocus() ) {
        GPU2DTextHitTestMetrics hHitTestMetrics;
        hTextLayout.HitTestTextPosition( &hHitTestMetrics, &(hDstPoint.fX), &(hDstPoint.fY), iCursor - iDisplayStart, false );

        hDstRect.fLeft = (Scalar)(rectClient.TopLeft.X) + hDstPoint.fX;
        hDstRect.fTop = (Scalar)(rectClient.TopLeft.Y) + hDstPoint.fY;
        hDstRect.fRight = hDstRect.fLeft + ( (bInsertMode) ? 1.0f : hHitTestMetrics.fWidth );
        hDstRect.fBottom = hDstRect.fTop + fLineHeight;

        pColorBrush->SetColor( (bHasSelection) ? &hContrastColor : &hTextColor );
        pGUIContext->FillRectangle( &hDstRect, pColorBrush );
    }

    // Done
    hTextLayout.UnBind();
}

