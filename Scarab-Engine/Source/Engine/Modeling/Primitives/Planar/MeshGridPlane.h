/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshGridPlane.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Grid-Plane
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHGRIDPLANE_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHGRIDPLANE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"
#include "../../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshGridPlane class
class MeshGridPlane : public TriangleListMesh
{
public:
    // Grid-Plane : Mesh =  W*H vertices, 2*(W-1)*(H-1) triangles, 6*(W-1)*(H-1) indices
    //              Graph = 4 vertices, 1 quad facet
    MeshGridPlane( const Plane & vPlane, Scalar fWidth, Scalar fHeight,
                   UInt iWidthSamples, UInt iHeightSamples, GPUInputLayout * pIL );
    virtual ~MeshGridPlane();

    // Geometry access
    inline const Plane & GetGeometry() const;
    inline Scalar GetWidth() const;
    inline Scalar GetHeight() const;

    inline UInt GetWidthSamples() const;
    inline UInt GetHeightSamples() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph );

    // Dynamic update support
    inline Void UpdateGeometry( const Plane & vPlane, Scalar fWidth, Scalar fHeight, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Plane m_vPlane;
    Scalar m_fWidth;
    Scalar m_fHeight;

    UInt m_iWidthSamples;
    UInt m_iHeightSamples;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshGridPlane.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHGRIDPLANE_H


