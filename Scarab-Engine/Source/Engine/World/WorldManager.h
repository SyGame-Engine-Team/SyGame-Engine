/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for manipulating the WorldGraph system
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : NOTE = Don't forget to give the world a camera before updating or
//                     rendering anything ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_WORLD_WORLDMANAGER_H
#define SCARAB_ENGINE_WORLD_WORLDMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../Rendering/RenderingManager.h"
#include "../Modeling/ModelingManager.h"
#include "../Physics/PhysicsManager.h"

#include "Nodes/WorldNode.h"
#include "Nodes/WorldLeaf.h"

#include "Container/WorldBSPNode.h"
#include "Container/WorldSwitchNode.h"
#include "Container/WorldLight.h"

#include "Outdoor/WorldOutdoor.h"
#include "Indoor/WorldIndoorArea.h"

#include "GlobalEffects/EffectPlanarShadows.h"

#include "WorldCamera.h"
#include "WorldCuller.h"
#include "WorldCaster.h"
#include "WorldPathFinder.h"

#include "WorldDebugDraw.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define WorldFn WorldManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The WorldManager class
class WorldManager : public Manager<WorldManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<WorldManager>;
    WorldManager();
    virtual ~WorldManager();

public:
    Void Initialize();
    Void Cleanup();

    // World tree access
    inline WorldNode * GetWorldRoot() const;
    WorldBaseNode * GetWorldNode( const GChar * strPath ) const;

    inline Void AddChild( WorldBaseNode * pNode );
    inline WorldBaseNode * RemoveChild( const GChar * strName );

    inline Void ResetWorld();
    Void DestroyWorld();

    // Visible group access
    inline const VisibleGroup & GetVisibleGroup() const;

    // World camera access
    inline WorldCamera * GetWorldCamera() const;
    inline Void SetWorldCamera( WorldCamera * pCamera );

    // Nodes management
    WorldNode * CreateNode( const GChar * strName = NULL );
    WorldLeaf * CreateLeaf( const GChar * strName = NULL );

    WorldBSPNode * CreateBSPNode( const GChar * strName = NULL );

    Void DestroyNode( WorldBaseNode * pNode, Bool bRecursive = false );

    // Animation & Rendering
    inline Void ToggleAnimation();
    inline Void PauseAnimation();
    inline Void ResumeAnimation();
    inline Void StepAnimation();

    Void UpdateWorld( Scalar fTimeStep );
    Void RenderWorld() const;

    // World casting
    inline Void RayCast( const Line3 & vRay, Scalar fMinDistance, Scalar fMaxDistance );

    inline UInt GetRayCastResultCount() const;
    inline const RayCastResult * GetRayCastResult( UInt iIndex ) const;

    inline const RayCastResult * GetClosestRayCastResult();
    inline const RayCastResult * GetClosestPositiveRayCastResult();
    inline const RayCastResult * GetClosestNegativeRayCastResult();
    inline Void SortRayCastResults();

    inline Void SphereCast( const Sphere & vSphere );

    inline UInt GetSphereCastResultCount() const;
    inline const SphereCastResult * GetSphereCastResult( UInt iIndex ) const;

    inline const SphereCastResult * GetClosestSphereCastResult();
    inline Void SortSphereCastResults();

    // World path-finding
    inline WorldPathFinder * GetPathFinder() const;

    // Global effects
    inline GlobalEffect * GetGlobalEffect() const;
    inline Void SetGlobalEffect( GlobalEffect * pEffect );

    // Debug Draw
    inline Void ToggleDebugDraw( Bool bEnable );

private:
    // Helpers
    Void _rec_DestroyNodes( WorldBaseNode * pNode );

    // World root
    WorldNode * m_pWorldRoot;

    // World camera
    WorldCamera * m_pWorldCamera;

    // World delegates
    WorldCuller * m_pWorldCuller;
    WorldCaster * m_pWorldCaster;
    WorldPathFinder * m_pWorldPathFinder;

    // Global effect
    GlobalEffect * m_pGlobalEffect;

    // Flags
    Bool m_bRunAnimation, m_bStepAnimation;

    // Debug Draw
    Bool m_bEnableDebugDraw;
    WorldDebugDraw * m_pWorldDebugDraw;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "WorldManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_WORLD_WORLDMANAGER_H

