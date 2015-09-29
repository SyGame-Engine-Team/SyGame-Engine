/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation : SkillAnimations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Animation implementation
inline AnimationType Animation::GetType() const {
    return m_iType;
}
inline AnimationID Animation::GetID() const {
    return m_idAnimation;
}

/////////////////////////////////////////////////////////////////////////////////
// AnimationInstance implementation
inline Animation * AnimationInstance::GetAnimation() const {
    return m_pAnimation;
}
inline BaseCharacter * AnimationInstance::GetSource() const {
    return m_pSource;
}
inline BaseCharacter * AnimationInstance::GetTarget() const {
    return m_pTarget;
}
inline const Vertex3 & AnimationInstance::GetTargetFixed() const {
    return m_vTarget;
}

inline AnimationType AnimationInstance::GetType() const {
    return m_pAnimation->GetType();
}
inline AnimationID AnimationInstance::GetID() const {
    return m_pAnimation->GetID();
}

inline Scalar AnimationInstance::GetDuration() const {
    return m_fDuration;
}
inline Scalar AnimationInstance::GetInvDuration() const {
    return m_fInvDuration;
}

inline Bool AnimationInstance::IsPlaying() const {
    return m_bPlaying;
}
inline Scalar AnimationInstance::GetElapsedTime() const {
    return m_fElapsedTime;
}

inline Byte * AnimationInstance::GetAnimationState() {
    return m_arrAnimationState;
}


