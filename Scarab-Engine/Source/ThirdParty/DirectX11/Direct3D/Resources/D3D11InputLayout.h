/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11InputLayout.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU resources : Input Layouts.
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
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11INPUTLAYOUT_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11INPUTLAYOUT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D3D11Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define D3D11INPUTLAYOUT_MAX_FIELDS    64
#define D3D11INPUTLAYOUT_MAX_COLORS    4
#define D3D11INPUTLAYOUT_MAX_TEXCOORDS 8

// Prototypes
class D3D11Renderer;

class D3D11ShaderCompiler;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11InputLayout class
class D3D11InputLayout
{
public:
    D3D11InputLayout( D3D11Renderer * pRenderer );
    virtual ~D3D11InputLayout();

    // Deferred construction
    inline Bool IsFinalized() const;

    Void PushField( const D3D11InputFieldDesc * pFieldDesc );
    Void Finalize();

    inline Bool IsCreated() const;

    Void Create( const D3D11ShaderCompiler * pCompiledShader );
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Getters
    inline UInt GetFieldCount() const;
    inline Void GetField( D3D11InputFieldDesc * outFieldDesc, UInt iField ) const;

private:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    UInt m_iFieldCount;
    __D3D11_INPUT_ELEMENT_DESC m_arrFields[D3D11INPUTLAYOUT_MAX_FIELDS];

    Bool m_bFinalized;
    ID3D11InputLayout * m_pInputLayout;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    typedef struct _creation_parameters {
        const D3D11ShaderCompiler * pCompiledShader;
    } _CreationParameters;

    Bool m_bTemporaryDestroyed;
    _CreationParameters m_hCreationParameters;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11InputLayout.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11INPUTLAYOUT_H

