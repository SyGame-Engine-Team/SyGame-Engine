/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUTexture.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Textures
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Please take care of GPUTexture2D's multisampling patterns :
// With 2 samples (A,B) =>
//  -----------------------------------------------------------------
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -8
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -7
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -6
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -5
//  |   |   |   |   | B |   |   |   | | |   |   |   |   |   |   |   | -4
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -3
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -2
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -1
//  |---|---|---|---|---|---|---|---|-|-|---|---|---|---|---|---|---| 0
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 1
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 2
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 3
//  |   |   |   |   |   |   |   |   | | |   |   |   | A |   |   |   | 4
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 5
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 6
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 7
//  -----------------------------------------------------------------
//   -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7
//
// With 4 samples (A,B,C,D) =>
//  -----------------------------------------------------------------
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -8
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -7
//  |   |   |   |   |   |   | A |   | | |   |   |   |   |   |   |   | -6
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -5
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -4
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -3
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   | B |   | -2
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -1
//  |---|---|---|---|---|---|---|---|-|-|---|---|---|---|---|---|---| 0
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 1
//  |   |   | C |   |   |   |   |   | | |   |   |   |   |   |   |   | 2
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 3
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 4
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 5
//  |   |   |   |   |   |   |   |   | | |   | D |   |   |   |   |   | 6
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 7
//  -----------------------------------------------------------------
//   -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7
//
// With 8 samples (A,B,C,D,E,F,G,H) =>
//  -----------------------------------------------------------------
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -8
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   | H | -7
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -6
//  |   |   |   |   |   | D |   |   | | |   |   |   |   |   |   |   | -5
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -4
//  |   |   |   |   |   |   |   |   | | | A |   |   |   |   |   |   | -3
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -2
//  |   | F |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -1
//  |---|---|---|---|---|---|---|---|-|-|---|---|---|---|---|---|---| 0
//  |   |   |   |   |   |   |   |   | | |   |   |   |   | C |   |   | 1
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 2
//  |   |   |   |   |   |   |   | B | | |   |   |   |   |   |   |   | 3
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 4
//  |   |   |   | E |   |   |   |   | | |   |   |   |   |   |   |   | 5
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   |   | 6
//  |   |   |   |   |   |   |   |   | | |   |   | G |   |   |   |   | 7
//  -----------------------------------------------------------------
//   -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7
//
// With 16 samples (A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P) =>
//  -----------------------------------------------------------------
//  |   | P |   |   |   |   |   |   | | |   |   |   |   |   |   |   | -8
//  |   |   |   |   |   |   |   |   | J |   |   |   |   |   |   |   | -7
//  |   |   |   |   | K |   |   |   | | |   |   |   |   |   |   |   | -6
//  |   |   |   |   |   |   |   |   | | |   |   | H |   |   |   |   | -5
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   |   | N | -4
//  |   |   |   |   |   |   |   | B | | |   |   |   |   |   |   |   | -3
//  |   |   |   | E |   |   |   |   | | |   |   |   |   |   |   |   | -2
//  |   |   |   |   |   |   |   |   | | |   |   |   | D |   |   |   | -1
//  |-M-|---|---|---|---|---|---|---|-|-|---|---|---|---|---|---|---| 0
//  |   |   |   |   |   |   |   |   | | | A |   |   |   |   |   |   | 1
//  |   |   |   |   |   | C |   |   | | |   |   |   |   |   |   |   | 2
//  |   |   |   |   |   |   |   |   | | |   |   |   |   | G |   |   | 3
//  |   |   | L |   |   |   |   |   | | |   |   |   |   |   |   |   | 4
//  |   |   |   |   |   |   |   |   | | |   | F |   |   |   |   |   | 5
//  |   |   |   |   |   |   | I |   | | |   |   |   |   |   |   |   | 6
//  |   |   |   |   |   |   |   |   | | |   |   |   |   |   | O |   | 7
//  -----------------------------------------------------------------
//   -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUTEXTURE_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUTEXTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUResource.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUTexture definitions
#define GPUTEXTURE_MAX_DIMENSIONS DEVICETEXTURE_MAX_DIMENSIONS // 3

#define GPUTEXTURE_MAX_MIPMAP_LEVELS 16 // Texture dimensions up to 65536 ...
#define GPUTEXTURE_MIPMAP_NONE       DEVICETEXTURE_MIPLEVELS_NONE // 1
#define GPUTEXTURE_MIPMAP_AUTO       DEVICETEXTURE_MIPLEVELS_AUTO // 0

