/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUShader.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Shaders interface
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUSHADER_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUSHADER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUResourceView.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Shader types
enum GPUShaderType {
    GPUSHADER_VERTEX   = DEVICESHADER_VERTEX,   // Per-Vertex computing
    GPUSHADER_GEOMETRY = DEVICESHADER_GEOMETRY, // Per-Primitive computing
    GPUSHADER_PIXEL    = DEVICESHADER_PIXEL,    // Per-Pixel computing
    GPUSHADER_HULL     = DEVICESHADER_HULL,     // Tesselation Pass 1
    GPUSHADER_DOMAIN   = DEVICESHADER_DOMAIN,   // Tesselation Pass 2
    GPUSHADER_COMPUTE  = DEVICESHADER_COMPUTE   // All-purpose computing
};

// GPUShaderCompiler definitions
enum GPUShaderCompilationFlags {
    GPUSHADER_COMPILE_NONE = DEVICESHADER_COMPILE_NONE,

    GPUSHADER_COMPILE_DEBUG                   = DEVICESHADER_COMPILE_DEBUG,                   // Add debug information to the output code.
    GPUSHADER_COMPILE_WARNINGS_AS_ERRORS      = DEVICESHADER_COMPILE_WARNINGS_AS_ERRORS,      // Treat warnings as errors
    GPUSHADER_COMPILE_STRICT                  = DEVICESHADER_COMPILE_STRICT,                  // Forces strict compile, might eject legacy syntax. Disabled on deprecated syntax by default.
    GPUSHADER_COMPILE_STRICT_IEEE             = DEVICESHADER_COMPILE_STRICT_IEEE,             // Forces IEEE strict compile.
    GPUSHADER_COMPILE_BACKWARDS_COMPATIBILITY = DEVICESHADER_COMPILE_BACKWARDS_COMPATIBILITY, // Enable older shaders to compile to 5_0 targets.
    GPUSHADER_COMPILE_SKIP_OPTIMIZATION       = DEVICESHADER_COMPILE_SKIP_OPTIMIZATION,       // Skip optimization steps during code generation. Debug purposes only.
    GPUSHADER_COMPILE_SKIP_VALIDATION         = DEVICESHADER_COMPILE_SKIP_VALIDATION,         // Don't validate the generated code. Trusted shaders only (tested & working).
    GPUSHADER_COMPILE_VS_SOFTWARE_NO_OPT      = DEVICESHADER_COMPILE_VS_SOFTWARE_NO_OPT,      // Compile vertexshader for next highest shader profile. Turns debug on / optimizations off.
    GPUSHADER_COMPILE_PS_SOFTWARE_NO_OPT      = DEVICESHADER_COMPILE_PS_SOFTWARE_NO_OPT,      // Compile pixelshader for next highest shader profile. Turns debug on / optimizations off.
    GPUSHADER_COMPILE_NO_PRESHADER            = DEVICESHADER_COMPILE_NO_PRESHADER,            // Disable Preshaders, the compiler does not pull out static expressions for evaluation.
    
    GPUSHADER_COMPILE_PARTIAL_PRECISION        = DEVICESHADER_COMPILE_PARTIAL_PRECISION,        // Use partial precision. The compiled code might run faster on some hardware.
    GPUSHADER_COMPILE_PACK_MATRIX_ROW_MAJOR    = DEVICESHADER_COMPILE_PACK_MATRIX_ROW_MAJOR,    // Pack matrices in row-major order on input and output from the shader.
    GPUSHADER_COMPILE_PACK_MATRIX_COLUMN_MAJOR = DEVICESHADER_COMPILE_PACK_MATRIX_COLUMN_MAJOR, // Pack matrices in column-major order on input and output from the shader (generally better for vector-matrix multiplication).
    GPUSHADER_COMPILE_AVOID_FLOW_CONTROL       = DEVICESHADER_COMPILE_AVOID_FLOW_CONTROL,       // Avoid flow-control constructs where possible.
    GPUSHADER_COMPILE_PREFER_FLOW_CONTROL      = DEVICESHADER_COMPILE_PREFER_FLOW_CONTROL,      // Prefer flow-control constructs where possible.

    GPUSHADER_COMPILE_OPTIMIZATION_LEVEL0       = DEVICESHADER_COMPILE_OPTIMIZATION_LEVEL0,
    GPUSHADER_COMPILE_OPTIMIZATION_LEVEL1       = DEVICESHADER_COMPILE_OPTIMIZATION_LEVEL1, // Default optimization level.
    GPUSHADER_COMPILE_OPTIMIZATION_LEVEL2       = DEVICESHADER_COMPILE_OPTIMIZATION_LEVEL2,
    GPUSHADER_COMPILE_OPTIMIZATION_LEVEL3       = DEVICESHADER_COMPILE_OPTIMIZATION_LEVEL3
};

typedef struct _gpu_shader_compilationdata {
    GPUShaderType iTarget;
    const AChar * strName;
    const AChar * strEntryPoint;

    struct _Macro { const AChar * strName; const AChar * strDefinition; };
    const _Macro * arrMacros;

    UInt iFlags;       // GPUShaderCompilationFlags
    UInt iEffectFlags; // GPUShaderEffectCompilationFlags, 0 for now
} GPUShaderCompilationData;

