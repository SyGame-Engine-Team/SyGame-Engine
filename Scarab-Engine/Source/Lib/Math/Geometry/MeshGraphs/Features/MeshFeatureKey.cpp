/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeatureKey.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet feature keys for MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MeshFeatureKey.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshFeatureKey implementation
MeshFeatureKey::MeshFeatureKey()
{
    // nothing to do
}
MeshFeatureKey::~MeshFeatureKey()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexKey implementation
const MeshVertexKey MeshVertexKey::Null = MeshVertexKey();

MeshVertexKey::MeshVertexKey():
    MeshFeatureKey()
{
    V = INVALID_OFFSET;
}
MeshVertexKey::MeshVertexKey( UInt iV ):
    MeshFeatureKey()
{
    V = iV;
}
MeshVertexKey::MeshVertexKey( const MeshVertexKey & rhs ):
    MeshFeatureKey()
{
    V = rhs.V;
}
MeshVertexKey::~MeshVertexKey()
{
    // nothing to do
}

Int MeshVertexKey::Compare( const MeshVertexKey * const & rLeft, const MeshVertexKey * const & rRight, Void * /*pUserData*/ )
{
    if ( rLeft->V < rRight->V )
        return +1;
    if ( rRight->V < rLeft->V )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeKey implementation
const MeshEdgeKey MeshEdgeKey::Null = MeshEdgeKey();

MeshEdgeKey::MeshEdgeKey():
    MeshFeatureKey()
{
    A = INVALID_OFFSET;
    B = INVALID_OFFSET;
}
MeshEdgeKey::MeshEdgeKey( UInt iA, UInt iB ):
    MeshFeatureKey()
{
    // Forbid degenerate edges
    Assert( iA != iB );

    // Ensure unicity
    if ( iA < iB ) {
        A = iA;
        B = iB;
    } else {
        A = iB;
        B = iA;
    }
}
MeshEdgeKey::MeshEdgeKey( const MeshEdgeKey & rhs ):
    MeshFeatureKey()
{
    A = rhs.A;
    B = rhs.B;
}
MeshEdgeKey::~MeshEdgeKey()
{
    // nothing to do
}

Int MeshEdgeKey::Compare( const MeshEdgeKey * const & rLeft, const MeshEdgeKey * const & rRight, Void * /*pUserData*/ )
{
    if ( rLeft->A < rRight->A )
        return +1;
    if ( rRight->A < rLeft->A )
        return -1;
    if ( rLeft->B < rRight->B )
        return +1;
    if ( rRight->B < rLeft->B )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetKey implementation
MeshFacetKey::MeshFacetKey():
    MeshFeatureKey()
{
    // nothing to do
}
MeshFacetKey::~MeshFacetKey()
{
    // nothing to do
}

Int MeshFacetKey::Compare( const MeshFacetKey * const & rLeft, const MeshFacetKey * const & rRight, Void * pUserData )
{
    if ( rLeft->GetFacetType() == MESH_FACET_TRIANGLE ) {
        const MeshTriangleKey * pLeftTri = (const MeshTriangleKey *)rLeft;

        if ( rRight->GetFacetType() == MESH_FACET_TRIANGLE ) {
            const MeshTriangleKey * pRightTri = (const MeshTriangleKey *)rRight;

            return MeshTriangleKey::Compare( pLeftTri, pRightTri, pUserData );
        }

        if ( rRight->GetFacetType() == MESH_FACET_QUAD ) {
            const MeshQuadKey * pRightQuad = (const MeshQuadKey *)rRight;

            if ( pLeftTri->A < pRightQuad->A )
                return +1;
            if ( pRightQuad->A < pLeftTri->A )
                return -1;
            if ( pLeftTri->B < pRightQuad->B )
                return +1;
            if ( pRightQuad->B < pLeftTri->B )
                return -1;
            if ( pLeftTri->C < pRightQuad->C )
                return +1;
            if ( pRightQuad->C < pLeftTri->C )
                return -1;

            return +1;
        }

        Assert( rRight->GetFacetType() == MESH_FACET_POLYGON );
        const MeshPolygonKey * pRightPoly = (const MeshPolygonKey *)rRight;

        if ( pLeftTri->A < pRightPoly->V[0] )
            return +1;
        if ( pRightPoly->V[0] < pLeftTri->A )
            return -1;
        if ( pLeftTri->B < pRightPoly->V[1] )
            return +1;
        if ( pRightPoly->V[1] < pLeftTri->B )
            return -1;
        if ( pLeftTri->C < pRightPoly->V[2] )
            return +1;
        if ( pRightPoly->V[2] < pLeftTri->C )
            return -1;

        if ( pRightPoly->SIZE == 3 )
            return 0;

        return +1;
    }

    if ( rLeft->GetFacetType() == MESH_FACET_QUAD ) {
        const MeshQuadKey * pLeftQuad = (const MeshQuadKey *)rLeft;

        if ( rRight->GetFacetType() == MESH_FACET_TRIANGLE ) {
            const MeshTriangleKey * pRightTri = (const MeshTriangleKey *)rRight;

            if ( pLeftQuad->A < pRightTri->A )
                return +1;
            if ( pRightTri->A < pLeftQuad->A )
                return -1;
            if ( pLeftQuad->B < pRightTri->B )
                return +1;
            if ( pRightTri->B < pLeftQuad->B )
                return -1;
            if ( pLeftQuad->C < pRightTri->C )
                return +1;
            if ( pRightTri->C < pLeftQuad->C )
                return -1;

            return -1;
        }

        if ( rRight->GetFacetType() == MESH_FACET_QUAD ) {
            const MeshQuadKey * pRightQuad = (const MeshQuadKey *)rRight;

            return MeshQuadKey::Compare( pLeftQuad, pRightQuad, pUserData );
        }

        Assert( rRight->GetFacetType() == MESH_FACET_POLYGON );
        const MeshPolygonKey * pRightPoly = (const MeshPolygonKey *)rRight;

        if ( pLeftQuad->A < pRightPoly->V[0] )
            return +1;
        if ( pRightPoly->V[0] < pLeftQuad->A )
            return -1;
        if ( pLeftQuad->B < pRightPoly->V[1] )
            return +1;
        if ( pRightPoly->V[1] < pLeftQuad->B )
            return -1;
        if ( pLeftQuad->C < pRightPoly->V[2] )
            return +1;
        if ( pRightPoly->V[2] < pLeftQuad->C )
            return -1;

        if ( pRightPoly->SIZE == 3 )
            return -1;

        if ( pLeftQuad->D < pRightPoly->V[3] )
            return +1;
        if ( pRightPoly->V[3] < pLeftQuad->D )
            return -1;

        if ( pRightPoly->SIZE == 4 )
            return 0;

        return +1;
    }

    Assert( rLeft->GetFacetType() == MESH_FACET_POLYGON );
    const MeshPolygonKey * pLeftPoly = (const MeshPolygonKey *)rLeft;

    if ( rRight->GetFacetType() == MESH_FACET_TRIANGLE ) {
        const MeshTriangleKey * pRightTri = (const MeshTriangleKey *)rRight;

        if ( pLeftPoly->V[0] < pRightTri->A )
            return +1;
        if ( pRightTri->A < pLeftPoly->V[0] )
            return -1;
        if ( pLeftPoly->V[1] < pRightTri->B )
            return +1;
        if ( pRightTri->B < pLeftPoly->V[1] )
            return -1;
        if ( pLeftPoly->V[2] < pRightTri->C )
            return +1;
        if ( pRightTri->C < pLeftPoly->V[2] )
            return -1;

        if ( pLeftPoly->SIZE == 3 )
            return 0;

        return -1;
    }

    if ( rRight->GetFacetType() == MESH_FACET_QUAD ) {
        const MeshQuadKey * pRightQuad = (const MeshQuadKey *)rRight;

        if ( pLeftPoly->V[0] < pRightQuad->A )
            return +1;
        if ( pRightQuad->A < pLeftPoly->V[0] )
            return -1;
        if ( pLeftPoly->V[1] < pRightQuad->B )
            return +1;
        if ( pRightQuad->B < pLeftPoly->V[1] )
            return -1;
        if ( pLeftPoly->V[2] < pRightQuad->C )
            return +1;
        if ( pRightQuad->C < pLeftPoly->V[2] )
            return -1;

        if ( pLeftPoly->SIZE == 3 )
            return +1;

        if ( pLeftPoly->V[3] < pRightQuad->D )
            return +1;
        if ( pRightQuad->D < pLeftPoly->V[3] )
            return -1;

        if ( pLeftPoly->SIZE == 4 )
            return 0;

        return -1;
    }

    Assert( rRight->GetFacetType() == MESH_FACET_POLYGON );
    const MeshPolygonKey * pRightPoly = (const MeshPolygonKey *)rRight;

    return MeshPolygonKey::Compare( pLeftPoly, pRightPoly, pUserData );
}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleKey implementation
const MeshTriangleKey MeshTriangleKey::Null = MeshTriangleKey();

MeshTriangleKey::MeshTriangleKey():
    MeshFacetKey()
{
    A = INVALID_OFFSET;
    B = INVALID_OFFSET;
    C = INVALID_OFFSET;
}
MeshTriangleKey::MeshTriangleKey( UInt iA, UInt iB, UInt iC ):
    MeshFacetKey()
{
    // Forbid degenerate facets
    Assert( iA != iB );
    Assert( iA != iC );
    Assert( iB != iC );

    // Ensure unicity, keep orientation
    if ( iA < iB ) {
        if ( iA < iC ) {
            A = iA;
            B = iB;
            C = iC;
        } else {
            A = iC;
            B = iA;
            C = iB;
        }
    } else {
        if ( iB < iC ) {
            A = iB;
            B = iC;
            C = iA;
        } else {
            A = iC;
            B = iA;
            C = iB;
        }
    }
}
MeshTriangleKey::MeshTriangleKey( const MeshPolygonKey & rhs ):
    MeshFacetKey()
{
    Assert( rhs.SIZE == 3 );
    A = rhs.V[0];
    B = rhs.V[1];
    C = rhs.V[2];
}
MeshTriangleKey::MeshTriangleKey( const MeshTriangleKey & rhs ):
    MeshFacetKey()
{
    A = rhs.A;
    B = rhs.B;
    C = rhs.C;
}
MeshTriangleKey::~MeshTriangleKey()
{
    // nothing to do
}

Bool MeshTriangleKey::operator==( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return false;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_TRIANGLE )
        return false;

    const MeshTriangleKey & rhsKey = (const MeshTriangleKey &)rhsFacet;
    return ( ( A == rhsKey.A ) && ( B == rhsKey.B ) && ( C == rhsKey.C ) );
}
Bool MeshTriangleKey::operator!=( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return true;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_TRIANGLE )
        return true;

    const MeshTriangleKey & rhsKey = (const MeshTriangleKey &)rhsFacet;
    return ( ( A != rhsKey.A ) || ( B != rhsKey.B ) || ( C != rhsKey.C ) );
}

