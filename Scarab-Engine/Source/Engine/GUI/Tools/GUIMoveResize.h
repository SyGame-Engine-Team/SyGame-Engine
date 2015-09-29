/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIMoveResize.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Window Moving & Sizing state management
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
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUIMOVERESIZE_H
#define SCARAB_ENGINE_GUI_TOOLS_GUIMOVERESIZE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Discrete/Rectangle2.h"

// Prototypes
class GUIWindow;

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef DWord GUIResizeFlag;
#define GUIRESIZE_TOP    0x00000001
#define GUIRESIZE_LEFT   0x00000002
#define GUIRESIZE_BOTTOM 0x00000004
#define GUIRESIZE_RIGHT  0x00000008

/////////////////////////////////////////////////////////////////////////////////
// The GUIMoveResize class
class GUIMoveResize
{
public:
    GUIMoveResize();
    ~GUIMoveResize();

    inline Bool IsActive() const;
    inline GUIWindow * GetWindow() const;

    inline Bool IsMoving() const;
    Void StartMoving( GUIWindow * pWindow, const Point2 & ptScreenDragPoint );
    Void UpdateMove( const Point2 & ptScreenPos );
    Void EndMoving();

    inline Bool IsResizing() const;
    Void StartResizing( GUIWindow * pWindow, GUIResizeFlag iFlags );
    Void UpdateResize( const Point2 & ptScreenPos );
    Void EndResizing();

private:
    // Target window
    GUIWindow * m_pWindow;

    // Move state
    Bool m_bIsMoving;    
    Offset2 m_offMoveDragPoint;

    // Resize state
    Bool m_bIsResizing;
    GUIResizeFlag m_iResizeFlags;
    Rectangle2 m_rectOldWinRect;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIMoveResize.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUIMOVERESIZE_H