// GPUShaderReflectConstantBuffer definitions
enum GPUShaderConstantBufferType {
    GPUSHADER_CONSTANTBUFFER_CBUFFER            = DEVICESHADER_CONSTANTBUFFER_CBUFFER,
    GPUSHADER_CONSTANTBUFFER_TBUFFER            = DEVICESHADER_CONSTANTBUFFER_TBUFFER,
    GPUSHADER_CONSTANTBUFFER_INTERFACE_POINTERS = DEVICESHADER_CONSTANTBUFFER_INTERFACE_POINTERS,
    GPUSHADER_CONSTANTBUFFER_RESOURCE_BIND_INFO = DEVICESHADER_CONSTANTBUFFER_RESOURCE_BIND_INFO
};
enum GPUShaderConstantBufferFlags {
    GPUSHADER_CONSTANTBUFFERFLAG_NONE       = DEVICESHADER_CONSTANTBUFFERFLAG_NONE,
    GPUSHADER_CONSTANTBUFFERFLAG_USERPACKED = DEVICESHADER_CONSTANTBUFFERFLAG_USERPACKED
};

typedef struct _gpu_shader_constantbuffer_desc {
    const AChar  * strName;
    
    GPUShaderConstantBufferType iType;

    UInt iByteSize;
    UInt iVariableCount;

    UInt iFlags; // GPUShaderConstantBufferFlags
} GPUShaderConstantBufferDesc;

// GPUShaderReflectVariable definitions
enum GPUShaderVariableFlags {
    GPUSHADER_VARIABLEFLAG_NONE                = DEVICESHADER_VARIABLEFLAG_NONE,
    GPUSHADER_VARIABLEFLAG_USERPACKED          = DEVICESHADER_VARIABLEFLAG_USERPACKED,
    GPUSHADER_VARIABLEFLAG_USED                = DEVICESHADER_VARIABLEFLAG_USED,
    GPUSHADER_VARIABLEFLAG_INTERFACE_POINTER   = DEVICESHADER_VARIABLEFLAG_INTERFACE_POINTER,
    GPUSHADER_VARIABLEFLAG_INTERFACE_PARAMETER = DEVICESHADER_VARIABLEFLAG_INTERFACE_PARAMETER
};

typedef struct _gpu_shader_variable_desc {
    const AChar  * strName;

    UInt iStartOffset; // Offset in constant buffer's backing store
    UInt iByteSize;    // Size of variable (in bytes)

    UInt iStartTextureSlot; // First texture index (or -1 if no textures used)
    UInt iTextureSlotCount; // Number of texture slots possibly used.

    UInt iStartSamplerSlot; // First sampler index (or -1 if no textures used)
    UInt iSamplerSlotCount; // Number of sampler slots possibly used.

    UInt iFlags; // GPUShaderVariableFlags

    Void * pDefaultValue;
} GPUShaderVariableDesc;

