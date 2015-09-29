/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIBackboard.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Virtual background representing the "desktop" region,
//               behind all windows ... (mostly for event tracking)
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
#ifndef SCARAB_ENGINE_GUI_GUIBACKBOARD_H
#define SCARAB_ENGINE_GUI_GUIBACKBOARD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUIEventProcessor.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class GUIMenuModel;

class GUIBackboardModel;
class GUIBackboard;

/////////////////////////////////////////////////////////////////////////////////
// The GUIBackboardModel class
class GUIBackboardModel : public GUIEventProcessorModel
{
public:
    GUIBackboardModel();
	virtual ~GUIBackboardModel();

    // Context menu support
    inline Bool HasContextMenu() const;
    inline GUIMenuModel * GetContextMenu() const;

    Void SetContextMenu( GUIMenuModel * pContextMenuModel );

	virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    inline GUIBackboard * _GetBackboard() const;

    // Context menu support
    GUIMenuModel * m_pContextMenuModel;

private:
    // Model switching support
    friend class GUIBackboard;
    inline Void _SetBackboard( GUIBackboard * pBackboard );
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIBackboard class
class GUIBackboard : public GUIEventProcessor
{
public:
    GUIBackboard( GUIBackboardModel * pModel );
	virtual ~GUIBackboard();

    // Getters
    inline const Rectangle2 & GetScreenRect() const;

    // Model switching
    Void SetModel( GUIBackboardModel * pModel );

private:
    // Event-Handling interface
    inline virtual Void _ToLocalCoords( Point2 * outLocalPos, const Point2 & ptScreenPos ) const;

    // Backboard data
    Rectangle2 m_rectScreen;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIBackboard.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIBACKBOARD_H
