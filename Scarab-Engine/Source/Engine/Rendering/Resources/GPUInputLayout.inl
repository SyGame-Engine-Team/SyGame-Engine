/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUInputLayout.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Input Layout
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPUInputLayout implementation
inline Bool GPUInputLayout::IsFinalized() const {
    return m_hDeviceInputLayout.IsFinalized();
}

inline Void GPUInputLayout::Finalize() {
    m_hDeviceInputLayout.Finalize();
}

inline Bool GPUInputLayout::IsBound() const {
    return m_hDeviceInputLayout.IsCreated();
}

inline UInt GPUInputLayout::GetFieldCount() const {
    Assert( IsFinalized() );
    return m_hDeviceInputLayout.GetFieldCount();
}
inline Void GPUInputLayout::GetField( GPUInputFieldDesc * outFieldDesc, UInt iField ) const {
    Assert( IsFinalized() );
    m_hDeviceInputLayout.GetField( (DeviceInputFieldDesc*)outFieldDesc, iField );
}

inline Bool GPUInputLayout::HasFieldMap() const {
    Assert( IsFinalized() );
    return m_bUseFieldMap;
}
inline Bool GPUInputLayout::HasField( const AChar * strSemantic, UInt iSemanticIndex ) const {
    Assert( IsFinalized() && m_bUseFieldMap );
    _FieldIndex hIndex;
    hIndex.strSemantic = strSemantic;
    hIndex.iSemanticIndex = iSemanticIndex;
    return m_mapFields.Contains( hIndex );
}
inline Void GPUInputLayout::GetFieldIndex( UInt * outField, const AChar * strSemantic, UInt iSemanticIndex ) const {
    Assert( IsFinalized() && m_bUseFieldMap );
    _FieldIndex hIndex;
    hIndex.strSemantic = strSemantic;
    hIndex.iSemanticIndex = iSemanticIndex;
    _FieldMap::Iterator itField = m_mapFields.Get( hIndex );
    Assert( !(itField.IsNull()) );
    *outField = itField.GetItem().iField;
}
inline Void GPUInputLayout::GetFieldRange( UInt * outOffset, UInt * outSize, const AChar * strSemantic, UInt iSemanticIndex ) const {
    Assert( IsFinalized() && m_bUseFieldMap );
    _FieldIndex hIndex;
    hIndex.strSemantic = strSemantic;
    hIndex.iSemanticIndex = iSemanticIndex;
    _FieldMap::Iterator itField = m_mapFields.Get( hIndex );
    Assert( !(itField.IsNull()) );
    *outOffset = itField.GetItem().iOffset;
    *outSize = itField.GetItem().iSize;
}
inline Void GPUInputLayout::GetField( GPUInputFieldDesc * outFieldDesc, const AChar * strSemantic, UInt iSemanticIndex ) const {
    Assert( IsFinalized() && m_bUseFieldMap );
    _FieldIndex hIndex;
    hIndex.strSemantic = strSemantic;
    hIndex.iSemanticIndex = iSemanticIndex;
    _FieldMap::Iterator itField = m_mapFields.Get( hIndex );
    Assert( !(itField.IsNull()) );
    GetField( outFieldDesc, itField.GetItem().iField );
}

inline UInt GPUInputLayout::GetSize() const {
    Assert( IsFinalized() );
    return m_iSize;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GPUInputLayout::_Compare_FieldIndices( const _FieldIndex & hLeft, const _FieldIndex & hRight, Void * /*pUserData*/ ) {
    Int iCmp = StringFn->CmpA( hLeft.strSemantic, hRight.strSemantic );
    if ( iCmp != 0 )
        return iCmp;
    if ( hLeft.iSemanticIndex < hRight.iSemanticIndex )
        return +1;
    if ( hLeft.iSemanticIndex > hRight.iSemanticIndex )
        return -1;
    return 0;
}


