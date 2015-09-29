/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIEventProcessor.cpp
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
// Includes
#include "GUIEventProcessor.h"

#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIEventProcessorModel implementation
GUIEventProcessorModel::GUIEventProcessorModel()
{
    m_pEventProcessor = NULL;
}
GUIEventProcessorModel::~GUIEventProcessorModel()
{
    // nothing to do
}

Void GUIEventProcessorModel::OnTick( UInt /*iTimerID*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnKeyPress( KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnKeyRelease( KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnMousePress( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnMouseRelease( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnClick( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnDblClick( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnScroll( const Point2 & /*ptLocalPos*/, Int /*iWheelDelta*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnRollIn()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnRollOut()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnRollOver( const Point2 & /*ptLocalPos*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnStartDrag( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnDragLeft( const Point2 & /*ptLocalPos*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnDragRight( const Point2 & /*ptLocalPos*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnDragBoth( const Point2 & /*ptLocalPos*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnStopDrag( const Point2 & /*ptLocalPos*/, KeyCode /*iKey*/, GUIEventFlag /*iFlags*/ )
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnFocusGain()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnFocusLoss()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

Void GUIEventProcessorModel::OnResize()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}
Void GUIEventProcessorModel::OnExit()
{
    Assert( m_pEventProcessor != NULL );
    // Stub method : do nothing
}

