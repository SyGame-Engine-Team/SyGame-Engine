/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Point3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 3D position
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Point3 implementation
inline Point3 & Point3::operator=(const Point3 & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    return (*this);
}

inline Point3::operator Int*() const {
    return (Int*)Values;
}
inline Point3::operator const Int*() const {
    return (const Int*)Values;
}

inline Int & Point3::operator[](Int i) {
    return Values[i];
}
inline Int Point3::operator[](Int i) const {
    return Values[i];
}
inline Int & Point3::operator[](UInt i) {
    return Values[i];
}
inline Int Point3::operator[](UInt i) const {
    return Values[i];
}

inline Point3 Point3::operator+() const {
    return Point3(X, Y, Z);
}
inline Point3 Point3::operator-() const {
    return Point3(-X, -Y, -Z);
}

inline Bool Point3::operator==(const Point3 & rhs) const {
    return ( X == rhs.X && Y == rhs.Y && Z == rhs.Z );
}
inline Bool Point3::operator!=(const Point3 & rhs) const {
    return ( X != rhs.X || Y != rhs.Y || Z != rhs.Z );
}

inline Point3 Point3::operator+(Int rhs) const {
    return Point3(X + rhs, Y + rhs, Z + rhs);
}
inline Point3 Point3::operator-(Int rhs) const {
    return Point3(X - rhs, Y - rhs, Z - rhs);
}
inline Point3 Point3::operator*(Int rhs) const {
    return Point3(X * rhs, Y * rhs, Z * rhs);
}
inline Point3 Point3::operator/(Int rhs) const {
    return Point3(X / rhs, Y / rhs, Z / rhs);
}

inline Point3 & Point3::operator+=(Int rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
inline Point3 & Point3::operator-=(Int rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
inline Point3 & Point3::operator*=(Int rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
inline Point3 & Point3::operator/=(Int rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}

inline Offset3 Point3::operator-(const Point3 & rhs) const {
    return Offset3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

inline Point3 Point3::operator+(const Offset3 & rhs) const {
    return Point3(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}
inline Point3 Point3::operator-(const Offset3 & rhs) const {
    return Point3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

inline Point3 & Point3::operator+=(const Offset3 & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    return (*this);
}
inline Point3 & Point3::operator-=(const Offset3 & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    return (*this);
}

inline Scalar Point3::Dist() const {
    return MathFn->Sqrt( (Scalar)(X*X + Y*Y + Z*Z) );
}
inline Int Point3::DistSqr() const {
    return (X*X + Y*Y + Z*Z);
}
inline Scalar Point3::InvDist() const {
    return MathFn->InvSqrt( (Scalar)(X*X + Y*Y + Z*Z) );
}
inline Scalar Point3::InvDistSqr() const {
    return MathFn->Invert( (Scalar)(X*X + Y*Y + Z*Z) );
}

