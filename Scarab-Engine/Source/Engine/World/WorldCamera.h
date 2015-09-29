/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCamera.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Camera management
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
#ifndef SCARAB_ENGINE_WORLD_WORLDCAMERA_H
#define SCARAB_ENGINE_WORLD_WORLDCAMERA_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Rendering/Resources/Camera.h"

#include "../Physics/Controllers/Motion/ManualController.h"

    // Prototypes
class WorldBaseNode;

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Camera types
enum WorldCameraType
{
    WORLDCAMERA_SUBJECTIVE = 0,
    WORLDCAMERA_3RDPERSON
};

#define WORLDCAMERA_MIN_RADIUS 0.1f
#define WORLDCAMERA_MAX_RADIUS 100.0f

/////////////////////////////////////////////////////////////////////////////////
// The WorldCamera class
class WorldCamera
{
public:
    WorldCamera( Camera * pCamera, WorldBaseNode * pTarget );
    virtual ~WorldCamera();

    // Type
    virtual WorldCameraType GetCameraType() const = 0;

    // Camera access
    inline Camera * GetCamera() const;

    // Target access
    inline WorldBaseNode * GetTarget() const;

    // Update support
    virtual Void Update( Scalar fTimeStep ) = 0;

protected:
    Camera * m_pCamera;
    WorldBaseNode * m_pTarget;
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldCameraSubjective class
class WorldCameraSubjective : public WorldCamera
{
public:
    WorldCameraSubjective( Camera * pCamera, WorldBaseNode * pTarget );
    virtual ~WorldCameraSubjective();

    // Type
    inline virtual WorldCameraType GetCameraType() const;

    // Update support
    virtual Void Update( Scalar fTimeStep );
};

/////////////////////////////////////////////////////////////////////////////////
// The WorldCamera3rdPerson class
class WorldCamera3rdPerson : public WorldCamera
{
public:
    WorldCamera3rdPerson( Camera * pCamera, WorldBaseNode * pTarget, ManualController * pCameraController,
                          Scalar fRadius = 1.0f, Scalar fAngleH = 0.0f, Scalar fAngleV = SCALAR_PI_4,
                          Axis iUpAxis = AXIS_Z );
    virtual ~WorldCamera3rdPerson();

    // Type
    inline virtual WorldCameraType GetCameraType() const;

    // Controller access
    inline ManualController * GetController() const;

    // Radius bounds
    inline Scalar GetMinRadius() const;
    inline Void SetMinRadius( Scalar fMinRadius );
    inline Scalar GetMaxRadius() const;
    inline Void SetMaxRadius( Scalar fMaxRadius );

    // Camera movement
    inline Void Zoom( Scalar fDistance );
    inline Void RotateH( Scalar fAngle );
    Void RotateV( Scalar fAngle );

    // Update support
    virtual Void Update( Scalar fTimeStep );

protected:
    ManualController * m_pCameraController; // Camera's body's controller

    Axis m_iUpAxis; Vector3 m_vUpAxis; // Reference up axis
    Scalar m_fMinRadius, m_fMaxRadius; // Min/Max camera distance from target

    Scalar m_fRadius, m_fAngleH, m_fAngleV; // Spherical coordinates of the camera
    Scalar m_fInvAngleV, m_fTargetHeight;   // "Look at tangent" case
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldCamera.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDCAMERA_H

//// Align with Camera
//if ( m_pCamera != NULL ) {
//    Matrix4 matTemp;
//    WorldTransform.Invert( matTemp );
//    Vertex4 vModelPos = ( matTemp * m_pCamera->GetViewEye() );

//    // Align the billboard, Axis Y is the Up axis in camera space
//    Scalar fAngle = MathFn->ArcTan2( vModelPos.X, vModelPos.Z );
//    matTemp.MakeRotate( AXIS_Y, fAngle );
//    WorldTransform.SetRotate( WorldTransform.GetRotate() * matTemp );
//}
