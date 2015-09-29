/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITextLog.h
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
// Header prelude
#ifndef SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITEXTLOG_H
#define SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITEXTLOG_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUITEXTLOG_LINE_SPLITS 64
#define GUITEXTLOG_LINE_LENGTH 1024

/////////////////////////////////////////////////////////////////////////////////
// The GUITextLogLine class
class GUITextLogLine
{
public:
    GUITextLogLine();
    GUITextLogLine( const GChar * strText );
    ~GUITextLogLine();

    // Getters
    inline UInt GetLineCount() const;
    inline UInt GetSplit( UInt iSplit ) const;

    inline UInt GetLength() const;
    inline const GChar * GetText( UInt * outLength = NULL ) const;

private:
    friend class GUITextLogModel;

    // State
    UInt m_iLineCount;
    UInt m_arrSplits[GUITEXTLOG_LINE_SPLITS];

    // User Data
    UInt m_iLength;
    GChar m_strText[GUITEXTLOG_LINE_LENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITextLogModel class
class GUITextLogModel : public GUIWidgetModel
{
public:
    GUITextLogModel( UInt iLineHeight );
    virtual ~GUITextLogModel();

    // Model
    inline UInt GetDisplayBottom() const;
    inline UInt GetDisplayBottomInner() const;
    inline UInt GetDisplayLineCount() const;

    inline UInt GetLineHeight() const;

    inline UInt GetLineCount() const;
    inline GUITextLogLine * GetLine( UInt iLine ) const;

    Void Log( const GChar * strText );
    Void Clear();

    Void SetScroll( UInt iScrollPos );
    Void ScrollUp( UInt iAmount = 1 );
    Void ScrollDown( UInt iAmount = 1 );

    // Controller
    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

    virtual Void OnResize();

    virtual Void OnTextLog();

protected:
    Void _SplitMultilines( GUITextLogLine * pLine );
    Bool _Decrease_DisplayTop();
    Bool _Increase_DisplayBottom();
    Void _UpdateDisplayFromBottom();
    Void _UpdateDisplayFromTop();

    UInt m_iDisplayTop, m_iDisplayTopInner;
    UInt m_iDisplayBottom, m_iDisplayBottomInner;
    UInt m_iDisplayLineCount;

    UInt m_iLineHeight;

    Array<GUITextLogLine> m_arrLines;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUITextLog class
class GUITextLog : public GUIWidget
{
public:
    GUITextLog( GUITextLogModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUITextLog();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUITextLog.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_EXTENDED_GUITEXTLOG_H

