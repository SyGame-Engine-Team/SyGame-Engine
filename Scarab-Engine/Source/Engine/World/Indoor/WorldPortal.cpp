/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldPortal.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Portal class, traditional convex room separator
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
#include "WorldPortal.h"

#include "../WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldPortal implementation
WorldPortal::WorldPortal( const Vertex3 * arrPolygon, UInt iPolygonSize ):
    m_vWorldPlane()
{
    Assert( iPolygonSize >= 3 );

    Closed = true;
    Target = NULL;

    // Save polygon
    WorldFn->SelectMemory( TEXT("Indoor") );
    m_iPolygonSize = iPolygonSize;
    m_arrWorldPolygon = New Vertex3[m_iPolygonSize];
    WorldFn->UnSelectMemory();

    for( UInt i = 0; i < m_iPolygonSize; ++i )
        m_arrWorldPolygon[i] = arrPolygon[i];

    // Compute polygon plane
    Vector3 vAB = ( arrPolygon[1] - arrPolygon[0] );
    Vector3 vAC = ( arrPolygon[2] - arrPolygon[0] );
    Vector3 vNormal = ( vAB ^ vAC );
    vNormal.Normalize();

    m_vWorldPlane = Plane( vNormal, arrPolygon[0] );
}
WorldPortal::~WorldPortal()
{
    // Destroy polygon
    WorldFn->SelectMemory( TEXT("Indoor") );
    DeleteA( m_arrWorldPolygon );
    WorldFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Bool WorldPortal::_ComputeReducedFrustum( Scalar outReducedFrustum[FRUSTRUM_COUNT], const WorldPortalCuller & wPortalCuller )
{
    const Camera * pCamera = wPortalCuller.GetCamera();

    Scalar arrFrustrum[FRUSTRUM_COUNT];
    wPortalCuller.GetFrustrumState( arrFrustrum );

    Vector3 vRay;
    Vertex3 vCameraVertex;
    UInt i;

    // Compute reduced AARect
    Scalar fRight = SCALAR_INFINITE;
    Scalar fLeft = -SCALAR_INFINITE;
    Scalar fBottom = SCALAR_INFINITE;
    Scalar fTop = -SCALAR_INFINITE;

    if ( pCamera->IsPerspective() ) {
        // Perspective case, use sign evolution
        const Scalar fEpsilon = 1.0e-6f;
        const Scalar fInvEpsilon = 1.0e+6f;
        Int iFirstSign = 0, iLastSign = 0;
        Bool bSignChange = false;

        Vector3 vEdge;
        Vertex3 vFirst = Vertex3::Null;
        Vertex3 vLast = Vertex3::Null;
        Scalar fDotS, fDotU, fDotD, fT;

        for( i = 0; i < m_iPolygonSize; ++i ) {
            // RayCast this vertex
            vRay = ( m_arrWorldPolygon[i] - pCamera->GetViewEye() );
            vCameraVertex.X = ( vRay * pCamera->GetViewSide() );
            vCameraVertex.Y = ( vRay * pCamera->GetViewUp() );
            vCameraVertex.Z = ( vRay * pCamera->GetViewDir() );

            if ( vCameraVertex.Z > fEpsilon ) {
                if ( iFirstSign == 0 ) {
                    iFirstSign = +1;
                    vFirst = vCameraVertex;
                }

                // Project on near plane
                fDotD = arrFrustrum[FRUSTRUM_NEAR] / vCameraVertex.Z;
                fDotS = fDotD * vCameraVertex.X;
                fDotU = fDotD * vCameraVertex.Y;

                // Update AARect
                if ( fDotS < fRight )
                    fRight = fDotS;
                if ( fDotS > fLeft )
                    fLeft = fDotS;
                if ( fDotU < fBottom )
                    fBottom = fDotU;
                if ( fDotU > fTop )
                    fTop = fDotU;

                if ( iLastSign < 0 )
                    bSignChange = true;
                iLastSign = +1;
            } else {
                if ( iFirstSign == 0 ) {
                    iFirstSign = -1;
                    vFirst = vCameraVertex;
                }

                // Ignore, cannot project

                if ( iLastSign > 0 )
                    bSignChange = true;
                iLastSign = -1;
            }
            if ( bSignChange ) {
                // Polygon edge
                vEdge = ( vCameraVertex - vLast );

                // Project on near plane
                fT = (fEpsilon - vLast.Z) / vEdge.Z;
                fDotD = arrFrustrum[FRUSTRUM_NEAR] * fInvEpsilon;
                fDotS = fDotD * ( vLast.X + fT * vEdge.X );
                fDotU = fDotD * ( vLast.Y + fT * vEdge.Y );

                // Update AARect
                if ( fDotS < fRight )
                    fRight = fDotS;
                if ( fDotS > fLeft )
                    fLeft = fDotS;
                if ( fDotU < fBottom )
                    fBottom = fDotU;
                if ( fDotU > fTop )
                    fTop = fDotU;

                bSignChange = false;
            }

            // Track last vertex
            vLast = vCameraVertex;
        }
        if ( (iFirstSign * iLastSign) < 0 ) {
            // Polygon edge
            vEdge = ( vFirst - vLast );

            // Project on near plane
            fT = (fEpsilon - vLast.Z) / vEdge.Z;
            fDotS = fInvEpsilon * ( vLast.X + fT * vEdge.X );
            fDotU = fInvEpsilon * ( vLast.Y + fT * vEdge.Y );

            // Update AARect
            if ( fDotS < fRight )
                fRight = fDotS;
            if ( fDotS > fLeft )
                fLeft = fDotS;
            if ( fDotU < fBottom )
                fBottom = fDotU;
            if ( fDotU > fTop )
                fTop = fDotU;
        }
    } else {
        // Orthographic case
        for( i = 0; i < m_iPolygonSize; ++i ) {
            // RayCast this vertex
            vRay = ( m_arrWorldPolygon[i] - pCamera->GetViewEye() );

            // Project on near plane
            vCameraVertex.X = ( vRay * pCamera->GetViewSide() );
            vCameraVertex.Y = ( vRay * pCamera->GetViewUp() );

            // Update AARect
            if ( vCameraVertex.X < fRight )
                fRight = vCameraVertex.X;
            if ( vCameraVertex.X > fLeft )
                fLeft = vCameraVertex.X;
            if ( vCameraVertex.Y < fBottom )
                fBottom = vCameraVertex.Y;
            if ( vCameraVertex.Y > fTop )
                fTop = vCameraVertex.Y;
        }
    }

    // Cull AARect with the real frustrum
    if ( fRight >= arrFrustrum[FRUSTRUM_LEFT] || fLeft <= arrFrustrum[FRUSTRUM_RIGHT] ||
         fBottom >= arrFrustrum[FRUSTRUM_TOP] || fTop <= arrFrustrum[FRUSTRUM_BOTTOM] ) {
        return false;
    }

    // Compute reducted frustrum
    for( i = 0; i < FRUSTRUM_COUNT; ++i )
        outReducedFrustum[i] = arrFrustrum[i];

    if ( fRight > outReducedFrustum[FRUSTRUM_RIGHT] )
        outReducedFrustum[FRUSTRUM_RIGHT] = fRight;
    if ( fLeft < outReducedFrustum[FRUSTRUM_LEFT] )
        outReducedFrustum[FRUSTRUM_LEFT] = fLeft;
    if ( fBottom > outReducedFrustum[FRUSTRUM_BOTTOM] )
        outReducedFrustum[FRUSTRUM_BOTTOM] = fBottom;
    if ( fTop < outReducedFrustum[FRUSTRUM_TOP] )
        outReducedFrustum[FRUSTRUM_TOP] = fTop;

    return true;
}

Void WorldPortal::_OnVisibleGroup( WorldPortalCuller & wPortalCuller, Bool bNoCull )
{
    // No target
    if ( Target == NULL )
        return;

    // Portal is closed
    if ( Closed )
        return;

    // Portal is not visible
    Bool bVisible = wPortalCuller.IsVisible( m_arrWorldPolygon, m_iPolygonSize, true );
    if ( !bVisible )
        return;

    // Portal is backfacing the camera
    Int iSide = m_vWorldPlane.ClassifyPointNP( wPortalCuller.GetCamera()->GetViewEye() );
    if ( iSide <= 0 )
        return;

    // Target room is potentially visible
    Scalar arrReducedFrustrum[FRUSTRUM_COUNT];
    bVisible = _ComputeReducedFrustum( arrReducedFrustrum, wPortalCuller );
    if ( bVisible ) {
        Scalar arrSavedFrustrum[FRUSTRUM_COUNT];
        wPortalCuller.GetFrustrumState( arrSavedFrustrum );

        wPortalCuller.SetFrustrumState(arrReducedFrustrum);
        Target->_OnVisibleGroup( wPortalCuller, bNoCull );
        wPortalCuller.SetFrustrumState(arrSavedFrustrum);
    }
}

