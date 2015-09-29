/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshBox.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Box
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_MESHBOX_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_MESHBOX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Primitives/3D/Box.h"
#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshBox class
class MeshBox : public TriangleListMesh
{
public:
    // Box : Aligned with axes,
    //       Mesh  = 24 vertices, 12 triangles, 36 indices
    //       Graph = 8 vertices, 6 quad facets
    MeshBox( const Box & vBox, GPUInputLayout * pIL, Bool bInterior = false );
    virtual ~MeshBox();

    // Geometry access
    inline const Box & GetGeometry() const;

    inline Bool IsInterior() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph, Bool bInterior = false );

    // Dynamic update support
    inline Void UpdateGeometry( const Box & vBox, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Box m_vBox;

    Bool m_bInterior;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshBox.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_MESHBOX_H


