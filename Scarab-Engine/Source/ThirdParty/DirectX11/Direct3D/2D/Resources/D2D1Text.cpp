/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Text.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Fonts & Texts.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d2d1.h>
#include <dwrite.h>

#undef DebugAssert
#undef GetGlyphIndices

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Text.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontCollection implementation
D2D1FontCollection::D2D1FontCollection()
{
    m_pFontCollection = NULL;
}
D2D1FontCollection::~D2D1FontCollection()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1FontCollection::Create()
{
    DebugAssert( !(IsCreated()) );

    m_pFontCollection = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->GetSystemFontCollection( &m_pFontCollection );
    DebugAssert( hRes == S_OK && m_pFontCollection != NULL );
}
Void D2D1FontCollection::Destroy()
{
    DebugAssert( IsCreated() );

    m_pFontCollection->Release();
    m_pFontCollection = NULL;
}

UInt D2D1FontCollection::GetLocaleNameMaxLength()
{
    return LOCALE_NAME_MAX_LENGTH - 1;
}
Void D2D1FontCollection::GetLocaleName( GChar * outLocaleName )
{    
    Int iRes = GetUserDefaultLocaleName( outLocaleName, LOCALE_NAME_MAX_LENGTH );
    DebugAssert( iRes != 0 );
}

UInt D2D1FontCollection::GetFontFamilyCount() const
{
    DebugAssert( IsCreated() );

    return m_pFontCollection->GetFontFamilyCount();
}
Void D2D1FontCollection::GetFontFamily( D2D1FontFamily * outFontFamily, UInt iIndex ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( iIndex < m_pFontCollection->GetFontFamilyCount() );

    IDWriteFontFamily * pFontFamily = NULL;
    HRESULT hRes = m_pFontCollection->GetFontFamily( iIndex, &pFontFamily );
    DebugAssert( hRes == S_OK && pFontFamily != NULL );

    if ( outFontFamily->m_pFontFamily != NULL )
        outFontFamily->m_pFontFamily->Release();
    outFontFamily->m_pFontFamily = pFontFamily;
}

Bool D2D1FontCollection::FindFontFamily( UInt * outIndex, const GChar * strFontFamilyName ) const
{
    DebugAssert( IsCreated() );

    BOOL bExists = FALSE;
    HRESULT hRes = m_pFontCollection->FindFamilyName( strFontFamilyName, outIndex, &bExists );
    DebugAssert( hRes == S_OK );

    return ( bExists != FALSE );
}

