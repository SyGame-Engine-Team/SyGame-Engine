/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Mesh.cpp
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
// Includes
#include "Mesh.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Mesh implementation
Mesh::Mesh()
{
    m_pIL = NULL;
    m_pVB = NULL;
    m_pIB = NULL;

    m_iVertexOffset = 0;
    m_iVertexCount = 0;
    m_iIndexOffset = 0;
    m_iIndexCount = 0;
}
Mesh::~Mesh()
{
    if ( m_pIB != NULL ) {
        m_pIB->UnBind();
        RenderingFn->DestroyResource( m_pIB );
    }
    if ( m_pVB != NULL ) {
        m_pVB->UnBind();
        RenderingFn->DestroyResource( m_pVB );
    }
}

Void Mesh::Transform( const Transform3 & vModelTransform, GPUDeferredContext * pContext )
{
    Assert( m_pIL != NULL && m_pVB != NULL );
    Assert( m_pIL->IsBound() && m_pVB->IsBound() );

    if ( vModelTransform.IsIdentity() )
        return;

    // Begin update
    Byte * pFirstVertex = NULL;
    if ( m_pVB->CanUpdate() ) {
        Assert( m_pVB->HasCPUData() );
        pFirstVertex = m_pVB->GetData( m_iVertexOffset );
    } else {
        Assert( m_pVB->CanLock() );
        UInt iByteSize = 0;
        pFirstVertex = (Byte*)( m_pVB->Lock( GPURESOURCE_LOCK_WRITE, 0, &iByteSize, pContext ) );
        Assert( iByteSize == m_pVB->GetSize() );
    }

    // Transform VB
    UInt iVertexSize = m_pVB->GetElementSize();

    UInt iOffset, iSize;
    Byte * arrPositions = NULL;
    Byte * arrNormals = NULL;
    Byte * arrTangents = NULL;
    Byte * arrBiNormals = NULL;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    arrPositions = ( pFirstVertex + iOffset );
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_NORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
        arrNormals = ( pFirstVertex + iOffset );
    }
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_TANGENT, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
        arrTangents = ( pFirstVertex + iOffset );
    }
    if ( m_pIL->HasField(GPUINPUTFIELD_SEMANTIC_BINORMAL, 0) ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
        arrTangents = ( pFirstVertex + iOffset );
    }
    
    Vertex4 * pPosition;
    Vector4 * pNormal;
    Vector4 * pTangent;
    Vector4 * pBiNormal;

    for( UInt i = 0; i < m_iVertexCount; ++i ) {
        pPosition = (Vertex4*)( arrPositions );
        *pPosition = ( vModelTransform * (*pPosition) );
        arrPositions += iVertexSize;

        if ( arrNormals != NULL ) {
            pNormal = (Vector4*)( arrNormals );
            *pNormal = ( vModelTransform * (*pNormal) );
            pNormal->Normalize();
            arrNormals += iVertexSize;
        }

        if ( arrTangents != NULL ) {
            pTangent = (Vector4*)( arrTangents );
            *pTangent = ( vModelTransform * (*pTangent) );
            pTangent->Normalize();
            arrTangents += iVertexSize;
        }

        if ( arrBiNormals != NULL ) {
            pBiNormal = (Vector4*)( arrBiNormals );
            *pBiNormal = ( vModelTransform * (*pBiNormal) );
            pBiNormal->Normalize();
            arrBiNormals += iVertexSize;
        }
    }

    // End update
    if ( m_pVB->CanUpdate() )
        m_pVB->Update( 0, INVALID_OFFSET, pContext );
    else
        m_pVB->UnLock( pContext );
}

/////////////////////////////////////////////////////////////////////////////////

