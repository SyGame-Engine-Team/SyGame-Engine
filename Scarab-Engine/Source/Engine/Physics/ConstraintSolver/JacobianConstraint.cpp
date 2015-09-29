/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/ConstraintSolver/JacobianConstraint.cpp
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
// Includes
#include "JacobianConstraint.h"

/////////////////////////////////////////////////////////////////////////////////
// JacobianConstraint implementation
JacobianConstraint::JacobianConstraint():
    vJLinearAxis(), vJAngularA(), vJAngularB(),
    vInvIAxJtAngA(), vInvIBxJtAngB()
{
    fInvDiagJWJtAB = 1.0f;
}
JacobianConstraint::JacobianConstraint( const JacobianConstraint & rhs ):
    vJLinearAxis( rhs.vJLinearAxis ), vJAngularA( rhs.vJAngularA ), vJAngularB( rhs.vJAngularB ),
    vInvIAxJtAngA( rhs.vInvIAxJtAngA ), vInvIBxJtAngB( rhs.vInvIBxJtAngB )
{
    fInvDiagJWJtAB = rhs.fInvDiagJWJtAB;
}
JacobianConstraint::~JacobianConstraint()
{
    // nothing to do
}

Void JacobianConstraint::MakeLinear( const Vector3 & vAxis, const Vector3 & vAnchorA, const Vector3 & vAnchorB,
                                     Scalar fInvMassA, const Matrix3 & matInvInertiaTensorA,
                                     Scalar fInvMassB, const Matrix3 & matInvInertiaTensorB )
{
    vJLinearAxis = vAxis;

    vJAngularA = ( vAnchorA ^ vAxis );
    vJAngularB = -( vAnchorB ^ vAxis );

    vInvIAxJtAngA = ( matInvInertiaTensorA * vJAngularA );
    vInvIBxJtAngB = ( matInvInertiaTensorB * vJAngularB );

    fInvDiagJWJtAB = ( fInvMassA + fInvMassB );
    fInvDiagJWJtAB += ( vInvIAxJtAngA * vJAngularA );
    fInvDiagJWJtAB += ( vInvIBxJtAngB * vJAngularB );

    // Should be PSD matrices
    Assert( fInvDiagJWJtAB > 0.0f );
    fInvDiagJWJtAB = JACOBIANCONSTRAINT_RELAXATION / fInvDiagJWJtAB;
}
Void JacobianConstraint::MakeAngular( const Vector3 & vAxis, const Matrix3 & matInvInertiaTensorA,
                                                             const Matrix3 & matInvInertiaTensorB )
{
    vJLinearAxis = Vector3::Null;

    vJAngularA = vAxis;
    vJAngularB = -vAxis;

    vInvIAxJtAngA = ( matInvInertiaTensorA * vJAngularA );
    vInvIBxJtAngB = ( matInvInertiaTensorB * vJAngularB );

    fInvDiagJWJtAB = 0.0f;
    fInvDiagJWJtAB += ( vInvIAxJtAngA * vJAngularA );
    fInvDiagJWJtAB += ( vInvIBxJtAngB * vJAngularB );

    // Should be PSD matrices
    Assert( fInvDiagJWJtAB > 0.0f );
    fInvDiagJWJtAB = MathFn->Invert( fInvDiagJWJtAB );
}
Void JacobianConstraint::MakeAngular( const Vector3 & vAxisA, const Vector3 & vAxisB, const Matrix3 & matInvInertiaTensorA,
                                                                                      const Matrix3 & matInvInertiaTensorB )
{
    vJLinearAxis = Vector3::Null;

    vJAngularA = vAxisA;
    vJAngularB = vAxisB;

    vInvIAxJtAngA = ( matInvInertiaTensorA * vJAngularA );
    vInvIBxJtAngB = ( matInvInertiaTensorB * vJAngularB );

    fInvDiagJWJtAB = 0.0f;
    fInvDiagJWJtAB += ( vInvIAxJtAngA * vJAngularA );
    fInvDiagJWJtAB += ( vInvIBxJtAngB * vJAngularB );

    // Should be PSD matrices
    Assert( fInvDiagJWJtAB > 0.0f );
    fInvDiagJWJtAB = MathFn->Invert( fInvDiagJWJtAB );
}