Void D2D1FontCollection::EnumerateFontFamilies( GChar ** outFamilyNames, UInt * outNameLengths, UInt iMaxNames, const GChar * strLocaleName ) const
{
    DebugAssert( IsCreated() );

    // Get family count
    UInt iFamilyCount = m_pFontCollection->GetFontFamilyCount();
    if ( iFamilyCount > iMaxNames )
        iFamilyCount = iMaxNames;

    // Retrieve family names
    for( UInt i = 0; i < iFamilyCount; ++i ) {
        D2D1FontFamily hFamily;
        GetFontFamily( &hFamily, i );

        Bool bFound = hFamily.GetFamilyName( outFamilyNames[i], outNameLengths + i, strLocaleName );
        if ( !bFound )
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontFamily implementation
D2D1FontFamily::D2D1FontFamily()
{
    m_pFontFamily = NULL;
}
D2D1FontFamily::~D2D1FontFamily()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1FontFamily::Destroy()
{
    DebugAssert( IsCreated() );

    m_pFontFamily->Release();
    m_pFontFamily = NULL;
}

Bool D2D1FontFamily::GetFamilyName( GChar * outFamilyName, UInt * pNameLength, const GChar * strLocaleName ) const
{
    DebugAssert( IsCreated() );

    IDWriteLocalizedStrings * pNameStrings = NULL;
    HRESULT hRes = m_pFontFamily->GetFamilyNames( &pNameStrings );
    DebugAssert( hRes == S_OK && pNameStrings != NULL );

    UInt iIndex = 0;
    BOOL bExists = FALSE;
    if ( strLocaleName == NULL ) {
        GChar strDefaultLocaleName[LOCALE_NAME_MAX_LENGTH];
        Int iRes = GetUserDefaultLocaleName( strDefaultLocaleName, LOCALE_NAME_MAX_LENGTH );
        DebugAssert( iRes != 0 );

        hRes = pNameStrings->FindLocaleName( strDefaultLocaleName, &iIndex, &bExists );
    } else
        hRes = pNameStrings->FindLocaleName( strLocaleName, &iIndex, &bExists );
    DebugAssert( hRes == S_OK );

    if ( !bExists ) {
        hRes = pNameStrings->FindLocaleName( LOCALENAME_EN_US, &iIndex, &bExists );
        DebugAssert( hRes == S_OK );
    }
    if ( !bExists )
        iIndex = 0;

    UInt iLength = 0;
    hRes = pNameStrings->GetStringLength( iIndex, &iLength );
    DebugAssert( hRes == S_OK );

    hRes = pNameStrings->GetString( iIndex, outFamilyName, *pNameLength + 1 );
    DebugAssert( hRes == S_OK );

    pNameStrings->Release();
    pNameStrings = NULL;

    *pNameLength = iLength;
    return ( bExists != FALSE );
}

Void D2D1FontFamily::GetFontCollection( D2D1FontCollection * outFontCollection ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontCollection * pFontCollection = NULL;
    HRESULT hRes = m_pFontFamily->GetFontCollection( &pFontCollection );
    DebugAssert( hRes == S_OK && pFontCollection != NULL );

    if ( outFontCollection->m_pFontCollection != NULL )
        outFontCollection->m_pFontCollection->Release();
    outFontCollection->m_pFontCollection = pFontCollection;
}

UInt D2D1FontFamily::GetFontCount() const
{
    DebugAssert( IsCreated() );

    return m_pFontFamily->GetFontCount();
}
Void D2D1FontFamily::GetFont( D2D1Font * outFont, UInt iIndex ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( iIndex < m_pFontFamily->GetFontCount() );

    IDWriteFont * pFont = NULL;
    HRESULT hRes = m_pFontFamily->GetFont( iIndex, &pFont );
    DebugAssert( hRes == S_OK && pFont != NULL );

    if ( outFont->m_pFont != NULL )
        outFont->m_pFont->Release();
    outFont->m_pFont = pFont;
}

Void D2D1FontFamily::MatchClosestFont( D2D1Font * outFont, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch ) const
{
    DebugAssert( IsCreated() );

    IDWriteFont * pFont = NULL;
    HRESULT hRes = m_pFontFamily->GetFirstMatchingFont( D2D1FontWeightToD2D1[iWeight], D2D1FontStretchToD2D1[iStretch], D2D1FontStyleToD2D1[iStyle], &pFont );
    DebugAssert( hRes == S_OK && pFont != NULL );

    if ( outFont->m_pFont != NULL )
        outFont->m_pFont->Release();
    outFont->m_pFont = pFont;
}
Void D2D1FontFamily::MatchFonts( D2D1FontList * outFontList, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontList * pFontList = NULL;
    HRESULT hRes = m_pFontFamily->GetMatchingFonts( D2D1FontWeightToD2D1[iWeight], D2D1FontStretchToD2D1[iStretch], D2D1FontStyleToD2D1[iStyle], &pFontList );
    DebugAssert( hRes == S_OK && pFontList != NULL );

    if ( outFontList->m_pFontList != NULL )
        outFontList->m_pFontList->Release();
    outFontList->m_pFontList = pFontList;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1Font implementation
D2D1Font::D2D1Font()
{
    m_pFont = NULL;
}
D2D1Font::~D2D1Font()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1Font::Destroy()
{
    DebugAssert( IsCreated() );

    m_pFont->Release();
    m_pFont = NULL;
}

Bool D2D1Font::IsSymbolFont() const
{
    DebugAssert( IsCreated() );

    return ( m_pFont->IsSymbolFont() != FALSE );
}
Bool D2D1Font::HasCharacter( UInt iCharacter ) const
{
    DebugAssert( IsCreated() );

    BOOL bExists = FALSE;
    HRESULT hRes = m_pFont->HasCharacter( iCharacter, &bExists );
    DebugAssert( hRes == S_OK );

    return ( bExists != FALSE );
}

Bool D2D1Font::GetFaceName( GChar * outFaceName, UInt * pNameLength, const GChar * strLocaleName ) const
{
    DebugAssert( IsCreated() );

    IDWriteLocalizedStrings * pNameStrings = NULL;
    HRESULT hRes = m_pFont->GetFaceNames( &pNameStrings );
    DebugAssert( hRes == S_OK && pNameStrings != NULL );

    UInt iIndex = 0;
    BOOL bExists = FALSE;
    if ( strLocaleName == NULL ) {
        GChar strDefaultLocaleName[LOCALE_NAME_MAX_LENGTH];
        Int iRes = GetUserDefaultLocaleName( strDefaultLocaleName, LOCALE_NAME_MAX_LENGTH );
        DebugAssert( iRes != 0 );

        hRes = pNameStrings->FindLocaleName( strDefaultLocaleName, &iIndex, &bExists );
    } else
        hRes = pNameStrings->FindLocaleName( strLocaleName, &iIndex, &bExists );
    DebugAssert( hRes == S_OK );

    if ( !bExists ) {
        hRes = pNameStrings->FindLocaleName( LOCALENAME_EN_US, &iIndex, &bExists );
        DebugAssert( hRes == S_OK );
    }
    if ( !bExists )
        iIndex = 0;

    UInt iLength = 0;
    hRes = pNameStrings->GetStringLength( iIndex, &iLength );
    DebugAssert( hRes == S_OK );

    hRes = pNameStrings->GetString( iIndex, outFaceName, *pNameLength + 1 );
    DebugAssert( hRes == S_OK );

    pNameStrings->Release();
    pNameStrings = NULL;

    *pNameLength = iLength;
    return ( bExists != FALSE );
}
Bool D2D1Font::GetInfoString( GChar * outInfoString, UInt * pStringLength, D2D1FontInfoStringID idInfoString, const GChar * strLocaleName ) const
{
    DebugAssert( IsCreated() );

    BOOL bExists = FALSE;
    IDWriteLocalizedStrings * pNameStrings = NULL;
    HRESULT hRes = m_pFont->GetInformationalStrings( D2D1FontInfoStringIDToD2D1[idInfoString], &pNameStrings, &bExists );
    DebugAssert( hRes == S_OK );

    if ( bExists == FALSE ) {
        if ( pNameStrings != NULL ) {
            pNameStrings->Release();
            pNameStrings = NULL;
        }
        return false;
    }

    UInt iIndex = 0;
    bExists = FALSE;
    if ( strLocaleName == NULL ) {
        GChar strDefaultLocaleName[LOCALE_NAME_MAX_LENGTH];
        Int iRes = GetUserDefaultLocaleName( strDefaultLocaleName, LOCALE_NAME_MAX_LENGTH );
        DebugAssert( iRes != 0 );

        hRes = pNameStrings->FindLocaleName( strDefaultLocaleName, &iIndex, &bExists );
    } else
        hRes = pNameStrings->FindLocaleName( strLocaleName, &iIndex, &bExists );
    DebugAssert( hRes == S_OK );

    if ( !bExists ) {
        hRes = pNameStrings->FindLocaleName( LOCALENAME_EN_US, &iIndex, &bExists );
        DebugAssert( hRes == S_OK );
    }
    if ( !bExists )
        iIndex = 0;

    UInt iLength = 0;
    hRes = pNameStrings->GetStringLength( iIndex, &iLength );
    DebugAssert( hRes == S_OK );

    hRes = pNameStrings->GetString( iIndex, outInfoString, *pStringLength + 1 );
    DebugAssert( hRes == S_OK );

    pNameStrings->Release();
    pNameStrings = NULL;

    *pStringLength = iLength;
    return ( bExists != FALSE );
}

D2D1FontWeight D2D1Font::GetWeight() const
{
    DebugAssert( IsCreated() );

    return D2D1FontWeightFromD2D1( m_pFont->GetWeight() );
}
D2D1FontStyle D2D1Font::GetStyle() const
{
    DebugAssert( IsCreated() );

    return D2D1FontStyleFromD2D1[m_pFont->GetStyle()];
}
D2D1FontStretch D2D1Font::GetStretch() const
{
    DebugAssert( IsCreated() );

    return D2D1FontStretchFromD2D1[m_pFont->GetStretch()];
}

D2D1FontSimulation D2D1Font::GetSimulation() const
{
    DebugAssert( IsCreated() );

    return D2D1FontSimulationFromD2D1[m_pFont->GetSimulations()];
}
Void D2D1Font::GetMetrics( D2D1FontMetrics * outFontMetrics ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_METRICS hMetrics;
    m_pFont->GetMetrics( &hMetrics );

    outFontMetrics->ConvertFrom( &hMetrics );
}

Void D2D1Font::GetFontFamily( D2D1FontFamily * outFontFamily ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontFamily * pFontFamily = NULL;
    HRESULT hRes = m_pFont->GetFontFamily( &pFontFamily );
    DebugAssert( hRes == S_OK && pFontFamily != NULL );

    if ( outFontFamily->m_pFontFamily != NULL )
        outFontFamily->m_pFontFamily->Release();
    outFontFamily->m_pFontFamily = pFontFamily;
}

Void D2D1Font::CreateFontFace( D2D1FontFace * outFontFace ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontFace * pFontFace = NULL;
    HRESULT hRes = m_pFont->CreateFontFace( &pFontFace );
    DebugAssert( hRes == S_OK && pFontFace != NULL );

    if ( outFontFace->m_pFontFace != NULL )
        outFontFace->m_pFontFace->Release();
    outFontFace->m_pFontFace = pFontFace;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontList implementation
D2D1FontList::D2D1FontList()
{
    m_pFontList = NULL;
}
D2D1FontList::~D2D1FontList()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1FontList::Destroy()
{
    DebugAssert( IsCreated() );

    m_pFontList->Release();
    m_pFontList = NULL;
}

Void D2D1FontList::GetFontCollection( D2D1FontCollection * outFontCollection ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontCollection * pFontCollection = NULL;
    HRESULT hRes = m_pFontList->GetFontCollection( &pFontCollection );
    DebugAssert( hRes == S_OK && pFontCollection != NULL );

    if ( outFontCollection->m_pFontCollection != NULL )
        outFontCollection->m_pFontCollection->Release();
    outFontCollection->m_pFontCollection = pFontCollection;
}

UInt D2D1FontList::GetFontCount() const
{
    DebugAssert( IsCreated() );

    return m_pFontList->GetFontCount();
}
Void D2D1FontList::GetFont( D2D1Font * outFont, UInt iIndex ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( iIndex < m_pFontList->GetFontCount() );

    IDWriteFont * pFont = NULL;
    HRESULT hRes = m_pFontList->GetFont( iIndex, &pFont );
    DebugAssert( hRes == S_OK && pFont != NULL );

    if ( outFont->m_pFont != NULL )
        outFont->m_pFont->Release();
    outFont->m_pFont = pFont;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontFace implementation
D2D1FontFace::D2D1FontFace()
{
    m_pFontFace = NULL;
}
D2D1FontFace::~D2D1FontFace()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1FontFace::Destroy()
{
    DebugAssert( IsCreated() );

    m_pFontFace->Release();
    m_pFontFace = NULL;
}

Bool D2D1FontFace::IsSymbolFont() const
{
    DebugAssert( IsCreated() );

    return ( m_pFontFace->IsSymbolFont() != FALSE );
}

UInt D2D1FontFace::GetIndex() const
{
    DebugAssert( IsCreated() );

    return m_pFontFace->GetIndex();
}
D2D1FontFaceType D2D1FontFace::GetType() const
{
    DebugAssert( IsCreated() );

    return D2D1FontFaceTypeFromD2D1[m_pFontFace->GetType()];
}

D2D1FontSimulation D2D1FontFace::GetSimulation() const
{
    DebugAssert( IsCreated() );

    return D2D1FontSimulationFromD2D1[m_pFontFace->GetSimulations()];
}
Void D2D1FontFace::GetMetrics( D2D1FontMetrics * outFontMetrics ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_METRICS hMetrics;
    m_pFontFace->GetMetrics( &hMetrics );

    outFontMetrics->ConvertFrom( &hMetrics );
}
Void D2D1FontFace::GetGdiCompatibleMetrics( D2D1FontMetrics * outFontMetrics, Float fSizeDIP, Float fPixelsPerDIP, const D2D1Matrix32 * pTransform ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_METRICS hMetrics;
    HRESULT hRes = m_pFontFace->GetGdiCompatibleMetrics( fSizeDIP, fPixelsPerDIP, (const DWRITE_MATRIX *)pTransform, &hMetrics );
    DebugAssert( hRes == S_OK );

    outFontMetrics->ConvertFrom( &hMetrics );
}

Void D2D1FontFace::GetRecommendedRenderingMode( D2D1TextRenderingMode * outMode, Float fSizeDIP, Float fPixelsPerDIP, D2D1TextMeasuringMode iMeasuringMode, const D2D1TextRenderState * pTextRenderState ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pTextRenderState->IsCreated() );

    DWRITE_RENDERING_MODE iTmpMode = DWRITE_RENDERING_MODE_DEFAULT;
    HRESULT hRes = m_pFontFace->GetRecommendedRenderingMode( fSizeDIP, fPixelsPerDIP, D2D1TextMeasuringModeToD2D1[iMeasuringMode], pTextRenderState->m_pTextRenderingParams, &iTmpMode );
    DebugAssert( hRes == S_OK );

    *outMode = D2D1TextRenderingModeFromD2D1[iTmpMode];
}

UInt D2D1FontFace::GetGlyphCount() const
{
    DebugAssert( IsCreated() );

    return (UInt)( m_pFontFace->GetGlyphCount() );
}
Void D2D1FontFace::GetGlyphIndices( UShort * outGlyphIndices, const UInt * arrCodePointsUSC4, UInt iCodePointCount ) const
{
    DebugAssert( IsCreated() );

#if ( defined(UNICODE) || defined(_UNICODE) )
    HRESULT hRes = m_pFontFace->GetGlyphIndicesW( arrCodePointsUSC4, iCodePointCount, outGlyphIndices );
#else
    HRESULT hRes = m_pFontFace->GetGlyphIndicesA( arrCodePointsUSC4, iCodePointCount, outGlyphIndices );
#endif
    DebugAssert( hRes == S_OK );
}

Void D2D1FontFace::GetDesignGlyphMetrics( D2D1FontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways ) const
{
    DebugAssert( IsCreated() );

    DWRITE_GLYPH_METRICS hGlyphMetrics;
    HRESULT hRes = m_pFontFace->GetDesignGlyphMetrics( arrGlyphIndices, iGlyphCount, &hGlyphMetrics, bIsSideways ? TRUE : FALSE );
    DebugAssert( hRes == S_OK );

    outGlyphMetrics->ConvertFrom( &hGlyphMetrics );
}
Void D2D1FontFace::GetGdiCompatibleGlyphMetrics( D2D1FontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Float fSizeDIP, Float fPixelsPerDIP, Bool bUseGdiNatural, const D2D1Matrix32 * pTransform ) const
{
    DebugAssert( IsCreated() );

    DWRITE_GLYPH_METRICS hGlyphMetrics;
    HRESULT hRes = m_pFontFace->GetGdiCompatibleGlyphMetrics( fSizeDIP, fPixelsPerDIP, (const DWRITE_MATRIX *)pTransform, bUseGdiNatural ? TRUE : FALSE,
                                                              arrGlyphIndices, iGlyphCount, &hGlyphMetrics, bIsSideways ? TRUE : FALSE );
    DebugAssert( hRes == S_OK );

    outGlyphMetrics->ConvertFrom( &hGlyphMetrics );
}

Void D2D1FontFace::GetGlyphRunOutline( D2D1PathGeometry * outGeometry, Float fSizeDIP, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Bool bIsRightToLeft,
                                                                       const Float * arrGlyphAdvances, const D2D1FontGlyphOffset * arrGlyphOffsets ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( outGeometry->IsCreated() );
    DebugAssert( outGeometry->IsOpened() );

    HRESULT hRes = m_pFontFace->GetGlyphRunOutline( fSizeDIP, arrGlyphIndices, arrGlyphAdvances, (const DWRITE_GLYPH_OFFSET *)arrGlyphOffsets, iGlyphCount,
                                                    bIsSideways ? TRUE : FALSE, bIsRightToLeft ? TRUE : FALSE, outGeometry->m_pGeometrySink );
    DebugAssert( hRes == S_OK );
}

//UInt D2D1FontFace::GetFontFileCount() const
//{
//    DebugAssert( IsCreated() );
//
//    UInt iFileCount = 0;
//    HRESULT hRes = m_pFontFace->GetFiles( &iFileCount, NULL );
//    DebugAssert( hRes == S_OK );
//}
//Void D2D1FontFace::GetFontFiles( D2D1FontFile ** outFontFiles, UInt iMaxFiles ) const
//{
//    DebugAssert( IsCreated() );
//
//    IDWriteFontFile ** arrTemp = (IDWriteFontFile**)( SystemFn->MemAlloc( iMaxFiles * sizeof(IDWriteFontFile*) ) );
//
//    UInt iTotalFiles = iMaxFiles;
//    HRESULT hRes = m_pFontFace->GetFiles( &iTotalFiles, arrTemp );
//    DebugAssert( hRes == S_OK );
//
//    for( UInt i = 0; i < iMaxFiles; ++i )
//        *(outFontFiles[i]) = D2D1FontFile( m_pContext2D, arrTemp[i] );
//
//    SystemFn->MemFree( arrTemp );
//    arrTemp = NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextFormat implementation
D2D1TextFormat::D2D1TextFormat()
{
    m_pTextFormat = NULL;
}
D2D1TextFormat::~D2D1TextFormat()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1TextFormat::Create( const GChar * strLocaleName, const GChar * strFontFamilyName, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch, Float fSize, D2D1FontCollection * pFontCollection )
{
    DebugAssert( !(IsCreated()) );

    m_pTextFormat = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->CreateTextFormat( strFontFamilyName, (pFontCollection != NULL) ? pFontCollection->m_pFontCollection : NULL,
                                                                          D2D1FontWeightToD2D1[iWeight], D2D1FontStyleToD2D1[iStyle], D2D1FontStretchToD2D1[iStretch], fSize, strLocaleName, &m_pTextFormat );
    DebugAssert( hRes == S_OK && m_pTextFormat != NULL );
}
Void D2D1TextFormat::Destroy()
{
    DebugAssert( IsCreated() );

    m_pTextFormat->Release();
    m_pTextFormat = NULL;
}

Void D2D1TextFormat::GetFontCollection( D2D1FontCollection * outFontCollection ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontCollection * pFontCollection = NULL;
    HRESULT hRes = m_pTextFormat->GetFontCollection( &pFontCollection );
    DebugAssert( hRes == S_OK && pFontCollection != NULL );

    if ( outFontCollection->m_pFontCollection != NULL )
        outFontCollection->m_pFontCollection->Release();
    outFontCollection->m_pFontCollection = pFontCollection;
}

UInt D2D1TextFormat::GetLocaleNameLength() const
{
    DebugAssert( IsCreated() );

    return m_pTextFormat->GetLocaleNameLength();
}
Void D2D1TextFormat::GetLocaleName( GChar * outLocaleName, UInt iMaxLength ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->GetLocaleName( outLocaleName, iMaxLength + 1 );
    DebugAssert( hRes == S_OK );
}
UInt D2D1TextFormat::GetFontFamilyNameLength() const
{
    DebugAssert( IsCreated() );

    return m_pTextFormat->GetFontFamilyNameLength();
}
Void D2D1TextFormat::GetFontFamilyName( GChar * outFontFamilyName, UInt iMaxLength ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->GetFontFamilyName( outFontFamilyName, iMaxLength + 1 );
    DebugAssert( hRes == S_OK );
}

D2D1FontWeight D2D1TextFormat::GetFontWeight() const
{
    DebugAssert( IsCreated() );

    return D2D1FontWeightFromD2D1( m_pTextFormat->GetFontWeight() );
}
D2D1FontStyle D2D1TextFormat::GetFontStyle() const
{
    DebugAssert( IsCreated() );

    return D2D1FontStyleFromD2D1[m_pTextFormat->GetFontStyle()];
}
D2D1FontStretch D2D1TextFormat::GetFontStretch() const
{
    DebugAssert( IsCreated() );

    return D2D1FontStretchFromD2D1[m_pTextFormat->GetFontStretch()];
}
Float D2D1TextFormat::GetFontSize() const
{
    DebugAssert( IsCreated() );

    return m_pTextFormat->GetFontSize();
}

D2D1TextFlowDirection D2D1TextFormat::GetFlowDirection() const
{
    DebugAssert( IsCreated() );

    return D2D1TextFlowDirectionFromD2D1[m_pTextFormat->GetFlowDirection()];
}
Void D2D1TextFormat::SetFlowDirection( D2D1TextFlowDirection iFlowDirection )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetFlowDirection( D2D1TextFlowDirectionToD2D1[iFlowDirection] );
    DebugAssert( hRes == S_OK );
}

D2D1TextReadingDirection D2D1TextFormat::GetReadingDirection() const
{
    DebugAssert( IsCreated() );

    return D2D1TextReadingDirectionFromD2D1[m_pTextFormat->GetReadingDirection()];
}
Void D2D1TextFormat::SetReadingDirection( D2D1TextReadingDirection iReadingDirection )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetReadingDirection( D2D1TextReadingDirectionToD2D1[iReadingDirection] );
    DebugAssert( hRes == S_OK );
}

D2D1TextLineSpacingMethod D2D1TextFormat::GetLineSpacing( Float * outLineSpacing, Float * outBaseline ) const
{
    DebugAssert( IsCreated() );

    DWRITE_LINE_SPACING_METHOD iMethod;
    Float fLineSpacing;
    Float fBaseline;
    HRESULT hRes = m_pTextFormat->GetLineSpacing( &iMethod, &fLineSpacing, &fBaseline );
    DebugAssert( hRes == S_OK );

    if ( outLineSpacing != NULL )
        *outLineSpacing = fLineSpacing;
    if ( outBaseline != NULL )
        *outBaseline = fBaseline;
    return D2D1TextLineSpacingMethodFromD2D1[iMethod];
}
Void D2D1TextFormat::SetLineSpacing( D2D1TextLineSpacingMethod iLineSpacingMethod, Float fLineSpacing, Float fBaseline )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetLineSpacing( D2D1TextLineSpacingMethodToD2D1[iLineSpacingMethod], fLineSpacing, fBaseline );
    DebugAssert( hRes == S_OK );
}

D2D1TextAlignment D2D1TextFormat::GetTextAlignment() const
{
    DebugAssert( IsCreated() );

    return D2D1TextAlignmentFromD2D1[m_pTextFormat->GetTextAlignment()];
}
Void D2D1TextFormat::SetTextAlignment( D2D1TextAlignment iTextAlignment )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetTextAlignment( D2D1TextAlignmentToD2D1[iTextAlignment] );
    DebugAssert( hRes == S_OK );
}

