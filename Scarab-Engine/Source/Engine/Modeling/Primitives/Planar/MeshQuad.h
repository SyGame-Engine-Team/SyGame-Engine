/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshQuad.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Quad
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHQUAD_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHQUAD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Quad3.h"
#include "../../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshQuad class
class MeshQuad : public TriangleStripMesh
{
public:
    // Quad : Mesh  = 4 vertices, 2 triangles, 4 indices
    //        Graph = 4 vertices, 1 quad facet
    MeshQuad( const Quad3 & vQuad, GPUInputLayout * pIL );
    virtual ~MeshQuad();

    // Geometry access
    inline const Quad3 & GetGeometry() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph );

    // Dynamic update support
    inline Void UpdateGeometry( const Quad3 & vQuad, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Quad3 m_vQuad;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshQuad.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHQUAD_H

