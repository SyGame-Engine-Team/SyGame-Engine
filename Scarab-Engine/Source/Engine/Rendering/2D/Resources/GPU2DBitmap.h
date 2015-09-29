/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBitmap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Bitmaps
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBITMAP_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBITMAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Error/ErrorManager.h"
#include "../../../../Lib/Math/Types/Vertex/Vertex2.h"
#include "../../../../Lib/Math/Types/Vector/Vector2.h"
#include "../../../../Lib/Math/Types/Discrete/Rectangle2.h"

#include "../../../Resource/Resource.h"

#include "../../DeviceRenderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef struct _gpu2d_point_i {
    inline struct _gpu2d_point_i()                                    { iX = 0; iY = 0; }
    inline struct _gpu2d_point_i( const Point2 & rhs )                { iX = rhs.X; iY = rhs.Y; }
    inline struct _gpu2d_point_i( const Offset2 & rhs )               { iX = rhs.X; iY = rhs.Y; }
    inline struct _gpu2d_point_i( const Vertex2 & rhs )               { iX = (UInt)(rhs.X); iY = (UInt)(rhs.Y); }
    inline struct _gpu2d_point_i( const Vector2 & rhs )               { iX = (UInt)(rhs.X); iY = (UInt)(rhs.Y); }
    inline struct _gpu2d_point_i( const struct _gpu2d_point_i & rhs ) { iX = rhs.iX; iY = rhs.iY; }

    inline struct _gpu2d_point_i & operator=( const Point2 & rhs )                { iX = rhs.X; iY = rhs.Y; return (*this); }
    inline struct _gpu2d_point_i & operator=( const Offset2 & rhs )               { iX = rhs.X; iY = rhs.Y; return (*this); }
    inline struct _gpu2d_point_i & operator=( const Vertex2 & rhs )               { iX = (UInt)(rhs.X); iY = (UInt)(rhs.Y); return (*this); }
    inline struct _gpu2d_point_i & operator=( const Vector2 & rhs )               { iX = (UInt)(rhs.X); iY = (UInt)(rhs.Y); return (*this); }
    inline struct _gpu2d_point_i & operator=( const struct _gpu2d_point_i & rhs ) { iX = rhs.iX; iY = rhs.iY; return (*this); }

    inline operator Point2() const { return Point2(iX, iY); }
    inline operator Offset2() const { return Offset2(iX, iY); }
    inline operator Vertex2() const { return Vertex2((Float)iX, (Float)iY); }
    inline operator Vector2() const { return Vector2((Float)iX, (Float)iY); }

    UInt iX;
    UInt iY;
} GPU2DPointI;

typedef struct _gpu2d_rectangle_i {
    inline struct _gpu2d_rectangle_i()                                        { iLeft = 0; iTop = 0; iRight = 1; iBottom = 1; }
    inline struct _gpu2d_rectangle_i( const Rectangle2 & rhs )                { iLeft = rhs.TopLeft.X; iTop = rhs.TopLeft.Y; iRight = rhs.DownRight.X; iBottom = rhs.DownRight.Y; }
    inline struct _gpu2d_rectangle_i( const struct _gpu2d_rectangle_i & rhs ) { iLeft = rhs.iLeft; iTop = rhs.iTop; iRight = rhs.iRight; iBottom = rhs.iBottom; }

    inline struct _gpu2d_rectangle_i & operator=( const Rectangle2 & rhs )                { iLeft = rhs.TopLeft.X; iTop = rhs.TopLeft.Y; iRight = rhs.DownRight.X; iBottom = rhs.DownRight.Y; return (*this); }
    inline struct _gpu2d_rectangle_i & operator=( const struct _gpu2d_rectangle_i & rhs ) { iLeft = rhs.iLeft; iTop = rhs.iTop; iRight = rhs.iRight; iBottom = rhs.iBottom; return (*this); }

    inline operator Rectangle2() const { return Rectangle2(iLeft, iTop, iRight, iBottom); }

    UInt iLeft;
    UInt iTop;
    UInt iRight;
    UInt iBottom;
} GPU2DRectangleI;

enum GPU2DBitmapInterpolationMode {
    GPU2DBITMAP_INTERPOLATION_NEAREST = DEVICE2DBITMAP_INTERPOLATION_NEAREST,
    GPU2DBITMAP_INTERPOLATION_LINEAR  = DEVICE2DBITMAP_INTERPOLATION_LINEAR
};