D2D1TextParagraphAlignment D2D1TextFormat::GetParagraphAlignment() const
{
    DebugAssert( IsCreated() );

    return D2D1TextParagraphAlignmentFromD2D1[m_pTextFormat->GetParagraphAlignment()];
}
Void D2D1TextFormat::SetParagraphAlignment( D2D1TextParagraphAlignment iParagraphAlignment )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetParagraphAlignment( D2D1TextParagraphAlignmentToD2D1[iParagraphAlignment] );
    DebugAssert( hRes == S_OK );
}

Float D2D1TextFormat::GetIncrementalTabStop() const
{
    DebugAssert( IsCreated() );

    return m_pTextFormat->GetIncrementalTabStop();
}
Void D2D1TextFormat::SetIncrementalTabStop( Float fIncrementalTabStop )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetIncrementalTabStop( fIncrementalTabStop );
    DebugAssert( hRes == S_OK );
}

D2D1TextTrimmingGranularity D2D1TextFormat::GetTrimming( GChar * outDelimiter, UInt * outDelimiterCount ) const
{
    DebugAssert( IsCreated() );

    DWRITE_TRIMMING hTrimmingOptions;
    HRESULT hRes = m_pTextFormat->GetTrimming( &hTrimmingOptions, NULL );
    DebugAssert( hRes == S_OK );

    if ( outDelimiter != NULL )
        *outDelimiter = (GChar)( hTrimmingOptions.delimiter );
    if ( outDelimiterCount != NULL )
        *outDelimiterCount = hTrimmingOptions.delimiterCount;
    return D2D1TextTrimmingGranularityFromD2D1[hTrimmingOptions.granularity];
}
Void D2D1TextFormat::SetTrimming( D2D1TextTrimmingGranularity iTrimmingGranularity, GChar chDelimiter, UInt iDelimiterCount )
{
    DebugAssert( IsCreated() );

    DWRITE_TRIMMING hTrimmingOptions;
    hTrimmingOptions.granularity = D2D1TextTrimmingGranularityToD2D1[iTrimmingGranularity];
    hTrimmingOptions.delimiter = (UInt)chDelimiter;
    hTrimmingOptions.delimiterCount = iDelimiterCount;

    HRESULT hRes = m_pTextFormat->SetTrimming( &hTrimmingOptions, NULL );
    DebugAssert( hRes == S_OK );
}

