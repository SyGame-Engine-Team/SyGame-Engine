/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/KinematicBody.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Kinematic bodies
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
#ifndef SCARAB_ENGINE_PHYSICS_BODIES_KINEMATICBODY_H
#define SCARAB_ENGINE_PHYSICS_BODIES_KINEMATICBODY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../IslandSolver/Nodes/BodyNode.h"

#include "../Controllers/KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The KinematicBody class
class KinematicBody : public BodyNode
{
public:
    KinematicBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                   const Vertex3 & vPosition, const Quaternion & qOrientation,
                   const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~KinematicBody();

    // Type
    inline virtual BodyType GetBodyType() const;

    // Behaviour
    inline virtual Bool IsInsulator() const;

    // Impulse access
    inline virtual Void ApplyLinearImpulse( const Vector3 & vImpulse );
	inline virtual Void ApplyAngularImpulse( const Vector3 & vImpulse );
    inline virtual Void ApplyImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint );
    inline virtual Void ApplyImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint );

    inline virtual Void ApplyAccumulatedImpulses();

    // Correction access
    inline virtual Void ApplyLinearCorrection( const Vector3 & vCorrection, Scalar fTimeStep );
	inline virtual Void ApplyAngularCorrection( const Vector3 & vCorrection, Scalar fTimeStep );
    inline virtual Void ApplyCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint, Scalar fTimeStep );
    inline virtual Void ApplyCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint, Scalar fTimeStep );

    inline virtual Void ApplyAccumulatedCorrections( Scalar fTimeStep );

    // Integration
    virtual Void IntegrateVelocity( Scalar fTimeStep );
    virtual Void IntegratePosition( Scalar fTimeStep );

    // Kinematic controllers
    inline ShapeController * GetShapeController() const;
    inline MotionController * GetMotionController() const;

    inline Void AttachController( KinematicController * pController );
    inline ShapeController * DetachShapeController();
    inline MotionController * DetachMotionController();

private:
    // Kinematic controllers
    Bool m_bControllerShapeUpdate;
    ShapeController * m_pShapeController;

    Bool m_bControllerMotionUpdate;
    MotionController * m_pMotionController;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "KinematicBody.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_BODIES_KINEMATICBODY_H

