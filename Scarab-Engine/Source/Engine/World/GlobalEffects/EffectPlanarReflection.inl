/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/GlobalEffects/EffectPlanarReflection.inl
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// EffectPlanarReflection implementation
//inline UInt EffectPlanarReflection::GetPlaneCount() const {
//    return m_iPlaneCount;
//}
//inline WorldTriangleMesh * EffectPlanarReflection::GetPlane( UInt iPlane ) const {
//    Assert( iPlane < m_iPlaneCount );
//    return m_arrPlanes[iPlane];
//}
//inline Void EffectPlanarReflection::SetPlane( UInt iPlane, WorldTriangleMesh * pPlaneMesh ) {
//    Assert( iPlane < m_iPlaneCount );
//    m_arrPlanes[iPlane] = pPlaneMesh;
//    m_arrPlanes[iPlane]->CullMode = WORLD_CULL_ALLWAYS;
//}
//
//inline Scalar EffectPlanarReflection::GetReflectance( UInt iPlane ) const {
//    Assert( iPlane < m_iPlaneCount );
//    return m_arrReflectances[iPlane];
//}
//inline Void EffectPlanarReflection::SetReflectance( UInt iPlane, Scalar fReflectance ) {
//    Assert( iPlane < m_iPlaneCount );
//    m_arrReflectances[iPlane] = fReflectance;
//}
