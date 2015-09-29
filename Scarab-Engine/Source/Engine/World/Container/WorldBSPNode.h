/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Container/WorldBSPNode.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Container : BSP nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_CONTAINER_WORLDBSPNODE_H
#define SCARAB_ENGINE_WORLD_CONTAINER_WORLDBSPNODE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"

#include "../Nodes/WorldNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldBSPNode class
class WorldBSPNode : public WorldNode
{
public:
    WorldBSPNode( const GChar * strName = NULL );
    virtual ~WorldBSPNode();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Children nodes access
    inline Bool HasPositiveChild() const;
    inline Bool HasCoplanarChild() const;
    inline Bool HasNegativeChild() const;

    inline WorldBaseNode * GetPositiveChild() const;
    inline WorldBaseNode * GetCoplanarChild() const;
    inline WorldBaseNode * GetNegativeChild() const;

    inline WorldBaseNode * SetPositiveChild( WorldBaseNode * pChild );
    inline WorldBaseNode * SetCoplanarChild( WorldBaseNode * pChild );
    inline WorldBaseNode * SetNegativeChild( WorldBaseNode * pChild );

    // Separating plane
    Plane WorldPlane;

    // Containment test
    WorldBaseNode * GetContainingNode( const Vertex3 & vPosition ) const;

protected:
    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

private:
    WorldBaseNode * m_pPositive;
    WorldBaseNode * m_pCoplanar;
    WorldBaseNode * m_pNegative;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldBSPNode.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_CONTAINER_WORLDBSPNODE_H

