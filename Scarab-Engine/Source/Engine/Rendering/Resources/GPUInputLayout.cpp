/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUInputLayout.cpp
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
// Includes
#include "GPUInputLayout.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUInputLayout implementation
UInt GPUInputLayout::GetComponentSize( GPUInputFieldType iType )
{
    switch( iType ) {
        case GPUINPUTFIELD_TYPE_FLOAT:    return 4;
        case GPUINPUTFIELD_TYPE_FLOAT2:   return 4;
        case GPUINPUTFIELD_TYPE_FLOAT3:   return 4;
        case GPUINPUTFIELD_TYPE_FLOAT4:   return 4;
        case GPUINPUTFIELD_TYPE_BYTE:     return 1;
        case GPUINPUTFIELD_TYPE_BYTEN:    return 1;
        case GPUINPUTFIELD_TYPE_BYTE2:    return 1;
        case GPUINPUTFIELD_TYPE_BYTE2N:   return 1;
        case GPUINPUTFIELD_TYPE_BYTE4:    return 1;
        case GPUINPUTFIELD_TYPE_BYTE4N:   return 1;
        case GPUINPUTFIELD_TYPE_SSHORT:   return 2;
        case GPUINPUTFIELD_TYPE_SSHORTN:  return 2;
        case GPUINPUTFIELD_TYPE_SSHORT2:  return 2;
        case GPUINPUTFIELD_TYPE_SSHORT2N: return 2;
        case GPUINPUTFIELD_TYPE_SSHORT4:  return 2;
        case GPUINPUTFIELD_TYPE_SSHORT4N: return 2;
        case GPUINPUTFIELD_TYPE_USHORT:   return 2;
        case GPUINPUTFIELD_TYPE_USHORTN:  return 2;
        case GPUINPUTFIELD_TYPE_USHORT2:  return 2;
        case GPUINPUTFIELD_TYPE_USHORT2N: return 2;
        case GPUINPUTFIELD_TYPE_USHORT4:  return 2;
        case GPUINPUTFIELD_TYPE_USHORT4N: return 2;
        case GPUINPUTFIELD_TYPE_SINT:     return 4;
        case GPUINPUTFIELD_TYPE_SINT2:    return 4;
        case GPUINPUTFIELD_TYPE_SINT3:    return 4;
        case GPUINPUTFIELD_TYPE_SINT4:    return 4;
        case GPUINPUTFIELD_TYPE_UINT:     return 4;
        case GPUINPUTFIELD_TYPE_UINT2:    return 4;
        case GPUINPUTFIELD_TYPE_UINT3:    return 4;
        case GPUINPUTFIELD_TYPE_UINT4:    return 4;
        default: Assert(false); break;
    }
    return 0;
}
UInt GPUInputLayout::GetComponentCount( GPUInputFieldType iType )
{
    switch( iType ) {
        case GPUINPUTFIELD_TYPE_FLOAT:    return 1;
        case GPUINPUTFIELD_TYPE_FLOAT2:   return 2;
        case GPUINPUTFIELD_TYPE_FLOAT3:   return 3;
        case GPUINPUTFIELD_TYPE_FLOAT4:   return 4;
        case GPUINPUTFIELD_TYPE_BYTE:     return 1;
        case GPUINPUTFIELD_TYPE_BYTEN:    return 1;
        case GPUINPUTFIELD_TYPE_BYTE2:    return 2;
        case GPUINPUTFIELD_TYPE_BYTE2N:   return 2;
        case GPUINPUTFIELD_TYPE_BYTE4:    return 4;
        case GPUINPUTFIELD_TYPE_BYTE4N:   return 4;
        case GPUINPUTFIELD_TYPE_SSHORT:   return 1;
        case GPUINPUTFIELD_TYPE_SSHORTN:  return 1;
        case GPUINPUTFIELD_TYPE_SSHORT2:  return 2;
        case GPUINPUTFIELD_TYPE_SSHORT2N: return 2;
        case GPUINPUTFIELD_TYPE_SSHORT4:  return 4;
        case GPUINPUTFIELD_TYPE_SSHORT4N: return 4;
        case GPUINPUTFIELD_TYPE_USHORT:   return 1;
        case GPUINPUTFIELD_TYPE_USHORTN:  return 1;
        case GPUINPUTFIELD_TYPE_USHORT2:  return 2;
        case GPUINPUTFIELD_TYPE_USHORT2N: return 2;
        case GPUINPUTFIELD_TYPE_USHORT4:  return 4;
        case GPUINPUTFIELD_TYPE_USHORT4N: return 4;
        case GPUINPUTFIELD_TYPE_SINT:     return 1;
        case GPUINPUTFIELD_TYPE_SINT2:    return 2;
        case GPUINPUTFIELD_TYPE_SINT3:    return 3;
        case GPUINPUTFIELD_TYPE_SINT4:    return 4;
        case GPUINPUTFIELD_TYPE_UINT:     return 1;
        case GPUINPUTFIELD_TYPE_UINT2:    return 2;
        case GPUINPUTFIELD_TYPE_UINT3:    return 3;
        case GPUINPUTFIELD_TYPE_UINT4:    return 4;
        default: Assert(false); break;
    }
    return 0;
}
UInt GPUInputLayout::GetTypeSize( GPUInputFieldType iType )
{
    switch( iType ) {
        case GPUINPUTFIELD_TYPE_FLOAT:    return 4;
        case GPUINPUTFIELD_TYPE_FLOAT2:   return 8;
        case GPUINPUTFIELD_TYPE_FLOAT3:   return 12;
        case GPUINPUTFIELD_TYPE_FLOAT4:   return 16;
        case GPUINPUTFIELD_TYPE_BYTE:     return 1;
        case GPUINPUTFIELD_TYPE_BYTEN:    return 1;
        case GPUINPUTFIELD_TYPE_BYTE2:    return 2;
        case GPUINPUTFIELD_TYPE_BYTE2N:   return 2;
        case GPUINPUTFIELD_TYPE_BYTE4:    return 4;
        case GPUINPUTFIELD_TYPE_BYTE4N:   return 4;
        case GPUINPUTFIELD_TYPE_SSHORT:   return 2;
        case GPUINPUTFIELD_TYPE_SSHORTN:  return 2;
        case GPUINPUTFIELD_TYPE_SSHORT2:  return 4;
        case GPUINPUTFIELD_TYPE_SSHORT2N: return 4;
        case GPUINPUTFIELD_TYPE_SSHORT4:  return 8;
        case GPUINPUTFIELD_TYPE_SSHORT4N: return 8;
        case GPUINPUTFIELD_TYPE_USHORT:   return 2;
        case GPUINPUTFIELD_TYPE_USHORTN:  return 2;
        case GPUINPUTFIELD_TYPE_USHORT2:  return 4;
        case GPUINPUTFIELD_TYPE_USHORT2N: return 4;
        case GPUINPUTFIELD_TYPE_USHORT4:  return 8;
        case GPUINPUTFIELD_TYPE_USHORT4N: return 8;
        case GPUINPUTFIELD_TYPE_SINT:     return 4;
        case GPUINPUTFIELD_TYPE_SINT2:    return 8;
        case GPUINPUTFIELD_TYPE_SINT3:    return 12;
        case GPUINPUTFIELD_TYPE_SINT4:    return 16;
        case GPUINPUTFIELD_TYPE_UINT:     return 4;
        case GPUINPUTFIELD_TYPE_UINT2:    return 8;
        case GPUINPUTFIELD_TYPE_UINT3:    return 12;
        case GPUINPUTFIELD_TYPE_UINT4:    return 16;
        default: Assert(false); break;
    }
    return 0;
}

