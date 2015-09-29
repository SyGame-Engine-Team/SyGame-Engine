/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/MorphController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex morphing
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
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_MORPHCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_MORPHCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MorphController class
//class MorphController : public ShapeController
//{
//public:
//    MorphController( const GChar * strName, UInt iMorphKeyCount, UInt iTargetCount );
//    virtual ~MorphController();
//
//    // Type
//    inline virtual ControllerType GetType() const;
//
//    // Target access
//    virtual Void SetTarget( RigidBody * pTarget );
//
//    // Morphing time-line
//    inline UInt GetMorphKeyCount() const;
//
//    inline Scalar GetMorphTime( UInt iKey ) const;
//    inline Void SetMorphTime( UInt iKey, Scalar fTime );
//
//    inline Scalar GetMorphWeight( UInt iKey, UInt iTarget ) const;
//    inline Void SetMorphWeight( UInt iKey, UInt iTarget, Scalar fWeight );
//
//    // Base + Target access
//    inline UInt GetVertexCount() const;
//    inline UInt GetTargetCount() const;
//
//    inline const Vertex3 & GetBaseVertex( UInt iVertex ) const;
//    inline Void SetBaseVertex( UInt iVertex, const Vertex3 & vBaseVertex );
//
//    inline const Vector3 & GetTargetVector( UInt iTarget, UInt iVertex ) const;
//    inline Void SetTargetVector( UInt iTarget, UInt iVertex, const Vector3 & vTargetVector );
//
//protected:
//    // Update support
//    virtual Bool _UpdateControllerState( Scalar fTimeStep );
//
//    // Helpers
//    Void _LookupKeyFrames( Scalar fControlTime, Scalar * outNormalTime, UInt * outPrevKey, UInt * outNextKey );
//
//    // Morphing time-line
//    UInt m_iMorphKeyCount; // = K
//    UInt m_iMorphLastKey;
//    Scalar * m_arrMorphTimes; // K times
//    Scalar * m_arrMorphKeys;  // T weights for each K times
//
//    // Target geometry
//    Mesh * m_pTargetMesh;
//
//    UInt m_iVertexCount; // = N
//    Vertex3 * m_arrBaseVertices; // N vertices
//
//    UInt m_iTargetCount; // = T
//    Vector3 * m_arrTargetVectors; // T target vectors for each N vertices
//
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MorphController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_SHAPE_MORPHCONTROLLER_H

