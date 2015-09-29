/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/Hardware.cpp
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
// Includes
#include "Hardware.h"

/////////////////////////////////////////////////////////////////////////////////
// Hardware implementation
Hardware::Hardware()
{
    // CPU /////////////////////////////////////////////////////////
    _InspectCPU();
}
Hardware::~Hardware()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

// CPU /////////////////////////////////////////////////////////
Bool Hardware::_CpuId_IsAvailable()
{
    DWord dwHasCpuId = 0;

    __asm {
        // Cpuid instruction is present if and only if we can set and clear
        // the ID flag (bit 21) in EFLAGS register using pushfd/popfd ...
        mov ebx, 1
        shl ebx, 21  // ebx = 0x00200000 mask giving bit 21 (ID flag)
        mov ecx, ebx
        not ecx      // ecx = 0xffdfffff ~mask giving bit 21 (ID flag)

        pushfd
        pop eax // eax = EFLAGS

        // try to set bit 21
        or eax, ebx  // set bit in eax
        push eax     // eax to stack
        popfd        // stack to EFLAGS
        pushfd       // read EFLAGS back to stack
        pop eax      // stack to eax
        and eax, ebx // read bit
        cmp eax, ebx // test if bit is set (it should)
        jne _CpuID_Unavailable

        pushfd
        pop eax // eax = EFLAGS

        // try to clear bit 21
        and eax, ecx // clear bit in eax
        push eax     // eax to stack
        popfd        // stack to EFLAGS
        pushfd       // read EFLAGS back to stack
        pop eax      // stack to eax
        and eax, ebx // read bit
        cmp eax, ebx // test if bit is set (it should not)
        je _CpuID_Unavailable

        // Ok we passed the test, cpuid is supported
        inc dwHasCpuId

    _CpuID_Unavailable:
    }

    return ( dwHasCpuId != 0 );
}
Void Hardware::_CpuId_VendorString()
{
    DWord dwMaxEAX;
    DWord dwVendor0, dwVendor1, dwVendor2;

    __asm {
        xor eax, eax
        cpuid

        mov dwMaxEAX, eax
        mov dwVendor0, ebx
        mov dwVendor1, edx
        mov dwVendor2, ecx
    }

    m_strCPUVendorString[0] = ( dwVendor0 & 0xff );
    m_strCPUVendorString[1] = ( (dwVendor0 >> 8) & 0xff );
    m_strCPUVendorString[2] = ( (dwVendor0 >> 16) & 0xff );
    m_strCPUVendorString[3] = ( (dwVendor0 >> 24) & 0xff );

    m_strCPUVendorString[4] = ( dwVendor1 & 0xff );
    m_strCPUVendorString[5] = ( (dwVendor1 >> 8) & 0xff );
    m_strCPUVendorString[6] = ( (dwVendor1 >> 16) & 0xff );
    m_strCPUVendorString[7] = ( (dwVendor1 >> 24) & 0xff );

    m_strCPUVendorString[8] = ( dwVendor2 & 0xff );
    m_strCPUVendorString[9] = ( (dwVendor2 >> 8) & 0xff );
    m_strCPUVendorString[10] = ( (dwVendor2 >> 16) & 0xff );
    m_strCPUVendorString[11] = ( (dwVendor2 >> 24) & 0xff );

    m_dwBasicInfosMaxEAX = dwMaxEAX;
}
Void Hardware::_CpuId_VersionAndFeatures()
{
    DWord dwSteppingID, dwModel, dwFamily;
    DWord dwProcType;
    DWord dwExtModel, dwExtFamily;

    Byte iBrandIndex;
    Byte iInstructionCacheLineSize;
    Byte iLocalAPICID;

    DWord dwFeatures; // from edx
    DWord dwExtFeatures; // from ecx

    __asm {
        mov eax, 1
        cpuid

        // get features in edx
        mov dwFeatures, edx

        // get features in ecx
        mov dwExtFeatures, ecx

        // get misc data in ebx
        mov iBrandIndex, bl
        mov iInstructionCacheLineSize, bh
        shr ebx, 24
        mov iLocalAPICID, bl

        // get version data in eax
        mov ebx, eax
        and ebx, 0x0000000f
        mov dwSteppingID, ebx

        mov ebx, eax
        and ebx, 0x000000f0
        shr ebx, 4
        mov dwModel, ebx

        mov ebx, eax
        and ebx, 0x00000f00
        shr ebx, 8
        mov dwFamily, ebx

        mov ebx, eax
        and ebx, 0x00003000
        shr ebx, 12
        mov dwProcType, ebx

        mov ebx, eax
        and ebx, 0x000f0000
        shr ebx, 16
        mov dwExtModel, ebx

        mov ebx, eax
        and ebx, 0x0ff00000
        shr ebx, 20
        mov dwExtFamily, ebx
    }

    m_dwSteppingID = dwSteppingID;
    m_dwModelID = dwModel;
    m_dwFamilyID = dwFamily;
    m_dwProcessorType = dwProcType;
    m_dwExtModelID = dwExtModel;
    m_dwExtFamilyID = dwExtFamily;

    m_iBrandIndex = iBrandIndex;
    m_iCacheLineSize = iInstructionCacheLineSize;
    m_idLocalAPIC = iLocalAPICID;

    m_hCpuFeatures.bHasFPU =       ( (dwFeatures & 0x00000001) != 0 );
    m_hCpuFeatures.bHasVME =       ( (dwFeatures & 0x00000002) != 0 );
    m_hCpuFeatures.bHasDE =        ( (dwFeatures & 0x00000004) != 0 );
    m_hCpuFeatures.bHasPSE =       ( (dwFeatures & 0x00000008) != 0 );
    m_hCpuFeatures.bHasTSC =       ( (dwFeatures & 0x00000010) != 0 );
    m_hCpuFeatures.bHasMSR =       ( (dwFeatures & 0x00000020) != 0 );
    m_hCpuFeatures.bHasPAE =       ( (dwFeatures & 0x00000040) != 0 );
    m_hCpuFeatures.bHasMCE =       ( (dwFeatures & 0x00000080) != 0 );
    m_hCpuFeatures.bHasCMPXCHG8B = ( (dwFeatures & 0x00000100) != 0 );
    m_hCpuFeatures.bHasAPIC =      ( (dwFeatures & 0x00000200) != 0 );
    //m_hCpuFeatures.b__ =         ( (dwFeatures & 0x00000400) != 0 );
    m_hCpuFeatures.bHasSEP =       ( (dwFeatures & 0x00000800) != 0 );
    m_hCpuFeatures.bHasMTRR =      ( (dwFeatures & 0x00001000) != 0 );
    m_hCpuFeatures.bHasPGE =       ( (dwFeatures & 0x00002000) != 0 );
    m_hCpuFeatures.bHasMCA =       ( (dwFeatures & 0x00004000) != 0 );
    m_hCpuFeatures.bHasCMOV =      ( (dwFeatures & 0x00008000) != 0 );
    m_hCpuFeatures.bHasPAT =       ( (dwFeatures & 0x00010000) != 0 );
    m_hCpuFeatures.bHasPSE36 =     ( (dwFeatures & 0x00020000) != 0 );
    m_hCpuFeatures.bHasPSN =       ( (dwFeatures & 0x00040000) != 0 );
    m_hCpuFeatures.bHasCLFLUSH =   ( (dwFeatures & 0x00080000) != 0 );
    //m_hCpuFeatures.b__ =         ( (dwFeatures & 0x00100000) != 0 );
    m_hCpuFeatures.bHasDS =        ( (dwFeatures & 0x00200000) != 0 );
    m_hCpuFeatures.bHasACPI =      ( (dwFeatures & 0x00400000) != 0 );
    m_hCpuFeatures.bHasMMX =       ( (dwFeatures & 0x00800000) != 0 );
    m_hCpuFeatures.bHasFXSR =      ( (dwFeatures & 0x01000000) != 0 );
    m_hCpuFeatures.bHasSSE =       ( (dwFeatures & 0x02000000) != 0 );
    m_hCpuFeatures.bHasSSE2 =      ( (dwFeatures & 0x04000000) != 0 );
    m_hCpuFeatures.bHasSS =        ( (dwFeatures & 0x08000000) != 0 );
    m_hCpuFeatures.bHasHTT =       ( (dwFeatures & 0x10000000) != 0 );
    m_hCpuFeatures.bHasTM =        ( (dwFeatures & 0x20000000) != 0 );
    //m_hCpuFeatures.b__ =         ( (dwFeatures & 0x40000000) != 0 );
    m_hCpuFeatures.bHasPBE =       ( (dwFeatures & 0x80000000) != 0 );

    m_hCpuFeatures.bHasSSE3 =       ( (dwExtFeatures & 0x00000001) != 0 );
    m_hCpuFeatures.bHasPCLMULQDQ =  ( (dwExtFeatures & 0x00000002) != 0 );
    m_hCpuFeatures.bHasDTES64 =     ( (dwExtFeatures & 0x00000004) != 0 );
    m_hCpuFeatures.bHasMONITOR =    ( (dwExtFeatures & 0x00000008) != 0 );
    m_hCpuFeatures.bHasDSCPL =      ( (dwExtFeatures & 0x00000010) != 0 );
    m_hCpuFeatures.bHasVMX =        ( (dwExtFeatures & 0x00000020) != 0 );
    m_hCpuFeatures.bHasSMX =        ( (dwExtFeatures & 0x00000040) != 0 );
    m_hCpuFeatures.bHasEST =        ( (dwExtFeatures & 0x00000080) != 0 );
    m_hCpuFeatures.bHasTM2 =        ( (dwExtFeatures & 0x00000100) != 0 );
    m_hCpuFeatures.bHasSSSE3 =      ( (dwExtFeatures & 0x00000200) != 0 );
    m_hCpuFeatures.bHasCNTXID =     ( (dwExtFeatures & 0x00000400) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x00000800) != 0 );
    m_hCpuFeatures.bHasFMA =        ( (dwExtFeatures & 0x00001000) != 0 );
    m_hCpuFeatures.bHasCMPXCHG16B = ( (dwExtFeatures & 0x00002000) != 0 );
    m_hCpuFeatures.bHasXTPR =       ( (dwExtFeatures & 0x00004000) != 0 );
    m_hCpuFeatures.bHasPDCM =       ( (dwExtFeatures & 0x00008000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x00010000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x00020000) != 0 );
    m_hCpuFeatures.bHasDCA =        ( (dwExtFeatures & 0x00040000) != 0 );
    m_hCpuFeatures.bHasSSE41 =      ( (dwExtFeatures & 0x00080000) != 0 );
    m_hCpuFeatures.bHasSSE42 =      ( (dwExtFeatures & 0x00100000) != 0 );
    m_hCpuFeatures.bHasX2APIC =     ( (dwExtFeatures & 0x00200000) != 0 );
    m_hCpuFeatures.bHasMOVBE =      ( (dwExtFeatures & 0x00400000) != 0 );
    m_hCpuFeatures.bHasPOPCNT =     ( (dwExtFeatures & 0x00800000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x01000000) != 0 );
    m_hCpuFeatures.bHasAES =        ( (dwExtFeatures & 0x02000000) != 0 );
    m_hCpuFeatures.bHasXSAVE =      ( (dwExtFeatures & 0x04000000) != 0 );
    m_hCpuFeatures.bHasOSXSAVE =    ( (dwExtFeatures & 0x08000000) != 0 );
    m_hCpuFeatures.bHasAVX =        ( (dwExtFeatures & 0x10000000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x20000000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x40000000) != 0 );
    //m_hCpuFeatures.b__ =          ( (dwExtFeatures & 0x80000000) != 0 );
}
Void Hardware::_CpuId_ExtendedMaxEAX()
{
    DWord dwMaxEAX;
    __asm {
        mov eax, 0x80000000
        cpuid

        mov dwMaxEAX, eax
    }

    m_dwExtendedInfosMaxEAX = dwMaxEAX;
}

Void Hardware::_InspectCPU()
{
    if ( !_CpuId_IsAvailable() )
        return;

    _CpuId_VendorString();
    _CpuId_VersionAndFeatures();
    _CpuId_ExtendedMaxEAX();
    /////////////////////////////////////
}

