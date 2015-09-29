/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Patching/PatchManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for generating & applying patches.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : This patcher uses the "bsdiff 6" algorithm described in the
//              thesis paper from Oxford university :
//                  http://www.daemonology.net/papers/thesis.pdf
//              Lots of maths in here ... stay focused !
//              Implementation delayed because we need FFTs ...
//              See Lib/Math/Analysis/Signal/*.h for math mayhem ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PATCHING_PATCHMANAGER_H
#define SCARAB_ENGINE_PATCHING_PATCHMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../../Lib/Math/Random/Random.h"
#include "../../Lib/Math/Analysis/Signal/Transforms.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define PatchFn PatchManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The PatchManager class
class PatchManager : public Manager<PatchManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<PatchManager>;
    PatchManager();
    virtual ~PatchManager();

public:
    Void Initialize();
    Void Cleanup();

    // Generates a patch file which will universally convert similar previous version of the
    // target file to the newer version provided.
    Void GeneratePatchFile( const GChar * strOutputPatchFile, const GChar * strPatchTargetFile );

    // Applies a generated patch file to update older versions of the file
    Void ApplyPatchFile( const GChar * strTargetFile, const GChar * strPatchFile );

private:
    // Matching With Mismatches algorithm
    Void _ComputeProjectionWeights( const Byte * strS, UInt iSizeS );
    Void _GeneratePrimeNumbers( UInt * outP, UInt iK, UInt iLowerBound, UInt iUpperBound );

    Bool _Halfabet_DichotomicSearch( UInt * outIndex, UInt iSize, Byte iValue );
    Void _Halfabet_Generate();
    Scalar _Halfabet_Project( Byte iValue );

    UInt _ComputeRho( UInt iN, UInt iM, UInt iJ, UInt iP );

    Void _OutputIndex( UInt * arrMatchIndices, Scalar * arrMaximalSums, UInt * pCurrentIndex, UInt iIndexCount, UInt iIndex, Scalar fSum );
    Void _MismatchSearch( UInt * outMatchIndices, UInt iIndexCount, const Byte * strS, UInt iSizeS, const Byte * strT, UInt iSizeT, Scalar fError );

    Scalar m_arrProjectionWeights[256];
    Byte m_arrHalfabet[128];

    // Patch data
    HFile m_hTargetFile;
    HFile m_hPatchFile;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PatchManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PATCHING_PATCHMANAGER_H