Bool Mesh::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    //if ( m_pIL != NULL || m_pVB != NULL )
    //    return false;

    //MeshHeader hMeshHeader;
    //pRDB->ResourceRead( hResource, sizeof(MeshHeader), (Byte*)(&hMeshHeader) );

    //Assert( hMeshHeader.dwMeshType == (DWord)m_iMeshType );
    //Assert( hMeshHeader.dwVertexSize > 0 );
    //Assert( hMeshHeader.dwVertexCount > 0 );
    //Assert( hMeshHeader.dwDataSize == (hMeshHeader.dwVertexSize * hMeshHeader.dwVertexCount +
    //                                   hMeshHeader.dwIndexSize * hMeshHeader.dwIndexCount) );

    //m_pVF = RenderingFn->CreateVertexFormat();
    //for( UInt i = 0; i < hMeshHeader.dwVFFieldCount; ++i ) {
    //    m_pVF->DeclareField( (VFFieldType)( hMeshHeader.arrVFFields[i].dwType ),
    //                         (VFFieldUsage)( hMeshHeader.arrVFFields[i].dwUsage ),
    //                         hMeshHeader.arrVFFields[i].dwUsageIndex );
    //}
    //m_pVF->Finalize();

    //m_pVB = RenderingFn->CreateVertexBuffer( VB_USAGE_STATIC, hMeshHeader.dwVertexSize, hMeshHeader.dwVertexCount );
    //m_pIB = NULL;
    //if ( hMeshHeader.dwIndexCount > 0 ) {
    //    Assert( hMeshHeader.dwIndexSize > 0 );
    //    m_pIB = RenderingFn->CreateIndexBuffer( IB_USAGE_STATIC, hMeshHeader.dwIndexSize, hMeshHeader.dwIndexCount );
    //}

    //m_iVertexOffset = hMeshHeader.dwVertexOffset;
    //m_iIndexOffset = hMeshHeader.dwIndexOffset;

    //pRDB->ResourceRead( hResource, m_pVB->GetSize(), m_pVB->GetData() );
    //if ( m_pIB != NULL )
    //    pRDB->ResourceRead( hResource, m_pIB->GetSize(), m_pIB->GetData() );

    //return true;

    Assert( false );
    return false;
}
Bool Mesh::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    //if ( m_pIL == NULL && m_pVB == NULL )
    //    return false;

    //MeshHeader hMeshHeader;
    //hMeshHeader.dwDataSize = m_pVB->GetSize();
    //if ( m_pIB != NULL )
    //    hMeshHeader.dwDataSize += m_pIB->GetSize();

    //hMeshHeader.dwVFFieldCount = m_pVF->GetFieldCount();
    //for( UInt i = 0; i < hMeshHeader.dwVFFieldCount; ++i ) {
    //    hMeshHeader.arrVFFields[i].dwType = m_pVF->GetFieldType( i );
    //    hMeshHeader.arrVFFields[i].dwUsage = m_pVF->GetFieldUsage( i );
    //    hMeshHeader.arrVFFields[i].dwUsageIndex = m_pVF->GetFieldUsageIndex( i );
    //}
    //for( UInt i = hMeshHeader.dwVFFieldCount; i < 64; ++i ) {
    //    hMeshHeader.arrVFFields[i].dwType = 0;
    //    hMeshHeader.arrVFFields[i].dwUsage = 0;
    //    hMeshHeader.arrVFFields[i].dwUsageIndex = 0;
    //}

    //hMeshHeader.dwVertexSize = m_pVB->GetVertexSize();
    //hMeshHeader.dwVertexCount = m_pVB->GetVertexCount();
    //hMeshHeader.dwVertexOffset = m_iVertexOffset;

    //hMeshHeader.dwIndexSize = 0;
    //hMeshHeader.dwIndexCount = 0;
    //hMeshHeader.dwIndexOffset = 0;
    //if ( m_pIB != NULL ) {
    //    hMeshHeader.dwIndexSize = m_pIB->GetIndexSize();
    //    hMeshHeader.dwIndexCount = m_pIB->GetIndexCount();
    //    hMeshHeader.dwIndexOffset = m_iIndexOffset;
    //}

    //hMeshHeader.dwMeshType = m_iMeshType;

    //pRDB->ResourceAppend( hResource, sizeof(MeshHeader), (const Byte*)(&hMeshHeader) );
    //pRDB->ResourceAppend( hResource, m_pVB->GetSize(), m_pVB->GetData() );
    //if ( m_pIB != NULL )
    //    pRDB->ResourceAppend( hResource, m_pIB->GetSize(), m_pIB->GetData() );

    //return true;

    Assert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// PointMesh implementation
PointMesh::PointMesh():
    Mesh()
{
    // nothing to do
}
PointMesh::~PointMesh()
{
    // nothing to do
}

UInt PointMesh::GetPointCount() const
{
    if ( m_pIB == NULL )
        return GetVertexCount();
    return GetIndexCount();
}
Void PointMesh::GetPoint( UInt iPoint, UInt & outA ) const
{
    Assert( iPoint < GetPointCount() );

    outA = iPoint;

    if ( m_pIB == NULL )
        return;

    const Byte * pIndexA = m_pIB->GetData( m_iIndexOffset + outA );
    if ( m_pIB->UseShorts() )
        outA = *( (const Word*)pIndexA );
    else
        outA = *( (const DWord*)pIndexA );
}

