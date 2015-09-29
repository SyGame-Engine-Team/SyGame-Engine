/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshDisk.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Disk
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
#ifndef SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHDISK_H
#define SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHDISK_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Primitives/3D/Planar/Circle3.h"
#include "../../../../Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.h"

#include "../../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshDisk class
class MeshDisk : public TriangleListMesh
{
public:
    // Disk : S-1 strips of 2R triangles and 1 fan of R triangles
    //        Mesh  = (1+S(R+1)) vertices, 2(S-1)R+R triangles, (2(S-1)R+R)*3 indices
    //        Graph = R vertices, 1 polygon facet
    MeshDisk( const Circle3 & vDisk, UInt iSliceSamples, UInt iRadialSamples, GPUInputLayout * pIL );
    virtual ~MeshDisk();

    // Geometry access
    inline const Circle3 & GetGeometry() const;

    inline UInt GetSliceSamples() const;
    inline UInt GetRadialSamples() const;

    // Graph generator
    static Void GenerateGraph( MeshVoronoiGraph * outGraph, UInt iRadialSamples );

    // Dynamic update support
    inline Void UpdateGeometry( const Circle3 & vDisk, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Circle3 m_vDisk;

    UInt m_iSliceSamples;
    UInt m_iRadialSamples;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshDisk.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_PRIMITIVES_PLANAR_MESHDISK_H