Int MeshTriangleKey::Compare( const MeshTriangleKey * const & rLeft, const MeshTriangleKey * const & rRight, Void * /*pUserData*/ )
{
    if ( rLeft->A < rRight->A )
        return +1;
    if ( rRight->A < rLeft->A )
        return -1;
    if ( rLeft->B < rRight->B )
        return +1;
    if ( rRight->B < rLeft->B )
        return -1;
    if ( rLeft->C < rRight->C )
        return +1;
    if ( rRight->C < rLeft->C )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadKey implementation
const MeshQuadKey MeshQuadKey::Null = MeshQuadKey();

MeshQuadKey::MeshQuadKey():
    MeshFacetKey()
{
    A = INVALID_OFFSET;
    B = INVALID_OFFSET;
    C = INVALID_OFFSET;
    D = INVALID_OFFSET;
}
MeshQuadKey::MeshQuadKey( UInt iA, UInt iB, UInt iC, UInt iD ):
    MeshFacetKey()
{
    // Forbid degenerate facets
    Assert( iA != iB );
    Assert( iA != iC );
    Assert( iA != iD );
    Assert( iB != iC );
    Assert( iB != iD );
    Assert( iC != iD );

    // Ensure unicity, keep orientation
    if ( iA < iB ) {
        if ( iA < iC ) {
            if ( iA < iD ) {
                A = iA;
                B = iB;
                C = iC;
                D = iD;
            } else {
                A = iD;
                B = iA;
                C = iB;
                D = iC;
            }
        } else {
            if ( iC < iD ) {
                A = iC;
                B = iD;
                C = iA;
                D = iB;
            } else {
                A = iD;
                B = iA;
                C = iB;
                D = iC;
            }
        }
    } else {
        if ( iB < iC ) {
            if ( iB < iD ) {
                A = iB;
                B = iC;
                C = iD;
                D = iA;
            } else {
                A = iD;
                B = iA;
                C = iB;
                D = iC;
            }
        } else {
            if ( iC < iD ) {
                A = iC;
                B = iD;
                C = iA;
                D = iB;
            } else {
                A = iD;
                B = iA;
                C = iB;
                D = iC;
            }
        }
    }
}
MeshQuadKey::MeshQuadKey( const MeshPolygonKey & rhs ):
    MeshFacetKey()
{
    Assert( rhs.SIZE == 4 );
    A = rhs.V[0];
    B = rhs.V[1];
    C = rhs.V[2];
    D = rhs.V[3];
}
MeshQuadKey::MeshQuadKey( const MeshQuadKey & rhs ):
    MeshFacetKey()
{
    A = rhs.A;
    B = rhs.B;
    C = rhs.C;
    D = rhs.D;
}
MeshQuadKey::~MeshQuadKey()
{
    // nothing to do
}

Bool MeshQuadKey::operator==( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return false;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_QUAD )
        return false;

    const MeshQuadKey & rhsKey = (const MeshQuadKey &)rhsFacet;
    return ( ( A == rhsKey.A ) && ( B == rhsKey.B ) && ( C == rhsKey.C ) && ( D == rhsKey.D ) );
}
Bool MeshQuadKey::operator!=( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return true;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_QUAD )
        return true;

    const MeshQuadKey & rhsKey = (const MeshQuadKey &)rhsFacet;
    return ( ( A != rhsKey.A ) || ( B != rhsKey.B ) || ( C != rhsKey.C ) || ( D != rhsKey.D ) );
}