D2D1TextWordWrapping D2D1TextFormat::GetWordWrapping() const
{
    DebugAssert( IsCreated() );

    return D2D1TextWordWrappingFromD2D1[m_pTextFormat->GetWordWrapping()];
}
Void D2D1TextFormat::SetWordWrapping( D2D1TextWordWrapping iWordWrapping )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextFormat->SetWordWrapping( D2D1TextWordWrappingToD2D1[iWordWrapping] );
    DebugAssert( hRes == S_OK );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextInlineObject implementation
class D2D1TextInlineObject::_DWriteInlineObject : public IDWriteInlineObject {
    public:
        _DWriteInlineObject( D2D1TextInlineObject * pParentHandler ):IDWriteInlineObject() { m_pParentHandler = pParentHandler; m_iRefCount = 0; }
        virtual ~_DWriteInlineObject()                                                     { }

        HRESULT __stdcall QueryInterface( IID const & riid, void ** ppvObject ) {
            if ( __uuidof(IDWriteInlineObject) == riid ) *ppvObject = dynamic_cast<IDWriteInlineObject*>(this);
            else if ( __uuidof(IUnknown) == riid )       *ppvObject = dynamic_cast<IUnknown*>(this);
            else                                         { *ppvObject = NULL; return E_FAIL; }
            return S_OK;
        }

