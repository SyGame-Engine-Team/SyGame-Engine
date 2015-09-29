/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Rectangle2.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 2D rectangular region
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
#include "Rectangle2.h"

/////////////////////////////////////////////////////////////////////////////////
// Rectangle2 implementation
Rectangle2::Rectangle2():
    TopLeft(), DownRight()
{
    // nothing to do
}
Rectangle2::Rectangle2(Int left, Int top, Int right, Int down):
    TopLeft(left, top), DownRight(right, down)
{
    // nothing to do
}
Rectangle2::Rectangle2(const Int vTopLeft[2], const Int vDownRight[2]):
    TopLeft(vTopLeft), DownRight(vDownRight)
{
    // nothing to do
}
Rectangle2::Rectangle2(const Point2 & vTopLeft, UInt width, UInt height):
    TopLeft(vTopLeft), DownRight(vTopLeft.X + width - 1, vTopLeft.Y + height - 1)
{
    // nothing to do
}
Rectangle2::Rectangle2(const Point2 & vTopLeft, const Point2 & vDownRight):
	TopLeft(vTopLeft), DownRight(vDownRight)
{
    // nothing to do
}
Rectangle2::Rectangle2(const Rectangle2 & rhs):
	TopLeft(rhs.TopLeft), DownRight(rhs.DownRight)
{
    // nothing to do
}
Rectangle2::~Rectangle2()
{
    // nothing to do
}

