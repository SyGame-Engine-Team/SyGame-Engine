/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Rectangle2.inl
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
// Rectangle2 implementation
inline Rectangle2 & Rectangle2::operator=(const Rectangle2 & rhs) {
	TopLeft = rhs.TopLeft;
	DownRight = rhs.DownRight;
	return (*this);
}

inline Bool Rectangle2::operator==(const Rectangle2 & rhs) const {
	return (TopLeft == rhs.TopLeft && DownRight == rhs.DownRight);
}
inline Bool Rectangle2::operator!=(const Rectangle2 & rhs) const {
	return (TopLeft != rhs.TopLeft || DownRight != rhs.DownRight);
}

inline UInt Rectangle2::Width() const {
    return (DownRight.X + 1 - TopLeft.X);
}
inline UInt Rectangle2::Height() const {
    return (DownRight.Y + 1 - TopLeft.Y);
}
inline Bool Rectangle2::Contains(const Point2 & vPt) const {
	return ( vPt.X >= TopLeft.X && vPt.Y >= TopLeft.Y &&
			 vPt.X <= DownRight.X && vPt.Y <= DownRight.Y );
}