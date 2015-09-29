/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Offset2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 2D displacement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Offset2 implementation
inline Offset2 & Offset2::operator=(const Offset2 & rhs) {
    X = rhs.X;
    Y = rhs.Y;
    return (*this);
}

inline Offset2::operator Int*() const {
    return (Int*)Values;
}
inline Offset2::operator const Int*() const {
    return (const Int*)Values;
}

inline Int & Offset2::operator[](Int i) {
    return Values[i];
}
inline Int Offset2::operator[](Int i) const {
    return Values[i];
}
inline Int & Offset2::operator[](UInt i) {
    return Values[i];
}
inline Int Offset2::operator[](UInt i) const {
    return Values[i];
}

inline Offset2 Offset2::operator+() const {
    return Offset2(X, Y);
}
inline Offset2 Offset2::operator-() const {
    return Offset2(-X, -Y);
}

inline Bool Offset2::operator==(const Offset2 & rhs) const {
    return ( X == rhs.X && Y == rhs.Y );
}
inline Bool Offset2::operator!=(const Offset2 & rhs) const {
    return ( X != rhs.X || Y != rhs.Y );
}

inline Offset2 Offset2::operator+(Int rhs) const {
    return Offset2(X + rhs, Y + rhs);
}
inline Offset2 Offset2::operator-(Int rhs) const {
    return Offset2(X - rhs, Y - rhs);
}
inline Offset2 Offset2::operator*(Int rhs) const {
    return Offset2(X * rhs, Y * rhs);
}
inline Offset2 Offset2::operator/(Int rhs) const {
    return Offset2(X / rhs, Y / rhs);
}

inline Offset2 & Offset2::operator+=(Int rhs) {
    X += rhs;
    Y += rhs;
    return (*this);
}
inline Offset2 & Offset2::operator-=(Int rhs) {
    X -= rhs;
    Y -= rhs;
    return (*this);
}
inline Offset2 & Offset2::operator*=(Int rhs) {
    X *= rhs;
    Y *= rhs;
    return (*this);
}
inline Offset2 & Offset2::operator/=(Int rhs) {
    X /= rhs;
    Y /= rhs;
    return (*this);
}

inline Offset2 Offset2::operator+(const Offset2 & rhs) const {
    return Offset2(X + rhs.X, Y + rhs.Y);
}
inline Offset2 Offset2::operator-(const Offset2 & rhs) const {
    return Offset2(X - rhs.X, Y - rhs.Y);
}

inline Offset2 & Offset2::operator+=(const Offset2 & rhs) {
    X += rhs.X;
    Y += rhs.Y;
    return (*this);
}
inline Offset2 & Offset2::operator-=(const Offset2 & rhs) {
    X -= rhs.X;
    Y -= rhs.Y;
    return (*this);
}

inline Int Offset2::operator*(const Offset2 & rhs) const {
    return ( (X * rhs.X) + (Y * rhs.Y) );
}

inline Scalar Offset2::Norm() const {
    return MathFn->Sqrt( (Scalar)(X*X + Y*Y) );
}
inline Int Offset2::NormSqr() const {
    return (X*X + Y*Y);
}
inline Scalar Offset2::InvNorm() const {
    return MathFn->InvSqrt( (Scalar)(X*X + Y*Y) );
}
inline Scalar Offset2::InvNormSqr() const {
    return MathFn->Invert( (Scalar)(X*X + Y*Y) );
}
inline Scalar Offset2::Normalize() {
    Int iNormSqr = (X*X + Y*Y);
    if ( iNormSqr == 0 ) {
        X = 0;
        Y = 0;
        return 0;
    } else {
        Scalar fNorm = MathFn->Sqrt( (Scalar)iNormSqr );
        Scalar fInvNorm = MathFn->Invert( fNorm );
        X = MathFn->Round( fInvNorm * (Scalar)X );
        Y = MathFn->Round( fInvNorm * (Scalar)Y );
        return fNorm;
    }
}

inline Offset2 Offset2::Perp() const {
    return Offset2(Y, -X);
}

inline Offset2 Offset2::ProjectToNormal(const Offset2 & vNormal) const {
    return ( vNormal * ( (*this) * vNormal ) );
}
inline Offset2 Offset2::ProjectToPlane(const Offset2 & vNormal) const {
    return ( (*this) - ProjectToNormal(vNormal) );
}

