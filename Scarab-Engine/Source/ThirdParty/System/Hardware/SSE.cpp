/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/SSEWrappers.cpp
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
// Includes
#include "SSE.h"

/////////////////////////////////////////////////////////////////////////////////
// _SSE namespace implementation
namespace _SSE
{
        // IGet
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetSD );
    _SSE_INITIALIZE_PACK( 4, _IGet, __GetPS, __GetSS, __GetPD, __GetSD );

        // IGetNT (non-temporal, bypasses all caching procedures)
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetNTPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __GetNTPD );
    _SSE_INITIALIZE_PACK( 4, _IGetNT, __GetNTPS, NULL, __GetNTPD, NULL );

        // ISet
    _SSE_INITIALIZE_INSTRUCTION_XMM( __SetPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __SetSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __SetPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __SetSD );
    _SSE_INITIALIZE_PACK( 4, _ISet, __SetPS, __SetSS, __SetPD, __SetSD );

        // ISetNT (non-temporal, bypasses all caching procedures)
    _SSE_INITIALIZE_INSTRUCTION_XMM( _ISetNT );

        // IMov
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovSD );
    _SSE_INITIALIZE_PACK( 4, _IMov, __MovPS, __MovSS, __MovPD, __MovSD );

        // IMovHL, IMovLH
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( _IMovHL );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( _IMovLH );

        // IMovDupH, IMovDupL
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovDupHS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovDupLS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MovDupD );
    _SSE_INITIALIZE_PACK( 4, _IMovDupH, __MovDupHS, __MovDupHS, __MovDupD, __MovDupD );
    _SSE_INITIALIZE_PACK( 4, _IMovDupL, __MovDupLS, __MovDupLS, __MovDupD, __MovDupD );

        // IUnpckH
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __UnpckHPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __UnpckHPD );
    _SSE_INITIALIZE_PACK( 4, _IUnpckH, __UnpckHPS, NULL, __UnpckHPD, NULL );

        // IUnpckL
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __UnpckLPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __UnpckLPD );
    _SSE_INITIALIZE_PACK( 4, _IUnpckL, __UnpckLPS, NULL, __UnpckLPD, NULL );

        // IShuf
    // Uncomment this if you dare ...
    //_SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8EX( __ShufPS );
    //_SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8EX( __ShufPD );
    //_SSE_INITIALIZE_PACK( 4, _IShuf, __ShufPS, NULL, __ShufPD, NULL );

    //_SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8EX( _IShufPS );

    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __ShufPD00, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __ShufPD01, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __ShufPD10, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __ShufPD11, 0x03 );
    _SSE_INITIALIZE_PACK( 4, _IShufPD, __ShufPD00, __ShufPD01, __ShufPD10, __ShufPD11 );

        // IBlend
    // Uncomment this if you dare ...
    //_SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8EX( __BlendPS );
    //_SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8EX( __BlendPD );
    //_SSE_INITIALIZE_PACK( 4, _IBlend, __BlendPS, NULL, __BlendPD, NULL );

    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0000, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0001, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0010, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0011, 0x03 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0100, 0x04 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0101, 0x05 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0110, 0x06 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS0111, 0x07 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1000, 0x08 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1001, 0x09 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1010, 0x0a );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1011, 0x0b );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1100, 0x0c );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1101, 0x0d );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1110, 0x0e );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPS1111, 0x0f );
    _SSE_INITIALIZE_PACK( 16, _IBlendPS, __BlendPS0000, __BlendPS0001, __BlendPS0010, __BlendPS0011,
                                         __BlendPS0100, __BlendPS0101, __BlendPS0110, __BlendPS0111,
                                         __BlendPS1000, __BlendPS1001, __BlendPS1010, __BlendPS1011,
                                         __BlendPS1100, __BlendPS1101, __BlendPS1110, __BlendPS1111 );

    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPD00, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPD01, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPD10, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __BlendPD11, 0x03 );
    _SSE_INITIALIZE_PACK( 4, _IBlendPD, __BlendPD00, __BlendPD01, __BlendPD10, __BlendPD11 );

        // ICmpE
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpEPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpESS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpEPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpESD );
    _SSE_INITIALIZE_PACK( 4, _ICmpE, __CmpEPS, __CmpESS, __CmpEPD, __CmpESD );

        // ICmpNE
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNEPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNESS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNEPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNESD );
    _SSE_INITIALIZE_PACK( 4, _ICmpNE, __CmpNEPS, __CmpNESS, __CmpNEPD, __CmpNESD );

        // ICmpL
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLSD );
    _SSE_INITIALIZE_PACK( 4, _ICmpL, __CmpLPS, __CmpLSS, __CmpLPD, __CmpLSD );

        // ICmpLE
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLEPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLESS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLEPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpLESD );
    _SSE_INITIALIZE_PACK( 4, _ICmpLE, __CmpLEPS, __CmpLESS, __CmpLEPD, __CmpLESD );

        // ICmpNL
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLSD );
    _SSE_INITIALIZE_PACK( 4, _ICmpNL, __CmpNLPS, __CmpNLSS, __CmpNLPD, __CmpNLSD );

        // ICmpNLE
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLEPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLESS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLEPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpNLESD );
    _SSE_INITIALIZE_PACK( 4, _ICmpNLE, __CmpNLEPS, __CmpNLESS, __CmpNLEPD, __CmpNLESD );

        // ICmpO
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpOPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpOSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpOPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpOSD );
    _SSE_INITIALIZE_PACK( 4, _ICmpO, __CmpOPS, __CmpOSS, __CmpOPD, __CmpOSD );

        // ICmpU
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpUPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpUSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpUPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __CmpUSD );
    _SSE_INITIALIZE_PACK( 4, _ICmpU, __CmpUPS, __CmpUSS, __CmpUPD, __CmpUSD );

        // IMin
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MinPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MinSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MinPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MinSD );
    _SSE_INITIALIZE_PACK( 4, _IMin, __MinPS, __MinSS, __MinPD, __MinSD );

        // IMax
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MaxPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MaxSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MaxPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MaxSD );
    _SSE_INITIALIZE_PACK( 4, _IMax, __MaxPS, __MaxSS, __MaxPD, __MaxSD );

        // IMask
    _SSE_INITIALIZE_INSTRUCTION_XMM( __MaskPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM( __MaskPD );
    _SSE_INITIALIZE_PACK( 4, _IMask, __MaskPS, NULL, __MaskPD, NULL );

        // IRound
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __RoundPS, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __RoundSS, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __RoundPD, 0x00 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __RoundSD, 0x00 );
    _SSE_INITIALIZE_PACK( 4, _IRound, __RoundPS, __RoundSS, __RoundPD, __RoundSD );

        // IFloor
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __FloorPS, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __FloorSS, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __FloorPD, 0x01 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __FloorSD, 0x01 );
    _SSE_INITIALIZE_PACK( 4, _IFloor, __FloorPS, __FloorSS, __FloorPD, __FloorSD );

        // ICeil
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __CeilPS, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __CeilSS, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __CeilPD, 0x02 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __CeilSD, 0x02 );
    _SSE_INITIALIZE_PACK( 4, _ICeil, __CeilPS, __CeilSS, __CeilPD, __CeilSD );

        // ITruncate
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __TruncatePS, 0x03 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __TruncateSS, 0x03 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __TruncatePD, 0x03 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __TruncateSD, 0x03 );
    _SSE_INITIALIZE_PACK( 4, _ITruncate, __TruncatePS, __TruncateSS, __TruncatePD, __TruncateSD );

        // IAdd
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddSD );
    _SSE_INITIALIZE_PACK( 4, _IAdd, __AddPS, __AddSS, __AddPD, __AddSD );

        // ISub
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SubPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SubSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SubPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SubSD );
    _SSE_INITIALIZE_PACK( 4, _ISub, __SubPS, __SubSS, __SubPD, __SubSD );

        // IMul
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MulPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MulSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MulPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __MulSD );
    _SSE_INITIALIZE_PACK( 4, _IMul, __MulPS, __MulSS, __MulPD, __MulSD );

        // IDiv
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __DivPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __DivSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __DivPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __DivSD );
    _SSE_INITIALIZE_PACK( 4, _IDiv, __DivPS, __DivSS, __DivPD, __DivSD );
        
        // IAddSub
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddSubPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __AddSubPD );
    _SSE_INITIALIZE_PACK( 4, _IAddSub, __AddSubPS, NULL, __AddSubPD, NULL );

        // IHAdd
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __HAddPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __HAddPD );
    _SSE_INITIALIZE_PACK( 4, _IHAdd, __HAddPS, NULL, __HAddPD, NULL );

        // IHSub
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __HSubPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __HSubPD );
    _SSE_INITIALIZE_PACK( 4, _IHSub, __HSubPS, NULL, __HSubPD, NULL );

        // IDot2
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot2PS, 0x3f );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot2SS, 0x31 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot2PD, 0x33 );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot2SD, 0x31 );
    _SSE_INITIALIZE_PACK( 4, _IDot2, __Dot2PS, __Dot2SS, __Dot2PD, __Dot2SD );

        // IDot3
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot3PS, 0x7f );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot3SS, 0x71 );
    _SSE_INITIALIZE_PACK( 4, _IDot3, __Dot3PS, __Dot3SS, NULL, NULL );

        // IDot4
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot4PS, 0xff );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM_IMM8( __Dot4SS, 0xf1 );
    _SSE_INITIALIZE_PACK( 4, _IDot4, __Dot4PS, __Dot4SS, NULL, NULL );

        // IInv
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __InvPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __InvSS );
    _SSE_INITIALIZE_PACK( 4, _IInv, __InvPS, __InvSS, NULL, NULL );

        // ISqrt
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SqrtPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SqrtSS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SqrtPD );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __SqrtSD );
    _SSE_INITIALIZE_PACK( 4, _ISqrt, __SqrtPS, __SqrtSS, __SqrtPD, __SqrtSD );

        // IInvSqrt
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __InvSqrtPS );
    _SSE_INITIALIZE_INSTRUCTION_XMM_XMM( __InvSqrtSS );
    _SSE_INITIALIZE_PACK( 4, _IInvSqrt, __InvSqrtPS, __InvSqrtSS, NULL, NULL );

}

/////////////////////////////////////////////////////////////////////////////////
// SSEStack implementation
SSEStack::SSEStack()
{
    m_pAlignedFloatScratch = (Float*)MemAlign128( (UInt)m_arrUnalignedScratch );
    m_pAlignedDoubleScratch = (Double*)MemAlign128( (UInt)m_arrUnalignedScratch );

    m_iTop = 0;
}
SSEStack::~SSEStack()
{
    // nothing to do
}

