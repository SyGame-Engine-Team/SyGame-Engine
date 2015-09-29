/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/ParticleSystem.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System : Particles
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ParticleSystem implementation
//inline SystemBodyType ParticleSystem::GetType() const {
//    return SYSTEMBODY_PARTICLES;
//}
//
//inline const Vector3 & ParticleSystem::GetLinearSpeed() const {
//    return m_vLinearSpeed;
//}
//inline const Vector3 & ParticleSystem::GetAngularSpeed() const {
//    return m_vAngularSpeed;
//}
//
//inline Void ParticleSystem::SetLinearSpeed( const Vector3 & vLinearSpeed ) {
//    m_vLinearSpeed = vLinearSpeed;
//}
//inline Void ParticleSystem::SetAngularSpeed( const Vector3 & vAngularSpeed ) {
//    m_vAngularSpeed = vAngularSpeed;
//}
//
//inline UInt ParticleSystem::GetParticleCount() const {
//    return m_arrParticles.Count();
//}
//
//inline UInt ParticleSystem::GetActiveParticleCount() const {
//    return m_iActiveParticleCount;
//}
//inline Void ParticleSystem::SetActiveParticleCount( UInt iActiveParticleCount ) {
//    Assert( iActiveParticleCount <= m_arrParticles.Count() );
//    m_iActiveParticleCount = iActiveParticleCount;
//}
//
//inline KinematicBody * ParticleSystem::GetParticleBody( UInt iParticle ) const {
//    Assert( iParticle < m_arrParticles.Count() );
//    return m_arrParticles[iParticle].pBody;
//}
//inline ManualController * ParticleSystem::GetParticleController( UInt iParticle ) const {
//    Assert( iParticle < m_arrParticles.Count() );
//    return m_arrParticles[iParticle].pController;
//}
//inline const Vector3 & ParticleSystem::GetParticleLinearSpeed( UInt iParticle ) const {
//    Assert( iParticle < m_arrParticles.Count() );
//    return m_arrParticles[iParticle].vLinearSpeed;
//}
//inline const Vector3 & ParticleSystem::GetParticleAngularSpeed( UInt iParticle ) const {
//    Assert( iParticle < m_arrParticles.Count() );
//    return m_arrParticles[iParticle].vAngularSpeed;
//}
//
//inline Void ParticleSystem::SetParticleLinearSpeed( UInt iParticle, const Vector3 & vLinearSpeed ) {
//    Assert( iParticle < m_arrParticles.Count() );
//    m_arrParticles[iParticle].vLinearSpeed = vLinearSpeed;
//}
//inline Void ParticleSystem::SetParticleAngularSpeed( UInt iParticle, const Vector3 & vAngularSpeed ) {
//    Assert( iParticle < m_arrParticles.Count() );
//    m_arrParticles[iParticle].vAngularSpeed = vAngularSpeed;
//}
//
///////////////////////////////////////////////////////////////////////////////////
//
//inline ParticleSystem::_Particle & ParticleSystem::_Particle::operator=( const _Particle & rhs ) {
//    pBody = rhs.pBody;
//    pController = rhs.pController;
//    vLinearSpeed = rhs.vLinearSpeed;
//    vAngularSpeed = rhs.vAngularSpeed;
//    return (*this);
//}
//
//inline Scalar ParticleSystem::_RandScalar_0_1() {
//    static const Scalar fInvRandMax = ( 1.0f / (Scalar)( RandFn->XorShiftMax() ) );
//    return ( fInvRandMax * (Scalar)(RandFn->XorShift128()) );
//}
//inline Scalar ParticleSystem::_RandScalar_M1_1() {
//    static const Scalar fInvRandMaxX2 = ( 2.0f / (Scalar)( RandFn->XorShiftMax() ) );
//    return ( fInvRandMaxX2 * (Scalar)(RandFn->XorShift128()) ) - 1.0f;
//}
//inline Scalar ParticleSystem::_RandScalar_Range( Scalar fMin, Scalar fMax ) {
//    return ( fMin + ( _RandScalar_0_1() * (fMax - fMin) ) );
//}

