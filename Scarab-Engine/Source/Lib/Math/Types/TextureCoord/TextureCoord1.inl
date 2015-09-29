/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord1.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 1D texture coordinates
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTextureCoord1 implementation
template<typename Real>
const TTextureCoord1<Real> TTextureCoord1<Real>::Null = TTextureCoord1<Real>( (Real)0 );

template<typename Real>
TTextureCoord1<Real>::TTextureCoord1()
{
    // nothing to do
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(Real t)
{
    T = t;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TVector2<Real> & rhs)
{
    T = rhs.X;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TVector3<Real> & rhs)
{
    T = rhs.X;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TVector4<Real> & rhs)
{
    T = rhs.X;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TTextureCoord1<Real> & rhs)
{
    T = rhs.T;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TTextureCoord2<Real> & rhs)
{
    T = rhs.T;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TTextureCoord3<Real> & rhs)
{
    T = rhs.T;
}
template<typename Real>
TTextureCoord1<Real>::TTextureCoord1(const TTextureCoord4<Real> & rhs)
{
    T = rhs.T;
}
template<typename Real>
TTextureCoord1<Real>::~TTextureCoord1()
{
    // nothing to do
}

template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator=(const TTextureCoord1<Real> & rhs) {
    T = rhs.T;
    return (*this);
}

template<typename Real>
inline TTextureCoord1<Real>::operator Real*() const {
    return (Real*)this;
}
template<typename Real>
inline TTextureCoord1<Real>::operator const Real*() const {
    return (const Real*)this;
}

template<typename Real>
inline Real & TTextureCoord1<Real>::operator[](Int i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord1<Real>::operator[](Int i) const {
    return *( ((const Real*)this) + i );
}
template<typename Real>
inline Real & TTextureCoord1<Real>::operator[](UInt i) {
    return *( ((Real*)this) + i );
}
template<typename Real>
inline Real TTextureCoord1<Real>::operator[](UInt i) const {
    return *( ((const Real*)this) + i );
}

template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator+() const {
    return TTextureCoord1<Real>(T);
}
template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator-() const {
    return TTextureCoord1<Real>(-T);
}

template<typename Real>
inline Bool TTextureCoord1<Real>::operator==(const TTextureCoord1<Real> & rhs) const {
    return ( MathRealFn->Equals(T,rhs.T) );
}
template<typename Real>
inline Bool TTextureCoord1<Real>::operator!=(const TTextureCoord1<Real> & rhs) const {
    return ( !(MathRealFn->Equals(T,rhs.T)) );
}

template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator+(Real rhs) const {
    return TTextureCoord1<Real>( T + rhs );
}
template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator-(Real rhs) const {
    return TTextureCoord1<Real>( T - rhs );
}
template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator*(Real rhs) const {
    return TTextureCoord1<Real>( T * rhs );
}
template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator/(Real rhs) const {
    return TTextureCoord1<Real>( T / rhs );
}

template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator+=(Real rhs) {
    T += rhs;
    return (*this);
}
template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator-=(Real rhs) {
    T -= rhs;
    return (*this);
}
template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator*=(Real rhs) {
    T *= rhs;
    return (*this);
}
template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator/=(Real rhs) {
    T /= rhs;
    return (*this);
}

template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator+(const TTextureCoord1<Real> & rhs) const {
    return TTextureCoord1<Real>( T + rhs.T );
}
template<typename Real>
inline TTextureCoord1<Real> TTextureCoord1<Real>::operator-(const TTextureCoord1<Real> & rhs) const {
    return TTextureCoord1<Real>( T - rhs.T );
}

template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator+=(const TTextureCoord1<Real> & rhs) {
    T += rhs.T;
    return (*this);
}
template<typename Real>
inline TTextureCoord1<Real> & TTextureCoord1<Real>::operator-=(const TTextureCoord1<Real> & rhs) {
    T -= rhs.T;
    return (*this);
}

template<typename Real>
inline Real TTextureCoord1<Real>::operator*(const TTextureCoord1<Real> & rhs) const {
    return ( T * rhs.T );
}