enum GPUTextureCubeFace {
    GPUTEXTURE_CUBEFACE_X_POS = DEVICETEXTURE_CUBEFACE_X_POS,
    GPUTEXTURE_CUBEFACE_X_NEG = DEVICETEXTURE_CUBEFACE_X_NEG,
    GPUTEXTURE_CUBEFACE_Y_POS = DEVICETEXTURE_CUBEFACE_Y_POS,
    GPUTEXTURE_CUBEFACE_Y_NEG = DEVICETEXTURE_CUBEFACE_Y_NEG,
    GPUTEXTURE_CUBEFACE_Z_POS = DEVICETEXTURE_CUBEFACE_Z_POS,
    GPUTEXTURE_CUBEFACE_Z_NEG = DEVICETEXTURE_CUBEFACE_Z_NEG
};

typedef struct _gpu_texture_index {
    UInt iArrayIndex, iMipIndex;
} GPUTextureIndex;

typedef struct _gpu_point2 {
    UInt iX;
    UInt iY;
} GPUPoint2;

typedef struct _gpu_point3 {
    UInt iX;
    UInt iY;
    UInt iZ;
} GPUPoint3;

typedef struct _gpu_segment {
    UInt iLeft;
    UInt iRight;
} GPUSegment;

typedef struct _gpu_rectangle {
    Long iLeft;
    Long iTop;
    Long iRight;
    Long iBottom;
} GPURectangle;

typedef struct _gpu_box {
    UInt iLeft;
    UInt iTop;
    UInt iFront;
    UInt iRight;
    UInt iBottom;
    UInt iBack;
} GPUBox;

// Prototypes
class RenderingManager;
class GPU2DContext;

/////////////////////////////////////////////////////////////////////////////////
// The GPUTexture class
class GPUTexture : public GPUResource
{
    // Texture Dimensions infos
public:
    inline static UInt GetDimensions( GPUResourceType iTextureType );
protected:
    static UInt sm_arrDimensions[4];

protected:
    friend class RenderingManager;
    GPUTexture( PixelFormat iFormat, UInt iDimensions, UInt arrBounds[GPUTEXTURE_MAX_DIMENSIONS], UInt iMipLevelCount, UInt iArrayCount, UInt iSampleCount, const Byte * arrTexels, Bool bCreateCPUData = true );
    virtual ~GPUTexture();

public:
    // Binding
    inline Bool IsBound() const;

    virtual Void Bind( UInt iGPUResourceBinds ) = 0;
    Void UnBind();

    inline Bool HasCPUData() const;

    Void DestroyCPUData(); // Content will no longer be safe from device loss.
    Void RestoreCPUData(); // Content is NOT restored, user must either use UpdateBack when possible
                           // or manually refill buffer data then commit to GPU.

    // Getters
    inline virtual ResourceType GetResourceType() const;

    inline Bool IsMipMapable() const;
    inline Bool IsCompressed() const;

    inline PixelFormat GetFormat() const;
    inline UInt GetStride() const;

    inline UInt GetDimensions() const;

    inline Bool HasMipMap() const;
    inline UInt GetMipMapSize() const;
    inline UInt GetMipLevelCount() const;
    inline UInt GetMipLevelBound( UInt iDimension, UInt iMipLevel ) const;
    inline UInt GetMipLevelOffset( UInt iMipLevel ) const;
    inline UInt GetMipLevelSize( UInt iMipLevel ) const;

    inline Bool IsArray() const;
    inline UInt GetArrayCount() const;

    inline UInt GetTextureCount() const;
    inline UInt GetSize() const;

    inline const Byte * GetTexture( const GPUTextureIndex * pIndex = NULL ) const;
    inline Byte * GetTexture( const GPUTextureIndex * pIndex = NULL );
    inline Void SetTexture( const Byte * pTexture, const GPUTextureIndex * pIndex = NULL );

    // Methods
    Float GetMinLOD( GPUDeferredContext * pContext = NULL ) const;
    Void SetMinLOD( Float fMinLOD = FLOAT_INFINITE, GPUDeferredContext * pContext = NULL ) const;

    virtual Void GenerateMipMap() = 0;

protected:
    // Access to Views
    friend class GPURenderTargetView;
    friend class GPUDepthStencilView;
    friend class GPUShaderView;
    friend class GPUUnorderedAccessView;

