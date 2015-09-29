/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/ConstraintSolver.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Iterative LCP solver using PGS/SOR-PGS with sequential impulses
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ConstraintSolver implementation
inline Void ConstraintSolver::SetIterations( UInt iIterations ) {
    m_iIterations = iIterations;
}

inline Void ConstraintSolver::UseSOR( Bool bUseSOR ) {
    m_bUseSOR = bUseSOR;
}
inline Void ConstraintSolver::SetSORPeriod( UInt iPeriod ) {
    m_iSORPeriod = iPeriod;
}

inline Void ConstraintSolver::UseCorrection( Bool bUseCorrection ) {
    m_bUseCorrection = bUseCorrection;
}
inline Void ConstraintSolver::SetCorrectionIterations( UInt iIterations ) {
    m_iCorrectionIterations = iIterations;
}
inline Void ConstraintSolver::SetCorrectionThreshold( Scalar fThreshold ) {
    m_fCorrectionThreshold = fThreshold;
}
inline Void ConstraintSolver::SetCorrectionERP( Scalar fERP ) {
    m_fCorrectionERP = fERP;
}

inline Void ConstraintSolver::UseWarmStart( Bool bUseWarmStart ) {
    m_bUseWarmStart = bUseWarmStart;
}
inline Void ConstraintSolver::SetWarmStartFactor( Scalar fFactor ) {
    m_fWarmStartFactor = fFactor;
}

inline Void ConstraintSolver::SetContactProcessThreshold( Scalar fThreshold ) {
    m_fContactProcessThreshold = fThreshold;
}
inline Void ConstraintSolver::SetContactRestingThreshold( UInt iThreshold ) {
    m_iContactRestingThreshold = iThreshold;
}

inline Void ConstraintSolver::SetContactNormalSlop( Scalar fSlop ) {
    m_fContactNormalSlop = fSlop;
}
inline Void ConstraintSolver::SetContactNormalERP( Scalar fERP ) {
    m_fContactNormalERP = fERP;
}
inline Void ConstraintSolver::SetContactNormalCFM( Scalar fCFM ) {
    m_fContactNormalCFM = fCFM;
}
inline Void ConstraintSolver::SetContactFrictionCFM( Scalar fCFM ) {
    m_fContactFrictionCFM = fCFM;
}
inline Void ConstraintSolver::SetContactRollingFrictionCFM( Scalar fCFM ) {
    m_fContactRollingFrictionCFM = fCFM;
}
inline Void ConstraintSolver::SetContactFrictionMAX( Scalar fMAX ) {
    m_fContactFrictionMAX = fMAX;
}

inline Void ConstraintSolver::UseVelocityDependantFrictionFrame( Bool bUseVelocityDependantFrictionFrame ) {
    m_bUseVelocityDependantFrictionFrame = bUseVelocityDependantFrictionFrame;
}
inline Void ConstraintSolver::SetSingleAxisRollingFrictionThreshold( Scalar fSingleAxisRollingFrictionThreshold ) {
    m_fSingleAxisRollingFrictionThreshold = fSingleAxisRollingFrictionThreshold;
}

inline Void ConstraintSolver::SetJointDefaultERP( Scalar fERP ) {
    m_fJointDefaultERP = fERP;
}
inline Void ConstraintSolver::SetJointDefaultCFM( Scalar fCFM ) {
    m_fJointDefaultCFM = fCFM;
}

/////////////////////////////////////////////////////////////////////////////////

inline Scalar ConstraintSolver::_ComputeMixedRestitution( Scalar fRestitutionA, Scalar fRestitutionB ) const {
    return ( fRestitutionA * fRestitutionB );
}
inline Scalar ConstraintSolver::_ComputeMixedFriction( Scalar fFrictionA, Scalar fFrictionB ) const {
    return Clamp<Scalar>( (fFrictionA * fFrictionB), -m_fContactFrictionMAX, +m_fContactFrictionMAX );
}

