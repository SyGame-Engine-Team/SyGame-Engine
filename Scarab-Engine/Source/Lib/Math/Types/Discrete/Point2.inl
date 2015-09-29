/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Point2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 2D position
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Point2 implementation
inline Point2 & Point2::operator=(const Point2 & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    return (*this);
}

inline Point2::operator Int*() const {
    return (Int*)Values;
}
inline Point2::operator const Int*() const {
    return (const Int*)Values;
}

inline Int & Point2::operator[](Int i) {
    return Values[i];
}
inline Int Point2::operator[](Int i) const {
    return Values[i];
}
inline Int & Point2::operator[](UInt i) {
    return Values[i];
}
inline Int Point2::operator[](UInt i) const {
    return Values[i];
}

inline Point2 Point2::operator+() const {
    return Point2(X, Y);
}
inline Point2 Point2::operator-() const {
    return Point2(-X, -Y);
}

inline Bool Point2::operator==(const Point2 & rhs) const {
    return ( X == rhs.X && Y == rhs.Y );
}
inline Bool Point2::operator!=(const Point2 & rhs) const {
    return ( X != rhs.X || Y != rhs.Y );
}

inline Point2 Point2::operator+(Int rhs) const {
    return Point2(X + rhs, Y + rhs);
}
inline Point2 Point2::operator-(Int rhs) const {
    return Point2(X - rhs, Y - rhs);
}
inline Point2 Point2::operator*(Int rhs) const {
    return Point2(X * rhs, Y * rhs);
}
inline Point2 Point2::operator/(Int rhs) const {
    return Point2(X / rhs, Y / rhs);
}

inline Point2 & Point2::operator+=(Int rhs) {
    X += rhs;
    Y += rhs;
    return (*this);
}
inline Point2 & Point2::operator-=(Int rhs) {
    X -= rhs;
    Y -= rhs;
    return (*this);
}
inline Point2 & Point2::operator*=(Int rhs) {
    X *= rhs;
    Y *= rhs;
    return (*this);
}
inline Point2 & Point2::operator/=(Int rhs) {
    X /= rhs;
    Y /= rhs;
    return (*this);
}

inline Offset2 Point2::operator-(const Point2 & rhs) const {
    return Offset2(X - rhs.X, Y - rhs.Y);
}

inline Point2 Point2::operator+(const Offset2 & rhs) const {
    return Point2(X + rhs.X, Y + rhs.Y);
}
inline Point2 Point2::operator-(const Offset2 & rhs) const {
    return Point2(X - rhs.X, Y - rhs.Y);
}

inline Point2 & Point2::operator+=(const Offset2 & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    return (*this);
}
inline Point2 & Point2::operator-=(const Offset2 & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    return (*this);
}

inline Scalar Point2::Dist() const {
    return MathFn->Sqrt( (Scalar)(X*X + Y*Y) );
}
inline Int Point2::DistSqr() const {
    return (X*X + Y*Y);
}
inline Scalar Point2::InvDist() const {
    return MathFn->InvSqrt( (Scalar)(X*X + Y*Y) );
}
inline Scalar Point2::InvDistSqr() const {
    return MathFn->Invert( (Scalar)(X*X + Y*Y) );
}

