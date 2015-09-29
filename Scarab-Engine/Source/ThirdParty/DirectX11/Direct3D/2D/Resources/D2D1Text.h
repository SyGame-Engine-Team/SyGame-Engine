/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Text.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1TEXT_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1TEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1RenderState.h"
#include "D2D1Brush.h"
#include "D2D1Geometry.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Standard Font families
#define D2D1_FONTFAMILY_NAME_MAX_LENGTH 64
#define D2D1_FONTFAMILY_ARIAL           TEXT("Arial")

// Prototypes
class D2D1RenderingContext;

class D2D1FontCollection;
class D2D1FontFamily;
class D2D1Font;
class D2D1FontList;
class D2D1FontFace;

class D2D1TextFormat;
class D2D1TextLayout;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1FontCollection class
class D2D1FontCollection
{
public:
    D2D1FontCollection();
    ~D2D1FontCollection();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create();
    // Void Create( ... ); // CreateCustomFontCollection
    Void Destroy();

    // Methods
    static UInt GetLocaleNameMaxLength();
    static Void GetLocaleName( GChar * outLocaleName );

    UInt GetFontFamilyCount() const;
    Void GetFontFamily( D2D1FontFamily * outFontFamily, UInt iIndex ) const;

    Bool FindFontFamily( UInt * outIndex, const GChar * strFontFamilyName ) const;

    Void EnumerateFontFamilies( GChar ** outFamilyNames, UInt * outNameLengths, UInt iMaxNames, const GChar * strLocaleName = NULL ) const;

private:
    friend class D2D1FontFamily;
    friend class D2D1FontList;

    friend class D2D1TextLayout;
    friend class D2D1TextFormat;

    IDWriteFontCollection * m_pFontCollection;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1FontFamily class
class D2D1FontFamily
{
public:
    D2D1FontFamily();
    ~D2D1FontFamily();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Destroy();

    // Methods
    Bool GetFamilyName( GChar * outFamilyName, UInt * pNameLength, const GChar * strLocaleName = NULL ) const;

    Void GetFontCollection( D2D1FontCollection * outFontCollection ) const;

    UInt GetFontCount() const;
    Void GetFont( D2D1Font * outFont, UInt iIndex ) const;

    Void MatchClosestFont( D2D1Font * outFont, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch ) const;
    Void MatchFonts( D2D1FontList * outFontList, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch ) const;

private:
    friend class D2D1FontCollection;
    friend class D2D1Font;

    IDWriteFontFamily * m_pFontFamily;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1Font class
class D2D1Font
{
public:
    D2D1Font();
    ~D2D1Font();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Destroy();

    // Methods
    Bool IsSymbolFont() const;
    Bool HasCharacter( UInt iCharacter ) const;

    Bool GetFaceName( GChar * outFaceName, UInt * pNameLength, const GChar * strLocaleName = NULL ) const;
    Bool GetInfoString( GChar * outInfoString, UInt * pStringLength, D2D1FontInfoStringID idInfoString, const GChar * strLocaleName = NULL ) const;

    D2D1FontWeight GetWeight() const;
    D2D1FontStyle GetStyle() const;
    D2D1FontStretch GetStretch() const;

    D2D1FontSimulation GetSimulation() const;
    Void GetMetrics( D2D1FontMetrics * outFontMetrics ) const;

    Void GetFontFamily( D2D1FontFamily * outFontFamily ) const;

    Void CreateFontFace( D2D1FontFace * outFontFace ) const;

private:
    friend class D2D1FontFamily;
    friend class D2D1FontList;

    IDWriteFont * m_pFont;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1FontList class
class D2D1FontList
{
public:
    D2D1FontList();
    ~D2D1FontList();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Destroy();

    // Methods
    Void GetFontCollection( D2D1FontCollection * outFontCollection ) const;

    UInt GetFontCount() const;
    Void GetFont( D2D1Font * outFont, UInt iIndex ) const;

private:
    friend class D2D1FontFamily;

    IDWriteFontList * m_pFontList;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1FontFace class
class D2D1FontFace
{
public:
    D2D1FontFace();
    ~D2D1FontFace();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Destroy();

    // Methods
    Bool IsSymbolFont() const;

    UInt GetIndex() const;
    D2D1FontFaceType GetType() const;

    D2D1FontSimulation GetSimulation() const;
    Void GetMetrics( D2D1FontMetrics * outFontMetrics ) const;
    Void GetGdiCompatibleMetrics( D2D1FontMetrics * outFontMetrics, Float fSizeDIP, Float fPixelsPerDIP, const D2D1Matrix32 * pTransform = NULL ) const;

    Void GetRecommendedRenderingMode( D2D1TextRenderingMode * outMode, Float fSizeDIP, Float fPixelsPerDIP, D2D1TextMeasuringMode iMeasuringMode, const D2D1TextRenderState * pTextRenderState ) const;

    // Glyphs access
    UInt GetGlyphCount() const;
    Void GetGlyphIndices( UShort * outGlyphIndices, const UInt * arrCodePointsUSC4, UInt iCodePointCount ) const;

