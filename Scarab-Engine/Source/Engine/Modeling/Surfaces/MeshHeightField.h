/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshHeightField.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Height-Field surface (landscapes)
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
#ifndef SCARAB_ENGINE_MODELING_SURFACES_MESHHEIGHTFIELD_H
#define SCARAB_ENGINE_MODELING_SURFACES_MESHHEIGHTFIELD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/MeshGraphs/MeshManifold3.h"

#include "../../Rendering/Resources/Mesh.h"
#include "../../Rendering/Resources/GPUTexture.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshHeightField class
class MeshHeightField : public TriangleListMesh
{
public:
    MeshHeightField( Scalar * arrHeightField, const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight,
                     UInt iSampleCountU, UInt iSampleCountV, GPUInputLayout * pIL );
    virtual ~MeshHeightField();

    // Geometry access
    inline const Scalar * GetGeometry() const;

    inline const Vertex2 & GetOrigin() const;
    inline const Vector2 & GetSize() const;
    inline Scalar GetMinHeight() const;
    inline Scalar GetMaxHeight() const;

    inline UInt GetSampleCountU() const;
    inline UInt GetSampleCountV() const;
    inline const Vector2 & GetSampleSize() const;
    inline Scalar GetHeight( UInt iSampleU, UInt iSampleV ) const;

    Scalar GetHeight( const Vertex2 & vPosition ) const;
    Vector3 GetNormal( const Vertex2 & vPosition ) const;

    UInt GetTriangleIndex( const Vertex2 & vPosition, UInt * outGridU = NULL, UInt * outGridV = NULL, Bool * outSecondTriangle = NULL ) const;

    // Graph generator
    static Void GenerateGraph( MeshManifold3 * outGraph, UInt iSampleCountU, UInt iSampleCountV );

    // Dynamic update support
    inline Scalar * UpdateGeometryBegin( UInt * outSampleCountU, UInt * outSampleCountV );
    inline Void UpdateGeometryEnd( const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight, GPUDeferredContext * pContext = NULL );

private:
    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL );

    // Geometry data
    Scalar * m_arrHeightField;
    Vertex2 m_vOrigin;
    Vector2 m_vSize;
    Scalar m_fMinHeight, m_fMaxHeight;

    UInt m_iSampleCountU;
    UInt m_iSampleCountV;

    Vector2 m_vSampleSize, m_vInvSampleSize;
    Scalar m_fSampleDiagSlope;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshHeightField.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_SURFACES_MESHHEIGHTFIELD_H

