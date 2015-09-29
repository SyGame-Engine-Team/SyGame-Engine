/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIOverlay.h
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
// Known Bugs : TODO = GUIDropList (needed by GUIComboBox) ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_GUI_GUIOVERLAY_H
#define SCARAB_ENGINE_GUI_GUIOVERLAY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUIEventProcessor.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIOVERLAY_MIN_DEPTH 0
#define GUIOVERLAY_MAX_DEPTH 31

enum GUIOverlayType
{
	GUIOVERLAY_UNDEFINED = 0,

	GUIOVERLAY_MENU,
	//GUIOVERLAY_TOOLTIP,
	//GUIOVERLAY_DROPDOWNLIST,

	GUIOVERLAY_COUNT
};

// Prototypes
class GUIManager;

class GUIOverlayModel;
class GUIOverlay;

/////////////////////////////////////////////////////////////////////////////////
// The GUIOverlayModel class
class GUIOverlayModel : public GUIEventProcessorModel
{
public:
    GUIOverlayModel();
	virtual ~GUIOverlayModel();

protected:
    inline GUIOverlay * _GetOverlay() const;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIOverlay class
class GUIOverlay : public GUIEventProcessor
{
public:
    GUIOverlay( GUIOverlayModel * pModel, const GChar * strName, const Rectangle2 & rectOverlay );
	virtual ~GUIOverlay();

    // Getters
    inline virtual GUIOverlayType GetType() const;

    inline const Rectangle2 & GetOverlayRect() const;
    inline UInt GetDepth() const;

    inline Bool IsVisible() const;
    inline Void SetVisible( Bool bVisible );

    // Picking interface
    Void ScreenToLocal( Point2 * outLocalPos, const Point2 & ptScreenPos ) const;
    Void LocalToScreen( Point2 * outScreenPos, const Point2 & ptLocalPos ) const;

protected:
    inline GUIOverlayModel * _GetOverlayModel() const;

    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );

private:
    // Give access to GUIManager
    friend class GUIManager;

    // Event-Handling interface
    inline virtual Void _ToLocalCoords( Point2 * outLocalPos, const Point2 & ptScreenPos ) const;

    virtual Void _PostEvent_FocusGain();
    virtual Void _PostEvent_FocusLoss();

    // Rendering interface
    Void _Render();

    // Overlay data
    Rectangle2 m_rectOverlay;
    UInt m_iDepth;

    Bool m_bIsVisible;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIOverlay.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_GUIOVERLAY_H
