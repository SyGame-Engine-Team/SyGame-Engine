/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/EffectPlanarReflection.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Global Effect : Reflection of silhouettes on planes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Max plane count depends on stencil buffer size, most of times
//              you'll have an 8-bits buffer, making the max 256.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARREFLECTION_H
#define SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARREFLECTION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GlobalEffect.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The EffectPlanarReflection class
//class EffectPlanarReflection : public GlobalEffect
//{
//public:
//    EffectPlanarReflection( UInt iPlaneCount );
//    virtual ~EffectPlanarReflection();
//
//    virtual Void Draw( const VisibleGroup & visibleNodes );
//
//    // Planes access
//    inline UInt GetPlaneCount() const;
//    inline WorldTriangleMesh * GetPlane( UInt iPlane ) const;
//    inline Void SetPlane( UInt iPlane, WorldTriangleMesh * pPlaneMesh );
//
//    // Reflectance factors access
//    inline Scalar GetReflectance( UInt iPlane ) const;
//    inline Void SetReflectance( UInt iPlane, Scalar fReflectance );
//
//protected:
//    Void _GetReflectionData( UInt iPlane, Matrix4 & outReflection, Plane & outModelPlane ) const;
//
//    UInt m_iPlaneCount;
//    WorldTriangleMesh ** m_arrPlanes;
//    Scalar * m_arrReflectances;
//
//    CullingState m_CullingState;
//    DepthState m_DepthState;
//    StencilState m_StencilState;
//    BlendingState m_BlendingState;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectPlanarReflection.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_GLOBALEFFECTS_EFFECTPLANARREFLECTION_H
