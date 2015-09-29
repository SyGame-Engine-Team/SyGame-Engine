/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIDragNDrop.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Drag & Drop state management
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
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUIDRAGNDROP_H
#define SCARAB_ENGINE_GUI_TOOLS_GUIDRAGNDROP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Discrete/Point2.h"

// Prototype
class GUIEventProcessor;

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GUIDRAG_THRESHOLD 2

enum GUIDragChannel
{
    GUIDRAG_LEFT = 0,
    GUIDRAG_RIGHT,

    GUIDRAG_CHANNELS
};

typedef struct _gui_drag_data
{
    // State
    Bool bPrepared;
    Bool bConfirmed;

    // Source data
    GUIEventProcessor * pSource;
    Point2 ptSourcePos;

    // Destination data
    GUIEventProcessor * pDest;
    Point2 ptDestPos;

    // Transport data
    Void * pUserData;
} GUIDragData;

/////////////////////////////////////////////////////////////////////////////////
// The GUIDragNDrop class
class GUIDragNDrop
{
public:
    GUIDragNDrop();
    ~GUIDragNDrop();

    inline Bool IsPrepared( GUIDragChannel iChannel ) const;
    inline Bool IsConfirmed( GUIDragChannel iChannel ) const;

    inline GUIEventProcessor * GetSource( GUIDragChannel iChannel ) const;
    inline const Point2 & GetSourcePos( GUIDragChannel iChannel ) const;

    inline GUIEventProcessor * GetDest( GUIDragChannel iChannel ) const;
    inline const Point2 & GetDestPos( GUIDragChannel iChannel ) const;

    inline Void * GetUserData( GUIDragChannel iChannel ) const;
    inline Void SetUserData( GUIDragChannel iChannel, Void * pUserData );

    Void Prepare( GUIDragChannel iChannel, GUIEventProcessor * pMousedOver, const Point2 & ptScreenPos );
    Bool Update( GUIDragChannel iChannel, GUIEventProcessor * pMousedOver, const Point2 & ptScreenPos );
    Void Abort( GUIDragChannel iChannel );
    Void Complete( GUIDragChannel iChannel );

private:
    // Helper test
    static Bool _TestProximity( const Point2 & ptReferencePos, const Point2 & ptTestPos );

    // Drag channels
    GUIDragData m_arrDragData[GUIDRAG_CHANNELS];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIDragNDrop.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUIDRAGNDROP_H

