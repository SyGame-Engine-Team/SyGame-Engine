/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/LOD/WorldDLODNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete Level Of Detail by model switching
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Move this away to World manager (CLOD meshes too)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_MODELING_LOD_WORLDDLODNODE_H
#define SCARAB_ENGINE_MODELING_LOD_WORLDDLODNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Rendering/Resources/Camera.h"

#include "../../World/Container/WorldSwitchNode.h"
#include "../../World/WorldCuller.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldDLODNode class
class WorldDLODNode : public WorldSwitchNode
{
public:
    WorldDLODNode( const GChar * strName, UInt iLODCount );
    virtual ~WorldDLODNode();

    // Access to center
    inline const Vertex3 & GetModelCenter() const;
    inline const Vertex3 & GetWorldCenter() const;
    inline Void SetModelCenter( const Vertex3 & vCenter );

    // Access to LOD distances intervals (all squared)
    inline UInt GetLODCount() const;
    inline Scalar GetModelMinDistance( UInt iLOD ) const;
    inline Scalar GetModelMaxDistance( UInt iLOD ) const;
    inline Scalar GetWorldMinDistance( UInt iLOD ) const;
    inline Scalar GetWorldMaxDistance( UInt iLOD ) const;
    inline Void SetModelDistance( UInt iLOD, Scalar fMin, Scalar fMax );

protected:
    Void _SelectLevelOfDetail( const Camera * pCamera );

    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

    // Center
    Vertex3 m_vModelCenter;
    Vertex3 m_vWorldCenter;

    // LOD intervals (squared distances)
    UInt m_iLODCount; // == child count
    Scalar * m_arrModelMinDist;
    Scalar * m_arrModelMaxDist;
    Scalar * m_arrWorldMinDist;
    Scalar * m_arrWorldMaxDist;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldDLODNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_LOD_WORLDDLODNODE_H

