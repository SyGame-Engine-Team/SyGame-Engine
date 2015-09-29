/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/Hardware.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Hardware abstraction layer
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Hardware implementation
inline Hardware * Hardware::GetInstancePtr() {
    static Hardware s_Instance;
    return &s_Instance;
}

// CPU /////////////////////////////////////////////////////////
inline const Char * Hardware::GetCPUVendorString() const {
    return m_strCPUVendorString;
}
inline DWord Hardware::GetCPUFamilyID( DWord * outExtendedFamilyID ) const {
    if ( outExtendedFamilyID != NULL )
        *outExtendedFamilyID = m_dwExtFamilyID;
    return m_dwFamilyID;
}
inline DWord Hardware::GetCPUModelID( DWord * outExtendedModelID ) const {
    if ( outExtendedModelID != NULL )
        *outExtendedModelID = m_dwExtModelID;
    return m_dwModelID;
}
inline DWord Hardware::GetCPUSteppingID() const {
    return m_dwSteppingID;
}
inline DWord Hardware::GetCPUType() const {
    return m_dwProcessorType;
}
inline Byte Hardware::GetCPUBrandIndex() const {
    return m_iBrandIndex;
}

inline const CpuFeatures * Hardware::GetCPUFeatures() const {
    return &m_hCpuFeatures;
}
