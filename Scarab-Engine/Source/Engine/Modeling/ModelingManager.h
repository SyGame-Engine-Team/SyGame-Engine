/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/ModelingManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for manipulating various modeling techniques
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Everything here uses counter-clockwise face winding ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_MODELING_MODELINGMANAGER_H
#define SCARAB_ENGINE_MODELING_MODELINGMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../Rendering/RenderingManager.h"

// Primitives
#include "Primitives/Linear/MeshSegment.h"
#include "Primitives/Planar/MeshTriangle.h"
#include "Primitives/Planar/MeshQuad.h"
#include "Primitives/Planar/MeshGridPlane.h"
#include "Primitives/Planar/MeshDisk.h"
#include "Primitives/MeshBox.h"
#include "Primitives/MeshSphere.h"
#include "Primitives/MeshCylinder.h"
#include "Primitives/MeshTorus.h"

// Curves & Surfaces
#include "Curves/MeshCurve.h"
#include "Surfaces/MeshSurface.h"
#include "Surfaces/MeshHeightField.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ModelingFn ModelingManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The ModelingManager class
class ModelingManager : public Manager<ModelingManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<ModelingManager>;
    ModelingManager();
    virtual ~ModelingManager();

public:
    Void Initialize();
    Void Cleanup();

private:

};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ModelingManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_MODELING_MODELINGMANAGER_H


