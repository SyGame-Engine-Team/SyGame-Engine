/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Mesh.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Universal Mesh container for rendering geometry
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_MESH_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_MESH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/Matrix/Matrix2.h"
#include "../../../Lib/Math/Types/Transform/Transform3.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord1.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord2.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord3.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord4.h"

#include "../../Resource/Resource.h"

#include "GPUInputLayout.h"
#include "GPUBuffer.h"
#include "GPUShader.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Mesh class
class Mesh : public Resource
{
public:
	Mesh();
	virtual ~Mesh();

    // Resource type
    inline virtual ResourceType GetResourceType() const;

    // Delayed construction
    inline Void SetIL( GPUInputLayout * pIL );
    inline Void SetVB( GPUVertexBuffer * pVB );
    inline Void SetIB( GPUIndexBuffer * pIB );

    inline Void SetVertexRange( UInt iVertexOffset, UInt iVertexCount );
    inline Void SetIndexRange( UInt iIndexOffset, UInt iIndexCount );

    // Mesh type
    virtual GPUShaderPrimitiveTopology GetTopology() const = 0;

    // Getters
    inline Bool IsPointMesh() const;
    inline Bool IsLineMesh() const;
    inline Bool IsTriangleMesh() const;
    inline Bool IsPatchList() const;

    inline GPUInputLayout * GetIL() const;
    inline GPUVertexBuffer * GetVB() const;
    inline GPUIndexBuffer * GetIB() const;

    // Vertices access
    inline UInt GetVertexSize() const;
    inline UInt GetVertexCapacity() const;
    inline UInt GetVertexCount() const;
    inline UInt GetVertexOffset() const;

    // Indices access
    inline Bool HasIndexBuffer() const;
    inline UInt GetIndexSize() const;
    inline UInt GetIndexCapacity() const;
    inline UInt GetIndexCount() const;
    inline UInt GetIndexOffset() const;

    // Facets access
    virtual UInt GetFacetCount() const = 0;

    // Model transform support
    Void Transform( const Transform3 & vModelTransform, GPUDeferredContext * pContext = NULL );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Dynamic update support
    virtual Void _Update( GPUDeferredContext * pContext = NULL ) = 0;

    GPUInputLayout * m_pIL;
    GPUVertexBuffer * m_pVB;
    GPUIndexBuffer * m_pIB;

    UInt m_iVertexOffset, m_iVertexCount;
    UInt m_iIndexOffset, m_iIndexCount;
};

/////////////////////////////////////////////////////////////////////////////////
// The PointMesh class
class PointMesh : public Mesh
{
public:
	PointMesh();
	virtual ~PointMesh();

    // Mesh type
    inline virtual GPUShaderPrimitiveTopology GetTopology() const;

    // Getters
    inline virtual UInt GetFacetCount() const;

    virtual UInt GetPointCount() const;
    virtual Void GetPoint( UInt iPoint, UInt & outA ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The LineMesh class
class LineMesh : public Mesh
{
public:
	LineMesh( Bool bAdjacency = false );
	virtual ~LineMesh();

    // Mesh type
    virtual GPUShaderPrimitiveTopology GetTopology() const = 0;

    // Getters
    inline virtual UInt GetFacetCount() const;

    inline Bool HasAdjacency() const;

    virtual UInt GetLineCount() const = 0;
    virtual Void GetLine( UInt iLine, UInt & outA, UInt & outB ) const = 0;

protected:
    Bool m_bHasAdjacency;
};

/////////////////////////////////////////////////////////////////////////////////
// The LineListMesh class
class LineListMesh : public LineMesh
{
public:
	LineListMesh( Bool bAdjacency = false );
	virtual ~LineListMesh();

    // Mesh type
    inline virtual GPUShaderPrimitiveTopology GetTopology() const;

    // Getters
    inline Bool HasAdjacency() const;

    virtual UInt GetLineCount() const;
    virtual Void GetLine( UInt iLine, UInt & outA, UInt & outB ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The LineStripMesh class
class LineStripMesh : public LineMesh
{
public:
	LineStripMesh( Bool bAdjacency = false );
	virtual ~LineStripMesh();

    // Mesh type
    inline virtual GPUShaderPrimitiveTopology GetTopology() const;

    // Getters

    virtual UInt GetLineCount() const;
    virtual Void GetLine( UInt iLine, UInt & outA, UInt & outB ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The TriangleMesh class
class TriangleMesh : public Mesh
{
public:
	TriangleMesh( Bool bAdjacency = false );
	virtual ~TriangleMesh();

    // Mesh type
    virtual GPUShaderPrimitiveTopology GetTopology() const = 0;

    // Getters
    inline virtual UInt GetFacetCount() const;

    inline Bool HasAdjacency() const;

    virtual UInt GetTriangleCount() const = 0;
    virtual Void GetTriangle( UInt iTriangle, UInt & outA, UInt & outB, UInt & outC ) const = 0;

    // Methods
    Void UpdateNormals( GPUDeferredContext * pContext = NULL );
    Void UpdateTangentsFromGeometry( GPUDeferredContext * pContext = NULL );
    Void UpdateTangentsFromTexCoords( GPUDeferredContext * pContext = NULL );

protected:
    Bool m_bHasAdjacency;

private:
    // Helpers
    Void _ComputeDerivateNormals( Matrix4 * outDerivateNormals, UInt iVertexCount, UInt iTriangleCount,
                                  const Byte * arrPositions, const Byte * arrNormals, UInt iVertexSize ) const;
    Vector3 _ComputeTexCoordTangent( const Vertex4 & vPosA, const TextureCoord2 & vTexCoordA,
                                     const Vertex4 & vPosB, const TextureCoord2 & vTexCoordB,
                                     const Vertex4 & vPosC, const TextureCoord2 & vTexCoordC ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The TriangleListMesh class
class TriangleListMesh : public TriangleMesh
{
public:
	TriangleListMesh( Bool bAdjacency = false );
	virtual ~TriangleListMesh();

    // Mesh type
    inline virtual GPUShaderPrimitiveTopology GetTopology() const;

    // Getters
    virtual UInt GetTriangleCount() const;
    virtual Void GetTriangle( UInt iTriangle, UInt & outA, UInt & outB, UInt & outC ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The TriangleStripMesh class
class TriangleStripMesh : public TriangleMesh
{
public:
	TriangleStripMesh( Bool bAdjacency = false );
	virtual ~TriangleStripMesh();

    // Mesh type
    inline virtual GPUShaderPrimitiveTopology GetTopology() const;

    // Getters
    virtual UInt GetTriangleCount() const;
    virtual Void GetTriangle( UInt iTriangle, UInt & outA, UInt & outB, UInt & outC ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Mesh.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_MESH_H