/////////////////////////////////////////////////////////////////////////////////
// LineMesh implementation
LineMesh::LineMesh( Bool bAdjacency ):
    Mesh()
{
    m_bHasAdjacency = bAdjacency;
}
LineMesh::~LineMesh()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// LineListMesh implementation
LineListMesh::LineListMesh( Bool bAdjacency ):
    LineMesh( bAdjacency )
{
    // nothing to do
}
LineListMesh::~LineListMesh()
{
    // nothing to do
}

UInt LineListMesh::GetLineCount() const
{
    if ( m_pIB == NULL )
        return ( GetVertexCount() >> 1 );
    return ( GetIndexCount() >> 1 );
}
Void LineListMesh::GetLine( UInt iLine, UInt & outA, UInt & outB ) const
{
    Assert( iLine < GetLineCount() );

    outA = ( iLine << 1 );
    outB = outA + 1;

    if ( m_pIB == NULL )
        return;

    const Byte * pIndexA = m_pIB->GetData( m_iIndexOffset + outA );
    const Byte * pIndexB = m_pIB->GetData( m_iIndexOffset + outB );
    if ( m_pIB->UseShorts() ) {
        outA = *( (const Word*)pIndexA );
        outB = *( (const Word*)pIndexB );
    } else {
        outA = *( (const DWord*)pIndexA );
        outB = *( (const DWord*)pIndexB );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// LineStripMesh implementation
LineStripMesh::LineStripMesh( Bool bAdjacency ):
    LineMesh( bAdjacency )
{
    // nothing to do
}
LineStripMesh::~LineStripMesh()
{
    // nothing to do
}

UInt LineStripMesh::GetLineCount() const
{
    if ( m_pIB == NULL )
        return ( GetVertexCount() - 1 );
    return ( GetIndexCount() - 1 );
}
Void LineStripMesh::GetLine( UInt iLine, UInt & outA, UInt & outB ) const
{
    Assert( iLine < GetLineCount() );

    outA = iLine;
    outB = iLine + 1;

    if ( m_pIB == NULL )
        return;

    const Byte * pIndexA = m_pIB->GetData( m_iIndexOffset + outA );
    const Byte * pIndexB = m_pIB->GetData( m_iIndexOffset + outB );
    if ( m_pIB->UseShorts() ) {
        outA = *( (const Word*)pIndexA );
        outB = *( (const Word*)pIndexB );
    } else {
        outA = *( (const DWord*)pIndexA );
        outB = *( (const DWord*)pIndexB );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleMesh implementation
TriangleMesh::TriangleMesh( Bool bAdjacency ):
    Mesh()
{
    m_bHasAdjacency = bAdjacency;
}
TriangleMesh::~TriangleMesh()
{
    // nothing to do
}

Void TriangleMesh::UpdateNormals( GPUDeferredContext * pContext )
{
    Assert( m_pIL != NULL && m_pVB != NULL );
    Assert( m_pIL->IsBound() && m_pVB->IsBound() );

    Bool bHasNormals = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    if ( !bHasNormals )
        return;

    // Begin update
    Byte * pFirstVertex = NULL;
    if ( m_pVB->CanUpdate() ) {
        Assert( m_pVB->HasCPUData() );
        pFirstVertex = m_pVB->GetData( m_iVertexOffset );
    } else {
        Assert( m_pVB->CanLock() );
        UInt iByteSize = 0;
        pFirstVertex = (Byte*)( m_pVB->Lock( GPURESOURCE_LOCK_WRITE, 0, &iByteSize, pContext ) );
        Assert( iByteSize == m_pVB->GetSize() );
    }

    // Update normals
    UInt iVertexSize = m_pVB->GetElementSize();

    UInt iOffset, iSize;
    const Byte * arrPositions = NULL;
    Byte * arrNormals = NULL;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    arrPositions = ( pFirstVertex + iOffset );
    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    arrNormals = ( pFirstVertex + iOffset );

    UInt i, iTriangleCount = GetTriangleCount();

        // Set all normals to null vector
    Byte * pCurNormal = arrNormals;
    Vector4 * pNormal;

    for( i = 0; i < m_iVertexCount; ++i ) {
        pNormal = (Vector4*)pCurNormal;
        pNormal->X = 0.0f;
        pNormal->Y = 0.0f;
        pNormal->Z = 0.0f;
        pCurNormal += iVertexSize;
    }

        // Weighted sum of facet normals
    UInt iA, iB, iC;
    const Vertex4 *pA, *pB, *pC;

    Vector4 vAB, vAC, vFaceNormal;

    for( i = 0; i < iTriangleCount; ++i ) {
        GetTriangle( i, iA, iB, iC );
        iA *= iVertexSize;
        iB *= iVertexSize;
        iC *= iVertexSize;

        pA = (const Vertex4 *)( arrPositions + iA );
        pB = (const Vertex4 *)( arrPositions + iB );
        pC = (const Vertex4 *)( arrPositions + iC );

        vAB = ( *pB - *pA );
        vAC = ( *pC - *pA );
        vFaceNormal = ( vAB ^ vAC );
        vFaceNormal.Normalize();

        pNormal = (Vector4*)( arrNormals + iA );
        *pNormal += vFaceNormal;
        pNormal = (Vector4*)( arrNormals + iB );
        *pNormal += vFaceNormal;
        pNormal = (Vector4*)( arrNormals + iC );
        *pNormal += vFaceNormal;
    }

        // Normalize all again
    pCurNormal = arrNormals;
    for( i = 0; i < m_iVertexCount; ++i ) {
        pNormal = (Vector4*)pCurNormal;
        pNormal->Normalize();
        pCurNormal += iVertexSize;
    }

    // End update
    if ( m_pVB->CanUpdate() )
        m_pVB->Update( 0, INVALID_OFFSET, pContext );
    else
        m_pVB->UnLock( pContext );
}
Void TriangleMesh::UpdateTangentsFromGeometry( GPUDeferredContext * pContext )
{
    Assert( m_pIL != NULL && m_pVB != NULL );
    Assert( m_pIL->IsBound() && m_pVB->IsBound() );

    Bool bHasNormals = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    if ( !bHasNormals )
        return;
    Bool bHasTangents =  m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
    Bool bHasBiNormals = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
    if ( !bHasTangents && !bHasBiNormals )
        return;

    // Begin update
    Byte * pFirstVertex = NULL;
    if ( m_pVB->CanUpdate() ) {
        Assert( m_pVB->HasCPUData() );
        pFirstVertex = m_pVB->GetData( m_iVertexOffset );
    } else {
        Assert( m_pVB->CanLock() );
        UInt iByteSize = 0;
        pFirstVertex = (Byte*)( m_pVB->Lock( GPURESOURCE_LOCK_WRITE, 0, &iByteSize, pContext ) );
        Assert( iByteSize == m_pVB->GetSize() );
    }

    // Update
    UInt iVertexSize = m_pVB->GetElementSize();

    UInt iOffset, iSize;
    const Byte * arrPositions = NULL;
    const Byte * arrNormals = NULL;
    Byte * arrTangents = NULL;
    Byte * arrBiNormals = NULL;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    arrPositions = ( pFirstVertex + iOffset );
    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    arrNormals = ( pFirstVertex + iOffset );
    if ( bHasTangents ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
        arrTangents = ( pFirstVertex + iOffset );
    }
    if ( bHasBiNormals ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
        arrBiNormals = ( pFirstVertex + iOffset );
    }

    UInt i, iTriangleCount = GetTriangleCount();

        // Start using temp storage
    RenderingFn->SelectMemory( TEXT("Scratch") );
    Matrix4 * arrDerivateNormals = New Matrix4[m_iVertexCount];

        // Compute derivate normals (dN/dX)
    _ComputeDerivateNormals( arrDerivateNormals, m_iVertexCount, iTriangleCount,
                             arrPositions, arrNormals, iVertexSize );

        // Perform update
    const Vector4 * pNormal;
    Vector4 *pTangent, *pBiNormal;

    Vector4 vU, vV;
    Scalar fS01, fS10, fSAvg;
    Matrix2 matS;
    Scalar fTrace, fDet, fDiscr, fRootDiscr;
    Scalar fMinCurvature; //, fMaxCurvature;
    Vector2 vEigen0, vEigen1;
    Vector4 vTangent, vBinormal;

    for( i = 0; i < m_iVertexCount; ++i ) {
        iOffset = ( i * iVertexSize );
        pNormal = (const Vector4 *)( arrNormals + iOffset );

        // Compute J = [U V] such that (U,V,N) is orthonormal
        Vector4::MakeComplementBasis( vU, vV, *pNormal );

        // Compute shape matrix S = tJ * dN/dX * J, enforce symmetry
        fS01 = ( vU * (arrDerivateNormals[i] * vV) );
        fS10 = ( vV * (arrDerivateNormals[i] * vU) );
        fSAvg = ( fS01 + fS10 ) * 0.5f;
        matS.m00 = ( vU * (arrDerivateNormals[i] * vU) ); matS.m01 = fSAvg;
        matS.m10 = fSAvg;                                 matS.m11 = ( vV * (arrDerivateNormals[i] * vV) );

        // Eigenvalues of S are principal curvatures (k0 and k1)
        fTrace = ( matS.m00 + matS.m11 );
        fDet = matS.Determinant();
        fDiscr = ( fTrace * fTrace - 4.0f * fDet );
        fRootDiscr = MathFn->Sqrt( MathFn->Abs(fDiscr) );
        fMinCurvature = (fTrace - fRootDiscr) * 0.5f;
        //fMaxCurvature = (fTrace + fRootDiscr) * 0.5f;

        // Eigenvectors of S (W0 and W1)
        vEigen0.X = matS.m01;
        vEigen0.Y = fMinCurvature - matS.m00;
        vEigen1.X = fMinCurvature - matS.m11;
        vEigen1.Y = matS.m10;

        // Deduce principal directions (S*W=k*W => k=J*W) and assign them as tangent & binormal
        if ( vEigen0.NormSqr() >= vEigen1.NormSqr() ) {
            vEigen0.Normalize();
            vTangent = ( (vU * vEigen0.X) + (vV * vEigen0.Y) );
            vBinormal = ( (*pNormal) ^ vTangent );
        } else {
            vEigen1.Normalize();
            vTangent = ( (vU * vEigen1.X) + (vV * vEigen1.Y) );
            vBinormal = ( (*pNormal) ^ vTangent );
        }

        // Update
        if ( bHasTangents ) {
            pTangent = (Vector4*)( arrTangents + iOffset );
            *pTangent = vTangent;
        }
        if ( bHasBiNormals ) {
            pBiNormal = (Vector4*)( arrBiNormals + iOffset );
            *pBiNormal = vBinormal;
        }
    }

        // End using temp storage
    DeleteA( arrDerivateNormals );

    RenderingFn->UnSelectMemory();

    // End update
    if ( m_pVB->CanUpdate() )
        m_pVB->Update( 0, INVALID_OFFSET, pContext );
    else
        m_pVB->UnLock( pContext );
}
Void TriangleMesh::UpdateTangentsFromTexCoords( GPUDeferredContext * pContext )
{
    Assert( m_pIL != NULL && m_pVB != NULL );
    Assert( m_pIL->IsBound() && m_pVB->IsBound() );

    Bool bHasNormals = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    Bool bHasTexCoords = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
    if ( !bHasNormals || !bHasTexCoords )
        return;
    Bool bHasTangents =  m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
    Bool bHasBiNormals = m_pIL->HasField( GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
    if ( !bHasTangents && !bHasBiNormals )
        return;

    // Begin update
    Byte * pFirstVertex = NULL;
    if ( m_pVB->CanUpdate() ) {
        Assert( m_pVB->HasCPUData() );
        pFirstVertex = m_pVB->GetData( m_iVertexOffset );
    } else {
        Assert( m_pVB->CanLock() );
        UInt iByteSize = 0;
        pFirstVertex = (Byte*)( m_pVB->Lock( GPURESOURCE_LOCK_WRITE, 0, &iByteSize, pContext ) );
        Assert( iByteSize == m_pVB->GetSize() );
    }

    // Update
    UInt iVertexSize = m_pVB->GetElementSize();

    UInt iOffset, iSize;
    const Byte * arrPositions = NULL;
    const Byte * arrNormals = NULL;
    const Byte * arrTexCoords = NULL;
    Byte * arrTangents = NULL;
    Byte * arrBiNormals = NULL;

    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );
    arrPositions = ( pFirstVertex + iOffset );
    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_NORMAL, 0 );
    arrNormals = ( pFirstVertex + iOffset );
    m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TEXCOORD, 0 );
    arrTexCoords = ( pFirstVertex + iOffset );
    if ( bHasTangents ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_TANGENT, 0 );
        arrTangents = ( pFirstVertex + iOffset );
    }
    if ( bHasBiNormals ) {
        m_pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_BINORMAL, 0 );
        arrBiNormals = ( pFirstVertex + iOffset );
    }

    UInt i, iTriangleCount = GetTriangleCount();

        // Set all to null vector
    Byte *pCurTangent, *pCurBiNormal;
    Vector4 *pTangent, *pBiNormal;

    if ( bHasTangents ) {
        pCurTangent = arrTangents;
        for( i = 0; i < m_iVertexCount; ++i ) {
            pTangent = (Vector4*)pCurTangent;
            *pTangent = Vector4::Null;
            pCurTangent += iVertexSize;
        }
    } 
    if ( bHasBiNormals ) {
        pCurBiNormal = arrBiNormals;
        for( i = 0; i < m_iVertexCount; ++i ) {
            pBiNormal = (Vector4*)pCurBiNormal;
            *pBiNormal = Vector4::Null;
            pCurBiNormal += iVertexSize;
        }
    }

    // Visit all vertices
    UInt iA, iB, iC;
    const Vertex4 * arrPositionABC[3];
    const Vector4 * arrNormalABC[3];
    const TextureCoord2 * arrTexCoordABC[3];
    Vector4 * arrTangentABC[3];
    Vector4 * arrBiNormalABC[3];  

    UInt j, iNext, iPrev;
    Vector4 vCur;
    Vector4 vNormal, vTangent, vBiNormal;

    for( i = 0; i < iTriangleCount; ++i ) {
        GetTriangle( i, iA, iB, iC );
        iA *= iVertexSize;
        iB *= iVertexSize;
        iC *= iVertexSize;

        arrPositionABC[0] = (const Vertex4 *)( arrPositions + iA );
        arrPositionABC[1] = (const Vertex4 *)( arrPositions + iB );
        arrPositionABC[2] = (const Vertex4 *)( arrPositions + iC );
        arrNormalABC[0] = (const Vector4 *)( arrNormals + iA );
        arrNormalABC[1] = (const Vector4 *)( arrNormals + iB );
        arrNormalABC[2] = (const Vector4 *)( arrNormals + iC );
        arrTexCoordABC[0] = (const TextureCoord2 *)( arrTexCoords + iA );
        arrTexCoordABC[1] = (const TextureCoord2 *)( arrTexCoords + iB );
        arrTexCoordABC[2] = (const TextureCoord2 *)( arrTexCoords + iC );
        if ( bHasTangents ) {
            arrTangentABC[0] = (Vector4 *)( arrTangents + iA );
            arrTangentABC[1] = (Vector4 *)( arrTangents + iB );
            arrTangentABC[2] = (Vector4 *)( arrTangents + iC );
        }
        if ( bHasBiNormals ) {
            arrBiNormalABC[0] = (Vector4 *)( arrBiNormals + iA );
            arrBiNormalABC[1] = (Vector4 *)( arrBiNormals + iB );
            arrBiNormalABC[2] = (Vector4 *)( arrBiNormals + iC );
        }

        for( j = 0; j < 3; ++j ) {
            vCur = ( bHasTangents ) ? *(arrTangentABC[j]) : *(arrBiNormalABC[j]);
            if ( vCur.X != 0.0f || vCur.Y != 0.0f || vCur.Z != 0.0f )
                continue; // already visited

            iNext = (j+1) % 3;
            iPrev = (j+2) % 3;

            // Compute tangent & binormal
            vNormal = *( arrNormalABC[j] );            
            vTangent = _ComputeTexCoordTangent( *(arrPositionABC[j]), *(arrTexCoordABC[j]),
                                                *(arrPositionABC[iNext]), *(arrTexCoordABC[iNext]),
                                                *(arrPositionABC[iPrev]), *(arrTexCoordABC[iPrev]) );
            vTangent -= ( vNormal * (vNormal * vTangent) );
            vTangent.Normalize();
            vBiNormal = ( vNormal ^ vTangent );

            // Update
            if ( bHasTangents )
                *( arrTangentABC[j] ) = vTangent;
            if ( bHasBiNormals )
                *( arrBiNormalABC[j] ) = vBiNormal;
        }
    }

    // End update
    if ( m_pVB->CanUpdate() )
        m_pVB->Update( 0, INVALID_OFFSET, pContext );
    else
        m_pVB->UnLock( pContext );
}

