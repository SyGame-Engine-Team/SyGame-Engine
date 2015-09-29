/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshTorus.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Torus
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_MESHTORUS_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_MESHTORUS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Primitives/3D/Torus.h"
//#include "../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshConcaveGraph.h"

#include "../../Rendering/Resources/Mesh.h"


/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshTorus class
class MeshTorus : public TriangleListMesh
{
public:
    // Torus : S strips of 2R triangles
    //         Mesh  = (S+1)(R+1) vertices, 2SR triangles, 6SR indices
    //         Graph = SR vertices, SR quad facets
    MeshTorus( const Torus & vTorus, UInt iSliceSamples, UInt iRadialSamples,
               GPUInputLayout * pIL, Bool bInterior = false );
    virtual ~MeshTorus();

    // Geometry access
    inline const Torus & GetGeometry() const;

    inline UInt GetSliceSamples() const;
    inline UInt GetRadialSamples() const;

    inline Bool IsInterior() const;

    // Graph generator
    //static Void GenerateGraph( MeshConcaveGraph * outGraph, UInt iSliceSamples, UInt iRadialSamples, Bool bInterior = false );

    // Dynamic update support
    inline Void UpdateGeometry( const Torus & vTorus, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Torus m_vTorus;

    UInt m_iSliceSamples;
    UInt m_iRadialSamples;

    Bool m_bInterior;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshTorus.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_MESHTORUS_H


