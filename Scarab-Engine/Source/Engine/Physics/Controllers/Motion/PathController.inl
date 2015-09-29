/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/PathController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : BSpline Path tracking
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// PathController implementation
inline ControllerType PathController::GetType() const {
    return CONTROLLER_MOTION_PATH;
}

inline Curve3 * PathController::GetLinearPath() const {
    return m_pLinearPath;
}
inline Curve2 * PathController::GetLinearTravel() const {
    return m_pLinearTravel;
}

inline Curve3 * PathController::GetAngularPath() const {
    return m_pAngularPath;
}
inline Curve2 * PathController::GetAngularTravel() const {
    return m_pAngularTravel;
}

inline EulerAngles PathController::GetEulerAnglesMode() const {
    return m_iEulerAnglesMode;
}
inline Void PathController::SetEulerAnglesMode( EulerAngles iEulerAnglesMode ) {
    m_iEulerAnglesMode = iEulerAnglesMode;
}

