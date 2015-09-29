/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBFileStructure.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, file structure descriptors
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
#include "RDBFileStructure.h"

/////////////////////////////////////////////////////////////////////////////////
// Chunk sizes
UInt RDB_ChunkSize[RDB_CHUNK_COUNT] =
{
    sizeof(RDB_ChunkHeader) + sizeof(RDB_Chunk_BTreeNode), // RDB_CHUNK_NODE
    sizeof(RDB_ChunkHeader) + sizeof(RDB_Chunk_BTreeLeaf), // RDB_CHUNK_LEAF
    sizeof(RDB_ChunkHeader) + sizeof(RDB_Chunk_Data)       // RDB_CHUNK_DATA
};

// Given 3 chunk sizes, first 2 fixed, last one is tweakable in a
// reasonable range, let them be Size0, Size1, Size2.
// We assume Size2 > Size1 && Size2 > Size0 (realism & convenience).
// We want to minimize block padding ...
// Block size is then computed using the following code :
//
//UInt iMinPadding0 = 0xffffffff;
//UInt iMinPadding1 = 0xffffffff;
//UInt iMinCount;
//UInt iMinDataChunkSize;
//
//UInt iSize0, iSize1, iSize2;
//UInt iPadding0, iPadding1;
//for( UInt iDataChunkSize = 30000; iDataChunkSize < 35000; ++iDataChunkSize ) {
//    for( UInt iCount = 32; iCount < 256; ++iCount ) {
//        iSize0 = ( RDB_ChunkSize[0] * iCount );
//        iSize1 = ( RDB_ChunkSize[1] * iCount );
//        iSize2 = ( iDataChunkSize * iCount );
//        iPadding0 = ( iSize2 % RDB_ChunkSize[0] );
//        iPadding1 = ( iSize2 % RDB_ChunkSize[1] );
//        if ( iPadding0 < iMinPadding0 && iPadding1 < iMinPadding1 ) {
//            iMinPadding0 = iPadding0;
//            iMinPadding1 = iPadding1;
//            iMinCount = iCount;
//            iMinDataChunkSize = iDataChunkSize;
//        } else if ( iPadding0 < iMinPadding0 || iPadding1 < iMinPadding1 ) {
//            if ( (iPadding0 + iPadding1) < (iMinPadding0 + iMinPadding1) ) {
//                iMinPadding0 = iPadding0;
//                iMinPadding1 = iPadding1;
//                iMinCount = iCount;
//                iMinDataChunkSize = iDataChunkSize;
//            }
//        }
//    }
//}
//
// Best result :
// iMinPadding0 = 0
// iMinPadding1 = 80
// iMinCount = 36
// iMinDataChunkSize = 30780
// iBlockSize = (36 * RDB_ChunkSize[2]) = 1 108 080 bytes
//

