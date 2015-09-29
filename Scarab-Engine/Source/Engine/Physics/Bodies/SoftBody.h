/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/SoftBody.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Soft bodies
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
#ifndef SCARAB_ENGINE_PHYSICS_BODIES_SOFTBODY_H
#define SCARAB_ENGINE_PHYSICS_BODIES_SOFTBODY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"
#include "../../../Lib/Math/Random/Random.h"

#include "../Shape/Concave/ShapeDeformable.h"

#include "../IslandSolver/Nodes/BodyNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // Soft body features
enum SoftBodyFeatureType
{
    SOFTBODY_FEATURE_UNDEFINED = 0,
    SOFTBODY_FEATURE_NODE,
    SOFTBODY_FEATURE_LINK,
    SOFTBODY_FEATURE_FACET,
    //SOFTBODY_FEATURE_TETRA,
    //SOFTBODY_FEATURE_CLUSTER,

    SOFTBODY_FEATURE_COUNT
};

    // Environment definition
typedef struct _softbody_environment
{
    // Fluids : Air
    Scalar fAirDensity;

    // Fluids : Water
    Scalar fWaterDensity;
    Scalar fWaterOffset;
    Vector3 vWaterNormal;

    // Forces
    Vector3 vGravity;

// btSparseSdf<3> m_sparsesdf;
// btBroadphaseInterface * m_broadphase;
// btDispatcher * m_dispatcher;
} SoftBodyEnvironment;

    // Aero-dynamics model
enum SoftBodyAeroDynamics
{
    SOFTBODY_AERODYNAMICS_V_Point = 0,
    SOFTBODY_AERODYNAMICS_V_OneSided,
    SOFTBODY_AERODYNAMICS_V_TwoSided,
    SOFTBODY_AERODYNAMICS_F_OneSided,
    SOFTBODY_AERODYNAMICS_F_TwoSided,

    SOFTBODY_AERODYNAMICS_COUNT
};

    // Prototypes
class RigidBody;


/////////////////////////////////////////////////////////////////////////////////
// The SoftBody class
class SoftBody : public BodyNode
{
public: // Nested types
    typedef struct _material {
        Scalar fLinearStiffness;  // m_kLST, [0,1]
        Scalar fAngularStiffness; // m_kAST, [0,1]
        Scalar fAreaStiffness;    // m_kAST, [0,1]
        Scalar fVolumeStiffness;  // m_kVST, [0,1]
        DWord dwFlags; // m_flags
    } Material;

    class Node : public ShapeDeformable::Node { public:
        Node( const MeshVertexKey & vertexKey );
        virtual ~Node();

        // Helpers
        Void ApplyClampedForce( const Vector3 & vForce, Scalar fTimeStep );

        // Material
        Material * pMaterial; // m_material

        // Mass
        Scalar Mass;
        Scalar InvMass;

        // State
        Vertex3 OldPosition; // m_q (Position = m_x)
        Vector3 Velocity;    // m_v
        Vector3 ForceBuffer; // m_f

        // Flags
        Bool IsAttached;        
    };
    class Link : public ShapeDeformable::Link { public:
        Link( const MeshEdgeKey & edgeKey );
        virtual ~Link();

        // Nodes access
        inline Node * GetNode( UInt iNode ) const;

        // Helpers
        Void UpdateConstants();

        // Material
        Material * pMaterial; // m_material

        // Constants
        Scalar RestLength;    // m_rl
        Scalar SqrRestLength; // m_c1
        Scalar LinearFactor;  // m_c0, ( (1/m0 + 1/m1) / fLST )

        // Flags
        Bool IsBridge;
    };
    class Facet : public ShapeDeformable::Facet { public: // Face
        Facet( const MeshTriangleKey & triangleKey );
        virtual ~Facet();

        // Nodes access
        inline Node * GetNode( UInt iNode ) const;

        // Helpers
        inline Vertex3 BaryEval( const Vector3 & vWeights, Bool bUseOldPositions ) const;
    };
    //class Tetra : public Feature { public: // Tetra
    //    Tetra( Node * pA, Node * pB, Node * pC, Node * pD );
    //    virtual ~Tetra();
    //    Node * pNodeA;           //
    //    Node * pNodeB;           //
    //    Node * pNodeC;           //
    //    Node * pNodeD;           // m_n[4]
    //    Scalar fRestVolume;      // m_rv
    //    Scalar fVolumeFactor;    // m_c1, ( (4 * fVST) / (1/m0 + 1/m1 + 1/m2 + 1/m3) )
    //    Vector3 arrGradients[4]; // m_c0[4]
    //    Scalar fGradientFactor;  // m_c2, ( fVolumeFactor / sum(|grad0..3|^2) )
    //    AABoxNode * pLeaf; // m_leaf
    //};

    inline static MeshVertex * NodeBuilder ( const MeshVertexKey & vertexKey );
    inline static MeshEdge *   LinkBuilder ( const MeshEdgeKey & edgeKey );
    inline static MeshFacet *  FacetBuilder( const MeshFacetKey & facetKey );

    //class Cluster { public:
    //    Cluster();
    //    ~Cluster();

    //    Array<Node*> arrNodes;              // m_nodes
    //    Array<Scalar> arrMasses;            // m_masses

