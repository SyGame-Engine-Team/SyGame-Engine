/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/MeshCurve.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Curve
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
#ifndef SCARAB_ENGINE_MODELING_CURVES_MESHCURVE_H
#define SCARAB_ENGINE_MODELING_CURVES_MESHCURVE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Curves/3D/Curve3.h"

#include "../../Rendering/Resources/Mesh.h"

//#include "../MeshGraphs/MeshManifold2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshCurve class
class MeshCurve : public LineStripMesh
{
public:
    MeshCurve( Curve3 * pCurve, Bool bClosed, UInt iSampleCount, GPUInputLayout * pIL );
    virtual ~MeshCurve();

    // Geometry access
    inline const Curve3 * GetGeometry( Bool * outIsClosed = NULL ) const;

    inline UInt GetSampleCount() const;
    inline Scalar GetSample( UInt iSample ) const;

    // Graph generator
    //static Void GenerateGraph( MeshManifold2 * outGraph, UInt iSampleCount );

    // Dynamic update support
    inline Curve3 * UpdateGeometryBegin( Bool * outIsClosed );
    inline Void UpdateGeometryEnd( Bool bClosed, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Curve3 * m_pCurve;
    Bool m_bClosed;

    UInt m_iSampleCount;
    Scalar * m_arrSamples;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshCurve.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_CURVES_MESHCURVE_H