Int MeshQuadKey::Compare( const MeshQuadKey * const & rLeft, const MeshQuadKey * const & rRight, Void * /*pUserData*/ )
{
    if ( rLeft->A < rRight->A )
        return +1;
    if ( rRight->A < rLeft->A )
        return -1;
    if ( rLeft->B < rRight->B )
        return +1;
    if ( rRight->B < rLeft->B )
        return -1;
    if ( rLeft->C < rRight->C )
        return +1;
    if ( rRight->C < rLeft->C )
        return -1;
    if ( rLeft->D < rRight->D )
        return +1;
    if ( rRight->D < rLeft->D )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonKey implementation
const MeshPolygonKey MeshPolygonKey::Null = MeshPolygonKey();

MeshPolygonKey::MeshPolygonKey():
    MeshFacetKey(), SIZE(0)
{
    V = NULL;
}
MeshPolygonKey::MeshPolygonKey( UInt iSize ):
    MeshFacetKey(), SIZE(iSize)
{
    // Create array
    V = New UInt[SIZE];

    for( UInt i = 0; i < SIZE; ++i )
        V[i] = INVALID_OFFSET;
}
MeshPolygonKey::MeshPolygonKey( const UInt * arrVertices, UInt iSize ):
    MeshFacetKey(), SIZE(iSize)
{
    Assert( arrVertices != NULL );
    Assert( SIZE >= 3 );

    // Create array
    V = New UInt[SIZE];

    // Forbid degenerate facets
    UInt i, j;
    for( i = 0; i < (SIZE - 1); ++i ) {
        for( j = (i + 1); j < SIZE; ++j )
            Assert( arrVertices[i] != arrVertices[j] );
    }

    // Ensure unicity, keep orientation
    UInt iMin = 0;
    for( i = 1; i < SIZE; ++i ) {
        if ( arrVertices[i] < arrVertices[iMin] )
            iMin = i;
    }
    i = 0;
    for( j = iMin; j < SIZE; ++j )
        V[i++] = arrVertices[j];
    for( j = 0; j < iMin; ++j )
        V[i++] = arrVertices[j];
}
MeshPolygonKey::MeshPolygonKey( const MeshTriangleKey & rhs ):
    MeshFacetKey(), SIZE(3)
{
    // Create array
    V = New UInt[SIZE];

    V[0] = rhs.A;
    V[1] = rhs.B;
    V[2] = rhs.C;
}
MeshPolygonKey::MeshPolygonKey( const MeshQuadKey & rhs ):
    MeshFacetKey(), SIZE(4)
{
    // Create array
    V = New UInt[SIZE];

    V[0] = rhs.A;
    V[1] = rhs.B;
    V[2] = rhs.C;
    V[3] = rhs.D;
}
MeshPolygonKey::MeshPolygonKey( const MeshPolygonKey & rhs ):
    MeshFacetKey(), SIZE(rhs.SIZE)
{
    // Create array
    V = New UInt[SIZE];

    for( UInt i = 0; i < SIZE; ++i )
        V[i] = rhs.V[i];
}
MeshPolygonKey::~MeshPolygonKey()
{
    // Destroy array
    if ( V != NULL )
        DeleteA( V );
}

Bool MeshPolygonKey::operator==( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return false;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_POLYGON )
        return false;

    const MeshPolygonKey & rhsKey = (const MeshPolygonKey &)rhsFacet;
    if ( rhsKey.SIZE != SIZE )
        return false;

    for( UInt i = 0; i < SIZE; ++i ) {
        if ( V[i] != rhsKey.V[i] )
            return false;
    }
    return true;
}
Bool MeshPolygonKey::operator!=( const MeshFeatureKey & rhs ) const
{
    if ( rhs.GetType() != MESH_FEATURE_FACET )
        return true;

    const MeshFacetKey & rhsFacet = (const MeshFacetKey &)rhs;
    if ( rhsFacet.GetFacetType() != MESH_FACET_POLYGON )
        return true;

    const MeshPolygonKey & rhsKey = (const MeshPolygonKey &)rhsFacet;
    if ( rhsKey.SIZE != SIZE )
        return true;

    for( UInt i = 0; i < SIZE; ++i ) {
        if ( V[i] != rhsKey.V[i] )
            return true;
    }
    return false;
}

Int MeshPolygonKey::Compare( const MeshPolygonKey * const & rLeft, const MeshPolygonKey * const & rRight, Void * /*pUserData*/ )
{
    UInt i = 0;
    while( true ) {
        if ( i == rLeft->SIZE ) {
            if ( i == rRight->SIZE )
                return 0;
            return +1;
        }
        if ( i == rRight->SIZE )
            return -1;

        if ( rLeft->V[i] < rRight->V[i] )
            return +1;
        if ( rRight->V[i] < rLeft->V[i] )
            return -1;

        ++i;
    }
}

