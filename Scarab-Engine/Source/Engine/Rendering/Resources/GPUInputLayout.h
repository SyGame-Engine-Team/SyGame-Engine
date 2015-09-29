/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUInputLayout.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Input Layout
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUINPUTLAYOUT_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUINPUTLAYOUT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Table/TreeMap.h"

#include "GPUDeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUInputLayout definitions
#define GPUINPUTLAYOUT_MAX_FIELDS    DEVICEINPUTLAYOUT_MAX_FIELDS
#define GPUINPUTLAYOUT_MAX_COLORS    DEVICEINPUTLAYOUT_MAX_COLORS
#define GPUINPUTLAYOUT_MAX_TEXCOORDS DEVICEINPUTLAYOUT_MAX_TEXCOORDS

enum GPUInputFieldType {
    GPUINPUTFIELD_TYPE_FLOAT  = DEVICEINPUTFIELD_FLOAT,
    GPUINPUTFIELD_TYPE_FLOAT2 = DEVICEINPUTFIELD_FLOAT2,
    GPUINPUTFIELD_TYPE_FLOAT3 = DEVICEINPUTFIELD_FLOAT3,
    GPUINPUTFIELD_TYPE_FLOAT4 = DEVICEINPUTFIELD_FLOAT4,

    GPUINPUTFIELD_TYPE_BYTE   = DEVICEINPUTFIELD_BYTE,
    GPUINPUTFIELD_TYPE_BYTEN  = DEVICEINPUTFIELD_BYTEN,
    GPUINPUTFIELD_TYPE_BYTE2  = DEVICEINPUTFIELD_BYTE2,
    GPUINPUTFIELD_TYPE_BYTE2N = DEVICEINPUTFIELD_BYTE2N,
    GPUINPUTFIELD_TYPE_BYTE4  = DEVICEINPUTFIELD_BYTE4,
    GPUINPUTFIELD_TYPE_BYTE4N = DEVICEINPUTFIELD_BYTE4N,

    GPUINPUTFIELD_TYPE_SSHORT   = DEVICEINPUTFIELD_SSHORT,
    GPUINPUTFIELD_TYPE_SSHORTN  = DEVICEINPUTFIELD_SSHORTN,
    GPUINPUTFIELD_TYPE_SSHORT2  = DEVICEINPUTFIELD_SSHORT2,
    GPUINPUTFIELD_TYPE_SSHORT2N = DEVICEINPUTFIELD_SSHORT2N,
    GPUINPUTFIELD_TYPE_SSHORT4  = DEVICEINPUTFIELD_SSHORT4,
    GPUINPUTFIELD_TYPE_SSHORT4N = DEVICEINPUTFIELD_SSHORT4N,

    GPUINPUTFIELD_TYPE_USHORT   = DEVICEINPUTFIELD_USHORT,
    GPUINPUTFIELD_TYPE_USHORTN  = DEVICEINPUTFIELD_USHORTN,
    GPUINPUTFIELD_TYPE_USHORT2  = DEVICEINPUTFIELD_USHORT2,
    GPUINPUTFIELD_TYPE_USHORT2N = DEVICEINPUTFIELD_USHORT2N,
    GPUINPUTFIELD_TYPE_USHORT4  = DEVICEINPUTFIELD_USHORT4,
    GPUINPUTFIELD_TYPE_USHORT4N = DEVICEINPUTFIELD_USHORT4N,

    GPUINPUTFIELD_TYPE_SINT  = DEVICEINPUTFIELD_SINT,
    GPUINPUTFIELD_TYPE_SINT2 = DEVICEINPUTFIELD_SINT2,
    GPUINPUTFIELD_TYPE_SINT3 = DEVICEINPUTFIELD_SINT3,
    GPUINPUTFIELD_TYPE_SINT4 = DEVICEINPUTFIELD_SINT4,

    GPUINPUTFIELD_TYPE_UINT  = DEVICEINPUTFIELD_UINT,
    GPUINPUTFIELD_TYPE_UINT2 = DEVICEINPUTFIELD_UINT2,
    GPUINPUTFIELD_TYPE_UINT3 = DEVICEINPUTFIELD_UINT3,
    GPUINPUTFIELD_TYPE_UINT4 = DEVICEINPUTFIELD_UINT4,

    GPUINPUTFIELD_TYPE_COUNT = DEVICEINPUTFIELD_COUNT
};