// GPUShaderReflectType definitions
enum GPUShaderVariableClass {
    GPUSHADER_VARIABLECLASS_SCALAR            = DEVICESHADER_VARIABLECLASS_SCALAR,
    GPUSHADER_VARIABLECLASS_VECTOR            = DEVICESHADER_VARIABLECLASS_VECTOR,
    GPUSHADER_VARIABLECLASS_MATRIX_ROWS       = DEVICESHADER_VARIABLECLASS_MATRIX_ROWS,
    GPUSHADER_VARIABLECLASS_MATRIX_COLUMNS    = DEVICESHADER_VARIABLECLASS_MATRIX_COLUMNS,
    GPUSHADER_VARIABLECLASS_STRUCT            = DEVICESHADER_VARIABLECLASS_STRUCT,
    GPUSHADER_VARIABLECLASS_OBJECT            = DEVICESHADER_VARIABLECLASS_OBJECT,
    GPUSHADER_VARIABLECLASS_INTERFACE_CLASS   = DEVICESHADER_VARIABLECLASS_INTERFACE_CLASS,
    GPUSHADER_VARIABLECLASS_INTERFACE_POINTER = DEVICESHADER_VARIABLECLASS_INTERFACE_POINTER
};
enum GPUShaderVariableType {
    GPUSHADER_VARIABLE_VOID                     = DEVICESHADER_VARIABLE_VOID,
    GPUSHADER_VARIABLE_BOOL                     = DEVICESHADER_VARIABLE_BOOL,
    GPUSHADER_VARIABLE_BYTE                     = DEVICESHADER_VARIABLE_BYTE,
    GPUSHADER_VARIABLE_SHORT                    = DEVICESHADER_VARIABLE_SHORT,
    GPUSHADER_VARIABLE_USHORT                   = DEVICESHADER_VARIABLE_USHORT,
    GPUSHADER_VARIABLE_INT                      = DEVICESHADER_VARIABLE_INT,
    GPUSHADER_VARIABLE_UINT                     = DEVICESHADER_VARIABLE_UINT,
    GPUSHADER_VARIABLE_FLOAT                    = DEVICESHADER_VARIABLE_FLOAT,
    GPUSHADER_VARIABLE_DOUBLE                   = DEVICESHADER_VARIABLE_DOUBLE,
    GPUSHADER_VARIABLE_INT12                    = DEVICESHADER_VARIABLE_INT12,
    GPUSHADER_VARIABLE_FLOAT8                   = DEVICESHADER_VARIABLE_FLOAT8,
    GPUSHADER_VARIABLE_FLOAT10                  = DEVICESHADER_VARIABLE_FLOAT10,
    GPUSHADER_VARIABLE_FLOAT16                  = DEVICESHADER_VARIABLE_FLOAT16,
    GPUSHADER_VARIABLE_INTERFACEPOINTER         = DEVICESHADER_VARIABLE_INTERFACEPOINTER,
    GPUSHADER_VARIABLE_STRING                   = DEVICESHADER_VARIABLE_STRING,
    GPUSHADER_VARIABLE_BUFFER                   = DEVICESHADER_VARIABLE_BUFFER,
    GPUSHADER_VARIABLE_RWBUFFER                 = DEVICESHADER_VARIABLE_RWBUFFER,
    GPUSHADER_VARIABLE_CBUFFER                  = DEVICESHADER_VARIABLE_CBUFFER,
    GPUSHADER_VARIABLE_TBUFFER                  = DEVICESHADER_VARIABLE_TBUFFER,
    GPUSHADER_VARIABLE_RAWBUFFER                = DEVICESHADER_VARIABLE_RAWBUFFER,
    GPUSHADER_VARIABLE_RWRAWBUFFER              = DEVICESHADER_VARIABLE_RWRAWBUFFER,
    GPUSHADER_VARIABLE_STRUCTUREDBUFFER         = DEVICESHADER_VARIABLE_STRUCTUREDBUFFER,
    GPUSHADER_VARIABLE_RWSTRUCTUREDBUFFER       = DEVICESHADER_VARIABLE_RWSTRUCTUREDBUFFER,
    GPUSHADER_VARIABLE_STRUCTUREDBUFFER_APPEND  = DEVICESHADER_VARIABLE_STRUCTUREDBUFFER_APPEND,
    GPUSHADER_VARIABLE_STRUCTUREDBUFFER_CONSUME = DEVICESHADER_VARIABLE_STRUCTUREDBUFFER_CONSUME,
    GPUSHADER_VARIABLE_TEXTURE                  = DEVICESHADER_VARIABLE_TEXTURE,
    GPUSHADER_VARIABLE_TEXTURE1D                = DEVICESHADER_VARIABLE_TEXTURE1D,
    GPUSHADER_VARIABLE_TEXTURE1DARRAY           = DEVICESHADER_VARIABLE_TEXTURE1DARRAY,
    GPUSHADER_VARIABLE_RWTEXTURE1D              = DEVICESHADER_VARIABLE_RWTEXTURE1D,
    GPUSHADER_VARIABLE_RWTEXTURE1DARRAY         = DEVICESHADER_VARIABLE_RWTEXTURE1DARRAY,
    GPUSHADER_VARIABLE_TEXTURE2D                = DEVICESHADER_VARIABLE_TEXTURE2D,
    GPUSHADER_VARIABLE_TEXTURE2DARRAY           = DEVICESHADER_VARIABLE_TEXTURE2DARRAY,
    GPUSHADER_VARIABLE_RWTEXTURE2D              = DEVICESHADER_VARIABLE_RWTEXTURE2D,
    GPUSHADER_VARIABLE_RWTEXTURE2DARRAY         = DEVICESHADER_VARIABLE_RWTEXTURE2DARRAY,
    GPUSHADER_VARIABLE_TEXTURE2DMS              = DEVICESHADER_VARIABLE_TEXTURE2DMS,
    GPUSHADER_VARIABLE_TEXTURE2DMSARRAY         = DEVICESHADER_VARIABLE_TEXTURE2DMSARRAY,
    GPUSHADER_VARIABLE_TEXTURE3D                = DEVICESHADER_VARIABLE_TEXTURE3D,
    GPUSHADER_VARIABLE_RWTEXTURE3D              = DEVICESHADER_VARIABLE_RWTEXTURE3D,
    GPUSHADER_VARIABLE_TEXTURECUBE              = DEVICESHADER_VARIABLE_TEXTURECUBE,
    GPUSHADER_VARIABLE_TEXTURECUBEARRAY         = DEVICESHADER_VARIABLE_TEXTURECUBEARRAY,
    GPUSHADER_VARIABLE_SAMPLER                  = DEVICESHADER_VARIABLE_SAMPLER,
    GPUSHADER_VARIABLE_SAMPLER1D                = DEVICESHADER_VARIABLE_SAMPLER1D,
    GPUSHADER_VARIABLE_SAMPLER2D                = DEVICESHADER_VARIABLE_SAMPLER2D,
    GPUSHADER_VARIABLE_SAMPLER3D                = DEVICESHADER_VARIABLE_SAMPLER3D,
    GPUSHADER_VARIABLE_SAMPLERCUBE              = DEVICESHADER_VARIABLE_SAMPLERCUBE,
    GPUSHADER_VARIABLE_VERTEXSHADER             = DEVICESHADER_VARIABLE_VERTEXSHADER,
    GPUSHADER_VARIABLE_GEOMETRYSHADER           = DEVICESHADER_VARIABLE_GEOMETRYSHADER,
    GPUSHADER_VARIABLE_PIXELSHADER              = DEVICESHADER_VARIABLE_PIXELSHADER,
    GPUSHADER_VARIABLE_HULLSHADER               = DEVICESHADER_VARIABLE_HULLSHADER,
    GPUSHADER_VARIABLE_DOMAINSHADER             = DEVICESHADER_VARIABLE_DOMAINSHADER,
    GPUSHADER_VARIABLE_COMPUTESHADER            = DEVICESHADER_VARIABLE_COMPUTESHADER,
    GPUSHADER_VARIABLE_VERTEXFRAGMENT           = DEVICESHADER_VARIABLE_VERTEXFRAGMENT,
    GPUSHADER_VARIABLE_PIXELFRAGMENT            = DEVICESHADER_VARIABLE_PIXELFRAGMENT,
    GPUSHADER_VARIABLE_RASTERIZER               = DEVICESHADER_VARIABLE_RASTERIZER,
    GPUSHADER_VARIABLE_DEPTHSTENCIL             = DEVICESHADER_VARIABLE_DEPTHSTENCIL,
    GPUSHADER_VARIABLE_BLEND                    = DEVICESHADER_VARIABLE_BLEND,
    GPUSHADER_VARIABLE_RENDERTARGETVIEW         = DEVICESHADER_VARIABLE_RENDERTARGETVIEW,
    GPUSHADER_VARIABLE_DEPTHSTENCILVIEW         = DEVICESHADER_VARIABLE_DEPTHSTENCILVIEW
};

