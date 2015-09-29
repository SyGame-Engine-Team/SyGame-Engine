/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/ContactManifold.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Contact points and manifolds construction/updating
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONTACTMANIFOLD_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONTACTMANIFOLD_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Error/ErrorManager.h"

#include "../../../../Lib/Math/Types/Vector/Vector3.h"
#include "../../../../Lib/Math/Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define CONTACT_MANIFOLD_MAX_POINTS 4
#define CONTACT_MANIFOLD_MAX_PLANES 6

/////////////////////////////////////////////////////////////////////////////////
// The ContactPointID class
class ContactPointID
{
public:
    ContactPointID();
    ContactPointID( const ContactPointID & rhs );
    ~ContactPointID();

    inline ContactPointID & operator=( const ContactPointID & rhs );

    inline Bool operator==( const ContactPointID & rhs ) const;
    inline Bool operator!=( const ContactPointID & rhs ) const;

    Void ReverseObjects();

    DWord dwFeatureA;
    DWord dwSpecifierA;
    DWord dwFeatureB;
    DWord dwSpecifierB;

    static const ContactPointID CPID_NULL;
};

/////////////////////////////////////////////////////////////////////////////////
// The ContactPoint class
class ContactPoint
{
public:
    ContactPoint();
    ContactPoint( const ContactPoint & rhs );
    ~ContactPoint();

    ContactPoint & operator=( const ContactPoint & rhs );

    // Contact-Point ID (speculation & caching)
    ContactPointID idCPID;

    // Contact points on A & B
    Vertex3 vWorldPointA;
    Vertex3 vWorldPointB;

    // Contact normal
    Vector3 vWorldNormalBA; // Allways B -> A
    Scalar fDistance;       // Signed distance, negative => penetration

    // Persistent data
    UInt iLifeTime;
    Scalar fNormalImpulse;
    Scalar fFrictionImpulse0;
    Scalar fFrictionImpulse1;
    Bool bFrictionInitialized;
    Vector3 vFrictionAxis0;
    Vector3 vFrictionAxis1;
};

/////////////////////////////////////////////////////////////////////////////////
// The ContactManifold class
class ContactManifold
{
public:
    ContactManifold();
    virtual ~ContactManifold();

    // Contact points access
    virtual Bool IsEmpty() const = 0;

    virtual UInt GetContactPointCount() const = 0;

    virtual const ContactPoint * GetContactPoint( UInt iIndex ) const = 0;
    virtual ContactPoint * GetContactPoint( UInt iIndex ) = 0;

    // Manifold construction
    virtual Void AddPoint( const ContactPointID & idCPID, const Vertex3 & vWorldPointA, const Vertex3 & vWorldPointB,
                                                          const Vector3 & vWorldNormalBA, Scalar fDistance ) = 0;
    virtual Void Clear() = 0;

    // Manifold Update
    virtual Void UpdateAfterCollision( const ContactManifold * pBaseCollisionManifold ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The SinglePlaneManifold class
class SinglePlaneManifold : public ContactManifold
{
public:
    SinglePlaneManifold();
    virtual ~SinglePlaneManifold();

    SinglePlaneManifold & operator=( const SinglePlaneManifold & rhs );

    // Contact points access
    inline virtual Bool IsEmpty() const;

    inline virtual UInt GetContactPointCount() const;

    inline virtual const ContactPoint * GetContactPoint( UInt iIndex ) const;
    inline virtual ContactPoint * GetContactPoint( UInt iIndex );

    // Manifold construction
    virtual Void AddPoint( const ContactPointID & idCPID, const Vertex3 & vWorldPointA, const Vertex3 & vWorldPointB,
                                                          const Vector3 & vWorldNormalBA, Scalar fDistance );
    inline virtual Void Clear();

    // Manifold Update
    virtual Void UpdateAfterCollision( const ContactManifold * pBaseCollisionManifold );

private:
    // Closest point is allways at index 0
    UInt m_iContactPointCount;
    ContactPoint m_arrContactPoints[CONTACT_MANIFOLD_MAX_POINTS];
};

/////////////////////////////////////////////////////////////////////////////////
// The MultiPlaneManifold class
class MultiPlaneManifold : public ContactManifold
{
public:
    MultiPlaneManifold();
    virtual ~MultiPlaneManifold();

    // Contact points access
    inline virtual Bool IsEmpty() const;

    inline virtual UInt GetContactPointCount() const;

    inline virtual const ContactPoint * GetContactPoint( UInt iIndex ) const;
    inline virtual ContactPoint * GetContactPoint( UInt iIndex );

    // Manifold construction
    virtual Void AddPoint( const ContactPointID & idCPID, const Vertex3 & vWorldPointA, const Vertex3 & vWorldPointB,
                                                          const Vector3 & vWorldNormalBA, Scalar fDistance );
    inline virtual Void Clear();

    // Manifold Update
    virtual Void UpdateAfterCollision( const ContactManifold * pBaseCollisionManifold );

private:
    // Closest plane is allways at index 0
    UInt m_iPlaneCount;
    UInt m_iContactPointCount; // Sum of all planes

    QWord m_arrPlaneIDs[CONTACT_MANIFOLD_MAX_PLANES];
    SinglePlaneManifold m_arrPlanes[CONTACT_MANIFOLD_MAX_PLANES];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ContactManifold.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONTACTMANIFOLD_H