/////////////////////////////////////////////////////////////////////////////////

Void TriangleMesh::_ComputeDerivateNormals( Matrix4 * outDerivateNormals, UInt iVertexCount, UInt iTriangleCount,
                                            const Byte * arrPositions, const Byte * arrNormals, UInt iVertexSize ) const
{
    // Create temp storage
    Matrix4 * arrProjTangents = New Matrix4[iVertexCount];
    Matrix4 * arrDeltaTangents = New Matrix4[iVertexCount];
    MemZero( arrProjTangents, iVertexCount * sizeof(Matrix4) );
    MemZero( arrDeltaTangents, iVertexCount * sizeof(Matrix4) );

    // Compute projected & delta tangents
    UInt i, iRow, iCol;
    UInt iTriangle[3];
    UInt iA, iB, iC;
    const Vertex4 *pA, *pB, *pC;
    const Vector4 *pNormA, *pNormB, *pNormC;

    Vector3 vEdge, vProj, vDelta;
    Matrix4 matInverse;

    for( i = 0; i < iTriangleCount; ++i ) {
        GetTriangle( i, iTriangle[0], iTriangle[1], iTriangle[2] );
        iTriangle[0] *= iVertexSize;
        iTriangle[1] *= iVertexSize;
        iTriangle[2] *= iVertexSize;

        for( UInt j = 0; j < 3; ++j ) {
            iA = iTriangle[j];
            iB = iTriangle[(j+1)%3];
            iC = iTriangle[(j+2)%3];
            pA = (const Vertex4 *)( arrPositions + iA );
            pB = (const Vertex4 *)( arrPositions + iB );
            pC = (const Vertex4 *)( arrPositions + iC );
            pNormA = (const Vector4 *)( arrNormals + iA );
            pNormB = (const Vector4 *)( arrNormals + iB );
            pNormC = (const Vector4 *)( arrNormals + iC );

            // Edge AB, project on tangent plane at A then
            // compute delta of adjacent normals
            vEdge = ( *pB - *pA );
            vProj = vEdge - ( (*pNormA) * (vEdge * (*pNormA)) );
            vDelta = ( (*pNormB) - (*pNormA) );
            for( iRow = 0; iRow < 3; ++iRow ) {
                for( iCol = 0; iCol < 3; ++iCol ) {
                    arrProjTangents[iA](iRow, iCol) += ( vProj[iRow] * vProj[iCol] );
                    arrDeltaTangents[iA](iRow, iCol) += ( vDelta[iRow] * vProj[iCol] );
                }
            }

            // Edge AC, project on tangent plane at A then
            // compute delta of adjacent normals
            vEdge = ( *pC - *pA );
            vProj = vEdge - ( (*pNormA) * (vEdge * (*pNormA)) );
            vDelta = ( (*pNormC) - (*pNormA) );
            for( iRow = 0; iRow < 3; ++iRow ) {
                for( iCol = 0; iCol < 3; ++iCol ) {
                    arrProjTangents[iA](iRow, iCol) += ( vProj[iRow] * vProj[iCol] );
                    arrDeltaTangents[iA](iRow, iCol) += ( vDelta[iRow] * vProj[iCol] );
                }
            }
        }
    }

    // Compute derivate normals
    for( i = 0; i < iVertexCount; ++i ) {
        pNormA = (const Vector4 *)( arrNormals + (i * iVertexSize) );

        // Add N*N^T to W*W^T for numerical stability.
        // In theory 0*0^T is added to D*W^T.
        for( iRow = 0; iRow < 3; ++iRow ) {
            for( iCol = 0; iCol < 3; ++iCol ) {
                arrProjTangents[i](iRow, iCol) *= 0.5f;
                arrProjTangents[i](iRow, iCol) += ( (*pNormA)[iRow] * (*pNormA)[iCol] );
                arrDeltaTangents[i](iRow, iCol) *= 0.5f;
            }
        }

        arrProjTangents[i].Invert( matInverse );
        outDerivateNormals[i] = ( arrDeltaTangents[i] * matInverse );
    }

    // Destroy temp storage
    DeleteA( arrProjTangents );
    DeleteA( arrDeltaTangents );
}
Vector3 TriangleMesh::_ComputeTexCoordTangent( const Vertex4 & vPosA, const TextureCoord2 & vTexCoordA,
                                               const Vertex4 & vPosB, const TextureCoord2 & vTexCoordB,
                                               const Vertex4 & vPosC, const TextureCoord2 & vTexCoordC ) const
{
    Vector4 vAB = ( vPosB - vPosA );
    Vector4 vAC = ( vPosC - vPosA );

    // Test for degenerate triangle
    if ( MathFn->EqualsZero(vAB.NormSqr()) || MathFn->EqualsZero(vAC.NormSqr()) )
        return Vector3::Null;

    // Delta along AB
    TextureCoord2 vDeltaAB = ( vTexCoordB - vTexCoordA );
    if ( MathFn->EqualsZero(vDeltaAB.U) ) {
        if ( MathFn->EqualsZero(vDeltaAB.T) )
            return Vector3::Null; // No variation at all
        return ( vAB * MathFn->Invert(vDeltaAB.T) ); // variation along T only
    }

    // Delta along AC
    TextureCoord2 vDeltaAC = ( vTexCoordC - vTexCoordA );
    Scalar fDet = ( (vDeltaAB.U * vDeltaAC.T) - (vDeltaAB.T * vDeltaAC.U) );
    if ( MathFn->EqualsZero(fDet) )
        return Vector3::Null; // Degenerate to line

    // Tangent
    return ( (vAC * vDeltaAB.U) - (vAB * vDeltaAC.U) ) * MathFn->Invert(fDet);
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleListMesh implementation
TriangleListMesh::TriangleListMesh( Bool bAdjacency ):
    TriangleMesh( bAdjacency )
{
    // nothing to do
}
TriangleListMesh::~TriangleListMesh()
{
    // nothing to do
}

UInt TriangleListMesh::GetTriangleCount() const
{
    if ( m_pIB == NULL )
        return ( GetVertexCount() / 3 );
    return ( GetIndexCount() / 3 );
}
Void TriangleListMesh::GetTriangle( UInt iTriangle, UInt & outA, UInt & outB, UInt & outC ) const
{
    Assert( iTriangle < GetTriangleCount() );

    outA = ( (iTriangle << 1) + iTriangle );
    outB = outA + 1;
    outC = outA + 2;

    if ( m_pIB == NULL )
        return;

    const Byte * pIndexA = m_pIB->GetData( m_iIndexOffset + outA );
    const Byte * pIndexB = m_pIB->GetData( m_iIndexOffset + outB );
    const Byte * pIndexC = m_pIB->GetData( m_iIndexOffset + outC );
    if ( m_pIB->UseShorts() ) {
        outA = *( (const Word*)pIndexA );
        outB = *( (const Word*)pIndexB );
        outC = *( (const Word*)pIndexC );
    } else {
        outA = *( (const DWord*)pIndexA );
        outB = *( (const DWord*)pIndexB );
        outC = *( (const DWord*)pIndexC );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// TriangleStripMesh implementation
TriangleStripMesh::TriangleStripMesh( Bool bAdjacency ):
    TriangleMesh( bAdjacency )
{
    // nothing to do
}
TriangleStripMesh::~TriangleStripMesh()
{
    // nothing to do
}

UInt TriangleStripMesh::GetTriangleCount() const
{
    if ( m_pIB == NULL )
        return ( GetVertexCount() - 2 );
    return ( GetIndexCount() - 2 );
}
Void TriangleStripMesh::GetTriangle( UInt iTriangle, UInt & outA, UInt & outB, UInt & outC ) const
{
    Assert( iTriangle < GetTriangleCount() );

    outA = iTriangle;
    if ( iTriangle & 1 ) {
        outB = iTriangle + 2;
        outC = iTriangle + 1;
    } else {
        outB = iTriangle + 1;
        outC = iTriangle + 2;
    }

    if ( m_pIB == NULL )
        return;

    const Byte * pIndexA = m_pIB->GetData( m_iIndexOffset + outA );
    const Byte * pIndexB = m_pIB->GetData( m_iIndexOffset + outB );
    const Byte * pIndexC = m_pIB->GetData( m_iIndexOffset + outC );
    if ( m_pIB->UseShorts() ) {
        outA = *( (const Word*)pIndexA );
        outB = *( (const Word*)pIndexB );
        outC = *( (const Word*)pIndexC );
    } else {
        outA = *( (const DWord*)pIndexA );
        outB = *( (const DWord*)pIndexB );
        outC = *( (const DWord*)pIndexC );
    }
}


