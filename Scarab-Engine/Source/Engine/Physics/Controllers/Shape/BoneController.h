/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/BoneController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex skinning using bones
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : BEWARE = Morphed objects losing their topology should be handled
//                       with the most general case for collisions ...
//                       Most of the time you'll want concave collision ...
//              TODO = V-Shader version ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_BONECONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_BONECONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class WorldBaseNode;

/////////////////////////////////////////////////////////////////////////////////
// The BoneController class
//class BoneController : public ShapeController
//{
//public:
//    BoneController( const GChar * strName, UInt iBoneCount );
//    virtual ~BoneController();
//
//    // Type
//    inline virtual ControllerType GetType() const;
//
//    // Target access
//    virtual Void SetTarget( RigidBody * pTarget );
//
//    // Bones access
//    inline UInt GetVertexCount() const;
//    inline UInt GetBoneCount() const;
//
//    inline RigidBody * GetBone( UInt iBone ) const;
//    inline Void SetBone( UInt iBone, RigidBody * pBone );
//
//    inline const Vector3 & GetBoneOffset( UInt iVertex, UInt iBone ) const;
//    inline Void SetBoneOffset( UInt iVertex, UInt iBone, const Vector3 & vOffset );
//
//    inline Scalar GetBoneWeight( UInt iVertex, UInt iBone ) const;
//    inline Void SetBoneWeight( UInt iVertex, UInt iBone, Scalar fWeight );
//
//protected:
//    // Update support
//    virtual Bool _UpdateControllerState( Scalar fTimeStep );
//
//    // Bones data
//    UInt m_iBoneCount;           // B bones
//    RigidBody ** m_arrBones; //
//    Vector3 * m_arrBoneOffsets;  // B bone offsets for each N vertices
//    Scalar * m_arrBoneWeights;   // B bone weights for each N vertices
//
//    // Target geometry
//    Mesh * m_pTargetMesh;
//
//    UInt m_iVertexCount; // N vertices
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BoneController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_BONECONTROLLER_H