    Void GetDesignGlyphMetrics( D2D1FontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways ) const;
    Void GetGdiCompatibleGlyphMetrics( D2D1FontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Float fSizeDIP, Float fPixelsPerDIP, Bool bUseGdiNatural, const D2D1Matrix32 * pTransform = NULL ) const;

    Void GetGlyphRunOutline( D2D1PathGeometry * outGeometry, Float fSizeDIP, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Bool bIsRightToLeft,
                                                             const Float * arrGlyphAdvances = NULL, const D2D1FontGlyphOffset * arrGlyphOffsets = NULL ) const;

    // Font files access
    //UInt GetFontFileCount() const;
    //Void GetFontFiles( D2D1FontFile ** outFontFiles, UInt iMaxFiles ) const;

private:
    friend class D2D1Font;

    IDWriteFontFace * m_pFontFace;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1TextFormat class
class D2D1TextFormat
{
public:
    D2D1TextFormat();
    ~D2D1TextFormat();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( const GChar * strLocaleName, const GChar * strFontFamilyName, D2D1FontWeight iWeight, D2D1FontStyle iStyle, D2D1FontStretch iStretch, Float fSize, D2D1FontCollection * pFontCollection = NULL );
    Void Destroy();

    // Methods
    Void GetFontCollection( D2D1FontCollection * outFontCollection ) const;

    UInt GetLocaleNameLength() const;
    Void GetLocaleName( GChar * outLocaleName, UInt iMaxLength ) const;
    UInt GetFontFamilyNameLength() const;
    Void GetFontFamilyName( GChar * outFontFamilyName, UInt iMaxLength ) const;

    D2D1FontWeight GetFontWeight() const;
    D2D1FontStyle GetFontStyle() const;
    D2D1FontStretch GetFontStretch() const;
    Float GetFontSize() const;

    D2D1TextFlowDirection GetFlowDirection() const;
    Void SetFlowDirection( D2D1TextFlowDirection iFlowDirection );

    D2D1TextReadingDirection GetReadingDirection() const;
    Void SetReadingDirection( D2D1TextReadingDirection iReadingDirection );

    D2D1TextLineSpacingMethod GetLineSpacing( Float * outLineSpacing = NULL, Float * outBaseline = NULL ) const;
    Void SetLineSpacing( D2D1TextLineSpacingMethod iLineSpacingMethod, Float fLineSpacing, Float fBaseline );

    D2D1TextAlignment GetTextAlignment() const;
    Void SetTextAlignment( D2D1TextAlignment iTextAlignment );

    D2D1TextParagraphAlignment GetParagraphAlignment() const;
    Void SetParagraphAlignment( D2D1TextParagraphAlignment iParagraphAlignment );

    Float GetIncrementalTabStop() const;
    Void SetIncrementalTabStop( Float fIncrementalTabStop );

    D2D1TextTrimmingGranularity GetTrimming( GChar * outDelimiter = NULL, UInt * outDelimiterCount = NULL ) const;
    Void SetTrimming( D2D1TextTrimmingGranularity iTrimmingGranularity, GChar chDelimiter, UInt iDelimiterCount );

    D2D1TextWordWrapping GetWordWrapping() const;
    Void SetWordWrapping( D2D1TextWordWrapping iWordWrapping );

private:
    friend class D2D1RenderingContext;
    friend class D2D1TextLayout;

    IDWriteTextFormat * m_pTextFormat;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1TextInlineObject class
class D2D1TextInlineObject
{
public:
    D2D1TextInlineObject( D2D1RenderingContext * pContext2D );
    virtual ~D2D1TextInlineObject();

    // Getters
    inline Void GetMetrics( D2D1TextInlineObjectMetrics * outMetrics ) const;
    inline Void SetMetrics( const D2D1TextInlineObjectMetrics * pMetrics );

    inline Void GetOverhangMetrics( D2D1TextOverhangMetrics * outOverhangMetrics ) const;
    inline Void SetOverhangMetrics( const D2D1TextOverhangMetrics * pOverhangMetrics );

    inline D2D1TextBreakCondition GetBreakConditionBefore() const;
    inline D2D1TextBreakCondition GetBreakConditionAfter() const;
    inline Void SetBreakConditionBefore( D2D1TextBreakCondition iBreakCondition );
    inline Void SetBreakConditionAfter( D2D1TextBreakCondition iBreakCondition );

    // Methods
    virtual Void Draw( const D2D1Point * pDstPoint, Bool bIsSideways, Bool bIsRightToleft ) const = 0;

protected:
    class _DWriteInlineObject;
    friend class _DWriteInlineObject;

    D2D1RenderingContext * m_pContext2D;

    // InlineObject data
    D2D1TextInlineObjectMetrics m_hInlineObjectMetrics;
    D2D1TextOverhangMetrics m_hOverhangMetrics;
    D2D1TextBreakCondition m_iBreakBefore, m_iBreakAfter;

