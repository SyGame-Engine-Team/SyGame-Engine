/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/PathController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Path tracking
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
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PATHCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PATHCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Curves/2D/Curve2.h"
#include "../../../../Lib/Math/Geometry/Curves/3D/Curve3.h"

#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The PathController class
class PathController : public MotionController
{
public:
    // All curves must be cubic at least (ensure C2 continuity)
    // Linear Path = 3D Positions / Angular Path = 3D Euler angles
    // Linear/Angular Travel = (Distance,Time) checkpoints along the path
    // Default Euler angles mode is Z(pitch) then Y(yaw) then X(roll) / ( qX * qY * qZ )
    //         <=> Fresnet Frame is X(tangent), Y(normal), Z(binormal)
    PathController( const GChar * strName, Curve3 * pLinearPath, Curve2 * pLinearTravel,
                                           Curve3 * pAngularPath, Curve2 * pAngularTravel,
                                           EulerAngles iEulerAnglesMode = EULER_ANGLES_ZYX );
    virtual ~PathController();

    // Type
    inline virtual ControllerType GetType() const;

    // Linear path access
    inline Curve3 * GetLinearPath() const;
    inline Curve2 * GetLinearTravel() const;

    // Angular path access
    inline Curve3 * GetAngularPath() const;
    inline Curve2 * GetAngularTravel() const;

    // Euler angles mode access
    inline EulerAngles GetEulerAnglesMode() const;
    inline Void SetEulerAnglesMode( EulerAngles iEulerAnglesMode );

protected:
    // Helpers
    Void _RootFindTravelParameters( Scalar fControlTime, Scalar * outLinearTravelParameter,
                                                         Scalar * outAngularTravelParameter ) const;

    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    // Linear path
    Curve3 * m_pLinearPath;
    Curve2 * m_pLinearTravel;

    // Angular path
    Curve3 * m_pAngularPath;
    Curve2 * m_pAngularTravel;

    // Euler angles interpretation
    EulerAngles m_iEulerAnglesMode;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PathController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PATHCONTROLLER_H