typedef struct _gpu_shader_type_desc {
    const AChar * strName; // Can be NULL

    GPUShaderVariableClass iClass; // Variable class (object, matrix, ...)
    GPUShaderVariableType iType;   // Variable type (float, sampler, ...)

    UInt iInterfaceCount;
    UInt iMemberCount;  // 0 if not a structure
    UInt iElementCount; // 0 if not an array

    UInt iColumnCount;  // for vectors & matrices, 1 for other numeric, 0 if not applicable
    UInt iRowCount;     // for matrices, 1 for other numeric, 0 if not applicable

    UInt iOffset;       // Offset from the start of structure (0 if not a structure member)
} GPUShaderTypeDesc;

// GPUShader definitions
enum GPUShaderPrimitive {
    GPUSHADER_PRIMITIVE_UNDEFINED              = DEVICESHADER_PRIMITIVE_UNDEFINED,
    GPUSHADER_PRIMITIVE_POINT                  = DEVICESHADER_PRIMITIVE_POINT,
    GPUSHADER_PRIMITIVE_LINE                   = DEVICESHADER_PRIMITIVE_LINE,
    GPUSHADER_PRIMITIVE_TRIANGLE               = DEVICESHADER_PRIMITIVE_TRIANGLE,
    GPUSHADER_PRIMITIVE_LINE_ADJ               = DEVICESHADER_PRIMITIVE_LINE_ADJ,
    GPUSHADER_PRIMITIVE_TRIANGLE_ADJ           = DEVICESHADER_PRIMITIVE_TRIANGLE_ADJ,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_1  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_1,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_2  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_2,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_3  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_3,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_4  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_4,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_5  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_5,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_6  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_6,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_7  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_7,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_8  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_8,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_9  = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_9,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_10 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_10,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_11 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_11,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_12 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_12,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_13 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_13,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_14 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_14,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_15 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_15,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_16 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_16,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_17 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_17,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_18 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_18,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_19 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_19,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_20 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_20,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_21 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_21,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_22 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_22,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_23 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_23,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_24 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_24,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_25 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_25,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_26 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_26,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_27 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_27,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_28 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_28,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_29 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_29,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_30 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_30,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_31 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_31,
    GPUSHADER_PRIMITIVE_CONTROL_POINT_PATCH_32 = DEVICESHADER_PRIMITIVE_CONTROL_POINT_PATCH_32
};
enum GPUShaderPrimitiveTopology {
    GPUSHADER_PRIMITIVETOPOLOGY_UNDEFINED                  = DEVICESHADER_PRIMITIVETOPOLOGY_UNDEFINED,
    GPUSHADER_PRIMITIVETOPOLOGY_POINTLIST                  = DEVICESHADER_PRIMITIVETOPOLOGY_POINTLIST,
    GPUSHADER_PRIMITIVETOPOLOGY_LINELIST                   = DEVICESHADER_PRIMITIVETOPOLOGY_LINELIST,
    GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP                  = DEVICESHADER_PRIMITIVETOPOLOGY_LINESTRIP,
    GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST               = DEVICESHADER_PRIMITIVETOPOLOGY_TRIANGLELIST,
    GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP              = DEVICESHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP,
    GPUSHADER_PRIMITIVETOPOLOGY_LINELIST_ADJ               = DEVICESHADER_PRIMITIVETOPOLOGY_LINELIST_ADJ,
    GPUSHADER_PRIMITIVETOPOLOGY_LINESTRIP_ADJ              = DEVICESHADER_PRIMITIVETOPOLOGY_LINESTRIP_ADJ,
    GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST_ADJ           = DEVICESHADER_PRIMITIVETOPOLOGY_TRIANGLELIST_ADJ,
    GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP_ADJ          = DEVICESHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP_ADJ,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_1  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_1,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_2  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_2,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_3  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_3,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_4  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_4,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_5  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_5,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_6  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_6,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_7  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_7,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_8  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_8,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_9  = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_9,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_10 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_10,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_11 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_11,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_12 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_12,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_13 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_13,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_14 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_14,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_15 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_15,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_16 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_16,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_17 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_17,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_18 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_18,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_19 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_19,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_20 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_20,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_21 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_21,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_22 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_22,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_23 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_23,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_24 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_24,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_25 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_25,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_26 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_26,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_27 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_27,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_28 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_28,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_29 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_29,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_30 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_30,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_31 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_31,
    GPUSHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_32 = DEVICESHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_32
};

