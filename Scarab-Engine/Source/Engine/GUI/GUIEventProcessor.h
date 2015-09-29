/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIEventProcessor.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for most GUI objects dealing with events ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_GUIEVENTPROCESSOR_H
#define SCARAB_ENGINE_GUI_GUIEVENTPROCESSOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Math/Types/Discrete/Rectangle2.h"

#include "../../Lib/Datastruct/Array/RingBuffer.h"

#include "Tools/GUIEvent.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUI_NAME_LENGTH 64
#define GUI_PATH_LENGTH 1024

#define GUIEVENT_MAXPENDING 128

// Prototypes
class GUIEventProcessorModel;
class GUIEventProcessor;

/////////////////////////////////////////////////////////////////////////////////
// The GUIEventProcessorModel interface
class GUIEventProcessorModel
{
public:
    GUIEventProcessorModel();
	virtual ~GUIEventProcessorModel();

    // Controller
    virtual Void OnTick( UInt iTimerID );

    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnKeyRelease( KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnMousePress( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnMouseRelease( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDblClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnScroll( const Point2 & ptLocalPos, Int iWheelDelta, GUIEventFlag iFlags );

    virtual Void OnRollIn();
    virtual Void OnRollOut();
    virtual Void OnRollOver( const Point2 & ptLocalPos, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnDragLeft( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnDragRight( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnDragBoth( const Point2 & ptLocalPos, GUIEventFlag iFlags );
	virtual Void OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnFocusGain();
    virtual Void OnFocusLoss();

    virtual Void OnResize();
    virtual Void OnExit();

protected:
    friend class GUIEventProcessor;
    GUIEventProcessor * m_pEventProcessor;
};


/////////////////////////////////////////////////////////////////////////////////
// The GUIEventProcessor interface
class GUIEventProcessor
{
public:
    GUIEventProcessor( GUIEventProcessorModel * pModel, const GChar * strName );
	virtual ~GUIEventProcessor();

    // Getters
    inline GUIEventProcessorModel * GetModel() const;

    inline const GChar * GetName() const;

    inline Bool HasFocus() const;
    inline Bool HasMouseOver() const;

protected:
    // Event-Handling interface
    virtual Void _ToLocalCoords( Point2 * outLocalPos, const Point2 & ptScreenPos ) const = 0;
    virtual Bool _ProcessNextEvent();

    GUIEventProcessorModel * m_pModel;

private:
    // Helpers
    static GUIEventFlag _ComputeInputFlags();

    // Event-Handling interface
    friend class GUIWidget;
    friend class GUIWindow;
    friend class GUIOverlay;
    friend class GUIManager;

    inline Void _PostEvent_Tick( UInt iTimerID );

    Void _PostEvent_KeyPress( KeyCode iKey );
    Void _PostEvent_KeyRelease( KeyCode iKey );

    Void _PostEvent_MouseMove( const Point2 & ptScreenPos );
    Void _PostEvent_MousePress( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseRelease( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseDblClick( const Point2 & ptScreenPos, KeyCode iKey );
    Void _PostEvent_MouseWheel( const Point2 & ptScreenPos, Int iWheelDelta );

    virtual Void _PostEvent_FocusGain();
    virtual Void _PostEvent_FocusLoss();

    Void _PostEvent_RollIn();
    Void _PostEvent_RollOut();

    Void _PostEvent_Resize();
    Void _PostEvent_Exit();

    // Event processing data
    GChar m_strName[GUI_NAME_LENGTH];

    Bool m_bHasFocus;
    Bool m_bHasMouseOver;

    RingBuffer<GUIEvent> m_bufEventQueue;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIEventProcessor.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIEVENTPROCESSOR_H