/////////////////////////////////////////////////////////////////////////////////
// GUIEventProcessor implementation
GUIEventProcessor::GUIEventProcessor( GUIEventProcessorModel * pModel, const GChar * strName ):
    m_bufEventQueue( GUIEVENT_MAXPENDING )
{
    Assert( strName != NULL );

    // Model
    m_pModel = pModel;
    if ( m_pModel != NULL )
        m_pModel->m_pEventProcessor = this;

    // Name identifier
    StringFn->NCopy( m_strName, strName, GUI_NAME_LENGTH - 1 );

    // States
    m_bHasFocus = false;
    m_bHasMouseOver = false;

    // Event queue
    m_bufEventQueue.UseMemoryContext( GUIFn->GetMemoryContext(), TEXT("Scratch") );
    m_bufEventQueue.Create();
}
GUIEventProcessor::~GUIEventProcessor()
{
    // Event queue
    m_bufEventQueue.Destroy();

    // Model
    if ( m_pModel != NULL )
        m_pModel->m_pEventProcessor = NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Bool GUIEventProcessor::_ProcessNextEvent()
{
    // No event to process
    if ( m_bufEventQueue.IsEmpty() )
        return false;

    // Handle an event
    GUIEvent processEvent;
    m_bufEventQueue.Shift( processEvent );

    if ( m_pModel == NULL )
        return true;

    const KeyCode * pKey;
    const Point2 * pLocalPos;
    const Int * pWheelDelta;

    // Pass control to the model
    switch( processEvent.Type() ) {
        case GUIEVENT_NOP:
            // nothing to do
            break;
        case GUIEVENT_KEYPRESS:
            Assert( processEvent.ParamCount() == 1 );
            pKey = (const KeyCode*)( processEvent.Parameter(0) );
            m_pModel->OnKeyPress( *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_KEYRELEASE:
            Assert( processEvent.ParamCount() == 1 );
            pKey = (const KeyCode*)( processEvent.Parameter(0) );
            m_pModel->OnKeyRelease( *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_MOUSEPRESS:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnMousePress( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_MOUSERELEASE:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnMouseRelease( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_CLICK:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnClick( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_DBLCLICK:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnDblClick( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_SCROLLING:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pWheelDelta = (const Int*)( processEvent.Parameter(1) );
            m_pModel->OnScroll( *pLocalPos, *pWheelDelta, processEvent.Flags() );
            break;
        case GUIEVENT_ROLLIN:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnRollIn();
            break;
        case GUIEVENT_ROLLOUT:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnRollOut();
            break;
        case GUIEVENT_ROLLOVER:
            Assert( processEvent.ParamCount() == 1 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            m_pModel->OnRollOver( *pLocalPos, processEvent.Flags() );
            break;
        case GUIEVENT_STARTDRAG:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnStartDrag( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_DRAGGING_LEFT:
            Assert( processEvent.ParamCount() == 1 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            m_pModel->OnDragLeft( *pLocalPos, processEvent.Flags() );
            break;
        case GUIEVENT_DRAGGING_RIGHT:
            Assert( processEvent.ParamCount() == 1 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            m_pModel->OnDragRight( *pLocalPos, processEvent.Flags() );
            break;
        case GUIEVENT_DRAGGING_BOTH:
            Assert( processEvent.ParamCount() == 1 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            m_pModel->OnDragBoth( *pLocalPos, processEvent.Flags() );
            break;
        case GUIEVENT_STOPDRAG:
            Assert( processEvent.ParamCount() == 2 );
            pLocalPos = (const Point2*)( processEvent.Parameter(0) );
            pKey = (const KeyCode*)( processEvent.Parameter(1) );
            m_pModel->OnStopDrag( *pLocalPos, *pKey, processEvent.Flags() );
            break;
        case GUIEVENT_FOCUSGAIN:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnFocusGain();
            break;
        case GUIEVENT_FOCUSLOSS:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnFocusLoss();
            break;
        case GUIEVENT_RESIZE:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnResize();
            break;
        case GUIEVENT_EXIT:
            Assert( processEvent.ParamCount() == 0 );
            m_pModel->OnExit();
            break;
        default: Assert(false); break;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

GUIEventFlag GUIEventProcessor::_ComputeInputFlags()
{
    GUIEventFlag iFlag = 0;
    if ( InputFn->IsCtrlDown() )
        iFlag |= GUIEVENT_FLAG_CTRL;
    if ( InputFn->IsAltDown() )
        iFlag |= GUIEVENT_FLAG_ALT;
    if ( InputFn->IsShiftDown() )
        iFlag |= GUIEVENT_FLAG_SHIFT;
    if ( InputFn->IsNumLock() )
        iFlag |= GUIEVENT_FLAG_NUMLOCK;
    if ( InputFn->IsCapsLock() )
        iFlag |= GUIEVENT_FLAG_CAPSLOCK;
    if ( InputFn->IsScrollLock() )
        iFlag |= GUIEVENT_FLAG_SCROLLLOCK;
    return iFlag;
}

Void GUIEventProcessor::_PostEvent_KeyPress( KeyCode iKey )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    // KeyPress event
    GUIEvent keyPress( GUIEVENT_KEYPRESS, iFlags );
    keyPress.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
    m_bufEventQueue.Push( keyPress );
}
Void GUIEventProcessor::_PostEvent_KeyRelease( KeyCode iKey )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    // KeyRelease event
    GUIEvent keyRelease( GUIEVENT_KEYRELEASE, iFlags );
    keyRelease.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
    m_bufEventQueue.Push( keyRelease );
}

Void GUIEventProcessor::_PostEvent_MouseMove( const Point2 & ptScreenPos )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    Point2 ptLocalPos;
    _ToLocalCoords( &ptLocalPos, ptScreenPos );

    // Get buttons state
    Bool bLeftButton = InputFn->IsButtonDown( KEYCODE_MOUSELEFT );
    Bool bRightButton = InputFn->IsButtonDown( KEYCODE_MOUSERIGHT );

    // No-Drag case
    if ( !bLeftButton && !bRightButton ) {
        // RollOver event
        GUIEvent rollOver( GUIEVENT_ROLLOVER, iFlags );
        rollOver.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
        m_bufEventQueue.Push( rollOver );
        return;
    }

    // DragNDrop handling
    GUIDragNDrop * pDragNDrop = GUIFn->_GetDragNDrop();

    Bool bDraggingLeft = false;
    Bool bDraggingRight = false;

    // Update DragLeft state
    if ( bLeftButton ) {
        // We MUST be in Prepared state
        Assert( pDragNDrop->IsPrepared(GUIDRAG_LEFT) );

        // Check confirmation
        if ( pDragNDrop->IsConfirmed(GUIDRAG_LEFT) ) {
            // Already in Confirmed state
            bDraggingLeft = pDragNDrop->Update( GUIDRAG_LEFT, this, ptScreenPos );
            Assert( bDraggingLeft );
        } else {
            // Try moving to Confirmed state
            bDraggingLeft = pDragNDrop->Update( GUIDRAG_LEFT, this, ptScreenPos );
            if ( bDraggingLeft ) {
                // StartDrag event (deferred from _PostEvent_MousePress)
                KeyCode iKey = KEYCODE_MOUSELEFT;
                GUIEvent startDrag( GUIEVENT_STARTDRAG, iFlags );
                startDrag.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
                startDrag.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );

                GUIEventProcessor * pDragSource = pDragNDrop->GetSource( GUIDRAG_LEFT );
                Assert( pDragSource != NULL );
                if ( pDragSource->m_bufEventQueue.IsFull() ) {
                    Assert( false );
                    return;
                }
                pDragSource->m_bufEventQueue.Push( startDrag );
            }
        }
    }

    // Update DragRight state
    if ( bRightButton ) {
        // We MUST be in Prepared state
        Assert( pDragNDrop->IsPrepared(GUIDRAG_RIGHT) );

        // Check confirmation
        if ( pDragNDrop->IsConfirmed(GUIDRAG_RIGHT) ) {
            // Already in Confirmed state
            bDraggingRight = pDragNDrop->Update( GUIDRAG_RIGHT, this, ptScreenPos );
            Assert( bDraggingRight );
        } else {
            // Try moving to Confirmed state
            bDraggingRight = pDragNDrop->Update( GUIDRAG_RIGHT, this, ptScreenPos );
            if ( bDraggingRight ) {
                // StartDrag event (deferred from _PostEvent_MousePress)
                KeyCode iKey = KEYCODE_MOUSERIGHT;
                GUIEvent startDrag( GUIEVENT_STARTDRAG, iFlags );
                startDrag.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
                startDrag.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );

                GUIEventProcessor * pDragSource = pDragNDrop->GetSource( GUIDRAG_RIGHT );
                Assert( pDragSource != NULL );
                if ( pDragSource->m_bufEventQueue.IsFull() ) {
                    Assert( false );
                    return;
                }
                pDragSource->m_bufEventQueue.Push( startDrag );
            }
        }
    }

    // Dragging events
    if ( bDraggingLeft ) {
        if ( bDraggingRight ) {
            GUIEvent dragBoth( GUIEVENT_DRAGGING_BOTH, iFlags );
            dragBoth.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
            m_bufEventQueue.Push( dragBoth );
        } else {
            GUIEvent dragLeft( GUIEVENT_DRAGGING_LEFT, iFlags );
            dragLeft.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
            m_bufEventQueue.Push( dragLeft );
        }
    } else {
        if ( bDraggingRight ) {
            GUIEvent dragRight( GUIEVENT_DRAGGING_RIGHT, iFlags );
            dragRight.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
            m_bufEventQueue.Push( dragRight );
        } else {
            GUIEvent rollOver( GUIEVENT_ROLLOVER, iFlags );
            rollOver.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
            m_bufEventQueue.Push( rollOver );
        }
    }
}
Void GUIEventProcessor::_PostEvent_MousePress( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    Point2 ptLocalPos;
    _ToLocalCoords( &ptLocalPos, ptScreenPos );

    // MousePress event
    GUIEvent mousePress( GUIEVENT_MOUSEPRESS, iFlags );
    mousePress.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
    mousePress.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
    m_bufEventQueue.Push( mousePress );

    // No-Drag case
    if ( iKey != KEYCODE_MOUSELEFT && iKey != KEYCODE_MOUSERIGHT )
        return;

    // DragNDrop handling
    GUIDragNDrop * pDragNDrop = GUIFn->_GetDragNDrop();
    GUIDragChannel iChannel = (GUIDragChannel)( iKey - KEYCODE_MOUSELEFT );

    Assert( !( pDragNDrop->IsPrepared(iChannel) ) );
    pDragNDrop->Prepare( iChannel, this, ptScreenPos );

    // StartDrag event deferred to _PostEvent_MouseMove,
    // when dragging conditions are confirmed.
}
Void GUIEventProcessor::_PostEvent_MouseRelease( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    Point2 ptLocalPos;
    _ToLocalCoords( &ptLocalPos, ptScreenPos );

    // MouseRelease event
    GUIEvent mouseRelease( GUIEVENT_MOUSERELEASE, iFlags );
    mouseRelease.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
    mouseRelease.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
    m_bufEventQueue.Push( mouseRelease );

    // No-Drag case
    if ( iKey != KEYCODE_MOUSELEFT && iKey != KEYCODE_MOUSERIGHT )
        return;

    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    // DragNDrop handling
    GUIDragNDrop * pDragNDrop = GUIFn->_GetDragNDrop();
    GUIDragChannel iChannel = (GUIDragChannel)( iKey - KEYCODE_MOUSELEFT );

    Assert( pDragNDrop->IsPrepared(iChannel) );
    if ( pDragNDrop->IsConfirmed(iChannel) ) {
        pDragNDrop->Complete( iChannel );

        // StopDrag event
        GUIEvent stopDrag( GUIEVENT_STOPDRAG, iFlags );
        stopDrag.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
        stopDrag.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
        m_bufEventQueue.Push( stopDrag );
    } else {
        pDragNDrop->Abort( iChannel );

        // Click event
        GUIEvent splClick( GUIEVENT_CLICK, iFlags );
        splClick.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
        splClick.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
        m_bufEventQueue.Push( splClick );
    }
}
Void GUIEventProcessor::_PostEvent_MouseDblClick( const Point2 & ptScreenPos, KeyCode iKey )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    Point2 ptLocalPos;
    _ToLocalCoords( &ptLocalPos, ptScreenPos );

    // Dbl-Click event
    GUIEvent dblClick( GUIEVENT_DBLCLICK, iFlags );
    dblClick.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
    dblClick.PushParameter( (const Byte *)(&iKey), sizeof(KeyCode) );
    m_bufEventQueue.Push( dblClick );
}
Void GUIEventProcessor::_PostEvent_MouseWheel( const Point2 & ptScreenPos, Int iWheelDelta )
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    Point2 ptLocalPos;
    _ToLocalCoords( &ptLocalPos, ptScreenPos );

    // Scrolling event
    GUIEvent wScroll( GUIEVENT_SCROLLING, iFlags );
    wScroll.PushParameter( (const Byte *)(&ptLocalPos), sizeof(Point2) );
    wScroll.PushParameter( (const Byte *)(&iWheelDelta), sizeof(Int) );
    m_bufEventQueue.Push( wScroll );
}

Void GUIEventProcessor::_PostEvent_FocusGain()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    m_bHasFocus = true;

    // FocusGain event
    GUIEvent focusGain( GUIEVENT_FOCUSGAIN, iFlags );
    m_bufEventQueue.Push( focusGain );
}
Void GUIEventProcessor::_PostEvent_FocusLoss()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    m_bHasFocus = false;

    // FocusLoss event
    GUIEvent focusLoss( GUIEVENT_FOCUSLOSS, iFlags );
    m_bufEventQueue.Push( focusLoss );
}

Void GUIEventProcessor::_PostEvent_RollIn()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    m_bHasMouseOver = true;

    // FocusGain event
    GUIEvent rollIn( GUIEVENT_ROLLIN, iFlags );
    m_bufEventQueue.Push( rollIn );
}
Void GUIEventProcessor::_PostEvent_RollOut()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    m_bHasMouseOver = false;

    // FocusGain event
    GUIEvent rollOut( GUIEVENT_ROLLOUT, iFlags );
    m_bufEventQueue.Push( rollOut );
}

Void GUIEventProcessor::_PostEvent_Resize()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    // Resize event
    GUIEvent winResize( GUIEVENT_RESIZE, iFlags );
    m_bufEventQueue.Push( winResize );
}
Void GUIEventProcessor::_PostEvent_Exit()
{
    // Event overflow
    if ( m_bufEventQueue.IsFull() ) {
        Assert( false );
        return;
    }

    GUIEventFlag iFlags = _ComputeInputFlags();

    // Exit event
    GUIEvent guiExit( GUIEVENT_EXIT, iFlags );
    m_bufEventQueue.Push( guiExit );
}