enum GPUShaderTesselatorDomain {
    GPUSHADER_TESSELLATORDOMAIN_UNDEFINED = DEVICESHADER_TESSELLATORDOMAIN_UNDEFINED,
    GPUSHADER_TESSELLATORDOMAIN_ISOLINE   = DEVICESHADER_TESSELLATORDOMAIN_ISOLINE,
    GPUSHADER_TESSELLATORDOMAIN_TRIANGLE  = DEVICESHADER_TESSELLATORDOMAIN_TRIANGLE,
    GPUSHADER_TESSELLATORDOMAIN_QUAD      = DEVICESHADER_TESSELLATORDOMAIN_QUAD
};
enum GPUShaderTesselatorOutput {
    GPUSHADER_TESSELLATOROUTPUT_UNDEFINED    = DEVICESHADER_TESSELLATOROUTPUT_UNDEFINED,
    GPUSHADER_TESSELLATOROUTPUT_POINT        = DEVICESHADER_TESSELLATOROUTPUT_POINT,
    GPUSHADER_TESSELLATOROUTPUT_LINE         = DEVICESHADER_TESSELLATOROUTPUT_LINE,
    GPUSHADER_TESSELLATOROUTPUT_TRIANGLE_CW  = DEVICESHADER_TESSELLATOROUTPUT_TRIANGLE_CW,
    GPUSHADER_TESSELLATOROUTPUT_TRIANGLE_CCW = DEVICESHADER_TESSELLATOROUTPUT_TRIANGLE_CCW
};
enum GPUShaderTesselatorPartitioning {
    GPUSHADER_TESSELLATORPARTITIONING_UNDEFINED       = DEVICESHADER_TESSELLATORPARTITIONING_UNDEFINED,
    GPUSHADER_TESSELLATORPARTITIONING_INTEGER         = DEVICESHADER_TESSELLATORPARTITIONING_INTEGER,
    GPUSHADER_TESSELLATORPARTITIONING_POW2            = DEVICESHADER_TESSELLATORPARTITIONING_POW2,
    GPUSHADER_TESSELLATORPARTITIONING_FRACTIONAL_ODD  = DEVICESHADER_TESSELLATORPARTITIONING_FRACTIONAL_ODD,
    GPUSHADER_TESSELLATORPARTITIONING_FRACTIONAL_EVEN = DEVICESHADER_TESSELLATORPARTITIONING_FRACTIONAL_EVEN
};

typedef struct _gpu_shader_desc {
    UInt iVersion;
    const AChar * strCreator;

    Bool bIsSampleFrequencyShader;

    UInt iCompilationFlags; // ShaderCompilationFlags
    UInt iRequirementFlags; // ShaderRequirementFlags

    UInt iInputParameterCount;
    UInt iOutputParameterCount;
    UInt iPatchConstantParameterCount;
    UInt iBindingCount;
    UInt iConstantBufferCount;

    UInt iInterfaceSlotCount;

    UInt iTempRegisterCount;
    UInt iTempArrayCount;
    
    UInt iInstructionCount;

    UInt iIntInstructionCount;
    UInt iUIntInstructionCount;
    UInt iFloatInstructionCount;
    UInt iArrayInstructionCount;

    UInt iBitwiseInstructionCount;
    UInt iConversionInstructionCount;
    UInt iMOVInstructionCount;
    UInt iMOVCInstructionCount;

    UInt iDefInstructionCount;
    UInt iDclInstructionCount;
    UInt iMacroInstructionCount;

    UInt iTextureNormalInstructionCount;
    UInt iTextureLoadInstructionCount;
    UInt iTextureCompInstructionCount;
    UInt iTextureBiasInstructionCount;
    UInt iTextureGradientInstructionCount;

    UInt iCutInstructionCount;
    UInt iEmitInstructionCount;

    UInt iStaticFlowControlCount;
    UInt iDynamicFlowControlCount;

    GPUShaderPrimitive iGSHSInputPrimitive;

    GPUShaderPrimitiveTopology iGSOutputTopology;
    UInt iGSMaxOutputVertexCount;
    UInt iGSInstanceCount;

    UInt iTesselatorControlPointCount;
    GPUShaderTesselatorDomain iTesselatorDomain;
    GPUShaderTesselatorOutput iHSOutputPrimitive;
    GPUShaderTesselatorPartitioning iHSPartitioningMode;

    UInt iCSBarrierInstructionCount;
    UInt iCSInterlockedInstructionCount;
    UInt iCSTextureStoreInstructionCount;
} GPUShaderDesc;

