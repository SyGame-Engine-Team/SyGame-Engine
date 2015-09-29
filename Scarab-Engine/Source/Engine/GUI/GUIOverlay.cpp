/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIOverlay.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Non-Hierarchic, Overlayed items (menus, tooltips, ...)
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
#include "GUIOverlay.h"

#include "GUIManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIOverlayModel implementation
GUIOverlayModel::GUIOverlayModel():
    GUIEventProcessorModel()
{
    // nothing to do
}
GUIOverlayModel::~GUIOverlayModel()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GUIOverlay implementation
GUIOverlay::GUIOverlay( GUIOverlayModel * pModel, const GChar * strName, const Rectangle2 & rectOverlay ):
    GUIEventProcessor( pModel, strName ), m_rectOverlay( rectOverlay )
{
    Assert( pModel != NULL );

    // Depth
    m_iDepth = GUIOVERLAY_MIN_DEPTH;

    // States
    m_bIsVisible = false;
}
GUIOverlay::~GUIOverlay()
{
    // nothing to do
}

Void GUIOverlay::ScreenToLocal( Point2 * outLocalPos, const Point2 & ptScreenPos ) const
{
    outLocalPos->X = ( ptScreenPos.X - m_rectOverlay.TopLeft.X );
    outLocalPos->Y = ( ptScreenPos.Y - m_rectOverlay.TopLeft.Y );
}
Void GUIOverlay::LocalToScreen( Point2 * outScreenPos, const Point2 & ptLocalPos ) const
{
    outScreenPos->X = ( m_rectOverlay.TopLeft.X + ptLocalPos.X );
    outScreenPos->Y = ( m_rectOverlay.TopLeft.Y + ptLocalPos.Y );
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIOverlay::_Draw( const Rectangle2 & /*rectClient*/ )
{
    // Stub method : do nothing
}

/////////////////////////////////////////////////////////////////////////////////

Void GUIOverlay::_PostEvent_FocusGain()
{
    GUIFn->_UpdateOverlayDepth( this, GUIOVERLAY_MIN_DEPTH );

    GUIEventProcessor::_PostEvent_FocusGain();
}
Void GUIOverlay::_PostEvent_FocusLoss()
{
    Assert( m_iDepth < GUIOVERLAY_MAX_DEPTH );

    GUIFn->_UpdateOverlayDepth( this, m_iDepth + 1 );

    GUIEventProcessor::_PostEvent_FocusLoss();
}

Void GUIOverlay::_Render()
{
    if ( !m_bIsVisible )
        return;

    _Draw( m_rectOverlay );
}

