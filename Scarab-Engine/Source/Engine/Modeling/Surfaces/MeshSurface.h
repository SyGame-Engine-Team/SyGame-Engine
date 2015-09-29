/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshSurface.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Surface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_MODELING_SURFACES_MESHSURFACE_H
#define SCARAB_ENGINE_MODELING_SURFACES_MESHSURFACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Surfaces/ParametricSurface.h"
#include "../../../Lib/Math/Geometry/MeshGraphs/MeshManifold3.h"

#include "../../Rendering/Resources/Mesh.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshSurface class
class MeshSurface : public TriangleListMesh
{
public:
    MeshSurface( ParametricSurface * pSurface, UInt iSampleCountU, UInt iSampleCountV, GPUInputLayout * pIL );
    virtual ~MeshSurface();

    // Geometry access
    inline const ParametricSurface * GetGeometry() const;

    inline UInt GetSampleCountU() const;
    inline UInt GetSampleCountV() const;
    inline Scalar GetSampleU( UInt iSampleU ) const;
    inline Scalar GetSampleV( UInt iSampleV ) const;

    // Graph generator
    static Void GenerateGraph( MeshManifold3 * outGraph, UInt iSampleCountU, UInt iSampleCountV );

    // Dynamic update support
    inline ParametricSurface * UpdateGeometryBegin();
    inline Void UpdateGeometryEnd( GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    ParametricSurface * m_pSurface;

    UInt m_iSampleCountU;
    UInt m_iSampleCountV;
    Scalar * m_arrSamplesU;
    Scalar * m_arrSamplesV;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshSurface.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_SURFACES_MESHSURFACE_H

