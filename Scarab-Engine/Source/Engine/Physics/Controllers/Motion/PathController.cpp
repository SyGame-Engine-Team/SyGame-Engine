/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/PathController.cpp
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
// Includes
#include "PathController.h"

/////////////////////////////////////////////////////////////////////////////////
// PathController implementation
PathController::PathController( const GChar * strName, Curve3 * pLinearPath, Curve2 * pLinearTravel,
                                                       Curve3 * pAngularPath, Curve2 * pAngularTravel,
                                                       EulerAngles iEulerAnglesMode ):
    MotionController( strName )
{
    m_pLinearPath = pLinearPath;
    m_pLinearTravel = pLinearTravel;

    m_pAngularPath = pAngularPath;
    m_pAngularTravel = pAngularTravel;

    m_iEulerAnglesMode = iEulerAnglesMode;
}
PathController::~PathController()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Void PathController::_RootFindTravelParameters( Scalar fControlTime, Scalar * outLinearTravelParameter,
                                                                     Scalar * outAngularTravelParameter ) const
{
    // Use Newton's root finding method (mixed with bisection)
    Scalar fNormalTime = ( (fControlTime - MinTime) / (MaxTime - MinTime) );
    UInt i, iMaxIterations = 32;

    // Initial guess
    Scalar fT = ( (1.0f - fNormalTime) * m_pLinearTravel->GetMinT() + fNormalTime * m_pLinearTravel->GetMaxT() );

    // Interval bounding
    Scalar fLow = m_pLinearTravel->GetMinT();
    Scalar fHigh = m_pLinearTravel->GetMaxT();
    Scalar fDelta, fTry;
    for( i = 0; i < iMaxIterations; ++i ) {
        fDelta = ( m_pLinearTravel->P(fT).Y - fControlTime );
        if ( MathFn->Abs(fDelta) < SCALAR_ERROR )
            break; // close enough
        fTry = fT - ( fDelta / m_pLinearTravel->PdT(fT).Y );
        if ( fDelta > 0.0f ) {
            fHigh = fT;
            if ( fTry <= fLow ) // out of bound
                fT = ( fLow + fHigh ) * 0.5f;
            else
                fT = fTry;
        } else {
            fLow = fT;
            if ( fTry >= fHigh ) // out of bound
                fT = ( fLow + fHigh ) * 0.5f;
            else
                fT = fTry;
        }
    }
    *outLinearTravelParameter = fT;

    // Initial guess
    fT = ( (1.0f - fNormalTime) * m_pAngularTravel->GetMinT() + fNormalTime * m_pAngularTravel->GetMaxT() );

    // Interval bounding
    fLow = m_pAngularTravel->GetMinT();
    fHigh = m_pAngularTravel->GetMaxT();
    fDelta, fTry;
    for( i = 0; i < iMaxIterations; ++i ) {
        fDelta = ( m_pAngularTravel->P(fT).Y - fControlTime );
        if ( MathFn->Abs(fDelta) < SCALAR_ERROR )
            break; // close enough
        fTry = fT - ( fDelta / m_pAngularTravel->PdT(fT).Y );
        if ( fDelta > 0.0f ) {
            fHigh = fT;
            if ( fTry <= fLow ) // out of bound
                fT = ( fLow + fHigh ) * 0.5f;
            else
                fT = fTry;
        } else {
            fLow = fT;
            if ( fTry >= fHigh ) // out of bound
                fT = ( fLow + fHigh ) * 0.5f;
            else
                fT = fTry;
        }
    }
    *outAngularTravelParameter = fT;
}

Bool PathController::_UpdateControllerState( Scalar /*fTimeStep*/ )
{
    // Root-Find parameters on travel splines
    Scalar fLinearTravelParameter;
    Scalar fAngularTravelParameter;
    _RootFindTravelParameters( m_fControlTime, &fLinearTravelParameter, &fAngularTravelParameter );

    // Get distances from travel splines
    Scalar fLinearDistance = m_pLinearTravel->P( fLinearTravelParameter ).X;
    Scalar fAngularDistance = m_pAngularTravel->P( fAngularTravelParameter ).X;

    // Root-Find parameters on path splines
    Scalar fLinearPathParameter = m_pLinearPath->Parameter( fLinearDistance );
    Scalar fAngularPathParameter = m_pAngularPath->Parameter( fAngularDistance );

    // Get Position & Orientation from path splines
    m_vPosition = m_pLinearPath->P( fLinearPathParameter );

    Vector3 vEulerAngles = m_pAngularPath->P( fAngularPathParameter );
    switch( m_iEulerAnglesMode ) {
        case EULER_ANGLES_XYZ: m_qOrientation.MakeRotate( vEulerAngles.X, vEulerAngles.Y, vEulerAngles.Z, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_XZY: m_qOrientation.MakeRotate( vEulerAngles.X, vEulerAngles.Z, vEulerAngles.Y, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_YXZ: m_qOrientation.MakeRotate( vEulerAngles.Y, vEulerAngles.X, vEulerAngles.Z, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_YZX: m_qOrientation.MakeRotate( vEulerAngles.Y, vEulerAngles.Z, vEulerAngles.X, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_ZXY: m_qOrientation.MakeRotate( vEulerAngles.Z, vEulerAngles.X, vEulerAngles.Y, m_iEulerAnglesMode ); break;
        case EULER_ANGLES_ZYX: m_qOrientation.MakeRotate( vEulerAngles.Z, vEulerAngles.Y, vEulerAngles.X, m_iEulerAnglesMode ); break;
        default: Assert(false); break;
    }

    // Compute Linear & Angular Speeds
    Vector3 vPathSpeed;
    Vector2 vTravelSpeed;
    Scalar fPathDerivative, fTravelDerivative;

    vPathSpeed = m_pLinearPath->PdT( fLinearPathParameter );
    vTravelSpeed = m_pLinearTravel->PdT( fLinearTravelParameter );
    fPathDerivative = ( vPathSpeed * vPathSpeed );
    if ( !(MathFn->EqualsZero(fPathDerivative)) ) {
        fPathDerivative = MathFn->InvSqrt( fPathDerivative );
        if ( !(MathFn->EqualsZero(vTravelSpeed.Y)) ) {
            fTravelDerivative = ( vTravelSpeed.X / vTravelSpeed.Y );
            m_vLinearSpeed = ( vPathSpeed * (fPathDerivative * fTravelDerivative) );
        }
    }

    vPathSpeed = m_pAngularPath->PdT( fAngularPathParameter );
    vTravelSpeed = m_pAngularTravel->PdT( fAngularTravelParameter );
    fPathDerivative = ( vPathSpeed * vPathSpeed );
    if ( !(MathFn->EqualsZero(fPathDerivative)) ) {
        fPathDerivative = MathFn->InvSqrt( fPathDerivative );
        if ( !(MathFn->EqualsZero(vTravelSpeed.Y)) ) {
            fTravelDerivative = ( vTravelSpeed.X / vTravelSpeed.Y );
            m_vAngularSpeed = ( vPathSpeed * (fPathDerivative * fTravelDerivative) );
        }
    }

    return true;
}

