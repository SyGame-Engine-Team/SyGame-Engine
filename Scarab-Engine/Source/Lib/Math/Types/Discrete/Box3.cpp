/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Box3.cpp
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
#include "Box3.h"

/////////////////////////////////////////////////////////////////////////////////
// Box3 implementation
Box3::Box3():
    LeftFrontTop(), RightBackDown()
{
    // nothing to do
}
Box3::Box3(Int front, Int left, Int top, Int back, Int right, Int down):
    LeftFrontTop(left, front, top), RightBackDown(right, back, down)
{
    // nothing to do
}
Box3::Box3(const Int vLeftFrontTop[3], const Int vRightBackDown[3]):
    LeftFrontTop(vLeftFrontTop), RightBackDown(vRightBackDown)
{
    // nothing to do
}
Box3::Box3(const Point3 & vLeftFrontTop, UInt width, UInt height, UInt depth):
    LeftFrontTop(vLeftFrontTop), RightBackDown(vLeftFrontTop.X + width - 1, vLeftFrontTop.Y + height - 1, vLeftFrontTop.Z + depth - 1)
{
    // nothing to do
}
Box3::Box3(const Point3 & vLeftFrontTop, const Point3 & vRightBackDown):
	LeftFrontTop(vLeftFrontTop), RightBackDown(vRightBackDown)
{
    // nothing to do
}
Box3::Box3(const Box3 & rhs):
	LeftFrontTop(rhs.LeftFrontTop), RightBackDown(rhs.RightBackDown)
{
    // nothing to do
}
Box3::~Box3()
{
    // nothing to do
}