enum GPUShaderParameterType {
    GPUSHADER_PARAMETER_UNDEFINED                     = DEVICESHADER_PARAMETER_UNDEFINED,
    GPUSHADER_PARAMETER_VERTEX_ID                     = DEVICESHADER_PARAMETER_VERTEX_ID,
    GPUSHADER_PARAMETER_PRIMITIVE_ID                  = DEVICESHADER_PARAMETER_PRIMITIVE_ID,
    GPUSHADER_PARAMETER_INSTANCE_ID                   = DEVICESHADER_PARAMETER_INSTANCE_ID,
    GPUSHADER_PARAMETER_POSITION                      = DEVICESHADER_PARAMETER_POSITION,
    GPUSHADER_PARAMETER_CLIP_DISTANCE                 = DEVICESHADER_PARAMETER_CLIP_DISTANCE,
    GPUSHADER_PARAMETER_CULL_DISTANCE                 = DEVICESHADER_PARAMETER_CULL_DISTANCE,
    GPUSHADER_PARAMETER_IS_FRONT_FACE                 = DEVICESHADER_PARAMETER_IS_FRONT_FACE,
    GPUSHADER_PARAMETER_SAMPLE_INDEX                  = DEVICESHADER_PARAMETER_SAMPLE_INDEX,
    GPUSHADER_PARAMETER_TARGET                        = DEVICESHADER_PARAMETER_TARGET,
    GPUSHADER_PARAMETER_RENDER_TARGET_ARRAY_INDEX     = DEVICESHADER_PARAMETER_RENDER_TARGET_ARRAY_INDEX,
    GPUSHADER_PARAMETER_VIEWPORT_ARRAY_INDEX          = DEVICESHADER_PARAMETER_VIEWPORT_ARRAY_INDEX,
    GPUSHADER_PARAMETER_DEPTH                         = DEVICESHADER_PARAMETER_DEPTH,
    GPUSHADER_PARAMETER_DEPTH_GREATER_EQUAL           = DEVICESHADER_PARAMETER_DEPTH_GREATER_EQUAL,
    GPUSHADER_PARAMETER_DEPTH_LESS_EQUAL              = DEVICESHADER_PARAMETER_DEPTH_LESS_EQUAL,
    GPUSHADER_PARAMETER_COVERAGE                      = DEVICESHADER_PARAMETER_COVERAGE,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DETAIL  = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DETAIL,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DENSITY = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DENSITY,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_TRI_EDGE     = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_TRI_EDGE,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_TRI_INSIDE   = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_TRI_INSIDE,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_EDGE    = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_EDGE,
    GPUSHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_INSIDE  = DEVICESHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_INSIDE
};
enum GPUShaderRegisterComponentType {
    GPUSHADER_REGISTERCOMPONENT_UNKNOWN = DEVICESHADER_REGISTERCOMPONENT_UNKNOWN,
    GPUSHADER_REGISTERCOMPONENT_FLOAT32 = DEVICESHADER_REGISTERCOMPONENT_FLOAT32,
    GPUSHADER_REGISTERCOMPONENT_UINT32  = DEVICESHADER_REGISTERCOMPONENT_UINT32,
    GPUSHADER_REGISTERCOMPONENT_SINT32  = DEVICESHADER_REGISTERCOMPONENT_SINT32
};
enum GPUShaderMinPrecision {
    GPUSHADER_MINPRECISION_FLOAT_32  = DEVICESHADER_MINPRECISION_FLOAT_32,
    GPUSHADER_MINPRECISION_FLOAT_16  = DEVICESHADER_MINPRECISION_FLOAT_16,
    GPUSHADER_MINPRECISION_FLOAT_2_8 = DEVICESHADER_MINPRECISION_FLOAT_2_8,
    GPUSHADER_MINPRECISION_UINT_16   = DEVICESHADER_MINPRECISION_UINT_16,
    GPUSHADER_MINPRECISION_SINT_16   = DEVICESHADER_MINPRECISION_SINT_16,
    GPUSHADER_MINPRECISION_ANY_16    = DEVICESHADER_MINPRECISION_ANY_16,
    GPUSHADER_MINPRECISION_ANY_10    = DEVICESHADER_MINPRECISION_ANY_10
};

typedef struct _gpu_shaderparameter_desc {
    UInt iStreamIndex;

    const AChar * strSemanticName;
    UInt iSemanticIndex;

    UInt iRegister;

    GPUShaderParameterType iSystemValueType;
    GPUShaderRegisterComponentType iComponentType;

    Byte iComponentMask; // Components to use
    Byte iReadWriteMask; // Never written (output) / Allways read (input) hints

    GPUShaderMinPrecision iMinPrecision; // For interpolation
} GPUShaderParameterDesc;

