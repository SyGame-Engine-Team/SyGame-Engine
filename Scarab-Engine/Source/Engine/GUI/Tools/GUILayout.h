/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUILayout.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Representation of layered 2D User Interfaces, scripting.
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
#ifndef SCARAB_ENGINE_GUI_TOOLS_GUILAYOUT_H
#define SCARAB_ENGINE_GUI_TOOLS_GUILAYOUT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Discrete/Rectangle2.h"
#include "../../../Lib/Math/Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GUILayoutKind
{
	GUILAYOUT_BOX = 0,
	GUILAYOUT_GRID
};

enum GUILayoutAnchor
{
	GUILAYOUT_ANCHOR_FIXED = 0,
	GUILAYOUT_ANCHOR_RELATIVE,

	GUILAYOUT_ANCHOR_CENTER,

	GUILAYOUT_ANCHOR_NORTH,
	GUILAYOUT_ANCHOR_WEST,
	GUILAYOUT_ANCHOR_SOUTH,
	GUILAYOUT_ANCHOR_EAST,

	GUILAYOUT_ANCHOR_NORTHWEST,
	GUILAYOUT_ANCHOR_SOUTHWEST,
	GUILAYOUT_ANCHOR_SOUTHEAST,
	GUILAYOUT_ANCHOR_NORTHEAST
};

enum GUILayoutFill
{
	GUILAYOUT_FILL_DEFAULT = 0,
	GUILAYOUT_FILL_FIXED,
	GUILAYOUT_FILL_RELATIVE
};

/////////////////////////////////////////////////////////////////////////////////
// The GUILayout interface
class GUILayout
{
public:
	virtual ~GUILayout() {}

	virtual GUILayoutKind Kind() const = 0;

	virtual Void ComputeLayout( Rectangle2 * outClientRect, const Rectangle2 & parentRect ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIGridLayout class
class GUIGridLayout : public GUILayout
{
public:
	GUIGridLayout();
	GUIGridLayout( const GUIGridLayout & rhs );
	virtual ~GUIGridLayout() {}

	inline GUILayoutKind Kind() const;

	Void ComputeLayout( Rectangle2 * outClientRect, const Rectangle2 & parentRect );

	// Parameters
	UInt GridColumns, GridRows;	// size of the grid, in cells
	Rectangle2 GridArea;		// rect on the grid, in cells = the area

    GUILayoutAnchor Anchor;		// anchor policy inside the area
	GUILayoutFill FillX, FillY;	// filling policy inside the area

	Point2 FixedPos;			// fixed position inside the area (in pixels)
	Point2 FixedSize;			// fixed size inside the area (in pixels)

	Vertex2 RelativePos;		// relative position inside the area (factor in [0;1])
	Vertex2 RelativeSize;		// relative size inside the area (factor in [0;1])
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUILayout.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_TOOLS_GUILAYOUT_H
