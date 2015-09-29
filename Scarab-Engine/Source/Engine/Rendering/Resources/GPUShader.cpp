/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUShader.cpp
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
// Includes
#include "GPUShader.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderCompiler implementation
GPUShaderCompiler::GPUShaderCompiler():
    m_hDeviceShaderCompiler()
{
    // nothing to do
}
GPUShaderCompiler::~GPUShaderCompiler()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectConstantBuffer implementation
GPUShaderReflectConstantBuffer::GPUShaderReflectConstantBuffer():
    m_hDeviceShaderReflectConstantBuffer()
{
    // nothing to do
}
GPUShaderReflectConstantBuffer::~GPUShaderReflectConstantBuffer()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectVariable implementation
GPUShaderReflectVariable::GPUShaderReflectVariable():
    m_hDeviceShaderReflectVariable()
{
    // nothing to do
}
GPUShaderReflectVariable::~GPUShaderReflectVariable()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectType implementation
GPUShaderReflectType::GPUShaderReflectType():
    m_hDeviceShaderReflectType()
{
    // nothing to do
}
GPUShaderReflectType::~GPUShaderReflectType()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShader implementation
GPUShader::GPUShader()
{
    m_pDeviceShader = NULL;
}
GPUShader::~GPUShader()
{
    // nothing to do
}

Void GPUShader::Bind( const GPUShaderCompiler * pCompiledShader, Bool bReflect, Bool bDynamicLinking )
{
    Assert( !(IsBound()) );

    m_pDeviceShader->Create( &(pCompiledShader->m_hDeviceShaderCompiler), bReflect, bDynamicLinking );

    RenderingFn->_RegisterShader( this );
}
Void GPUShader::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterShader( this );

    m_pDeviceShader->Destroy();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUVertexShader implementation
GPUVertexShader::GPUVertexShader():
    GPUShader(), m_hDeviceVertexShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDeviceVertexShader;
}
GPUVertexShader::~GPUVertexShader()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUGeometryShader implementation
GPUGeometryShader::GPUGeometryShader():
    GPUShader(), m_hDeviceGeometryShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDeviceGeometryShader;
}
GPUGeometryShader::~GPUGeometryShader()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUPixelShader implementation
GPUPixelShader::GPUPixelShader():
    GPUShader(), m_hDevicePixelShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDevicePixelShader;
}
GPUPixelShader::~GPUPixelShader()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUHullShader implementation
GPUHullShader::GPUHullShader():
    GPUShader(), m_hDeviceHullShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDeviceHullShader;
}
GPUHullShader::~GPUHullShader()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUDomainShader implementation
GPUDomainShader::GPUDomainShader():
    GPUShader(), m_hDeviceDomainShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDeviceDomainShader;
}
GPUDomainShader::~GPUDomainShader()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUComputeShader implementation
GPUComputeShader::GPUComputeShader():
    GPUShader(), m_hDeviceComputeShader( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceShader = &m_hDeviceComputeShader;
}
GPUComputeShader::~GPUComputeShader()
{
    if ( IsBound() )
        UnBind();
}