enum GPUShaderInputType {
    GPUSHADER_INPUT_CBUFFER                       = DEVICESHADER_INPUT_CBUFFER,
    GPUSHADER_INPUT_TBUFFER                       = DEVICESHADER_INPUT_TBUFFER,
    GPUSHADER_INPUT_TEXTURE                       = DEVICESHADER_INPUT_TEXTURE,
    GPUSHADER_INPUT_SAMPLER                       = DEVICESHADER_INPUT_SAMPLER,
    GPUSHADER_INPUT_BYTEADDRESS                   = DEVICESHADER_INPUT_BYTEADDRESS,
    GPUSHADER_INPUT_STRUCTURED                    = DEVICESHADER_INPUT_STRUCTURED,
    GPUSHADER_INPUT_UAV_RWBYTEADDRESS             = DEVICESHADER_INPUT_UAV_RWBYTEADDRESS,
    GPUSHADER_INPUT_UAV_RWSTRUCTURED              = DEVICESHADER_INPUT_UAV_RWSTRUCTURED,
    GPUSHADER_INPUT_UAV_RWSTRUCTURED_APPEND       = DEVICESHADER_INPUT_UAV_RWSTRUCTURED_APPEND,
    GPUSHADER_INPUT_UAV_RWSTRUCTURED_CONSUME      = DEVICESHADER_INPUT_UAV_RWSTRUCTURED_CONSUME,
    GPUSHADER_INPUT_UAV_RWSTRUCTURED_WITH_COUNTER = DEVICESHADER_INPUT_UAV_RWSTRUCTURED_WITH_COUNTER,
    GPUSHADER_INPUT_UAV_RWTYPED                   = DEVICESHADER_INPUT_UAV_RWTYPED
};
enum GPUShaderInputFlags {
    GPUSHADER_INPUTFLAG_NONE                = DEVICESHADER_INPUTFLAG_NONE,
    GPUSHADER_INPUTFLAG_USERPACKED          = DEVICESHADER_INPUTFLAG_USERPACKED,
    GPUSHADER_INPUTFLAG_COMPARISON_SAMPLER  = DEVICESHADER_INPUTFLAG_COMPARISON_SAMPLER,
    GPUSHADER_INPUTFLAG_TEXTURE_COMPONENT_0 = DEVICESHADER_INPUTFLAG_TEXTURE_COMPONENT_0,
    GPUSHADER_INPUTFLAG_TEXTURE_COMPONENT_1 = DEVICESHADER_INPUTFLAG_TEXTURE_COMPONENT_1,
    GPUSHADER_INPUTFLAG_UNUSED              = DEVICESHADER_INPUTFLAG_UNUSED,
    GPUSHADER_INPUTFLAG_TEXTURE_COMPONENTS  = DEVICESHADER_INPUTFLAG_TEXTURE_COMPONENTS
};
enum GPUShaderReturnType {
    GPUSHADER_RETURN_UNORM     = DEVICESHADER_RETURN_UNORM,
    GPUSHADER_RETURN_SNORM     = DEVICESHADER_RETURN_SNORM,
    GPUSHADER_RETURN_UINT      = DEVICESHADER_RETURN_UINT,
    GPUSHADER_RETURN_SINT      = DEVICESHADER_RETURN_SINT,
    GPUSHADER_RETURN_FLOAT     = DEVICESHADER_RETURN_FLOAT,
    GPUSHADER_RETURN_DOUBLE    = DEVICESHADER_RETURN_DOUBLE,
    GPUSHADER_RETURN_MIXED     = DEVICESHADER_RETURN_MIXED,
    GPUSHADER_RETURN_CONTINUED = DEVICESHADER_RETURN_CONTINUED

};

typedef struct _gpu_shaderbinding_desc {
    const AChar * strName;

    GPUShaderInputType iType;
    UInt iFlags; // GPUShaderInputFlags

    UInt iBindPoint;
    UInt iBindCount;

    GPUShaderReturnType iReturnType;
    GPUShaderViewDimension iViewDimension;
    UInt iSampleCount; // 0 = Multisampling disabled
} GPUShaderBindingDesc;

typedef struct _gpu_streamoutput_declaration {
    UInt iStream;

    const AChar * strSemanticName; // Set to NULL to create a gap in the stream output with no written data
                                   // In this case, iComponentCount can be greater than 4
    UInt iSemanticIndex;

    Byte iStartComponent; // First component to write out, 0-3
    Byte iComponentCount; // Components to write out, 1-4
    Byte iOutputSlot;     // Associated stream output buffer, 0-3
} GPUStreamOutputDeclaration;

// Prototypes
class RenderingManager;
class GPUInputLayout;

class GPUShaderReflectConstantBuffer;
class GPUShaderReflectVariable;
class GPUShaderReflectType;

class GPUShader;

/////////////////////////////////////////////////////////////////////////////////
// The GPUShaderCompiler class
class GPUShaderCompiler
{
public:
    GPUShaderCompiler();
    ~GPUShaderCompiler();

    // Compilation
    inline Bool IsCompiled() const;

    inline Void Compile( const AChar * strHLSLCode, UInt iHLSLCodeLength, const GPUShaderCompilationData * pCompilationData );
    inline Void Compile( const GChar * strFilename, const GPUShaderCompilationData * pCompilationData );
    inline Void Clear();

    // Getters
    inline const Byte * GetByteCode() const;
    inline UInt GetByteCodeLength() const;

private:
    friend class GPUInputLayout;
    friend class GPUShader;

    // Device data
    DeviceShaderCompiler m_hDeviceShaderCompiler;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUShaderReflectConstantBuffer class
class GPUShaderReflectConstantBuffer
{
public:
    GPUShaderReflectConstantBuffer();
    ~GPUShaderReflectConstantBuffer();

    // Methods
    inline Void GetDesc( GPUShaderConstantBufferDesc * outDesc ) const;

    inline UInt GetVariableCount() const;
    inline Void GetVariable( GPUShaderReflectVariable * outVariable, UInt iIndex ) const;
    inline Void GetVariable( GPUShaderReflectVariable * outVariable, const AChar * strName ) const;

private:
    friend class GPUShader;
    friend class GPUShaderReflectVariable;

    // Device data
    DeviceShaderReflectConstantBuffer m_hDeviceShaderReflectConstantBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUShaderReflectVariable class
class GPUShaderReflectVariable
{
public:
    GPUShaderReflectVariable();
    ~GPUShaderReflectVariable();

    // Methods
    inline Void GetDesc( GPUShaderVariableDesc * outDesc ) const;

    inline UInt GetInterfaceSlot( UInt iArrayIndex ) const;

    inline Void GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer ) const;
    inline Void GetType( GPUShaderReflectType * outType ) const;

private:
    friend class GPUShader;
    friend class GPUShaderReflectConstantBuffer;
    friend class GPUShaderReflectType;

    // Device data
    DeviceShaderReflectVariable m_hDeviceShaderReflectVariable;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUShaderReflectType class
class GPUShaderReflectType
{
public:
    GPUShaderReflectType();
    ~GPUShaderReflectType();

    // Methods
    inline Void GetDesc( GPUShaderTypeDesc * outDesc ) const;

