/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Camera.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Camera rendering primitive.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Camera implementation
inline Void Camera::SetPreViewMatrix( const Matrix4 & matPreView ) {
    m_matPreView = matPreView;
    m_bHasPreView = true;
    _UpdateCameraMatrix();
}
inline Void Camera::DisablePreView() {
    m_matPreView = Matrix4::Identity;
    m_bHasPreView = false;
    _UpdateCameraMatrix();
}
inline Bool Camera::HasPreView() const {
    return m_bHasPreView;
}

inline const Matrix4 & Camera::GetPreViewMatrix() const {
    return m_matPreView;
}

inline const Vertex3 & Camera::GetViewEye() const {
    return m_vEye;
}
inline const Vector3 & Camera::GetViewDir() const {
    return m_vView;
}
inline const Vector3 & Camera::GetViewUp() const {
    return m_vUp;
}
inline const Vector3 & Camera::GetViewSide() const {
    return m_vSide;
}

inline const Matrix4 & Camera::GetViewMatrix() const {
    return m_matView;
}
inline const Matrix4 & Camera::GetInvViewMatrix() const {
    return m_matInvView;
}
inline const Matrix4 & Camera::GetBillboardMatrix() const {
    return m_matBillboard;
}

inline const Scalar * Camera::GetFrustrum() const {
    return m_fFrustrum;
}
inline Scalar Camera::GetFrustrum( FrustrumLimit iLimit ) const {
    Assert(iLimit < FRUSTRUM_COUNT);
    return m_fFrustrum[iLimit];
}

inline ProjectionDepthMapping Camera::GetDepthMapping() const {
    return m_iDepthMapping;
}

inline Bool Camera::IsPerspective() const {
    return m_bPerspective;
}
inline Void Camera::SetPerspective( Bool bPerspective ) {
    m_bPerspective = bPerspective;
    _UpdateProjectionMatrix();
}

inline const Matrix4 & Camera::GetProjectionMatrix() const {
    return m_matProjection;
}

inline Void Camera::SetPostProjectionMatrix( const Matrix4 & matPostProjection ) {
    m_matPostProjection = matPostProjection;
    m_bHasPostProjection = true;
    _UpdateCameraMatrix();
}
inline Void Camera::DisablePostProjection() {
    m_matPostProjection = Matrix4::Identity;
    m_bHasPostProjection = false;
    _UpdateCameraMatrix();
}
inline Bool Camera::HasPostProjection() const {
    return m_bHasPostProjection;
}

inline const Matrix4 & Camera::GetPostProjectionMatrix() const {
    return m_matPostProjection;
}

inline const Matrix4 & Camera::GetCameraMatrix() const {
    return m_matCamera;
}

inline Void Camera::SetDefaultDepthMapping( ProjectionDepthMapping iDepthMapping ) {
    sm_iDefaultDepthMapping = iDepthMapping;
}
inline ProjectionDepthMapping Camera::GetDefaultDepthMapping() {
    return sm_iDefaultDepthMapping;
}
