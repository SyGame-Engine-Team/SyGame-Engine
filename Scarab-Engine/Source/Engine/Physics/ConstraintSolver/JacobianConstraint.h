/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/JacobianConstraint.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Jacobian rows generator for constraint modeling ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_JACOBIANCONSTRAINT_H
#define SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_JACOBIANCONSTRAINT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Vector/Vector3.h"
#include "../../../Lib/Math/Types/Vertex/Vertex3.h"
#include "../../../Lib/Math/Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define JACOBIANCONSTRAINT_RELAXATION 1.0f

/////////////////////////////////////////////////////////////////////////////////
// The JacobianConstraint class
class JacobianConstraint
{
public:
    JacobianConstraint();
    JacobianConstraint( const JacobianConstraint & rhs );
    virtual ~JacobianConstraint();

    // operators
    inline JacobianConstraint & operator=( const JacobianConstraint & rhs );

    // Relative velocity (J.u)
    inline Scalar GetRelativeVelocity( const Vector3 & vLinSpeedA, const Vector3 & vAngSpeedA,
                                       const Vector3 & vLinSpeedB, const Vector3 & vAngSpeedB ) const;

    // Non-Diagonal Term accross 2 constraints on same bodies A & B
    // Beware, this is NOT a symmetric operation ...
    //inline static Scalar GetNonDiagonalTerm( const JacobianConstraint & jac0, const JacobianConstraint & jac1,
    //                                         Scalar fInvMassA, Scalar fInvMassB );

    // Constraint builders ( Axes MUST be normalized )
    Void MakeLinear( const Vector3 & vAxis, const Vector3 & vAnchorA, const Vector3 & vAnchorB,
                     Scalar fInvMassA, const Matrix3 & matInvInertiaTensorA,
                     Scalar fInvMassB, const Matrix3 & matInvInertiaTensorB );
    Void MakeAngular( const Vector3 & vAxis, const Matrix3 & matInvInertiaTensorA,
                                             const Matrix3 & matInvInertiaTensorB );
    Void MakeAngular( const Vector3 & vAxisA, const Vector3 & vAxisB, const Matrix3 & matInvInertiaTensorA,
                                                                      const Matrix3 & matInvInertiaTensorB );

    // Linear part
    Vector3 vJLinearAxis; // JLa = +vJLinearAxis
                          // JLb = -vJLinearAxis

    // Angular part
    Vector3 vJAngularA; // JAa
    Vector3 vJAngularB; // JAb

    // (W*Jt) column terms
    // By definition, JLa . ( (invMa*Id) * JtLa ) = invMa * ( JLa . JtLa ) = invMa
    //                and the same goes for B => linear diag term is simply ( invMa + invMb )
    Vector3 vInvIAxJtAngA; // invIa * JtAa
    Vector3 vInvIBxJtAngB; // invIb * JtAb

    // (J*W*Jt) diagonal term
    Scalar fInvDiagJWJtAB; // = Relaxation / ( invMa + invMb + (JLb . invMbJLb) + (JAb . invMbJAb) )
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "JacobianConstraint.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONSTRAINTSOLVER_JACOBIANCONSTRAINT_H