    inline Bool IsEqual( const GPUShaderReflectType * pType ) const;
    inline Bool InheritsFrom( const GPUShaderReflectType * pBaseType ) const;
    inline Bool ImplementsInterface( const GPUShaderReflectType * pInterface ) const;

    inline Void GetBaseClass( GPUShaderReflectType * outBaseClass ) const;
    inline Void GetVariableBaseClass( GPUShaderReflectType * outBaseClass ) const;

    inline UInt GetInterfaceCount() const;
    inline Void GetInterface( GPUShaderReflectType * outInterface, UInt iIndex ) const;

    inline UInt GetMemberTypeCount() const;
    inline const AChar * GetMemberTypeName( UInt iIndex ) const;
    inline Void GetMemberType( GPUShaderReflectType * outMember, UInt iIndex ) const;
    inline Void GetMemberType( GPUShaderReflectType * outMember, const AChar * strName ) const;

private:
    friend class GPUShaderReflectVariable;

    // Device data
    DeviceShaderReflectType m_hDeviceShaderReflectType;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUShader class
class GPUShader
{
protected:
    GPUShader();
public:
    virtual ~GPUShader();

    // Binding
    inline Bool IsBound() const;

    Void Bind( const GPUShaderCompiler * pCompiledShader, Bool bReflect = false, Bool bDynamicLinking = false );
    Void UnBind();

    // Getters
    inline GPUShaderType GetType() const;

    // Reflection
    inline Bool HasReflection() const;

    inline Void GetDesc( GPUShaderDesc * outDesc ) const;

    inline UInt GetInputParameterCount() const;
    inline Void GetInputParameterDesc( GPUShaderParameterDesc * outInputParameterDesc, UInt iIndex ) const;

    inline UInt GetOutputParameterCount() const;
    inline Void GetOutputParameterDesc( GPUShaderParameterDesc * outOutputParameterDesc, UInt iIndex ) const;

    inline UInt GetPatchConstantParameterCount() const;
    inline Void GetPatchConstantParameterDesc( GPUShaderParameterDesc * outPatchConstantParameterDesc, UInt iIndex ) const;

    inline UInt GetBindingCount() const;
    inline Void GetBindingDesc( GPUShaderBindingDesc * outBindingDesc, UInt iIndex ) const;
    inline Void GetBindingDesc( GPUShaderBindingDesc * outBindingDesc, const AChar * strName ) const;

    inline UInt GetConstantBufferCount() const;
    inline Void GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer, UInt iIndex ) const;
    inline Void GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer, const AChar * strName ) const;

    inline Void GetVariable( GPUShaderReflectVariable * outVariable, const AChar * strName ) const;

    inline UInt GetInterfaceSlotCount() const;

    inline Void GetThreadGroupSize( UInt * outSizeX, UInt * outSizeY, UInt * outSizeZ ) const;

    // Dynamic linkage table (you must add them all before using the shader)
    inline Bool HasDynamicLinkage() const;
    inline Void AddDynamicLink( const AChar * strInterfaceInstance, const AChar * strClassInstance );

protected:
    friend class RenderingManager;

    // Device data
    DeviceShader * m_pDeviceShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUVertexShader class
class GPUVertexShader : public GPUShader
{
public:
    GPUVertexShader();
    virtual ~GPUVertexShader();

protected:
    friend class RenderingManager;

    // Device data
    DeviceVertexShader m_hDeviceVertexShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUGeometryShader class
class GPUGeometryShader : public GPUShader
{
public:
    GPUGeometryShader();
    virtual ~GPUGeometryShader();

    // Deferred construction
    inline Bool IsFinalized() const;

    inline Void PushSODeclaration( const GPUStreamOutputDeclaration * pSODeclaration );
    inline Void Finalize( const UInt * arrStrides = NULL, UInt iStrideCount = 0, UInt iRasterStream = INVALID_OFFSET );

    // Getters
    inline UInt GetSODeclarationCount() const;
    inline Void GetSODeclaration( GPUStreamOutputDeclaration * outSODeclaration, UInt iSODeclaration ) const;

    inline UInt GetStrideCount() const;
    inline UInt GetStride( UInt iStreamOutputSlot ) const;

    inline UInt GetRasterStream() const; // INVALID_OFFET => no raster stream

protected:
    friend class RenderingManager;

    // Device data
    DeviceGeometryShader m_hDeviceGeometryShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUPixelShader class
class GPUPixelShader : public GPUShader
{
public:
    GPUPixelShader();
    virtual ~GPUPixelShader();

protected:
    friend class RenderingManager;

    // Device data
    DevicePixelShader m_hDevicePixelShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUHullShader class
class GPUHullShader : public GPUShader
{
public:
    GPUHullShader();
    virtual ~GPUHullShader();

protected:
    friend class RenderingManager;

    // Device data
    DeviceHullShader m_hDeviceHullShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUDomainShader class
class GPUDomainShader : public GPUShader
{
public:
    GPUDomainShader();
    virtual ~GPUDomainShader();

protected:
    friend class RenderingManager;

    // Device data
    DeviceDomainShader m_hDeviceDomainShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUComputeShader class
class GPUComputeShader : public GPUShader
{
public:
    GPUComputeShader();
    virtual ~GPUComputeShader();

protected:
    friend class RenderingManager;

    // Device data
    DeviceComputeShader m_hDeviceComputeShader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUShader.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUSHADER_H