        unsigned long __stdcall AddRef() { return InterlockedIncrement( &m_iRefCount ); }
        unsigned long __stdcall Release() {
            unsigned long newCount = InterlockedDecrement( &m_iRefCount );
            if ( newCount == 0 ) { delete this; return 0; }
            return newCount;
        }

        virtual __declspec(nothrow) HRESULT __stdcall Draw( void * /*pClientDrawingContext*/, IDWriteTextRenderer * /*pRenderer*/, FLOAT fOriginX, FLOAT fOriginY, BOOL bIsSideways, BOOL bIsRightToLeft, IUnknown * /*pClientDrawingEffect*/ ) {
            D2D1Point hDstPoint; hDstPoint.fX = fOriginX; hDstPoint.fY = fOriginY;
            m_pParentHandler->Draw( &hDstPoint, (bIsSideways != FALSE), (bIsRightToLeft != FALSE) );
            return S_OK;
        }
        virtual HRESULT __stdcall GetMetrics( DWRITE_INLINE_OBJECT_METRICS * outMetrics )            { m_pParentHandler->m_hInlineObjectMetrics.ConvertTo( outMetrics ); return S_OK; }
        virtual HRESULT __stdcall GetOverhangMetrics( DWRITE_OVERHANG_METRICS * outOverhangMetrics ) { m_pParentHandler->m_hOverhangMetrics.ConvertTo( outOverhangMetrics ); return S_OK; }
        virtual HRESULT __stdcall GetBreakConditions( DWRITE_BREAK_CONDITION * outBreakConditionBefore, DWRITE_BREAK_CONDITION * outBreakConditionAfter ) {
            *outBreakConditionBefore = D2D1TextBreakConditionToD2D1[m_pParentHandler->m_iBreakBefore];
            *outBreakConditionAfter = D2D1TextBreakConditionToD2D1[m_pParentHandler->m_iBreakAfter];
            return S_OK;
        }

