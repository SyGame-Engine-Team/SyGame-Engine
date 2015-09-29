/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/NarrowPhase.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase layer for collision detection
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhase implementation

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhaseConvex implementation
inline Bool NarrowPhaseConvex::IsConvex() const {
    return true;
}
inline Bool NarrowPhaseConvex::IsConcave() const {
    return false;
}

inline TransformedShapeConvex * NarrowPhaseConvex::GetShapeA() const {
    return m_pShapeA;
}
inline TransformedShapeConvex * NarrowPhaseConvex::GetShapeB() const {
    return m_pShapeB;
}

inline const SinglePlaneManifold * NarrowPhaseConvex::GetManifold() const {
    return m_pCollisionManifold;
}

/////////////////////////////////////////////////////////////////////////////////
// NarrowPhaseConcave implementation
inline Bool NarrowPhaseConcave::IsConvex() const {
    return false;
}
inline Bool NarrowPhaseConcave::IsConcave() const {
    return true;
}

inline TransformedShape * NarrowPhaseConcave::GetShapeA() const {
    return m_pShapeA;
}
inline TransformedShape * NarrowPhaseConcave::GetShapeB() const {
    return m_pShapeB;
}

inline const MultiPlaneManifold * NarrowPhaseConcave::GetManifold() const {
    return m_pCollisionManifold;
}