    // Virtual delegate
    friend class D2D1TextLayout;
    _DWriteInlineObject * m_pTextInlineObject;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1TextLayout class
class D2D1TextLayout
{
public:
    D2D1TextLayout();
    ~D2D1TextLayout();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( D2D1TextFormat * pTextFormat, Float fMaxWidth, Float fMaxHeight, const GChar * strText, UInt iTextLength = INVALID_OFFSET );
    Void Destroy();

    // Methods
    Float GetMaxWidth() const;
    Void SetMaxWidth( Float fMaxWidth );

    Float GetMaxHeight() const;
    Void SetMaxHeight( Float fMaxHeight );

    Void GetMetrics( D2D1TextMetrics * outTextMetrics ) const;
    Void GetOverhangMetrics( D2D1TextOverhangMetrics * outOverhangMetrics ) const;

    UInt GetLineMetricCount() const;
    Void GetLineMetrics( D2D1TextLineMetrics * outLineMetrics, UInt iMaxLines ) const;

    UInt GetClusterMetricCount() const;
    Void GetClusterMetrics( D2D1TextClusterMetrics * outClusterMetrics, UInt iMaxClusters ) const;

        // Computes the minimal width of the layout box
    Float ComputeMinWidth() const;

        // Computes the string hit-region at a given pixel location in DIP, relative to layout box's top-left corner
    Void HitTestPoint( D2D1TextHitTestMetrics * outHitTestMetrics, Bool * outIsInside, Bool * outIsTrailingHit, Float fX, Float fY ) const;

        // Computes the hit-region and pixel location in DIP, given a character position and its logical side (leading or trailing)
    Void HitTestTextPosition( D2D1TextHitTestMetrics * outHitTestMetrics, Float * outX, Float * outY, UInt iPosition, Bool bIsTrailingHit ) const;

        // Computes a set of hit-regions for the given range of character positions
    UInt HitTestTextRangeCount( const D2D1TextRange * pRange, Float fOriginX = 0.0f, Float fOriginY = 0.0f ) const;
    Void HitTestTextRange( D2D1TextHitTestMetrics * outHitTestMetrics, UInt iMaxHitTests, const D2D1TextRange * pRange, Float fOriginX = 0.0f, Float fOriginY = 0.0f ) const;

    // TextRange-specific layout
    UInt GetLocaleNameLength( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void GetLocaleName( GChar * outLocaleName, UInt iNameSize, UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetLocaleName( const GChar * strLocaleName, const D2D1TextRange * pRange );

    Void GetFontCollection( D2D1FontCollection * outFontCollection, UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontCollection( D2D1FontCollection * pFontCollection, const D2D1TextRange * pRange );

    UInt GetFontFamilyNameLength( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void GetFontFamilyName( GChar * outFamilyName, UInt iNameSize, UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontFamilyName( const GChar * strFontFamilyName, const D2D1TextRange * pRange );

    D2D1FontWeight GetFontWeight( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontWeight( D2D1FontWeight iWeight, const D2D1TextRange * pRange );

    D2D1FontStyle GetFontStyle( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontStyle( D2D1FontStyle iStyle, const D2D1TextRange * pRange );

    D2D1FontStretch GetFontStretch( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontStretch( D2D1FontStretch iStretch, const D2D1TextRange * pRange );

    Float GetFontSize( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetFontSize( Float fSize, const D2D1TextRange * pRange );

    Bool HasUnderline( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetUnderline( Bool bUnderline, const D2D1TextRange * pRange );

    Bool HasStrikethrough( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetStrikethrough( Bool bStrikethrough, const D2D1TextRange * pRange );

    Void GetForegroundBrush( D2D1SolidColorBrush * outBrush, UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetForegroundBrush( D2D1SolidColorBrush * pBrush, const D2D1TextRange * pRange );

    D2D1TextInlineObject * GetInlineObject( UInt iPosition, D2D1TextRange * outRange = NULL ) const;
    Void SetInlineObject( D2D1TextInlineObject * pInlineObject, const D2D1TextRange * pRange );

//Void GetTypography( D2D1TextTypography * outTypography, UInt iPosition, D2D1TextRange * outRange = NULL ) const;
//Void SetTypography( D2D1TextTypography * pTypography, const D2D1TextRange * pRange );


private:
    friend class D2D1RenderingContext;

    IDWriteTextLayout * m_pTextLayout;
};

/////////////////////////////////////////////////////////////////////////////////
// The D2D1GlyphRun class
//class D2D1GlyphRun
//{
//public:
//    D2D1GlyphRun( D2D1RenderingContext * pContext2D,  );
//    virtual ~D2D1GlyphRun();
//
//    // Getters
//
//protected:
//    friend class D2D1RenderingContext;
//    D2D1RenderingContext * m_pContext2D;
//
////    struct DWRITE_GLYPH_RUN {
////  IDWriteFontFace           *fontFace;
////  FLOAT                     fontEmSize;
////  UINT32                    glyphCount;
////  const UINT16              *glyphIndices;
////  const FLOAT               *glyphAdvances;
////  const DWRITE_GLYPH_OFFSET *glyphOffsets;
////  BOOL                      isSideways;
////  UINT32                    bidiLevel;
////};
//
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1Text.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1TEXT_H

