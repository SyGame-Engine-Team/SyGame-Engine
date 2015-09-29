/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Hardware/Hardware.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_HARDWARE_HARDWARE_H
#define SCARAB_THIRDPARTY_SYSTEM_HARDWARE_HARDWARE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Platform.h"

#include "FPU.h"
#include "SSE.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HardwareFn Hardware::GetInstancePtr()

// CPU //////////////////////////////////////////////////////////////////////////
#define CPU_VENDOR_AMD        "AuthenticAMD"
#define CPU_VENDOR_CENTAUR    "CentaurHauls"
#define CPU_VENDOR_CYRIX      "CyrixInstead"
#define CPU_VENDOR_INTEL      "GenuineIntel"
#define CPU_VENDOR_TRANSMETA1 "GenuineTMx86"
#define CPU_VENDOR_TRANSMETA2 "TransmetaCPU"
#define CPU_VENDOR_NSC        "Geode by NSC"
#define CPU_VENDOR_NEXGEN     "NexGenDriven"
#define CPU_VENDOR_RISE       "RiseRiseRise"
#define CPU_VENDOR_SIS        "SiS SiS SiS "
#define CPU_VENDOR_UMC        "UMC UMC UMC "
#define CPU_VENDOR_VIA        "VIA VIA VIA "

#define CPU_INTEL_PROCESSOR_TYPE_ORIGINAL_OEM 0x00000000
#define CPU_INTEL_PROCESSOR_TYPE_OVERDRIVE    0x00000001
#define CPU_INTEL_PROCESSOR_TYPE_DUAL         0x00000002
#define CPU_INTEL_PROCESSOR_TYPE_RESERVED     0x00000003

typedef struct _cpu_features
{
    // Basic Features
    Bool bHasFPU;        // integrated x87 FPU
    Bool bHasVME;        // virtual 8086 mode enhancements (CR4.VME, CR4.PVI, EFLAGS.VIF, EFLAGS.VIP)
    Bool bHasDE;         // debugging extensions (CR4.DE, DR4, DR5)
    Bool bHasPSE;        // page size extension (CR4.PSE, PDEs, PTEs, CR3)
    Bool bHasTSC;        // time stamp counter, RDTSC instruction (CR4.TSD)
    Bool bHasMSR;        // model specific registers, RDMSR / WRMSR instructions
    Bool bHasPAE;        // physical address extension for pointers greater than 32 bits
    Bool bHasMCE;        // machine check exception, exception 18 is defined (CR4.MCE, model dependant)
    Bool bHasCMPXCHG8B;  // compare and exchange 8 bytes (64 bits) instruction
    Bool bHasAPIC;       // integrated advanced programmable interrupt controller (memory mapped commands in physical 0xfffe0000 - 0xfffe0fff)
    Bool bHasSEP;        // SYSENTER / SYSEXIT instructions and associated MSRs
    Bool bHasMTRR;       // memory type range registers are supported (MTRRcap MSR contains feature bits)
    Bool bHasPGE;        // page global bit support for page mapping, indicating TLB common to diff processes don't need flushing (CR4.PGE)
    Bool bHasMCA;        // machine check architecture for error reporting (MCG_CAP MSR contains feature bits)
    Bool bHasCMOV;       // conditional MOV instructions : CMOV, FCMOV and also FCOMI, FCOMIP, FUCOMI, FUCOMIP
    Bool bHasPAT;        // page attribute table
    Bool bHasPSE36;      // 36-bits page size extension (physical adresses up to 40 bits)
    Bool bHasPSN;        // processor serial number (96 bits identification number)
    Bool bHasCLFLUSH;    // CLFLUSH instruction
    Bool bHasDS;         // debug store, memory resident buffer (BTS and PEBS facilities)
    Bool bHasACPI;       // thermal monitor and software controlled clock facilities (via MSRs)
    Bool bHasMMX;        // intel MMX technology supported
    Bool bHasFXSR;       // FXSAVE / FXRSTOR instructions for fast floating point context switches (CR4.OSFXSR)
    Bool bHasSSE;        // streaming single-instruction multiple-data extensions
    Bool bHasSSE2;       // streaming SIMD extensions 2
    Bool bHasSS;         // self snoop for conflicting memory types management
    Bool bHasHTT;        // multi-threading with more than one logical processor
    Bool bHasTM;         // thermal monitor, automatic thermal control circuitry (TCC)
    Bool bHasPBE;        // pending break enable
    // Extended features
    Bool bHasSSE3;       // streaming SIMD extensions 3
    Bool bHasPCLMULQDQ;  // carryless multiplication instruction
    Bool bHasDTES64;     // DS area using 64-bit layout
    Bool bHasMONITOR;    // MONITOR/MWAIT available
    Bool bHasDSCPL;      // CPL qualified debug store (for branch messages)
    Bool bHasVMX;        // virtual machine extensions
    Bool bHasSMX;        // safer mode extensions
    Bool bHasEST;        // enhanced intel speedstep technology
    Bool bHasTM2;        // thermal monitor 2
    Bool bHasSSSE3;      // supplemental SSE3
    Bool bHasCNTXID;     // L1 data cache can be set to either adaptive or shared mode (IA32_MISC_ENABLE MSR bit 24)
    Bool bHasFMA;        // fused multiply add
    Bool bHasCMPXCHG16B; // compare and exchange 16 bytes instruction
    Bool bHasXTPR;       // xTPR update control (IA32_MISC_ENABLE MSR bit 23)
    Bool bHasPDCM;       // performance & debug capability MSR (IA32_PERF_CAPABILITIES MSR)
    Bool bHasDCA;        // direct cache access (prefetch data from a memory mapped device)
    Bool bHasSSE41;      // streaming SIMD extensions 4.1
    Bool bHasSSE42;      // streaming SIMD extensions 4.2
    Bool bHasX2APIC;     // x2APIC supported
    Bool bHasMOVBE;      // move data after swapping bytes instruction
    Bool bHasPOPCNT;     // return number of bits set to 1 instruction
    Bool bHasAES;        // advanced encryption standard instructions (AES 128 192 and 256 bits, symmetric encrypt/decrypt, all RJINDAEL cyphers)
    Bool bHasXSAVE;      // XSAVE / XRSTOR extended states, XSETBV / XGETBV instructions and XCR0 register (XFEATURE_ENABLED_MASK)
    Bool bHasOSXSAVE;    // OS has enabled XSETBV / XGETBV to access XCR0 and extended state management using XSAVE / XRSTOR
    Bool bHasAVX;        // (???)
} CpuFeatures;