GPUInputLayout::GPUInputLayout( Bool bUseFieldMap ):
    m_bUseFieldMap( bUseFieldMap ), m_mapFields(), m_hDeviceInputLayout( RenderingFn->m_pDeviceRenderer )
{
    if ( m_bUseFieldMap ) {
        m_mapFields.UseMemoryContext( RenderingFn->GetMemoryContext(), TEXT("Scratch") );
        m_mapFields.SetComparator( _Compare_FieldIndices, NULL );
        m_mapFields.Create();
    }

    m_iSize = 0;
}
GPUInputLayout::~GPUInputLayout()
{
    if ( IsBound() )
        UnBind();

    if ( m_bUseFieldMap )
        m_mapFields.Destroy();
}

Void GPUInputLayout::PushField( const GPUInputFieldDesc * pFieldDesc )
{
    Assert( !(IsFinalized()) );

    UInt iTypeSize = GetTypeSize( pFieldDesc->iType );

    if ( m_bUseFieldMap ) {
        _FieldIndex hIndex;
        hIndex.strSemantic = pFieldDesc->strSemantic;
        hIndex.iSemanticIndex = pFieldDesc->iSemanticIndex;

        _FieldData hData;
        hData.iField = m_hDeviceInputLayout.GetFieldCount();
        hData.iOffset = m_iSize;
        hData.iSize = iTypeSize;

        Bool bInserted = m_mapFields.Insert( hIndex, hData );
        Assert( bInserted );
    }

    m_iSize += iTypeSize;

    m_hDeviceInputLayout.PushField( (const DeviceInputFieldDesc *)pFieldDesc );
}

Void GPUInputLayout::Bind( const GPUShaderCompiler * pCompiledShader )
{
    Assert( IsFinalized() );
    Assert( !(IsBound()) );

    m_hDeviceInputLayout.Create( &(pCompiledShader->m_hDeviceShaderCompiler) );

    RenderingFn->_RegisterInputLayout( this );
}
Void GPUInputLayout::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterInputLayout( this );

    m_hDeviceInputLayout.Destroy();
}