#define GPUINPUTFIELD_SEMANTIC_POSITION      DEVICEINPUTFIELD_SEMANTIC_POSITION
#define GPUINPUTFIELD_SEMANTIC_TPOSITION     DEVICEINPUTFIELD_SEMANTIC_TPOSITION
#define GPUINPUTFIELD_SEMANTIC_NORMAL        DEVICEINPUTFIELD_SEMANTIC_NORMAL
#define GPUINPUTFIELD_SEMANTIC_TANGENT       DEVICEINPUTFIELD_SEMANTIC_TANGENT
#define GPUINPUTFIELD_SEMANTIC_BINORMAL      DEVICEINPUTFIELD_SEMANTIC_BINORMAL
#define GPUINPUTFIELD_SEMANTIC_COLOR         DEVICEINPUTFIELD_SEMANTIC_COLOR
#define GPUINPUTFIELD_SEMANTIC_TEXCOORD      DEVICEINPUTFIELD_SEMANTIC_TEXCOORD
#define GPUINPUTFIELD_SEMANTIC_BLEND_INDICES DEVICEINPUTFIELD_SEMANTIC_BLEND_INDICES
#define GPUINPUTFIELD_SEMANTIC_BLEND_WEIGHTS DEVICEINPUTFIELD_SEMANTIC_BLEND_WEIGHTS
#define GPUINPUTFIELD_SEMANTIC_POINTSIZE     DEVICEINPUTFIELD_SEMANTIC_POINTSIZE

enum GPUInputFieldSlotClass {
    GPUINPUTFIELD_SLOTCLASS_PER_VERTEX   = DEVICEINPUTFIELD_SLOTCLASS_PER_VERTEX,
    GPUINPUTFIELD_SLOTCLASS_PER_INSTANCE = DEVICEINPUTFIELD_SLOTCLASS_PER_INSTANCE
};

typedef struct _gpu_input_field_desc {
    GPUInputFieldType iType;

    const AChar * strSemantic;
    UInt iSemanticIndex;

    UInt iSlot; // 0-15
    GPUInputFieldSlotClass iSlotClass;

    UInt iInstanceDataStepRate; // Per-Instance fields only, number of instances
                                // to pass with a given per-instance data before
                                // advancing to next per-instance data
} GPUInputFieldDesc;

// Prototypes
class RenderingManager;

class GPUShaderCompiler;

/////////////////////////////////////////////////////////////////////////////////
// The GPUInputLayout class
class GPUInputLayout
{
    // InputFieldType infos
public:
    static UInt GetComponentSize( GPUInputFieldType iType );
    static UInt GetComponentCount( GPUInputFieldType iType );
    static UInt GetTypeSize( GPUInputFieldType iType );

public:
    GPUInputLayout( Bool bUseFieldMap = true );
    ~GPUInputLayout();

    // Deferred construction
    inline Bool IsFinalized() const;

    Void PushField( const GPUInputFieldDesc * pFieldDesc );
    inline Void Finalize();

    // Binding
    inline Bool IsBound() const;

    Void Bind( const GPUShaderCompiler * pCompiledShader );
    Void UnBind();

    // Getters
    inline UInt GetFieldCount() const;
    inline Void GetField( GPUInputFieldDesc * outFieldDesc, UInt iField ) const;

    inline Bool HasFieldMap() const;
    inline Bool HasField( const AChar * strSemantic, UInt iSemanticIndex = 0 ) const;
    inline Void GetFieldIndex( UInt * outField, const AChar * strSemantic, UInt iSemanticIndex = 0 ) const;
    inline Void GetFieldRange( UInt * outOffset, UInt * outSize, const AChar * strSemantic, UInt iSemanticIndex = 0 ) const;
    inline Void GetField( GPUInputFieldDesc * outFieldDesc, const AChar * strSemantic, UInt iSemanticIndex = 0 ) const;

    inline UInt GetSize() const;

private:
    friend class RenderingManager;

    // Fields data
    struct _FieldIndex {
        const AChar * strSemantic;
        UInt iSemanticIndex;
    };
    struct _FieldData {
        UInt iField;
        UInt iOffset, iSize;
    };

    typedef TreeMap<_FieldIndex, _FieldData> _FieldMap;
    inline static Int _Compare_FieldIndices( const _FieldIndex & hLeft, const _FieldIndex & hRight, Void * pUserData );

    const Bool m_bUseFieldMap;
    _FieldMap m_mapFields;

    UInt m_iSize;

    // Device data
    DeviceInputLayout m_hDeviceInputLayout;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUInputLayout.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUINPUTLAYOUT_H