    protected:
        friend class D2D1TextLayout;
        D2D1TextInlineObject * m_pParentHandler;
        unsigned long m_iRefCount;
};

D2D1TextInlineObject::D2D1TextInlineObject( D2D1RenderingContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_hInlineObjectMetrics.fWidth = 1.0f;
    m_hInlineObjectMetrics.fHeight = 1.0f;
    m_hInlineObjectMetrics.fBaseline = 1.0f;
    m_hInlineObjectMetrics.bSupportSideways = false;

    m_hOverhangMetrics.fLeft = 0.0f;
    m_hOverhangMetrics.fTop = 0.0f;
    m_hOverhangMetrics.fRight = 0.0f;
    m_hOverhangMetrics.fBottom = 0.0f;

    m_iBreakBefore = D2D1TEXT_BREAKCONDITION_NEUTRAL;
    m_iBreakAfter = D2D1TEXT_BREAKCONDITION_NEUTRAL;

    m_pTextInlineObject = new _DWriteInlineObject( this );
    m_pTextInlineObject->AddRef();
}
D2D1TextInlineObject::~D2D1TextInlineObject()
{
    m_pTextInlineObject->Release();
    m_pTextInlineObject = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextLayout implementation
D2D1TextLayout::D2D1TextLayout()
{
    m_pTextLayout = NULL;
}
D2D1TextLayout::~D2D1TextLayout()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1TextLayout::Create( D2D1TextFormat * pTextFormat, Float fMaxWidth, Float fMaxHeight, const GChar * strText, UInt iTextLength )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( pTextFormat->IsCreated() );
    if ( iTextLength == INVALID_OFFSET )
        iTextLength = StringFn->Length( strText );

    m_pTextLayout = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pDWFactory->CreateTextLayout( strText, iTextLength, pTextFormat->m_pTextFormat, fMaxWidth, fMaxHeight, &m_pTextLayout );
    DebugAssert( hRes == S_OK && m_pTextLayout != NULL );
}
Void D2D1TextLayout::Destroy()
{
    DebugAssert( IsCreated() );

    m_pTextLayout->Release();
    m_pTextLayout = NULL;
}

Float D2D1TextLayout::GetMaxWidth() const
{
    DebugAssert( IsCreated() );

    return m_pTextLayout->GetMaxWidth();
}
Void D2D1TextLayout::SetMaxWidth( Float fMaxWidth )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetMaxWidth( fMaxWidth );
    DebugAssert( hRes == S_OK );
}

