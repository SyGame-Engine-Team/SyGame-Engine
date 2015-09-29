/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeBox.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Box
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShapeBox.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeBox implementation
ShapeBox::ShapeBox( const Box & vBox ):
    ShapeConvex( NULL ), m_hGraph(), m_vBox( vBox )
{
    // Graph data
    m_hGraph.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGraph.Create();

    MeshBox::GenerateGraph( &m_hGraph );

    Vertex3 arrV[8];
    m_vBox.ExtractVertices( arrV );
    m_hGraph.ComputeFeatureData( arrV );

    // Local AAB
    m_bvLocalAAB = BVAABox( m_vBox.Center, m_vBox.Extents );
}
ShapeBox::ShapeBox( MeshBox * pBoxMesh ):
    ShapeConvex( pBoxMesh ), m_hGraph(), m_vBox( pBoxMesh->GetGeometry() )
{
    // Graph data
    m_hGraph.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGraph.Create();

    MeshBox::GenerateGraph( &m_hGraph );

    Vertex3 arrV[8];
    m_vBox.ExtractVertices( arrV );
    m_hGraph.ComputeFeatureData( arrV );

    // Local AAB
    m_bvLocalAAB = BVAABox( m_vBox.Center, m_vBox.Extents );
}
ShapeBox::~ShapeBox()
{
    m_hGraph.Destroy();
}

Void ShapeBox::ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    Vector3 vSize = ( m_vBox.Extents * 2.0f );
    Vector3 vSqrSize (
        vSize.X * vSize.X,
        vSize.Y * vSize.Y,
        vSize.Z * vSize.Z
    );

    *outMass = ( vSize.X * vSize.Y * vSize.Z ) * fDensity;

    *outCenterOfMass = m_vBox.Center;
    
    outInertiaTensor->MakeIdentity();
    (*outInertiaTensor) *= ( *outMass / 12.0f );
    outInertiaTensor->m00 *= (vSqrSize.Y + vSqrSize.Z);
    outInertiaTensor->m11 *= (vSqrSize.X + vSqrSize.Z);
    outInertiaTensor->m22 *= (vSqrSize.X + vSqrSize.Y);
}

Void ShapeBox::QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax, UInt * outMin ) const
{
    if ( vDirection.X >= 0.0f ) {
        if ( vDirection.Y >= 0.0f ) {
            if ( vDirection.Z >= 0.0f ) {
                if ( outMax != NULL )
                    *outMax = 4;
                if ( outMin != NULL )
                    *outMin = 2;
            } else {
                if ( outMax != NULL )
                    *outMax = 7;
                if ( outMin != NULL )
                    *outMin = 3;
            }
        } else {
            if ( vDirection.Z >= 0.0f ) {
                if ( outMax != NULL )
                    *outMax = 5;
                if ( outMin != NULL )
                    *outMin = 1;
            } else {
                if ( outMax != NULL )
                    *outMax = 6;
                if ( outMin != NULL )
                    *outMin = 0;
            }
        }
    } else {
        if ( vDirection.Y >= 0.0f ) {
            if ( vDirection.Z >= 0.0f ) {
                if ( outMax != NULL )
                    *outMax = 0;
                if ( outMin != NULL )
                    *outMin = 6;
            } else {
                if ( outMax != NULL )
                    *outMax = 1;
                if ( outMin != NULL )
                    *outMin = 5;
            }
        } else {
            if ( vDirection.Z >= 0.0f ) {
                if ( outMax != NULL )
                    *outMax = 3;
                if ( outMin != NULL )
                    *outMin = 7;
            } else {
                if ( outMax != NULL )
                    *outMax = 2;
                if ( outMin != NULL )
                    *outMin = 4;
            }
        }
    }
}
Void ShapeBox::QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax, Vertex3 * outMin ) const
{
    if ( outMax != NULL ) {
        *outMax = m_vBox.Center;
        if ( vDirection.X > SCALAR_ERROR )
            outMax->X += m_vBox.Extents.X;
        else if ( vDirection.X < -SCALAR_ERROR )
            outMax->X -= m_vBox.Extents.X;

        if ( vDirection.Y > SCALAR_ERROR )
            outMax->Y += m_vBox.Extents.Y;
        else if ( vDirection.Y < -SCALAR_ERROR )
            outMax->Y -= m_vBox.Extents.Y;

        if ( vDirection.Z > SCALAR_ERROR )
            outMax->Z += m_vBox.Extents.Z;
        else if ( vDirection.Z < -SCALAR_ERROR )
            outMax->Z -= m_vBox.Extents.Z;
    }
    if ( outMin != NULL ) {
        *outMin = m_vBox.Center;
        if ( vDirection.X > SCALAR_ERROR )
            outMin->X -= m_vBox.Extents.X;
        else if ( vDirection.X < -SCALAR_ERROR )
            outMin->X += m_vBox.Extents.X;

        if ( vDirection.Y > SCALAR_ERROR )
            outMin->Y -= m_vBox.Extents.Y;
        else if ( vDirection.Y < -SCALAR_ERROR )
            outMin->Y += m_vBox.Extents.Y;

        if ( vDirection.Z > SCALAR_ERROR )
            outMin->Z -= m_vBox.Extents.Z;
        else if ( vDirection.Z < -SCALAR_ERROR )
            outMin->Z += m_vBox.Extents.Z;
    }
}

