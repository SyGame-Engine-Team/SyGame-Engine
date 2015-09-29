/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/EffectPlanarShadows.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Global Effect : Shadows casting on planes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Max plane count depends on stencil buffer size, most of times
//              you'll have an 8-bits buffer, making the max 256.
//              TODO = Compute visible sets from each projector ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARSHADOWS_H
#define SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARSHADOWS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Rendering/Resources/Light.h"
#include "../../Rendering/Effects/EffectLibrary/Lighting/EffectMaterial.h"

#include "../../Modeling/Primitives/Planar/MeshQuad.h"

#include "GlobalEffect.h"

#include "../Nodes/WorldLeaf.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

///////////////////////////////////////////////////////////////////////////////////
//// The ShadowPlane class
//class ShadowPlane : public WorldLeaf
//{
//public:
//    ShadowPlane( MeshQuad * pPlaneMesh, EffectInstance * pPlaneEffect,
//                 const GChar * strName = NULL );
//    virtual ~ShadowPlane();
//
//    // Plane access
//    inline const Plane & GetPlane() const;
//
//    // ShadowColor access
//    inline const Color4 & GetShadowColor() const;
//    inline Void SetShadowColor( const Color4 & colShadowColor );
//
//    // Projection matrix
//    Bool ComputeProjection( Matrix4 * outProjection, const Light * pProjector, const WorldLeaf * pShadowCaster ) const;
//
//protected:
//    Plane m_vPlane;
//    Color4 m_colShadowColor;
//};
//
///////////////////////////////////////////////////////////////////////////////////
//// The EffectPlanarShadows class
//class EffectPlanarShadows : public GlobalEffect
//{
//public:
//    EffectPlanarShadows( UInt iPlaneCount, UInt iProjectorCount );
//    virtual ~EffectPlanarShadows();
//
//    // Planes access
//    inline UInt GetPlaneCount() const;
//    inline ShadowPlane * GetPlane( UInt iPlane ) const;
//    inline Void SetPlane( UInt iPlane, ShadowPlane * pPlane );
//
//    // Projector access
//    inline UInt GetProjectorCount() const;
//    inline Light * GetProjector( UInt iProjector ) const;
//    inline Void SetProjector( UInt iProjector, Light * pProjector );
//
//    // Rendering support
//    virtual Void Render( WorldLeaf * pShadowCaster );
//
//protected:
//    UInt m_iPlaneCount;
//    ShadowPlane ** m_arrPlanes;
//
//    UInt m_iProjectorCount;
//    Light ** m_arrProjectors;
//
//    DepthState m_DepthState;
//    StencilState m_StencilState;
//    BlendingState m_BlendingState;
//
//    Material m_Material;
//    EffectMaterial m_MaterialEffect;
//    EffectInstance * m_pMaterialEffectInstance;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectPlanarShadows.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARSHADOWS_H