/////////////////////////////////////////////////////////////////////////////////
// The Hardware class
class Hardware
{
    // Discrete singleton interface
public:
    inline static Hardware * GetInstancePtr();

private:
    Hardware();
    ~Hardware();

public:
    // CPU /////////////////////////////////////////////////////////
    inline const Char * GetCPUVendorString() const;
    inline DWord GetCPUFamilyID( DWord * outExtendedFamilyID = NULL ) const;
    inline DWord GetCPUModelID( DWord * outExtendedModelID = NULL ) const;
    inline DWord GetCPUSteppingID() const;
    inline DWord GetCPUType() const;
    inline Byte GetCPUBrandIndex() const;

    inline const CpuFeatures * GetCPUFeatures() const;    

private:
    // CPU /////////////////////////////////////////////////////////
    Bool _CpuId_IsAvailable();
    Void _CpuId_VendorString();
    Void _CpuId_VersionAndFeatures();
    Void _CpuId_ExtendedMaxEAX();
    /////////////////////////////////////

    Void _InspectCPU();

    Bool m_bHasCpuId;
    DWord m_dwBasicInfosMaxEAX;     // 0x0000000X
    DWord m_dwExtendedInfosMaxEAX;; // 0x8000000X, 0 if unavailable

    Char m_strCPUVendorString[12];
    DWord m_dwFamilyID, m_dwExtFamilyID; // Family
    DWord m_dwModelID, m_dwExtModelID;   // Model
    DWord m_dwSteppingID;
    DWord m_dwProcessorType;
    Byte m_iBrandIndex;                  // index in the brand string table

    Byte m_iCacheLineSize; // Instruction cache line size (clflush)
    Byte m_idLocalAPIC;    // Local APIC ID

    CpuFeatures m_hCpuFeatures;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Hardware.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_HARDWARE_HARDWARE_H
