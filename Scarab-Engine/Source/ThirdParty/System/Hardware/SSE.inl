/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/SSEWrappers.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SSE low level abstraction layer
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// _SSE namespace implementation

/////////////////////////////////////////////////////////////////////////////////
// SSEStack implementation
inline SSEStack * SSEStack::GetInstancePtr() {
    static SSEStack s_Instance;
    return &s_Instance;
}

inline Void SSEStack::Push( Float fValue0 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    _IPush( _SSE_SS, (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Push( Float fValue0, Float fValue1 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = 1.0f;
    m_pAlignedFloatScratch[3] = 1.0f;
    _IPush( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Push( Float fValue0, Float fValue1, Float fValue2 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = fValue2;
    m_pAlignedFloatScratch[3] = 1.0f;
    _IPush( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Push( Float fValue0, Float fValue1, Float fValue2, Float fValue3 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = fValue2;
    m_pAlignedFloatScratch[3] = fValue3;
    _IPush( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Push( Float * fValues ) {
    m_pAlignedFloatScratch[0] = fValues[0];
    m_pAlignedFloatScratch[1] = fValues[1];
    m_pAlignedFloatScratch[2] = fValues[2];
    m_pAlignedFloatScratch[3] = fValues[3];
    _IPush( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
}

inline Void SSEStack::Push( Double fValue0 ) {
    m_pAlignedDoubleScratch[0] = fValue0;
    _IPush( _SSE_SD, (QWord*)m_pAlignedDoubleScratch );
}
inline Void SSEStack::Push( Double fValue0, Double fValue1 ) {
    m_pAlignedDoubleScratch[0] = fValue0;
    m_pAlignedDoubleScratch[1] = fValue1;
    _IPush( _SSE_PD, (QWord*)m_pAlignedDoubleScratch );
}
inline Void SSEStack::Push( Double * fValues ) {
    m_pAlignedDoubleScratch[0] = fValues[0];
    m_pAlignedDoubleScratch[1] = fValues[1];
    _IPush( _SSE_PD, (QWord*)m_pAlignedDoubleScratch );
}

inline Void SSEStack::PopF() {
    _IPop( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Pop( Float & outValue0 ) {
    _IPop( _SSE_SS, (QWord*)m_pAlignedFloatScratch );
    outValue0 = m_pAlignedFloatScratch[0];
}
inline Void SSEStack::Pop( Float & outValue0, Float & outValue1 ) {
    _IPop( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
}
inline Void SSEStack::Pop( Float & outValue0, Float & outValue1, Float & outValue2 ) {
    _IPop( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
    outValue2 = m_pAlignedFloatScratch[2];
}
inline Void SSEStack::Pop( Float & outValue0, Float & outValue1, Float & outValue2, Float & outValue3 ) {
    _IPop( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
    outValue2 = m_pAlignedFloatScratch[2];
    outValue3 = m_pAlignedFloatScratch[3];
}
inline Void SSEStack::Pop( Float * outValues ) {
    _IPop( _SSE_PS, (QWord*)m_pAlignedFloatScratch );
    outValues[0] = m_pAlignedFloatScratch[0];
    outValues[1] = m_pAlignedFloatScratch[1];
    outValues[2] = m_pAlignedFloatScratch[2];
    outValues[3] = m_pAlignedFloatScratch[3];
}

inline Void SSEStack::PopD() {
    _IPop( _SSE_PD, (QWord*)m_pAlignedDoubleScratch );
}
inline Void SSEStack::Pop( Double & outValue0 ) {
    _IPop( _SSE_SD, (QWord*)m_pAlignedDoubleScratch );
    outValue0 = m_pAlignedDoubleScratch[0];
}
inline Void SSEStack::Pop( Double & outValue0, Double & outValue1 ) {
    _IPop( _SSE_PD, (QWord*)m_pAlignedDoubleScratch );
    outValue0 = m_pAlignedDoubleScratch[0];
    outValue1 = m_pAlignedDoubleScratch[1];
}
inline Void SSEStack::Pop( Double * outValues ) {
    _IPop( _SSE_PD, (QWord*)m_pAlignedDoubleScratch );
    outValues[0] = m_pAlignedDoubleScratch[0];
    outValues[1] = m_pAlignedDoubleScratch[1];
}

inline Void SSEStack::Set( UInt iStackIndex, Float fValue0 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    _SSE::_ISet( _SSE_SS, _SSE_ST(iStackIndex), (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Float fValue0, Float fValue1 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = 1.0f;
    m_pAlignedFloatScratch[3] = 1.0f;
    _SSE::_ISet( _SSE_PS, _SSE_ST(iStackIndex), (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Float fValue0, Float fValue1, Float fValue2 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = fValue2;
    m_pAlignedFloatScratch[3] = 1.0f;
    _SSE::_ISet( _SSE_PS, _SSE_ST(iStackIndex), (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Float fValue0, Float fValue1, Float fValue2, Float fValue3 ) {
    m_pAlignedFloatScratch[0] = fValue0;
    m_pAlignedFloatScratch[1] = fValue1;
    m_pAlignedFloatScratch[2] = fValue2;
    m_pAlignedFloatScratch[3] = fValue3;
    _SSE::_ISet( _SSE_PS, _SSE_ST(iStackIndex), (QWord*)m_pAlignedFloatScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Float * fValues ) {
    m_pAlignedFloatScratch[0] = fValues[0];
    m_pAlignedFloatScratch[1] = fValues[1];
    m_pAlignedFloatScratch[2] = fValues[2];
    m_pAlignedFloatScratch[3] = fValues[3];
    _SSE::_ISet( _SSE_PS, _SSE_ST(iStackIndex), (QWord*)m_pAlignedFloatScratch );
}

inline Void SSEStack::Set( UInt iStackIndex, Double fValue0 ) {
    m_pAlignedDoubleScratch[0] = fValue0;
    _SSE::_ISet( _SSE_SD, _SSE_ST(iStackIndex), (QWord*)m_pAlignedDoubleScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Double fValue0, Double fValue1 ) {
    m_pAlignedDoubleScratch[0] = fValue0;
    m_pAlignedDoubleScratch[1] = fValue1;
    _SSE::_ISet( _SSE_PD, _SSE_ST(iStackIndex), (QWord*)m_pAlignedDoubleScratch );
}
inline Void SSEStack::Set( UInt iStackIndex, Double * fValues ) {
    m_pAlignedDoubleScratch[0] = fValues[0];
    m_pAlignedDoubleScratch[1] = fValues[1];
    _SSE::_ISet( _SSE_PD, _SSE_ST(iStackIndex), (QWord*)m_pAlignedDoubleScratch );
}

inline Void SSEStack::Get( UInt iStackIndex, Float & outValue0 ) {
    _SSE::_IGet( _SSE_SS, (QWord*)m_pAlignedFloatScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedFloatScratch[0];
}
inline Void SSEStack::Get( UInt iStackIndex, Float & outValue0, Float & outValue1 ) {
    _SSE::_IGet( _SSE_PS, (QWord*)m_pAlignedFloatScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
}
inline Void SSEStack::Get( UInt iStackIndex, Float & outValue0, Float & outValue1, Float & outValue2 ) {
    _SSE::_IGet( _SSE_PS, (QWord*)m_pAlignedFloatScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
    outValue2 = m_pAlignedFloatScratch[2];
}
inline Void SSEStack::Get( UInt iStackIndex, Float & outValue0, Float & outValue1, Float & outValue2, Float & outValue3 ) {
    _SSE::_IGet( _SSE_PS, (QWord*)m_pAlignedFloatScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedFloatScratch[0];
    outValue1 = m_pAlignedFloatScratch[1];
    outValue2 = m_pAlignedFloatScratch[2];
    outValue3 = m_pAlignedFloatScratch[3];
}
inline Void SSEStack::Get( UInt iStackIndex, Float * outValues ) {
    _SSE::_IGet( _SSE_PS, (QWord*)m_pAlignedFloatScratch, _SSE_ST(iStackIndex) );
    outValues[0] = m_pAlignedFloatScratch[0];
    outValues[1] = m_pAlignedFloatScratch[1];
    outValues[2] = m_pAlignedFloatScratch[2];
    outValues[3] = m_pAlignedFloatScratch[3];
}

inline Void SSEStack::Get( UInt iStackIndex, Double & outValue0 ) {
    _SSE::_IGet( _SSE_SD, (QWord*)m_pAlignedDoubleScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedDoubleScratch[0];
}
inline Void SSEStack::Get( UInt iStackIndex, Double & outValue0, Double & outValue1 ) {
    _SSE::_IGet( _SSE_PD, (QWord*)m_pAlignedDoubleScratch, _SSE_ST(iStackIndex) );
    outValue0 = m_pAlignedDoubleScratch[0];
    outValue1 = m_pAlignedDoubleScratch[1];
}
inline Void SSEStack::Get( UInt iStackIndex, Double * outValues ) {
    _SSE::_IGet( _SSE_PD, (QWord*)m_pAlignedDoubleScratch, _SSE_ST(iStackIndex) );
    outValues[0] = m_pAlignedDoubleScratch[0];
    outValues[1] = m_pAlignedDoubleScratch[1];
}

inline Void SSEStack::PushF( UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    ++m_iTop;
}
inline Void SSEStack::PushD( UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    ++m_iTop;
}
inline Void SSEStack::PopF( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::PopD( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::MovF( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
}
inline Void SSEStack::MovD( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
}

inline Void SSEStack::AddF() { _SSE::_IAdd( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::AddD() { _SSE::_IAdd( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::SubF() { _SSE::_ISub( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::SubD() { _SSE::_ISub( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::MulF() { _SSE::_IMul( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::MulD() { _SSE::_IMul( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::DivF() { _SSE::_IDiv( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::DivD() { _SSE::_IDiv( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }

inline Void SSEStack::AddF( UInt iDestIndex ) { _SSE::_IAdd( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::AddD( UInt iDestIndex ) { _SSE::_IAdd( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::SubF( UInt iDestIndex ) { _SSE::_ISub( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::SubD( UInt iDestIndex ) { _SSE::_ISub( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::MulF( UInt iDestIndex ) { _SSE::_IMul( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::MulD( UInt iDestIndex ) { _SSE::_IMul( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::DivF( UInt iDestIndex ) { _SSE::_IDiv( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::DivD( UInt iDestIndex ) { _SSE::_IDiv( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }

inline Void SSEStack::AddF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IAdd( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::AddD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IAdd( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::SubF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_ISub( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::SubD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_ISub( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::MulF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IMul( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::MulD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IMul( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::DivF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IDiv( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::DivD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IDiv( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }

inline Void SSEStack::SubRF() {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_ISub( _SSE_PS, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(1), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::SubRD() {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_ISub( _SSE_PD, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(1), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::DivRF() {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_IDiv( _SSE_PS, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(1), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::DivRD() {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_IDiv( _SSE_PD, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(1), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(0), _SSE_ST(-1) );
}

inline Void SSEStack::SubRF( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_ISub( _SSE_PS, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::SubRD( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_ISub( _SSE_PD, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::DivRF( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_IDiv( _SSE_PS, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(0), _SSE_ST(-1) );
}
inline Void SSEStack::DivRD( UInt iDestIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(0) );
    _SSE::_IDiv( _SSE_PD, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(0), _SSE_ST(-1) );
}

inline Void SSEStack::SubRF( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    _SSE::_ISub( _SSE_PS, _SSE_ST(iSrcIndex), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iSrcIndex), _SSE_ST(-1) );
}
inline Void SSEStack::SubRD( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    _SSE::_ISub( _SSE_PD, _SSE_ST(iSrcIndex), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iSrcIndex), _SSE_ST(-1) );
}
inline Void SSEStack::DivRF( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PS, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    _SSE::_IDiv( _SSE_PS, _SSE_ST(iSrcIndex), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iSrcIndex), _SSE_ST(-1) );
}
inline Void SSEStack::DivRD( UInt iDestIndex, UInt iSrcIndex ) {
    _SSE::_IMov( _SSE_PD, _SSE_ST(-1), _SSE_ST(iSrcIndex) );
    _SSE::_IDiv( _SSE_PD, _SSE_ST(iSrcIndex), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iSrcIndex), _SSE_ST(-1) );
}

inline Void SSEStack::AddPF() { _SSE::_IAdd( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::AddPD() { _SSE::_IAdd( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::SubPF() { _SSE::_ISub( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::SubPD() { _SSE::_ISub( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::MulPF() { _SSE::_IMul( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::MulPD() { _SSE::_IMul( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::DivPF() { _SSE::_IDiv( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::DivPD() { _SSE::_IDiv( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); --m_iTop; }

inline Void SSEStack::AddPF( UInt iDestIndex ) { _SSE::_IAdd( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::AddPD( UInt iDestIndex ) { _SSE::_IAdd( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::SubPF( UInt iDestIndex ) { _SSE::_ISub( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::SubPD( UInt iDestIndex ) { _SSE::_ISub( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::MulPF( UInt iDestIndex ) { _SSE::_IMul( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::MulPD( UInt iDestIndex ) { _SSE::_IMul( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::DivPF( UInt iDestIndex ) { _SSE::_IDiv( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }
inline Void SSEStack::DivPD( UInt iDestIndex ) { _SSE::_IDiv( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); --m_iTop; }

inline Void SSEStack::SubRPF() {
    _SSE::_ISub( _SSE_PS, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(1), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::SubRPD() {
    _SSE::_ISub( _SSE_PD, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(1), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::DivRPF() {
    _SSE::_IDiv( _SSE_PS, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(1), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::DivRPD() {
    _SSE::_IDiv( _SSE_PD, _SSE_ST(0), _SSE_ST(1) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(1), _SSE_ST(0) );
    --m_iTop;
}

inline Void SSEStack::SubRPF( UInt iDestIndex ) {
    _SSE::_ISub( _SSE_PS, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::SubRPD( UInt iDestIndex ) {
    _SSE::_ISub( _SSE_PD, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::DivRPF( UInt iDestIndex ) {
    _SSE::_IDiv( _SSE_PS, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}
inline Void SSEStack::DivRPD( UInt iDestIndex ) {
    _SSE::_IDiv( _SSE_PD, _SSE_ST(0), _SSE_ST(iDestIndex) );
    _SSE::_IMov( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) );
    --m_iTop;
}

inline Void SSEStack::HAddF() { _SSE::_IHAdd( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::HAddD() { _SSE::_IHAdd( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::HSubF() { _SSE::_IHSub( _SSE_PS, _SSE_ST(1), _SSE_ST(0) ); }
inline Void SSEStack::HSubD() { _SSE::_IHSub( _SSE_PD, _SSE_ST(1), _SSE_ST(0) ); }

inline Void SSEStack::HAddF( UInt iDestIndex ) { _SSE::_IHAdd( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::HAddD( UInt iDestIndex ) { _SSE::_IHAdd( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::HSubF( UInt iDestIndex ) { _SSE::_IHSub( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(0) ); }
inline Void SSEStack::HSubD( UInt iDestIndex ) { _SSE::_IHSub( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(0) ); }

inline Void SSEStack::HAddF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IHAdd( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::HAddD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IHAdd( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::HSubF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IHSub( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::HSubD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IHSub( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }

inline Void SSEStack::SqrtF() { _SSE::_ISqrt( _SSE_PS, _SSE_ST(0), _SSE_ST(0) ); }
inline Void SSEStack::SqrtD() { _SSE::_ISqrt( _SSE_PD, _SSE_ST(0), _SSE_ST(0) ); }
inline Void SSEStack::InvF()  { _SSE::_IInv( _SSE_PS, _SSE_ST(0), _SSE_ST(0) ); }
inline Void SSEStack::InvSqrtF() { _SSE::_IInvSqrt( _SSE_PS, _SSE_ST(0), _SSE_ST(0) ); }

inline Void SSEStack::SqrtF( UInt iStackIndex ) { _SSE::_ISqrt( _SSE_PS, _SSE_ST(iStackIndex), _SSE_ST(iStackIndex) ); }
inline Void SSEStack::SqrtD( UInt iStackIndex ) { _SSE::_ISqrt( _SSE_PD, _SSE_ST(iStackIndex), _SSE_ST(iStackIndex) ); }
inline Void SSEStack::InvF( UInt iStackIndex )  { _SSE::_IInv( _SSE_PS, _SSE_ST(iStackIndex), _SSE_ST(iStackIndex) ); }
inline Void SSEStack::InvSqrtF( UInt iStackIndex ) { _SSE::_IInvSqrt( _SSE_PS, _SSE_ST(iStackIndex), _SSE_ST(iStackIndex) ); }

inline Void SSEStack::SqrtF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_ISqrt( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::SqrtD( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_ISqrt( _SSE_PD, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::InvF( UInt iDestIndex, UInt iSrcIndex )  { _SSE::_IInv( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }
inline Void SSEStack::InvSqrtF( UInt iDestIndex, UInt iSrcIndex ) { _SSE::_IInvSqrt( _SSE_PS, _SSE_ST(iDestIndex), _SSE_ST(iSrcIndex) ); }

/////////////////////////////////////////////////////////////////////////////////

inline Void SSEStack::_IPush( UInt iVariant, QWord inMem128[2] ) {
    _SSE::_ISet( iVariant, m_iTop, inMem128 );
    ++m_iTop;
}
inline Void SSEStack::_IPop( UInt iVariant, QWord outMem128[2] ) {
    --m_iTop;
    _SSE::_IGet( iVariant, outMem128, m_iTop );
}

