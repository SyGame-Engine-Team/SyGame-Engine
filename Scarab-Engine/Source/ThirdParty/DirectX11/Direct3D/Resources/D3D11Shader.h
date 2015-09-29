/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Shader.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Shaders.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Improve memory management for dynamic linkage tables and
//                     Stream-Output declarations ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11SHADER_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11SHADER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11DeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Shader types
enum D3D11ShaderType {
    D3D11SHADER_VERTEX = 0, // Per-Vertex computing
    D3D11SHADER_GEOMETRY,   // Per-Primitive computing
    D3D11SHADER_PIXEL,      // Per-Pixel computing

    D3D11SHADER_HULL,       // Tesselation Pass 1
    D3D11SHADER_DOMAIN,     // Tesselation Pass 2

    D3D11SHADER_COMPUTE,    // All-purpose computing

    D3D11SHADER_COUNT
};

// Shader compilation data
typedef struct _d3d11_shader_compilationdata {
    D3D11ShaderType iTarget;
    const AChar * strName;
    const AChar * strEntryPoint;

    struct _Macro { const AChar * strName; const AChar * strDefinition; };
    const _Macro * arrMacros;

    UInt iFlags;       // D3D11ShaderCompilationFlags
    UInt iEffectFlags; // D3D11ShaderEffectCompilationFlags
} D3D11ShaderCompilationData;

// Prototypes
class D3D11Renderer;

class D3D11ShaderReflectConstantBuffer;
class D3D11ShaderReflectVariable;
class D3D11ShaderReflectType;

class D3D11Shader;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11ShaderCompiler class
class D3D11ShaderCompiler
{
public:
    D3D11ShaderCompiler();
    ~D3D11ShaderCompiler();

    // Deferred construction
    inline Bool IsCompiled() const;

    Void Compile( const AChar * strHLSLCode, UInt iHLSLCodeLength, const D3D11ShaderCompilationData * pCompilationData );
    Void Compile( const GChar * strFilename, const D3D11ShaderCompilationData * pCompilationData );
    Void Destroy();

    // Getters
    const Byte * GetByteCode() const;
    UInt GetByteCodeLength() const;

private:
    static const AChar * sm_arrTargets[D3D11SHADER_COUNT];

    ID3DBlob * m_pByteCode;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11ShaderReflectConstantBuffer class
class D3D11ShaderReflectConstantBuffer
{
public:
    D3D11ShaderReflectConstantBuffer();
    ~D3D11ShaderReflectConstantBuffer();

    // Methods
    Void GetDesc( D3D11ShaderConstantBufferDesc * outDesc ) const;

    UInt GetVariableCount() const;
    Void GetVariable( D3D11ShaderReflectVariable * outVariable, UInt iIndex ) const;
    Void GetVariable( D3D11ShaderReflectVariable * outVariable, const AChar * strName ) const;

private:
    friend class D3D11Shader;
    friend class D3D11ShaderReflectVariable;

    ID3D11ShaderReflectionConstantBuffer * m_pConstantBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11ShaderReflectVariable class
class D3D11ShaderReflectVariable
{
public:
    D3D11ShaderReflectVariable();
    ~D3D11ShaderReflectVariable();

    // Methods
    Void GetDesc( D3D11ShaderVariableDesc * outDesc ) const;

    UInt GetInterfaceSlot( UInt iArrayIndex ) const;

    Void GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer ) const;
    Void GetType( D3D11ShaderReflectType * outType ) const;

private:
    friend class D3D11Shader;
    friend class D3D11ShaderReflectConstantBuffer;

    ID3D11ShaderReflectionVariable * m_pVariable;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11ShaderReflectType class
class D3D11ShaderReflectType
{
public:
    D3D11ShaderReflectType();
    ~D3D11ShaderReflectType();

    // Methods
    Void GetDesc( D3D11ShaderTypeDesc * outDesc ) const;

    Bool IsEqual( const D3D11ShaderReflectType * pType ) const;
    Bool InheritsFrom( const D3D11ShaderReflectType * pBaseType ) const;
    Bool ImplementsInterface( const D3D11ShaderReflectType * pInterface ) const;

    Void GetBaseClass( D3D11ShaderReflectType * outBaseClass ) const;
    Void GetVariableBaseClass( D3D11ShaderReflectType * outBaseClass ) const;

    UInt GetInterfaceCount() const;
    Void GetInterface( D3D11ShaderReflectType * outInterface, UInt iIndex ) const;

    UInt GetMemberTypeCount() const;
    const AChar * GetMemberTypeName( UInt iIndex ) const;
    Void GetMemberType( D3D11ShaderReflectType * outMember, UInt iIndex ) const;
    Void GetMemberType( D3D11ShaderReflectType * outMember, const AChar * strName ) const;

private:
    friend class D3D11ShaderReflectVariable;

    ID3D11ShaderReflectionType * m_pType;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Shader class
class D3D11Shader
{
protected:
    D3D11Shader( D3D11Renderer * pRenderer );
public:
    virtual ~D3D11Shader();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( const D3D11ShaderCompiler * pCompiledShader, Bool bReflect = false, Bool bDynamicLinking = false );
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Getters
    virtual D3D11ShaderType GetType() const = 0;

    inline const D3D11ShaderCompiler * GetCompiledShader() const;

    // Reflection
    inline Bool HasReflection() const;

    Void GetDesc( D3D11ShaderDesc * outDesc ) const;

    UInt GetInputParameterCount() const;
    Void GetInputParameterDesc( D3D11ShaderParameterDesc * outInputParameterDesc, UInt iIndex ) const;

