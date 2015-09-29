/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/JacobianConstraint.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Constraint modeling using Jacobian rows  ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JacobianConstraint implementation
inline JacobianConstraint & JacobianConstraint::operator=( const JacobianConstraint & rhs ) {
    vJLinearAxis = rhs.vJLinearAxis;
    vJAngularA = rhs.vJAngularA;
    vJAngularB = rhs.vJAngularB;
    vInvIAxJtAngA = rhs.vInvIAxJtAngA;
    vInvIBxJtAngB = rhs.vInvIBxJtAngB;
    fInvDiagJWJtAB = rhs.fInvDiagJWJtAB;
    return (*this);
}

inline Scalar JacobianConstraint::GetRelativeVelocity( const Vector3 & vLinSpeedA, const Vector3 & vAngSpeedA,
                                                       const Vector3 & vLinSpeedB, const Vector3 & vAngSpeedB ) const {
    Scalar fRelativeVelocity = ( vJLinearAxis * (vLinSpeedA - vLinSpeedB) );
    fRelativeVelocity += ( vJAngularA * vAngSpeedA );
    fRelativeVelocity += ( vJAngularB * vAngSpeedB );
    return fRelativeVelocity;
}

//inline Scalar JacobianConstraint::GetNonDiagonalTerm( const JacobianConstraint & jac0, const JacobianConstraint & jac1,
//                                                      Scalar fInvMassA, Scalar fInvMassB )
//{
//    Scalar fDot = ( jac0.vJLinearAxis * jac1.vJLinearAxis );
//    Scalar fAngularTermA = ( jac0.vJAngularA * jac1.vInvIAxJtAngA );
//    Scalar fAngularTermB = ( jac0.vJAngularB * jac1.vInvIBxJtAngB );
//    return ( ((fInvMassA + fInvMassB) * fDot) + fAngularTermA + fAngularTermB );
//}