    //    Scalar fInvMass;                    // m_imass
    //    Vertex3 vCenterOfMass;              // m_com
    //    Matrix3 matLocalInvInertiaTensor;   // m_locii
    //    Matrix3 matInvInertiaTensor;        // m_invwi

    //    Array<Vector3> arrFrameReferences;  // m_framerefs
    //    Transform3 vFrameTransform;         // m_framexform

    //    Vector3 vLinearVelocity;            // m_lv
    //    Vector3 vAngularVelocity;           // m_av

    //    Vector3 vLinearImpulseV;            //
    //    Vector3 vAngularImpulseV;           // m_vimpulses[2]
    //    Vector3 vLinearImpulseD;            //
    //    Vector3 vAngularImpulseD;           // m_dimpulses[2]
    //    UInt iImpulseCountV;                // m_nvimpulses
    //    UInt iImpulseCountD;                // m_ndimpulses

    //    Scalar fNodeDamping;                // m_ndamping, (def = 0.0f)
    //    Scalar fLinearDamping;              // m_ldamping, (def = 0.0f)
    //    Scalar fAngularDamping;             // m_adamping, (def = 0.0f)

    //    Scalar fMatching;                   // m_matching, (def = 0.0f)

    //    Scalar fSelfCollisionImpulseFactor; // m_selfCollisionImpulseFactor (def = 0.01f)
    //    Scalar fSelfCollisionImpulseMax;    // m_maxSelfCollisionImpulse (def = 100.0f)

    //    Bool bHasAnchor;                    // m_containsAnchor (def = false)
    //    Bool bCollide;                      // m_collide
    //    UInt iIndex;                        // m_clusterIndex
    //    AABoxNode * pLeaf; // m_leaf
    //};
    //Array<Cluster*> m_arrClusters; // tClusterArray m_clusters
    //AABoxTree m_hClusterTree;
    //Array<Bool> m_arrClusterConnectivity;

public:
    SoftBody( Bool bStatic, ShapeDeformable * pLocalShape, SoftBodyEnvironment * pEnvironment, 
              const Vertex3 & vPosition, const Quaternion & qOrientation,
              const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~SoftBody();

    // Type
    inline virtual BodyType GetBodyType() const;

    // Behaviour
    inline virtual Bool IsInsulator() const;

    // Environment access
    inline SoftBodyEnvironment * GetEnvironment() const;

    // Materials access
    Material * PushMaterial();

    // Nodes access
    inline UInt GetNodeCount() const;
    inline Void EnumNodes() const;
    inline Node * EnumNextNode() const;
    inline Node * GetNode( UInt iNode ) const;

    // Links access
    inline UInt GetLinkCount() const;
    inline Void EnumLinks() const;
    inline Link * EnumNextLink() const;

    // Facets access
    inline UInt GetFacetCount() const;
    inline Void EnumFacets() const;
    inline Facet * EnumNextFacet() const;

    // Integration
    virtual Void IntegrateVelocity( Scalar fTimeStep );
    virtual Void IntegratePosition( Scalar fTimeStep );

private:
    // Helpers
    Scalar _ComputeVolume() const;        // Update density
    Scalar _ComputeMass() const;          // Convert to (m_fMass, m_fInvMass) update
    Vertex3 _ComputeCenterOfMass() const; // Convert to m_vCenterOfMass update

    typedef struct _medium
    {
        Vector3 vVelocity;
        Scalar fPressure;
        Scalar fDensity;
    } Medium;
    Void _EvaluateMedium( Medium * outMedium, const Vertex3 & vPosition );

    Void _ApplyInternalForces( Scalar fTimeStep );

    // SoftBody environment
    SoftBodyEnvironment * m_pEnvironment; // m_worldInfo

    // Shape data
    ShapeDeformable * m_pDeformableShape;

    // Physical properties
    SoftBodyAeroDynamics m_iAeroDynamicsModel; // m_cfg.aeromodel
    Scalar m_fDrag;               // m_cfg.kDG, [0,+inf]
    Scalar m_fLift;               // m_cfg.kLF, [0,+inf]
    Scalar m_fPressure;           // m_cfg.kPR, [-inf,+inf]
    Scalar m_fVolumeConversation; // m_cfg.kVC, [0,+inf]
    //Scalar m_fDynamicFriction;  // m_cfg.kDF, [0,1]

    Array<Material*> m_arrMaterials; // tMaterialArray m_materials

    // Pose-Matching support
    Void _SetupPose( Bool bIsVolume, Bool bIsFrame );
    Void _UpdatePose();

    typedef struct _shape_pose {
        // Setup
        Array<Vector3> arrDeltas;
        Array<Scalar> arrWeights;

        Bool bIsVolume; // Volume shape else surface / curve
        Bool bIsFrame;

        Scalar fVolume;
        Matrix3 matBaseScaling;

        // State
        Vertex3 vCenterOfMass; // Remove, use m_fMass, m_fInvMass & m_vCenterOfMass
                               // Inertia tensor is not used ...
        Matrix3 matRotation;
        Matrix3 matScaling;
    } ShapePose;

    ShapePose m_hPose;

    Scalar m_fPoseMatching;
    Scalar m_fMaxVolumeRatio;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SoftBody.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_BODIES_SOFTBODY_H

