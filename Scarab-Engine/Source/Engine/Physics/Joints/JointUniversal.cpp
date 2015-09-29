/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointUniversal.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Universal joint (often called Cardan)
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
#include "JointUniversal.h"

/////////////////////////////////////////////////////////////////////////////////
// JointUniversal implementation
JointUniversal::JointUniversal( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vJointFrame, EulerAngles iEulerAnglesOrdering ):
    JointGeneric6DOF( pBodyA, pBodyB, vJointFrame, iEulerAnglesOrdering )
{
    // Setup Universal joint
    EnableLimits( JOINTAXIS_LINEAR_0, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_1, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_2, 0.0f, 0.0f );

    EnableLimits( JOINTAXIS_ANGULAR_0, -SCALAR_PI_2 + SCALAR_EPSILON, SCALAR_PI_2 - SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_1, -SCALAR_PI_2 + SCALAR_EPSILON, SCALAR_PI_2 - SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_2, -SCALAR_EPSILON, SCALAR_EPSILON );

    _EvaluateJointState();
}
JointUniversal::JointUniversal( BodyNode * pBodyA, BodyNode * pBodyB, const Transform3 & vFrameInA, const Transform3 & vFrameInB, EulerAngles iEulerAnglesOrdering ):
    JointGeneric6DOF( pBodyA, pBodyB, vFrameInA, vFrameInB, iEulerAnglesOrdering )
{
    // Setup Universal joint
    EnableLimits( JOINTAXIS_LINEAR_0, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_1, 0.0f, 0.0f );
    EnableLimits( JOINTAXIS_LINEAR_2, 0.0f, 0.0f );

    EnableLimits( JOINTAXIS_ANGULAR_0, -SCALAR_PI_2 + SCALAR_EPSILON, SCALAR_PI_2 - SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_1, -SCALAR_PI_2 + SCALAR_EPSILON, SCALAR_PI_2 - SCALAR_EPSILON );
    EnableLimits( JOINTAXIS_ANGULAR_2, -SCALAR_EPSILON, SCALAR_EPSILON );

    _EvaluateJointState();
}
JointUniversal::~JointUniversal()
{
    // nothing to do
}

