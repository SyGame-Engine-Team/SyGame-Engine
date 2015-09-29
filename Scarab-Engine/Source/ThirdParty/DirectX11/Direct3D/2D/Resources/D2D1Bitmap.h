/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Bitmap.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Bitmaps.
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
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1BITMAP_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1BITMAP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D2D1Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D2D1RenderingContext;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1Bitmap class
class D2D1Bitmap
{
public:
    D2D1Bitmap( D2D1RenderingContext * pContext2D );
    virtual ~D2D1Bitmap();

    // Deferred construction
    inline Bool IsCreated() const;
    inline Void UpdateDataSource( const Void * pData );

    Void Create( const D2D1BitmapDesc * pDesc, const Void * pData = NULL, UInt iDataPitch = 0 );
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Methods
    inline PixelFormat GetPixelFormat() const;
    inline D2D1BitmapAlphaMode GetAlphaMode() const;

    inline Float GetDpiX() const;
    inline Float GetDpiY() const;

    inline UInt GetWidth() const;
    inline UInt GetHeight() const;

    inline Float GetDIPWidth() const;
    inline Float GetDIPHeight() const;

    inline Void GetDesc( D2D1BitmapDesc * outDesc ) const;

    Void CopyFrom( const Void * pSrcData, UInt iSrcPitch, const D2D1RectangleI * pDstRect = NULL );
    Void CopyFrom( D2D1Bitmap * pSrcBitmap, const D2D1PointI * pDstPoint = NULL, const D2D1RectangleI * pSrcRect = NULL );

protected:
    friend class D2D1BitmapBrush;
    friend class D2D1RenderingContext;
    D2D1RenderingContext * m_pContext2D;

    __D2D1_BITMAP_PROPERTIES m_hBitmapProperties;
    __D2D1_SIZE_U m_hSize;
    __D2D1_SIZE_F m_hDIPSize;

    ID2D1Bitmap * m_pBitmap;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    typedef struct _creation_parameters {
        const Void * pData;
        UInt iDataPitch;
    } _CreationParameters;

    Bool m_bTemporaryDestroyed;
    _CreationParameters m_hCreationParameters;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1Bitmap.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1BITMAP_H