enum GPU2DBitmapOpacityMaskContent {
    GPU2DBITMAP_OPACITYMASK_GRAPHICS           = DEVICE2DBITMAP_OPACITYMASK_GRAPHICS,
    GPU2DBITMAP_OPACITYMASK_TEXT_NATURAL       = DEVICE2DBITMAP_OPACITYMASK_TEXT_NATURAL,
    GPU2DBITMAP_OPACITYMASK_TEXT_GDICOMPATIBLE = DEVICE2DBITMAP_OPACITYMASK_TEXT_GDICOMPATIBLE
};

enum GPU2DBitmapAlphaMode {
    GPU2DBITMAP_ALPHAMODE_UNKNOWN       = DEVICE2DBITMAP_ALPHAMODE_UNKNOWN,
    GPU2DBITMAP_ALPHAMODE_PREMULTIPLIED = DEVICE2DBITMAP_ALPHAMODE_PREMULTIPLIED,
    GPU2DBITMAP_ALPHAMODE_STRAIGHT      = DEVICE2DBITMAP_ALPHAMODE_STRAIGHT,
    GPU2DBITMAP_ALPHAMODE_IGNORE        = DEVICE2DBITMAP_ALPHAMODE_IGNORE
};

typedef struct _gpu2d_bitmap_desc {
    PixelFormat iFormat;
    GPU2DBitmapAlphaMode iAlphaMode;
    Float fDpiX;
    Float fDpiY;
    UInt iWidth;
    UInt iHeight;
    Float fDIPWidth;  // DIP = pixels / ( Dpi/96 )
    Float fDIPHeight; // pixels = DIP * ( Dpi/96 )
} GPU2DBitmapDesc;

// Prototypes
class GPU2DContext;
class GPU2DBitmapBrush;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DBitmap class
class GPU2DBitmap : public Resource
{
private:
    friend class GPU2DContext;
    GPU2DBitmap( GPU2DContext * pContext2D, PixelFormat iFormat, UInt iWidth, UInt iHeight, const Byte * arrBitmap = NULL, Bool bCreateCPUData = true );
    ~GPU2DBitmap();

public:
    // Binding
    inline Bool IsBound() const;

    Void Bind();
    Void UnBind();

    inline Bool HasCPUData() const;

    Void DestroyCPUData(); // Content will no longer be safe from device loss.
    Void RestoreCPUData(); // Content is NOT restored, user must either use UpdateBack when possible
                           // or manually refill buffer data then commit to GPU.

    // Getters
    inline virtual ResourceType GetResourceType() const;

    // Methods
    inline PixelFormat GetFormat() const;
    inline GPU2DBitmapAlphaMode GetAlphaMode() const;

    inline UInt GetStride() const;
    inline UInt GetPitch() const;

    inline UInt GetWidth() const;
    inline UInt GetHeight() const;

    inline Float GetDpiX() const;
    inline Float GetDpiY() const;

    inline Float GetDIPWidth() const;
    inline Float GetDIPHeight() const;

    inline Void GetDesc( GPU2DBitmapDesc * outDesc ) const;

    inline const Byte * GetBitmap() const;
    inline Byte * GetBitmap();

    inline const Byte * GetPixel( const GPU2DPointI * pPoint = NULL ) const;
    inline Byte * GetPixel( const GPU2DPointI * pPoint = NULL );
    inline Void SetPixel( const Byte * pPixel, const GPU2DPointI * pPoint );
    Void SetPixels( const Byte * arrPixels, UInt iPixelsPitch, const GPU2DRectangleI * pRect = NULL );

    template<typename T> inline const T * GetPixel( const GPU2DPointI * pPoint = NULL ) const;
    template<typename T> inline T * GetPixel( const GPU2DPointI * pPoint = NULL );
    template<typename T> inline Void SetPixel( const T * pPixel, const GPU2DPointI * pPoint );
    template<typename T> inline Void SetPixels( const T * arrPixels, UInt iPixelsPitch, const GPU2DRectangleI * pRect = NULL );

    inline Void Update( const GPU2DRectangleI * pRect = NULL );

    inline Void CopyFrom( const Void * pSrcData, UInt iSrcPitch, const GPU2DRectangleI * pDstRect = NULL );
    inline Void CopyFrom( GPU2DBitmap * pSrcBitmap, const GPU2DPointI * pDstPoint = NULL, const GPU2DRectangleI * pSrcRect = NULL );

private:
    friend class GPU2DBitmapBrush;

    GPU2DContext * m_pContext2D;

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Bitmap data
    PixelFormat m_iFormat;
    GPU2DBitmapAlphaMode m_iAlphaMode;
    UInt m_iStride;

    UInt m_iWidth;
    UInt m_iHeight;
    UInt m_iPitch;

    Float m_fDpiX;
    Float m_fDpiY;

    UInt m_iSize;
    Byte * m_arrBitmap;

    // Device data
    Device2DBitmap m_hDevice2DBitmap;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DBitmap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBITMAP_H
