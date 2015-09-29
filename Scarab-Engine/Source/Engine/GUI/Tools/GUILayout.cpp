/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUILayout.cpp
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GUILayout.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIGridLayout implementation
GUIGridLayout::GUIGridLayout():
	GridArea(0,0,0,0),
    FixedPos(0,0), FixedSize(0,0),
    RelativePos(0.0f,0.0f), RelativeSize(0.0f,0.0f)
{
	GridRows = 1;
    GridColumns = 1;

	Anchor = GUILAYOUT_ANCHOR_CENTER;

	FillX = GUILAYOUT_FILL_DEFAULT;
	FillY = GUILAYOUT_FILL_DEFAULT;
}
GUIGridLayout::GUIGridLayout( const GUIGridLayout & rhs ):
	GridArea(rhs.GridArea),
    FixedPos(rhs.FixedPos), FixedSize(rhs.FixedSize),
	RelativePos(rhs.RelativePos), RelativeSize(rhs.RelativeSize)
{
	GridRows = rhs.GridRows;
    GridColumns = rhs.GridColumns;

	Anchor = rhs.Anchor;

	FillX = rhs.FillX;
	FillY = rhs.FillY;
}

Void GUIGridLayout::ComputeLayout( Rectangle2 * outClientRect, const Rectangle2 & parentRect )
{
    // Check grid dimension
    Assert( GridColumns > 0 );
    Assert( GridRows > 0 );

    // Deduce grid dimensions
	UInt iCellWidth = ( parentRect.Width() / GridColumns );
	UInt iCellHeight = ( parentRect.Height() / GridRows );

    // Clamp grid-area
    GridArea.TopLeft.X = Clamp<Int>( GridArea.TopLeft.X, 0, (Int)(GridColumns - 1) );
    GridArea.TopLeft.Y = Clamp<Int>( GridArea.TopLeft.Y, 0, (Int)(GridRows - 1) );
    GridArea.DownRight.X = Clamp<Int>( GridArea.DownRight.X, 0, (Int)(GridColumns - 1) );
    GridArea.DownRight.Y = Clamp<Int>( GridArea.DownRight.Y, 0, (Int)(GridRows - 1) );

    // Deduce pixel-area
	Rectangle2 pixelArea;
    pixelArea.TopLeft = GridArea.TopLeft;
	pixelArea.TopLeft.X *= iCellWidth;
	pixelArea.TopLeft.Y *= iCellHeight;

	pixelArea.DownRight = pixelArea.TopLeft;
	pixelArea.DownRight.X += ( GridArea.Width() * iCellWidth ) - 1;
	pixelArea.DownRight.Y += ( GridArea.Height() * iCellHeight ) - 1;

    pixelArea.TopLeft.X += parentRect.TopLeft.X;
	pixelArea.TopLeft.Y += parentRect.TopLeft.Y;
    pixelArea.DownRight.X += parentRect.TopLeft.X;
	pixelArea.DownRight.Y += parentRect.TopLeft.Y;

    // Clamp fixed values
    FixedPos.X = Clamp<Int>( FixedPos.X, 0, pixelArea.Width() - 1 );
    FixedPos.Y = Clamp<Int>( FixedPos.Y, 0, pixelArea.Height() - 1 );
    FixedSize.X = Clamp<Int>( FixedSize.X, 0, pixelArea.Width() - FixedPos.X );
    FixedSize.Y = Clamp<Int>( FixedSize.Y, 0, pixelArea.Height() - FixedPos.Y );

    // Clamp relative values
    RelativePos.X = Clamp<Scalar>( RelativePos.X, 0.0f, 1.0f );
    RelativePos.Y = Clamp<Scalar>( RelativePos.Y, 0.0f, 1.0f );
    RelativeSize.X = Clamp<Scalar>( RelativeSize.X, 0.0f, 1.0f );
    RelativeSize.Y = Clamp<Scalar>( RelativeSize.Y, 0.0f, 1.0f );

    // Deduce client rect's size
	UInt iClientWidth, iClientHeight;
	switch( FillX ) {
		case GUILAYOUT_FILL_DEFAULT:
			iClientWidth = pixelArea.Width();
			break;
		case GUILAYOUT_FILL_FIXED:
			iClientWidth = FixedSize.X;
			break;
		case GUILAYOUT_FILL_RELATIVE:
			iClientWidth = MathFn->Floor( ( (Scalar)pixelArea.Width() ) * RelativeSize.X );
			break;
		default: Assert(false); break; // never happen
	}
	switch( FillY ) {
		case GUILAYOUT_FILL_DEFAULT:
			iClientHeight = pixelArea.Height();
			break;
		case GUILAYOUT_FILL_FIXED:
			iClientHeight = FixedSize.Y;
			break;
		case GUILAYOUT_FILL_RELATIVE:
			iClientHeight = MathFn->Floor( ( (Scalar)pixelArea.Height() ) * RelativeSize.Y );
			break;
		default: Assert(false); break; // never happen
	}

    // Deduce client rect's position
	Point2 clientPosition = pixelArea.TopLeft;
	switch( Anchor ) {
		case GUILAYOUT_ANCHOR_FIXED:
			clientPosition.X += FixedPos.X;
			clientPosition.Y += FixedPos.Y;
			break;
		case GUILAYOUT_ANCHOR_RELATIVE:
			clientPosition.X += MathFn->Floor( ( (Scalar)(pixelArea.Width() - 1) ) * RelativePos.X );
            clientPosition.Y += MathFn->Floor( ( (Scalar)(pixelArea.Height() - 1) ) * RelativePos.Y );
			break;

		case GUILAYOUT_ANCHOR_CENTER:
			clientPosition.X += ( (pixelArea.Width() - iClientWidth) >> 1 ); // /2
			clientPosition.Y += ( (pixelArea.Height() - iClientHeight) >> 1 ); // /2
			break;

		case GUILAYOUT_ANCHOR_NORTH:
			clientPosition.X += ( (pixelArea.Width() - iClientWidth) >> 1 ); // /2
			break;
		case GUILAYOUT_ANCHOR_WEST:
			clientPosition.Y += ( (pixelArea.Height() - iClientHeight) >> 1 ); // /2
			break;
		case GUILAYOUT_ANCHOR_SOUTH:
			clientPosition.X += ( (pixelArea.Width() - iClientWidth) >> 1 ); // /2
			clientPosition.Y += ( pixelArea.Height() - iClientHeight );
			break;
		case GUILAYOUT_ANCHOR_EAST:
			clientPosition.X += ( pixelArea.Width() - iClientWidth );
			clientPosition.Y += ( (pixelArea.Height() - iClientHeight) >> 1 ); // /2
			break;

		case GUILAYOUT_ANCHOR_NORTHWEST:
			// nothing to do
			break;
		case GUILAYOUT_ANCHOR_SOUTHWEST:
			clientPosition.Y += ( pixelArea.Height() - iClientHeight );
			break;
		case GUILAYOUT_ANCHOR_SOUTHEAST:
			clientPosition.X += ( pixelArea.Width() - iClientWidth );
			clientPosition.Y += ( pixelArea.Height() - iClientHeight );
			break;
		case GUILAYOUT_ANCHOR_NORTHEAST:
			clientPosition.X += ( pixelArea.Width() - iClientWidth );
			break;

		default: Assert(false); break; // never happen
	}

    // Done
	*outClientRect = Rectangle2( clientPosition, iClientWidth, iClientHeight );
}