    // Helpers
    inline static UInt * _MakeBoundArray( UInt iBound0, UInt iBound1 = 0, UInt iBound2 = 0 );
    inline Byte * _GetSubResource( const GPUTextureIndex * pIndex ) const;
    inline Byte * _GetSubResource( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) const;
    inline UInt _GetSubResourceBound( UInt iDimension, const GPUTextureIndex * pIndex ) const;
    inline UInt _GetSubResourcePitch( const GPUTextureIndex * pIndex ) const;
    inline UInt _GetSubResourceSlice( const GPUTextureIndex * pIndex ) const;
    inline UInt _GetSubResourceOffset( const GPUTextureIndex * pIndex ) const;
    inline UInt _GetSubResourceSize( const GPUTextureIndex * pIndex ) const;

    Void _ComputeTextureLayout();

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) = 0;
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const = 0;

    // Texture data
    PixelFormat m_iFormat;
    UInt m_iStride;

    UInt m_iDimensions;
    UInt m_arrMipLevelBounds[GPUTEXTURE_MAX_MIPMAP_LEVELS][GPUTEXTURE_MAX_DIMENSIONS];

    UInt m_iMipLevelCount;
    UInt m_arrMipLevelOffsets[GPUTEXTURE_MAX_MIPMAP_LEVELS];
    UInt m_arrMipLevelSizes[GPUTEXTURE_MAX_MIPMAP_LEVELS];

    UInt m_iArrayCount;
    UInt m_iMipMapSize;

    UInt m_iSubResourceCount;

    UInt m_iSize;
    Byte * m_arrTexture;

    // Device data
    DeviceTexture * m_pDeviceTexture;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUTexture1D class
class GPUTexture1D : public GPUTexture
{
protected:
    friend class RenderingManager;
    GPUTexture1D( PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iArrayCount = 1, const Byte * arrTexels = NULL, Bool bCreateCPUData = true );
    virtual ~GPUTexture1D();

public:
    // Binding
    virtual Void Bind( UInt iGPUResourceBinds );

    // Getters
    inline UInt GetWidth( UInt iMipLevel = 0 ) const;

    inline const Byte * GetTexel( UInt iPoint = 0, const GPUTextureIndex * pIndex = NULL ) const;
    inline Byte * GetTexel( UInt iPoint = 0, const GPUTextureIndex * pIndex = NULL );
    inline Void SetTexel( const Byte * pTexel, UInt iPoint, const GPUTextureIndex * pIndex = NULL );
    Void SetTexels( const Byte * arrTexels, const GPUSegment * pSeg = NULL, const GPUTextureIndex * pIndex = NULL );

    template<typename T> inline const T * GetTexel( UInt iPoint = 0, const GPUTextureIndex * pIndex = NULL ) const;
    template<typename T> inline T * GetTexel( UInt iPoint = 0, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexel( const T * pTexel, UInt iPoint, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexels( const T * arrTexels, const GPUSegment * pSeg = NULL, const GPUTextureIndex * pIndex = NULL );

    // Methods
    virtual Void GenerateMipMap();

    Void * Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, GPUDeferredContext * pContext = NULL );
    Void UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext = NULL );

    Void Update( const GPUTextureIndex * pIndex, const GPUSegment * pSeg = NULL, GPUDeferredContext * pContext = NULL );
    Void UpdateBack( const GPUTextureIndex * pIndex, const GPUSegment * pSeg = NULL, GPUDeferredContext * pContext = NULL );

    Void CopyCPUData( GPUTexture1D * pDstTexture, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyCPUData( GPUTexture1D * pDstTexture, const GPUTextureIndex * pDstIndex, UInt iDstPoint, const GPUTextureIndex * pSrcIndex, const GPUSegment * pSrcSeg = NULL,
                      Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture1D * pDstTexture, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture1D * pDstTexture, const GPUTextureIndex * pDstIndex, UInt iDstPoint, const GPUTextureIndex * pSrcIndex, const GPUSegment * pSrcSeg = NULL,
                      Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;

protected:
    // Helpers
    static Void _ValidateCopy( GPUSegment * outSrcSeg, UInt iDstWidth, UInt iSrcWidth, UInt iDstPoint = 0, const GPUSegment * pSrcSeg = NULL );
    static Void _CopyTexels( Byte * pDst, UInt iDstWidth, const Byte * pSrc, UInt iSrcWidth, UInt iStride, UInt iDstPoint = 0, const GPUSegment * pSrcSeg = NULL );

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceTexture1D m_hDeviceTexture1D;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUTexture2D class
class GPUTexture2D : public GPUTexture
{
protected:
    friend class RenderingManager;
    GPUTexture2D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iArrayCount = 1, UInt iSampleCount = 1,
                  const Byte * arrTexels = NULL, Bool bCreateCPUData = true );
    virtual ~GPUTexture2D();

public:
    // Binding
    inline Bool IsBoundToBackBuffer( UInt * outBackBuffer = NULL ) const;

    Void BindToBackBuffer( UInt iBackBuffer );
    virtual Void Bind( UInt iGPUResourceBinds );

    // Getters
    inline Bool IsMultiSampled() const;
    inline UInt GetSampleCount() const;

    inline UInt GetWidth( UInt iMipLevel = 0 ) const;
    inline UInt GetHeight( UInt iMipLevel = 0 ) const;

    inline UInt GetPitch( UInt iMipLevel = 0 ) const;

    inline const Byte * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL ) const;
    inline Byte * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL );
    inline Void SetTexel( const Byte * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex = NULL );
    Void SetTexels( const Byte * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect = NULL, const GPUTextureIndex * pIndex = NULL );

