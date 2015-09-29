/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Camera.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Camera and Projector rendering primitives.
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_CAMERA_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_CAMERA_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Vertex/Vertex3.h"
#include "../../../Lib/Math/Types/Vertex/Vertex4.h"
#include "../../../Lib/Math/Types/Vector/Vector3.h"
#include "../../../Lib/Math/Types/Vector/Vector4.h"
#include "../../../Lib/Math/Types/Matrix/Matrix3.h"
#include "../../../Lib/Math/Types/Matrix/Matrix4.h"
#include "../../../Lib/Math/Types/Transform/Transform3.h"

#include "../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum FrustrumLimit
{
    FRUSTRUM_LEFT = 0, // Side min
    FRUSTRUM_RIGHT,    // Side max
    FRUSTRUM_BOTTOM,   // Up min
    FRUSTRUM_TOP,      // Up max
    FRUSTRUM_NEAR,     // View min
    FRUSTRUM_FAR,      // View max

    FRUSTRUM_COUNT
};

enum ProjectionDepthMapping
{
    PROJECT_DEPTH_ZERO_ONE = 0, // Direct3D-like
    PROJECT_DEPTH_MINUSONE_ONE, // OpenGL-like

    PROJECT_DEPTH_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The Camera class
class Camera
{
public:
    Camera( Bool bPerspective = true );
    virtual ~Camera();

    // Pre-View transform
    inline Void SetPreViewMatrix( const Matrix4 & matPreView );
    inline Void DisablePreView();
    inline Bool HasPreView() const;

    inline const Matrix4 & GetPreViewMatrix() const;

    // View transform
    Void SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp );
    Void SetLocation( const Vertex3 & vEye );
    Void SetOrientation( const Vector3 & vDir, const Vector3 & vUp );

    inline const Vertex3 & GetViewEye() const;
    inline const Vector3 & GetViewDir() const;
    inline const Vector3 & GetViewUp() const;
    inline const Vector3 & GetViewSide() const;

    inline const Matrix4 & GetViewMatrix() const;
    inline const Matrix4 & GetInvViewMatrix() const;
    inline const Matrix4 & GetBillboardMatrix() const;

    // View frustrum
    Void SetFrustrum( const Scalar arrValues[FRUSTRUM_COUNT] );
    Void SetFrustrum( FrustrumLimit iLimit, Scalar fValue );
    Void SetFrustrum( Scalar fLeft, Scalar fRight, Scalar fBottom, Scalar fTop, Scalar fNear, Scalar fFar );
    Void SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar );

    inline const Scalar * GetFrustrum() const;
    inline Scalar GetFrustrum( FrustrumLimit iLimit ) const;
    Void GetFrustrum( Scalar & outLeft, Scalar & outRight, Scalar & outBottom, Scalar & outTop, Scalar & outNear, Scalar & outFar ) const;
    Bool GetFrustrum( Scalar & outFOV, Scalar & outAspect, Scalar & outNear, Scalar & outFar ) const;

    Void ExtractFrustrumPlanes( Plane arrPlanes[FRUSTRUM_COUNT], const Scalar arrOverride[FRUSTRUM_COUNT] = NULL ) const;
    Void ExtractFrustrumVertices( Vertex3 arrVertices[8], const Scalar arrOverride[FRUSTRUM_COUNT] = NULL ) const;

    // Projection transform
    inline ProjectionDepthMapping GetDepthMapping() const;

    inline Bool IsPerspective() const;
    inline Void SetPerspective( Bool bPerspective );

    inline const Matrix4 & GetProjectionMatrix() const;

    // Post-Projection transform
    inline Void SetPostProjectionMatrix( const Matrix4 & matPostProjection );
    inline Void DisablePostProjection();
    inline Bool HasPostProjection() const;

    inline const Matrix4 & GetPostProjectionMatrix() const;

    // Helpers
    inline const Matrix4 & GetCameraMatrix() const;

    Void ComputeAABB( const Byte * pVertices, UInt iVertexCount, UInt iStride, const Matrix4 & matWorld,
                      Scalar & outRight, Scalar & outLeft, Scalar & outBottom, Scalar & outTop );

private:
    // Helpers
    Void _UpdateViewMatrix();
    Void _UpdateProjectionMatrix();
    Void _UpdateCameraMatrix();

    // Pre-View transform
    Matrix4 m_matPreView;
    Bool m_bHasPreView;

    // View transform
    Vertex3 m_vEye;
    Vector3 m_vView;
    Vector3 m_vUp;
    Vector3 m_vSide; // Right in LH / Left in RH
    Matrix4 m_matView;
    Matrix4 m_matInvView;
    Matrix4 m_matBillboard;

    // Projection transform
    Scalar m_fFrustrum[FRUSTRUM_COUNT];
    Bool m_bPerspective;
    Matrix4 m_matProjection;

    // Post-Projection transform
    Matrix4 m_matPostProjection;
    Bool m_bHasPostProjection;

    // Concatenated transform
    Matrix4 m_matCamera;

    // DepthMapping independance
public:
    // Set this according to the renderer's API, before creating any Camera.
    inline static Void SetDefaultDepthMapping( ProjectionDepthMapping iDepthMapping );
    inline static ProjectionDepthMapping GetDefaultDepthMapping();
private:
    friend class Projector;
    static ProjectionDepthMapping sm_iDefaultDepthMapping;
    ProjectionDepthMapping m_iDepthMapping;
};

/////////////////////////////////////////////////////////////////////////////////
// The Projector class
class Projector : public Camera
{
public:
    Projector( ProjectionDepthMapping iDepthMapping, Bool bPerspective = true );
    virtual ~Projector();

    // 0 => x' = (1+x)/2 and y' = (1-y)/2
    // 1 => x' = (1+x)/2 and y' = (1+y)/2
    static const Matrix4 BiasScaleMatrix[2];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Camera.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_CAMERA_H

