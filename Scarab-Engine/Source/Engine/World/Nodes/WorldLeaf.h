/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldLeaf.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Leaves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_NODES_WORLDLEAF_H
#define SCARAB_ENGINE_WORLD_NODES_WORLDLEAF_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Rendering/Resources/Mesh.h"
#include "../../Rendering/Effects/EffectInstance.h"

#include "../../Physics/IslandSolver/Nodes/BodyNode.h"

#include "WorldBaseNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The WorldLeaf class
class WorldLeaf : public WorldBaseNode
{
public:
    WorldLeaf( const GChar * strName = NULL );
    virtual ~WorldLeaf();

    // Type
    inline virtual WorldNodeType GetType() const;

    // Geometry access
    inline Mesh * GetMesh() const;
    inline Void SetMesh( Mesh * pMesh );

    // Effect access
    inline EffectInstance * GetEffectInstance() const;
    inline Void SetEffectInstance( EffectInstance * pEffectInstance );

    // RigidBody access
    inline BodyNode * GetBody() const;
    inline Void SetBody( BodyNode * pBody );

    // Rendering support
    Void Render() const;

    Bool CastShadows; // Default = false

    // Model BV
    WorldGraphBV ModelBV;

protected:
    // Update support
    virtual Void _UpdateWorldTransform( Scalar fTimeStep );
    virtual Void _UpdateWorldBV();

    // Culling support
    virtual Void _OnVisibleGroup( WorldCuller & wCuller, Bool bNoCull );

    // Rendering data
    Mesh * m_pMesh;
    EffectInstance * m_pEffectInstance;

    // Animation data
    BodyNode * m_pBody;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldLeaf.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_NODES_WORLDLEAF_H

