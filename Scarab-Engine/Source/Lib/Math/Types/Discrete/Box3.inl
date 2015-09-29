/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Box3.inl
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
// Box3 implementation
inline Box3 & Box3::operator=(const Box3 & rhs) {
	LeftFrontTop = rhs.LeftFrontTop;
	RightBackDown = rhs.RightBackDown;
	return (*this);
}

inline Bool Box3::operator==(const Box3 & rhs) const {
	return (LeftFrontTop == rhs.LeftFrontTop && RightBackDown == rhs.RightBackDown);
}
inline Bool Box3::operator!=(const Box3 & rhs) const {
	return (LeftFrontTop != rhs.LeftFrontTop || RightBackDown != rhs.RightBackDown);
}

inline UInt Box3::Width() const {
    return (RightBackDown.X + 1 - LeftFrontTop.X);
}
inline UInt Box3::Height() const {
    return (RightBackDown.Y + 1 - LeftFrontTop.Y);
}
inline UInt Box3::Depth() const {
    return (RightBackDown.Z + 1 - LeftFrontTop.Z);
}
inline Bool Box3::Contains(const Point3 & vPt) const {
	return ( vPt.X >= LeftFrontTop.X && vPt.Y >= LeftFrontTop.Y && vPt.Z >= LeftFrontTop.Z &&
			 vPt.X <= RightBackDown.X && vPt.Y <= RightBackDown.Y && vPt.Z <= RightBackDown.Z );
}