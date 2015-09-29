/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/D3D11Window.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant window implementation.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D3D11WindowCallbacks implementation
inline Void D3D11WindowCallbacks::SetUserData( Void * pUserData ) {
    m_pUserData = pUserData;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Window implementation
inline Bool D3D11Window::IsActive() const {
    return m_bActive;
}
inline Bool D3D11Window::IsMinimized() const {
    return m_bMinimized;
}
inline Bool D3D11Window::IsMaximized() const {
    return m_bMaximized;
}
inline Bool D3D11Window::IsInSizeMove() const {
    return m_bIsInSizeMove;
}

inline const GChar * D3D11Window::GetTitle() const {
    return m_strTitle;
}

inline UInt D3D11Window::GetAdapterCount() const {
    return m_iAdapterCount;
}
inline Void D3D11Window::GetAdapterDesc( D3D11AdapterDesc * outAdapterDesc, UInt iAdapter ) const {
    UInt iAdapterIndex = iAdapter;
    DebugAssert( iAdapterIndex < m_iAdapterCount );
    outAdapterDesc->ConvertFrom( m_arrAdapters + iAdapterIndex );
}

inline UInt D3D11Window::GetCurrentAdapter() const {
    return m_iAdapter;
}
inline const D3D11AdapterDesc * D3D11Window::GetCurrentAdapterDesc() const {
    return &m_hAdapterDesc;
}

inline UInt D3D11Window::GetOutputCount( UInt iAdapter ) const {
    if ( iAdapter == INVALID_OFFSET )
        iAdapter = m_iAdapter;
    UInt iAdapterIndex = iAdapter;
    DebugAssert( iAdapterIndex < m_iAdapterCount );
    return m_arrOutputCounts[iAdapterIndex];
}
inline Void D3D11Window::GetOutputDesc( D3D11OutputDesc * outOutputDesc, UInt iOutput, UInt iAdapter ) const {
    if ( iAdapter == INVALID_OFFSET )
        iAdapter = m_iAdapter;
    UInt iAdapterIndex = iAdapter;
    DebugAssert( iAdapterIndex < m_iAdapterCount );
    UInt iOutputIndex = ( iAdapterIndex * D3D11WINDOW_MAX_OUTPUTS ) + iOutput;
    DebugAssert( iOutputIndex < m_arrOutputCounts[iAdapterIndex] );
    outOutputDesc->ConvertFrom( m_arrOutputs + iOutputIndex );
}

inline UInt D3D11Window::GetCurrentOutput() const {
    return m_iOutput;
}
inline const D3D11OutputDesc * D3D11Window::GetCurrentOutputDesc() const {
    return &m_hOutputDesc;
}

inline UInt D3D11Window::GetDisplayModeCount( UInt iOutput, UInt iAdapter ) const {
    if ( iOutput == INVALID_OFFSET )
        iOutput = m_iOutput;
    if ( iAdapter == INVALID_OFFSET )
        iAdapter = m_iAdapter;
    UInt iAdapterIndex = iAdapter;
    DebugAssert( iAdapterIndex < m_iAdapterCount );
    UInt iOutputIndex = ( iAdapterIndex * D3D11WINDOW_MAX_OUTPUTS ) + iOutput;
    DebugAssert( iOutputIndex < m_arrOutputCounts[iAdapterIndex] );
    return m_arrDisplayModeCounts[iOutputIndex];
}
inline Void D3D11Window::GetDisplayModeDesc( D3D11DisplayModeDesc * outDisplayModeDesc, UInt iDisplayMode, UInt iOutput, UInt iAdapter ) const {
    if ( iOutput == INVALID_OFFSET )
        iOutput = m_iOutput;
    if ( iAdapter == INVALID_OFFSET )
        iAdapter = m_iAdapter;
    UInt iAdapterIndex = iAdapter;
    DebugAssert( iAdapterIndex < m_iAdapterCount );
    UInt iOutputIndex = ( iAdapterIndex * D3D11WINDOW_MAX_OUTPUTS ) + iOutput;
    DebugAssert( iOutputIndex < m_arrOutputCounts[iAdapterIndex] );
    UInt iDisplayModeIndex = ( iOutputIndex * D3D11WINDOW_MAX_DISPLAYMODES ) + iDisplayMode;
    DebugAssert( iDisplayModeIndex < m_arrDisplayModeCounts[iOutputIndex] );
    outDisplayModeDesc->ConvertFrom( m_arrDisplayModes + iDisplayModeIndex, iDisplayMode );
}

inline UInt D3D11Window::GetCurrentDisplayMode() const {
    return m_iDisplayMode;
}
inline const D3D11DisplayModeDesc * D3D11Window::GetCurrentDisplayModeDesc() const {
    return &m_hDisplayModeDesc;
}

inline Bool D3D11Window::IsFullScreenWindowed() const {
    return m_bFullScreenWindowed;
}

