/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Offset3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 3D displacement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Offset3 implementation
inline Offset3 & Offset3::operator=(const Offset3 & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    return (*this);
}

inline Offset3::operator Int*() const {
    return (Int*)Values;
}
inline Offset3::operator const Int*() const {
    return (const Int*)Values;
}

inline Int & Offset3::operator[](Int i) {
    return Values[i];
}
inline Int Offset3::operator[](Int i) const {
    return Values[i];
}
inline Int & Offset3::operator[](UInt i) {
    return Values[i];
}
inline Int Offset3::operator[](UInt i) const {
    return Values[i];
}

inline Offset3 Offset3::operator+() const {
    return Offset3(X, Y, Z);
}
inline Offset3 Offset3::operator-() const {
    return Offset3(-X, -Y, -Z);
}

inline Bool Offset3::operator==(const Offset3 & rhs) const {
    return ( X == rhs.X && Y == rhs.Y && Z == rhs.Z );
}
inline Bool Offset3::operator!=(const Offset3 & rhs) const {
    return ( X != rhs.X || Y != rhs.Y || Z != rhs.Z );
}

inline Offset3 Offset3::operator+(Int rhs) const {
    return Offset3(X + rhs, Y + rhs, Z + rhs);
}
inline Offset3 Offset3::operator-(Int rhs) const {
    return Offset3(X - rhs, Y - rhs, Z - rhs);
}
inline Offset3 Offset3::operator*(Int rhs) const {
    return Offset3(X * rhs, Y * rhs, Z * rhs);
}
inline Offset3 Offset3::operator/(Int rhs) const {
    return Offset3(X / rhs, Y / rhs, Z / rhs);
}

inline Offset3 & Offset3::operator+=(Int rhs) {
    X += rhs;
    Y += rhs;
    Z += rhs;
    return (*this);
}
inline Offset3 & Offset3::operator-=(Int rhs) {
    X -= rhs;
    Y -= rhs;
    Z -= rhs;
    return (*this);
}
inline Offset3 & Offset3::operator*=(Int rhs) {
    X *= rhs;
    Y *= rhs;
    Z *= rhs;
    return (*this);
}
inline Offset3 & Offset3::operator/=(Int rhs) {
    X /= rhs;
    Y /= rhs;
    Z /= rhs;
    return (*this);
}

inline Offset3 Offset3::operator+(const Offset3 & rhs) const {
    return Offset3(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}
inline Offset3 Offset3::operator-(const Offset3 & rhs) const {
    return Offset3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

inline Offset3 & Offset3::operator+=(const Offset3 & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    Z += rhs.Z;
    return (*this);
}
inline Offset3 & Offset3::operator-=(const Offset3 & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    Z -= rhs.Z;
    return (*this);
}

inline Int Offset3::operator*(const Offset3 & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) + (Z * rhs.Z) );
}
inline Offset3 Offset3::operator^(const Offset3 & rhs) const {
    return Offset3 (
        (Y * rhs.Z) - (Z * rhs.Y),
        (Z * rhs.X) - (X * rhs.Z),
        (X * rhs.Y) - (Y * rhs.X)
    );
}

inline Scalar Offset3::Norm() const {
    return MathFn->Sqrt( (Scalar)(X*X + Y*Y + Z*Z) );
}
inline Int Offset3::NormSqr() const {
    return (X*X + Y*Y + Z*Z);
}
inline Scalar Offset3::InvNorm() const {
    return MathFn->InvSqrt( (Scalar)(X*X + Y*Y + Z*Z) );
}
inline Scalar Offset3::InvNormSqr() const {
    return MathFn->Invert( (Scalar)(X*X + Y*Y + Z*Z) );
}
inline Scalar Offset3::Normalize() {
    Int iNormSqr = (X*X + Y*Y + Z*Z);
    if ( iNormSqr == 0 ) {
        X = 0;
        Y = 0;
        Z = 0;
        return 0;
    } else {
        Scalar fNorm = MathFn->Sqrt( (Scalar)iNormSqr );
        Scalar fInvNorm = MathFn->Invert( fNorm );
        X = MathFn->Floor( fInvNorm * (Scalar)X );
        Y = MathFn->Floor( fInvNorm * (Scalar)Y );
        Z = MathFn->Floor( fInvNorm * (Scalar)Z );
        return fNorm;
    }
}

inline Offset3 Offset3::ProjectToNormal(const Offset3 & vNormal) const {
    return ( vNormal * ( (*this) * vNormal ) );
}
inline Offset3 Offset3::ProjectToPlane(const Offset3 & vNormal) const {
    return ( (*this) - ProjectToNormal(vNormal) );
}