Float D2D1TextLayout::GetMaxHeight() const
{
    DebugAssert( IsCreated() );

    return m_pTextLayout->GetMaxHeight();
}
Void D2D1TextLayout::SetMaxHeight( Float fMaxHeight )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetMaxHeight( fMaxHeight );
    DebugAssert( hRes == S_OK );
}

Void D2D1TextLayout::GetMetrics( D2D1TextMetrics * outTextMetrics ) const
{
    DebugAssert( IsCreated() );

    DWRITE_TEXT_METRICS hTextMetrics;
    HRESULT hRes = m_pTextLayout->GetMetrics( &hTextMetrics );
    DebugAssert( hRes == S_OK );

    outTextMetrics->ConvertFrom( &hTextMetrics );
}
Void D2D1TextLayout::GetOverhangMetrics( D2D1TextOverhangMetrics * outOverhangMetrics ) const
{
    DebugAssert( IsCreated() );

    DWRITE_OVERHANG_METRICS hOverhangMetrics;
    HRESULT hRes = m_pTextLayout->GetOverhangMetrics( &hOverhangMetrics );
    DebugAssert( hRes == S_OK );

    outOverhangMetrics->ConvertFrom( &hOverhangMetrics );
}

UInt D2D1TextLayout::GetLineMetricCount() const
{
    DebugAssert( IsCreated() );

    UInt iLineMetricsCount = 0;
    DWRITE_LINE_METRICS hLineMetrics;
    HRESULT hRes = m_pTextLayout->GetLineMetrics( &hLineMetrics, 1, &iLineMetricsCount );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );

    return iLineMetricsCount;
}
Void D2D1TextLayout::GetLineMetrics( D2D1TextLineMetrics * outLineMetrics, UInt iMaxLines ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->GetLineMetrics( (DWRITE_LINE_METRICS*)outLineMetrics, iMaxLines, &iMaxLines );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );
}

UInt D2D1TextLayout::GetClusterMetricCount() const
{
    DebugAssert( IsCreated() );

    UInt iClusterMetricsCount = 0;
    DWRITE_CLUSTER_METRICS hClusterMetrics;
    HRESULT hRes = m_pTextLayout->GetClusterMetrics( &hClusterMetrics, 1, &iClusterMetricsCount );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );

    return iClusterMetricsCount;
}
Void D2D1TextLayout::GetClusterMetrics( D2D1TextClusterMetrics * outClusterMetrics, UInt iMaxClusters ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->GetClusterMetrics( (DWRITE_CLUSTER_METRICS*)outClusterMetrics, iMaxClusters, &iMaxClusters );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );
}

Float D2D1TextLayout::ComputeMinWidth() const
{
    DebugAssert( IsCreated() );

    Float fMinWidth = 0.0f;
    HRESULT hRes = m_pTextLayout->DetermineMinWidth( &fMinWidth );
    DebugAssert( hRes == S_OK );

    return fMinWidth;
}

Void D2D1TextLayout::HitTestPoint( D2D1TextHitTestMetrics * outHitTestMetrics, Bool * outIsInside, Bool * outIsTrailingHit, Float fX, Float fY ) const
{
    DebugAssert( IsCreated() );

    BOOL bIsTrailing, bIsInside;
    HRESULT hRes = m_pTextLayout->HitTestPoint( fX, fY, &bIsTrailing, &bIsInside, (DWRITE_HIT_TEST_METRICS*)outHitTestMetrics );
    DebugAssert( hRes == S_OK );

    *outIsInside = ( bIsInside != FALSE );
    *outIsTrailingHit = ( bIsTrailing != FALSE );
}

Void D2D1TextLayout::HitTestTextPosition( D2D1TextHitTestMetrics * outHitTestMetrics, Float * outX, Float * outY, UInt iPosition, Bool bIsTrailingHit ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->HitTestTextPosition( iPosition, (bIsTrailingHit) ? TRUE : FALSE, outX, outY, (DWRITE_HIT_TEST_METRICS*)outHitTestMetrics );
    DebugAssert( hRes == S_OK );
}

UInt D2D1TextLayout::HitTestTextRangeCount( const D2D1TextRange * pRange, Float fOriginX, Float fOriginY ) const
{
    DebugAssert( IsCreated() );

    UInt iHitTestCount = 0;
    HRESULT hRes = m_pTextLayout->HitTestTextRange( pRange->iPosition, pRange->iLength, fOriginX, fOriginY, NULL, 0, &iHitTestCount );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );

    return iHitTestCount;
}
Void D2D1TextLayout::HitTestTextRange( D2D1TextHitTestMetrics * outHitTestMetrics, UInt iMaxHitTests, const D2D1TextRange * pRange, Float fOriginX, Float fOriginY ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->HitTestTextRange( pRange->iPosition, pRange->iLength, fOriginX, fOriginY, (DWRITE_HIT_TEST_METRICS*)outHitTestMetrics, iMaxHitTests, &iMaxHitTests );
    DebugAssert( hRes == S_OK || hRes == E_NOT_SUFFICIENT_BUFFER );
}