    UInt GetOutputParameterCount() const;
    Void GetOutputParameterDesc( D3D11ShaderParameterDesc * outOutputParameterDesc, UInt iIndex ) const;

    UInt GetPatchConstantParameterCount() const;
    Void GetPatchConstantParameterDesc( D3D11ShaderParameterDesc * outPatchConstantParameterDesc, UInt iIndex ) const;

    UInt GetBindingCount() const;
    Void GetBindingDesc( D3D11ShaderBindingDesc * outBindingDesc, UInt iIndex ) const;
    Void GetBindingDesc( D3D11ShaderBindingDesc * outBindingDesc, const AChar * strName ) const;

    UInt GetConstantBufferCount() const;
    Void GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer, UInt iIndex ) const;
    Void GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer, const AChar * strName ) const;

    Void GetVariable( D3D11ShaderReflectVariable * outVariable, const AChar * strName ) const;

    UInt GetInterfaceSlotCount() const;

    Void GetThreadGroupSize( UInt * outSizeX, UInt * outSizeY, UInt * outSizeZ ) const;

    // Dynamic linkage table (you must add them all before using the shader)
    inline Bool HasDynamicLinkage() const;

    Void AddDynamicLink( const AChar * strInterfaceInstance, const AChar * strClassInstance );

protected:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    const D3D11ShaderCompiler * m_pCompiledShader;
    ID3D11DeviceChild * m_pShader;

    // Reflection
    Void _InitializeReflection();
    Void _CleanupReflection();

    Bool m_bHasReflection;
    ID3D11ShaderReflection * m_pReflector;

    // Dynamic linkage
    Void _InitializeDynamicLinkage();
    Void _CleanupDynamicLinkage();

    Bool m_bHasDynamicLinkage;
    ID3D11ClassLinkage * m_pClassLinkage;
    UInt m_iClassInstanceCount;
    ID3D11ClassInstance ** m_arrClassInstances;

    // Auto-Regen system
    virtual Void _NakedCreate() = 0;
    virtual Void _NakedDestroy() = 0;

    Bool m_bTemporaryDestroyed;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11VertexShader class
class D3D11VertexShader : public D3D11Shader
{
public:
    D3D11VertexShader( D3D11Renderer * pRenderer );
    virtual ~D3D11VertexShader();

    // Getters
    inline virtual D3D11ShaderType GetType() const;

protected:
    friend class D3D11Renderer;

    ID3D11VertexShader * m_pVertexShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11GeometryShader class
class D3D11GeometryShader : public D3D11Shader
{
public:
    D3D11GeometryShader( D3D11Renderer * pRenderer );
    virtual ~D3D11GeometryShader();

    // Deferred construction
    inline Bool IsFinalized() const;

    Void PushSODeclaration( const D3D11StreamOutputDeclaration * pSODeclaration );
    Void Finalize( const UInt * arrStrides = NULL, UInt iStrideCount = 0, UInt iRasterStream = INVALID_OFFSET );

    // Getters
    inline virtual D3D11ShaderType GetType() const;

    inline UInt GetSODeclarationCount() const;
    inline Void GetSODeclaration( D3D11StreamOutputDeclaration * outSODeclaration, UInt iSODeclaration ) const;

    inline UInt GetStrideCount() const;
    inline UInt GetStride( UInt iStreamOutputSlot ) const;

    inline UInt GetRasterStream() const; // INVALID_OFFET => no raster stream

protected:
    friend class D3D11Renderer;

    UInt m_iSODeclarationCount;
    __D3D11_SO_DECLARATION_ENTRY m_arrSODeclarations[D3D11RENDERER_MAX_STREAMOUTPUT_DECLARATIONS];

    UInt m_iStrideCount;
    UInt m_arrStrides[D3D11RENDERER_MAX_STREAMOUTPUT_SLOTS];

    UInt m_iRasterStream;

    Bool m_bFinalized;
    ID3D11GeometryShader * m_pGeometryShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};


/////////////////////////////////////////////////////////////////////////////////
// The D3D11PixelShader class
class D3D11PixelShader : public D3D11Shader
{
public:
    D3D11PixelShader( D3D11Renderer * pRenderer );
    virtual ~D3D11PixelShader();

    // Getters
    inline virtual D3D11ShaderType GetType() const;

protected:
    friend class D3D11Renderer;

    ID3D11PixelShader * m_pPixelShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};


/////////////////////////////////////////////////////////////////////////////////
// The D3D11HullShader class
class D3D11HullShader : public D3D11Shader
{
public:
    D3D11HullShader( D3D11Renderer * pRenderer );
    virtual ~D3D11HullShader();

    // Getters
    inline virtual D3D11ShaderType GetType() const;

protected:
    friend class D3D11Renderer;

    ID3D11HullShader * m_pHullShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};


/////////////////////////////////////////////////////////////////////////////////
// The D3D11DomainShader class
class D3D11DomainShader : public D3D11Shader
{
public:
    D3D11DomainShader( D3D11Renderer * pRenderer );
    virtual ~D3D11DomainShader();

    // Getters
    inline virtual D3D11ShaderType GetType() const;

protected:
    friend class D3D11Renderer;

    ID3D11DomainShader * m_pDomainShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};


/////////////////////////////////////////////////////////////////////////////////
// The D3D11ComputeShader class
class D3D11ComputeShader : public D3D11Shader
{
public:
    D3D11ComputeShader( D3D11Renderer * pRenderer );
    virtual ~D3D11ComputeShader();

    // Getters
    inline virtual D3D11ShaderType GetType() const;

protected:
    friend class D3D11Renderer;

    ID3D11ComputeShader * m_pComputeShader;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Shader.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11SHADER_H

