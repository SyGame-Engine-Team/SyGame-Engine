/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointWheel.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Wheel joint (in reference to car wheels)
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
#include "JointWheel.h"

/////////////////////////////////////////////////////////////////////////////////
// JointWheel implementation
JointWheel::JointWheel( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame, EulerAngles iEulerAnglesOrdering ):
    JointGeneric6DOF( pBodyA, pBodyB, vJointFrame, iEulerAnglesOrdering )
{
    // Setup Wheel joint
    EnableLimits( JOINTAXIS_LINEAR_0, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_1, 0.0f, 0.0f );
    EnableSpring( JOINTAXIS_LINEAR_2, 0.5f, 0.0f );

    EnableLimits( JOINTAXIS_ANGULAR_1, -SCALAR_EPSILON, SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_2, -SCALAR_EPSILON, SCALAR_EPSILON );

    _EvaluateJointState();
}
JointWheel::JointWheel( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB, EulerAngles iEulerAnglesOrdering ):
    JointGeneric6DOF( pBodyA, pBodyB, vFrameInA, vFrameInB, iEulerAnglesOrdering )
{
    // Setup Wheel joint
    EnableLimits( JOINTAXIS_LINEAR_0, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_1, 0.0f, 0.0f );
    EnableSpring( JOINTAXIS_LINEAR_2, 0.5f, 0.0f );

    EnableLimits( JOINTAXIS_ANGULAR_1, -SCALAR_EPSILON, SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_2, -SCALAR_EPSILON, SCALAR_EPSILON );

    _EvaluateJointState();
}
JointWheel::~JointWheel()
{
    // nothing to do
}

