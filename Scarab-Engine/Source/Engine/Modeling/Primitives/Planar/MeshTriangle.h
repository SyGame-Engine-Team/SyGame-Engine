/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshTriangle.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Triangle
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHTRIANGLE_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHTRIANGLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Triangle3.h"
#include "../../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../../Rendering/Resources/Mesh.h"


/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshTriangle class
class MeshTriangle : public TriangleListMesh
{
public:
    // Triangle : Mesh  = 3 vertices, 1 triangle, 3 indices
    //            Graph = 3 vertices, 1 triangle facet
    MeshTriangle( const Triangle3 & vTriangle, GPUInputLayout * pIL );
    virtual ~MeshTriangle();

    // Geometry access
    inline const Triangle3 & GetGeometry() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph );

    // Dynamic update support
    inline Void UpdateGeometry( const Triangle3 & vTriangle, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Triangle3 m_vTriangle;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshTriangle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHTRIANGLE_H