    template<typename T> inline const T * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL ) const;
    template<typename T> inline T * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexel( const T * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexels( const T * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect = NULL, const GPUTextureIndex * pIndex = NULL );

    // Methods
    virtual Void GenerateMipMap();

    Void * Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, GPUDeferredContext * pContext = NULL );
    Void UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext = NULL );

    Void Update( const GPUTextureIndex * pIndex, const GPURectangle * pRect = NULL, GPUDeferredContext * pContext = NULL );
    Void UpdateBack( const GPUTextureIndex * pIndex, const GPURectangle * pRect = NULL, GPUDeferredContext * pContext = NULL );

    Void CopyCPUData( GPUTexture2D * pDstTexture, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyCPUData( GPUTexture2D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint2 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPURectangle * pSrcRect = NULL,
                      Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture2D * pDstTexture, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture2D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint2 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPURectangle * pSrcRect = NULL,
                      Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;

protected:
    // Helpers
    static Void _ValidateCopy( GPUPoint2 * outDstPoint, GPURectangle * outSrcRect, UInt iDstWidth, UInt iDstHeight, UInt iSrcWidth, UInt iSrcHeight,
                               const GPUPoint2 * pDstPoint = NULL, const GPURectangle * pSrcRect = NULL );
    static Void _CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstPitch, const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcPitch, UInt iStride,
                             const GPUPoint2 * pDstPoint = NULL, const GPURectangle * pSrcRect = NULL );

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Multisampling data
    UInt m_iSampleCount;

    // Device data
    friend class GPU2DContext;
    static Void _OnBackBufferResize( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iSampleCount, Void * pUserData );

    DeviceTexture2D m_hDeviceTexture2D;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUTexture3D class
class GPUTexture3D : public GPUTexture
{
protected:
    friend class RenderingManager;
    GPUTexture3D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, const Byte * arrTexels = NULL, Bool bCreateCPUData = true );
    virtual ~GPUTexture3D();

public:
    // Binding
    virtual Void Bind( UInt iGPUResourceBinds );

    // Getters
    inline UInt GetWidth( UInt iMipLevel = 0 ) const;
    inline UInt GetHeight( UInt iMipLevel = 0 ) const;
    inline UInt GetDepth( UInt iMipLevel = 0 ) const;

    inline UInt GetPitch( UInt iMipLevel = 0 ) const;
    inline UInt GetSlice( UInt iMipLevel = 0 ) const;

    inline const Byte * GetTexel( const GPUPoint3 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL ) const;
    inline Byte * GetTexel( const GPUPoint3 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL );
    inline Void SetTexel( const Byte * pTexel, const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex = NULL );
    Void SetTexels( const Byte * arrTexels, UInt iTexelsPitch, UInt iTexelsSlice, const GPUBox * pBox = NULL, const GPUTextureIndex * pIndex = NULL );

    template<typename T> inline const T * GetTexel( const GPUPoint3 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL ) const;
    template<typename T> inline T * GetTexel( const GPUPoint3 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexel( const T * pTexel, const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex = NULL );
    template<typename T> inline Void SetTexels( const T * arrTexels, UInt iTexelsPitch, UInt iTexelsSlice, const GPUBox * pBox = NULL, const GPUTextureIndex * pIndex = NULL );

    // Methods
    virtual Void GenerateMipMap();

    Void * Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, UInt * outSlice, GPUDeferredContext * pContext = NULL );
    Void UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext = NULL );

    Void Update( const GPUTextureIndex * pIndex, const GPUBox * pBox = NULL, GPUDeferredContext * pContext = NULL );
    Void UpdateBack( const GPUTextureIndex * pIndex, const GPUBox * pBox = NULL, GPUDeferredContext * pContext = NULL );

    Void CopyCPUData( GPUTexture3D * pDstTexture, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyCPUData( GPUTexture3D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint3 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPUBox * pSrcBox = NULL,
                      Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture3D * pDstTexture, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTexture3D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint3 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPUBox * pSrcBox = NULL,
                      Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;

protected:
    // Helpers
    static Void _ValidateCopy( GPUPoint3 * outDstPoint, GPUBox * outSrcBox, UInt iDstWidth, UInt iDstHeight, UInt iDstDepth, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcDepth,
                               const GPUPoint3 * pDstPoint = NULL, const GPUBox * pSrcBox = NULL );
    static Void _CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstDepth, UInt iDstPitch, UInt iDstSlice,
                             const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcDepth, UInt iSrcPitch, UInt iSrcSlice, UInt iStride,
                             const GPUPoint3 * pDstPoint = NULL, const GPUBox * pSrcBox = NULL );

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceTexture3D m_hDeviceTexture3D;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUTextureCube class
class GPUTextureCube : public GPUTexture
{
protected:
    friend class RenderingManager;
    GPUTextureCube( PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iCubeCount = 1, const Byte * arrTexels = NULL, Bool bCreateCPUData = true );
    virtual ~GPUTextureCube();

public:
    // Binding
    virtual Void Bind( UInt iGPUResourceBinds );

    // Getters
    inline Bool IsCubeArray() const;
    inline UInt GetCubeCount() const;

    inline const Byte * GetCubeFace( const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS ) const;
    inline Byte * GetCubeFace( const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );
    inline Void SetCubeFace( const Byte * pCubeFace, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );

    inline UInt GetWidth( UInt iMipLevel = 0 ) const;
    inline UInt GetHeight( UInt iMipLevel = 0 ) const;

    inline UInt GetPitch( UInt iMipLevel = 0 ) const;

    inline const Byte * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS ) const;
    inline Byte * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );
    inline Void SetTexel( const Byte * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );
    Void SetTexels( const Byte * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );

    template<typename T> inline const T * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS ) const;
    template<typename T> inline T * GetTexel( const GPUPoint2 * pPoint = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );
    template<typename T> inline Void SetTexel( const T * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );
    template<typename T> inline Void SetTexels( const T * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect = NULL, const GPUTextureIndex * pIndex = NULL, GPUTextureCubeFace iFace = GPUTEXTURE_CUBEFACE_X_POS );

    // Methods
    virtual Void GenerateMipMap();

    Void * Lock( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, GPUDeferredContext * pContext = NULL );
    Void UnLock( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, GPUDeferredContext * pContext = NULL );

    Void Update( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, const GPURectangle * pRect = NULL, GPUDeferredContext * pContext = NULL );
    Void UpdateBack( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, const GPURectangle * pRect = NULL, GPUDeferredContext * pContext = NULL );

    Void CopyCPUData( GPUTextureCube * pDstTexture, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyCPUData( GPUTextureCube * pDstTexture, const GPUTextureIndex * pDstIndex, GPUTextureCubeFace iDstFace, const GPUPoint2 * pDstPoint,
                      const GPUTextureIndex * pSrcIndex, GPUTextureCubeFace iSrcFace, const GPURectangle * pSrcRect = NULL, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTextureCube * pDstTexture, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUTextureCube * pDstTexture, const GPUTextureIndex * pDstIndex, GPUTextureCubeFace iDstFace, const GPUPoint2 * pDstPoint,
                      const GPUTextureIndex * pSrcIndex, GPUTextureCubeFace iSrcFace, const GPURectangle * pSrcRect = NULL, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;

protected:
    // Helpers
    static Void _ValidateCopy( GPUPoint2 * outDstPoint, GPURectangle * outSrcRect, UInt iDstWidth, UInt iDstHeight, UInt iSrcWidth, UInt iSrcHeight,
                               const GPUPoint2 * pDstPoint = NULL, const GPURectangle * pSrcRect = NULL );
    static Void _CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstPitch, const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcPitch, UInt iStride,
                             const GPUPoint2 * pDstPoint = NULL, const GPURectangle * pSrcRect = NULL );

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceTextureCube m_hDeviceTextureCube;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUTexture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUTEXTURE_H