UInt D2D1TextLayout::GetLocaleNameLength( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    UInt iLength = 0;
    HRESULT hRes = m_pTextLayout->GetLocaleNameLength( iPosition, &iLength, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return iLength;
}
Void D2D1TextLayout::GetLocaleName( GChar * outLocaleName, UInt iNameSize, UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->GetLocaleName( iPosition, outLocaleName, iNameSize, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );
}
Void D2D1TextLayout::SetLocaleName( const GChar * strLocaleName, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetLocaleName( strLocaleName, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

Void D2D1TextLayout::GetFontCollection( D2D1FontCollection * outFontCollection, UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    IDWriteFontCollection * pFontCollection = NULL;
    HRESULT hRes = m_pTextLayout->GetFontCollection( iPosition, &pFontCollection, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK && pFontCollection != NULL );

    if ( outFontCollection->m_pFontCollection != NULL )
        outFontCollection->m_pFontCollection->Release();
    outFontCollection->m_pFontCollection = pFontCollection;
}
Void D2D1TextLayout::SetFontCollection( D2D1FontCollection * pFontCollection, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontCollection( pFontCollection->m_pFontCollection, *( (const DWRITE_TEXT_RANGE*)pRange ) );
    DebugAssert( hRes == S_OK );
}

UInt D2D1TextLayout::GetFontFamilyNameLength( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    UInt iLength = 0;
    HRESULT hRes = m_pTextLayout->GetFontFamilyNameLength( iPosition, &iLength, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return iLength;
}
Void D2D1TextLayout::GetFontFamilyName( GChar * outFamilyName, UInt iNameSize, UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->GetFontFamilyName( iPosition, outFamilyName, iNameSize, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );
}
Void D2D1TextLayout::SetFontFamilyName( const GChar * strFontFamilyName, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontFamilyName( strFontFamilyName, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

D2D1FontWeight D2D1TextLayout::GetFontWeight( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_WEIGHT iWeight;
    HRESULT hRes = m_pTextLayout->GetFontWeight( iPosition, &iWeight, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return D2D1FontWeightFromD2D1( iWeight );
}
Void D2D1TextLayout::SetFontWeight( D2D1FontWeight iWeight, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontWeight( D2D1FontWeightToD2D1[iWeight], *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

D2D1FontStyle D2D1TextLayout::GetFontStyle( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_STYLE iStyle;
    HRESULT hRes = m_pTextLayout->GetFontStyle( iPosition, &iStyle, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return D2D1FontStyleFromD2D1[iStyle];
}
Void D2D1TextLayout::SetFontStyle( D2D1FontStyle iStyle, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontStyle( D2D1FontStyleToD2D1[iStyle], *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

D2D1FontStretch D2D1TextLayout::GetFontStretch( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    DWRITE_FONT_STRETCH iStretch;
    HRESULT hRes = m_pTextLayout->GetFontStretch( iPosition, &iStretch, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return D2D1FontStretchFromD2D1[iStretch];
}
Void D2D1TextLayout::SetFontStretch( D2D1FontStretch iStretch, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontStretch( D2D1FontStretchToD2D1[iStretch], *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

Float D2D1TextLayout::GetFontSize( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    Float fSize;
    HRESULT hRes = m_pTextLayout->GetFontSize( iPosition, &fSize, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return fSize;
}
Void D2D1TextLayout::SetFontSize( Float fSize, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetFontSize( fSize, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

Bool D2D1TextLayout::HasUnderline( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    BOOL bHasUnderline;
    HRESULT hRes = m_pTextLayout->GetUnderline( iPosition, &bHasUnderline, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return ( bHasUnderline != FALSE );
}
Void D2D1TextLayout::SetUnderline( Bool bUnderline, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetUnderline( (bUnderline) ? TRUE : FALSE, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

Bool D2D1TextLayout::HasStrikethrough( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    BOOL bHasStrikethrough;
    HRESULT hRes = m_pTextLayout->GetStrikethrough( iPosition, &bHasStrikethrough, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    return ( bHasStrikethrough != FALSE );
}
Void D2D1TextLayout::SetStrikethrough( Bool bStrikethrough, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetStrikethrough( (bStrikethrough) ? TRUE : FALSE, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

Void D2D1TextLayout::GetForegroundBrush( D2D1SolidColorBrush * outBrush, UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( !(outBrush->IsCreated()) );

    IUnknown * pBrush = NULL;
    HRESULT hRes = m_pTextLayout->GetDrawingEffect( iPosition, &pBrush, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    if ( pBrush == NULL )
        return;

    // Retrieve brush
    ID2D1SolidColorBrush * pColorBrush = NULL;
    hRes = pBrush->QueryInterface( __uuidof(ID2D1SolidColorBrush), (Void**)&pColorBrush );
    DebugAssert( hRes == S_OK && pColorBrush != NULL );

    outBrush->m_hBrushProperties.opacity = pColorBrush->GetOpacity();
    pColorBrush->GetTransform( (D2D1_MATRIX_3X2_F*)&(outBrush->m_hBrushProperties.transform) );

    outBrush->m_pBrush = NULL;
    hRes = pColorBrush->QueryInterface( __uuidof(ID2D1Brush), (Void**)&(outBrush->m_pBrush) );
    DebugAssert( hRes == S_OK && outBrush->m_pBrush != NULL );

    D2D1_COLOR_F hColor = pColorBrush->GetColor();
    outBrush->m_hBrushColor.R = hColor.r;
    outBrush->m_hBrushColor.G = hColor.g;
    outBrush->m_hBrushColor.B = hColor.b;
    outBrush->m_hBrushColor.A = hColor.a;
    outBrush->m_pSolidColorBrush = pColorBrush;
}
Void D2D1TextLayout::SetForegroundBrush( D2D1SolidColorBrush * pBrush, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    HRESULT hRes = m_pTextLayout->SetDrawingEffect( pBrush->m_pSolidColorBrush, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

D2D1TextInlineObject * D2D1TextLayout::GetInlineObject( UInt iPosition, D2D1TextRange * outRange ) const
{
    DebugAssert( IsCreated() );

    IDWriteInlineObject * pInlineObject = NULL;
    HRESULT hRes = m_pTextLayout->GetInlineObject( iPosition, &pInlineObject, (DWRITE_TEXT_RANGE*)outRange );
    DebugAssert( hRes == S_OK );

    if ( pInlineObject == NULL )
        return NULL;

    // Retrieve inline object
    D2D1TextInlineObject::_DWriteInlineObject * pDelegate = (D2D1TextInlineObject::_DWriteInlineObject*)pInlineObject;
    return pDelegate->m_pParentHandler;
}
Void D2D1TextLayout::SetInlineObject( D2D1TextInlineObject * pInlineObject, const D2D1TextRange * pRange )
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pTextLayout->SetInlineObject( pInlineObject->m_pTextInlineObject, *( (const DWRITE_TEXT_RANGE *)pRange ) );
    DebugAssert( hRes == S_OK );
}

