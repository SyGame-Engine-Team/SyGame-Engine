/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshSphere.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Sphere
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_MESHSPHERE_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_MESHSPHERE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Primitives/3D/Sphere.h"
#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshSphere class
class MeshSphere : public TriangleListMesh
{
public:
    // Sphere : S-1 strips of 2R triangles and 2 fans of R triangles
    //          Mesh  = (2+S(R+1)) vertices, 2SR triangles, 6SR indices
    //          Graph = (2+SR) vertices, (S-1)R quad facets, 2R triangle facets
    MeshSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples,
                GPUInputLayout * pIL, Bool bInterior = false );
    virtual ~MeshSphere();

    // Geometry access
    inline const Sphere & GetGeometry() const;
    
    inline UInt GetSliceSamples() const;
    inline UInt GetRadialSamples() const;

    inline Bool IsInterior() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior = false );

    // Dynamic update support
    inline Void UpdateGeometry( const Sphere & vSphere, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Sphere m_vSphere;

    UInt m_iSliceSamples;
    UInt m_iRadialSamples;

    Bool m_bInterior;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshSphere.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_MESHSPHERE_H


