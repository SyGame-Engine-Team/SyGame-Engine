/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshCylinder.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Cylinder
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_MESHCYLINDER_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_MESHCYLINDER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Primitives/3D/Cylinder.h"
#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshCylinder class
class MeshCylinder : public TriangleListMesh
{
public:
    // Cylinder : S-1 strips of 2R triangles and 2 fans of R triangles
    //            Mesh  = (2+S(R+1)) vertices, 2SR triangles, 6SR indices
    //            Graph = 2R vertices, R quad facets, 2 polygon facets
    MeshCylinder( const Cylinder & vCylinder, UInt iSliceSamples, UInt iRadialSamples,
                  GPUInputLayout * pIL, Bool bInterior = false );
    virtual ~MeshCylinder();

    // Geometry access
    inline const Cylinder & GetGeometry() const;

    inline UInt GetSliceSamples() const;
    inline UInt GetRadialSamples() const;

    inline Bool IsInterior() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior = false );

    // Dynamic update support
    inline Void UpdateGeometry( const Cylinder & vCylinder, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Cylinder m_vCylinder;

    UInt m_iSliceSamples;
    UInt m_iRadialSamples;

    Bool m_bInterior;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshCylinder.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_MESHCYLINDER_H


