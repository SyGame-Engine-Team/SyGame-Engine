/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DText.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Fonts & Texts
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DTEXT_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DTEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DRenderState.h"
#include "GPU2DGeometry.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Standard Font families
#define GPU2DFONTFAMILY_NAME_MAX_LENGTH DEVICE2DFONT_FAMILY_NAME_MAX_LENGTH
#define GPU2DFONTFAMILY_ARIAL           DEVICE2DFONT_FAMILY_ARIAL

// GPU2DFont definitions
enum GPU2DFontWeight {
    GPU2DFONT_WEIGHT_THIN        = DEVICE2DFONT_WEIGHT_THIN,
    GPU2DFONT_WEIGHT_EXTRALIGHT  = DEVICE2DFONT_WEIGHT_EXTRALIGHT,
    GPU2DFONT_WEIGHT_LIGHT       = DEVICE2DFONT_WEIGHT_LIGHT,
    GPU2DFONT_WEIGHT_NORMAL      = DEVICE2DFONT_WEIGHT_NORMAL,
    GPU2DFONT_WEIGHT_MEDIUM      = DEVICE2DFONT_WEIGHT_MEDIUM,
    GPU2DFONT_WEIGHT_SEMIBOLD    = DEVICE2DFONT_WEIGHT_SEMIBOLD,
    GPU2DFONT_WEIGHT_BOLD        = DEVICE2DFONT_WEIGHT_BOLD,
    GPU2DFONT_WEIGHT_EXTRABOLD   = DEVICE2DFONT_WEIGHT_EXTRABOLD,
    GPU2DFONT_WEIGHT_BLACK       = DEVICE2DFONT_WEIGHT_BLACK,
    GPU2DFONT_WEIGHT_EXTRA_BLACK = DEVICE2DFONT_WEIGHT_EXTRA_BLACK
};

enum GPU2DFontStyle {
    GPU2DFONT_STYLE_NORMAL  = DEVICE2DFONT_STYLE_NORMAL,
    GPU2DFONT_STYLE_OBLIQUE = DEVICE2DFONT_STYLE_OBLIQUE,
    GPU2DFONT_STYLE_ITALIC  = DEVICE2DFONT_STYLE_ITALIC
};

enum GPU2DFontStretch {
    GPU2DFONT_STRETCH_UNDEFINED      = DEVICE2DFONT_STRETCH_UNDEFINED,
    GPU2DFONT_STRETCH_ULTRACONDENSED = DEVICE2DFONT_STRETCH_ULTRACONDENSED,
    GPU2DFONT_STRETCH_EXTRACONDENSED = DEVICE2DFONT_STRETCH_EXTRACONDENSED,
    GPU2DFONT_STRETCH_CONDENSED      = DEVICE2DFONT_STRETCH_CONDENSED,
    GPU2DFONT_STRETCH_SEMICONDENSED  = DEVICE2DFONT_STRETCH_SEMICONDENSED,
    GPU2DFONT_STRETCH_NORMAL         = DEVICE2DFONT_STRETCH_NORMAL,
    GPU2DFONT_STRETCH_SEMIEXPANDED   = DEVICE2DFONT_STRETCH_SEMIEXPANDED,
    GPU2DFONT_STRETCH_EXPANDED       = DEVICE2DFONT_STRETCH_EXPANDED,
    GPU2DFONT_STRETCH_EXTRAEXPANDED  = DEVICE2DFONT_STRETCH_EXTRAEXPANDED,
    GPU2DFONT_STRETCH_ULTRAEXPANDED  = DEVICE2DFONT_STRETCH_ULTRAEXPANDED
};

enum GPU2DFontInfoStringID {
    GPU2DFONT_INFOSTRING_NONE                 = DEVICE2DFONT_INFOSTRING_NONE,
    GPU2DFONT_INFOSTRING_COPYRIGHT            = DEVICE2DFONT_INFOSTRING_COPYRIGHT,
    GPU2DFONT_INFOSTRING_VERSION              = DEVICE2DFONT_INFOSTRING_VERSION,
    GPU2DFONT_INFOSTRING_TRADEMARK            = DEVICE2DFONT_INFOSTRING_TRADEMARK,
    GPU2DFONT_INFOSTRING_MANUFACTURER         = DEVICE2DFONT_INFOSTRING_MANUFACTURER,
    GPU2DFONT_INFOSTRING_DESIGNER             = DEVICE2DFONT_INFOSTRING_DESIGNER,
    GPU2DFONT_INFOSTRING_DESIGNER_URL         = DEVICE2DFONT_INFOSTRING_DESIGNER_URL,
    GPU2DFONT_INFOSTRING_DESCRIPTION          = DEVICE2DFONT_INFOSTRING_DESCRIPTION,
    GPU2DFONT_INFOSTRING_VENDOR_URL           = DEVICE2DFONT_INFOSTRING_VENDOR_URL,
    GPU2DFONT_INFOSTRING_LICENSE_DESCRIPTION  = DEVICE2DFONT_INFOSTRING_LICENSE_DESCRIPTION,
    GPU2DFONT_INFOSTRING_LICENSE_URL          = DEVICE2DFONT_INFOSTRING_LICENSE_URL,
    GPU2DFONT_INFOSTRING_WIN32_FAMILYNAMES    = DEVICE2DFONT_INFOSTRING_WIN32_FAMILYNAMES,
    GPU2DFONT_INFOSTRING_WIN32_SUBFAMILYNAMES = DEVICE2DFONT_INFOSTRING_WIN32_SUBFAMILYNAMES,
    GPU2DFONT_INFOSTRING_PREF_FAMILYNAMES     = DEVICE2DFONT_INFOSTRING_PREF_FAMILYNAMES,
    GPU2DFONT_INFOSTRING_PREF_SUBFAMILYNAMES  = DEVICE2DFONT_INFOSTRING_PREF_SUBFAMILYNAMES,
    GPU2DFONT_INFOSTRING_SAMPLE_TEXT          = DEVICE2DFONT_INFOSTRING_SAMPLE_TEXT
};

enum GPU2DFontSimulation {
    GPU2DFONT_SIMULATION_NONE    = DEVICE2DFONT_SIMULATION_NONE,
    GPU2DFONT_SIMULATION_BOLD    = DEVICE2DFONT_SIMULATION_BOLD,
    GPU2DFONT_SIMULATION_OBLIQUE = DEVICE2DFONT_SIMULATION_OBLIQUE
};

enum GPU2DFontFaceType {
    GPU2DFONT_FACE_CFF                 = DEVICE2DFONT_FACE_CFF,
    GPU2DFONT_FACE_TRUETYPE            = DEVICE2DFONT_FACE_TRUETYPE,
    GPU2DFONT_FACE_TRUETYPE_COLLECTION = DEVICE2DFONT_FACE_TRUETYPE_COLLECTION,
    GPU2DFONT_FACE_TYPE1               = DEVICE2DFONT_FACE_TYPE1,
    GPU2DFONT_FACE_VECTOR              = DEVICE2DFONT_FACE_VECTOR,
    GPU2DFONT_FACE_BITMAP              = DEVICE2DFONT_FACE_BITMAP,
    GPU2DFONT_FACE_UNKNOWN             = DEVICE2DFONT_FACE_UNKNOWN
};

typedef struct _gpu2d_font_metrics {
    UShort iDesignUnitsPerEM;
    UShort iAscent;
    UShort iDescent;
    UShort iLineGap;
    UShort iCapHeight;
    UShort iXHeight;
    UShort iUnderlinePosition;
    UShort iUnderlineThickness;
    UShort iStrikethroughPosition;
    UShort iStrikethroughThickness;
} GPU2DFontMetrics;

typedef struct _gpu2d_font_glyph_metrics {
    Int iVerticalOriginY;
    Int iLeftSideBearing;
    Int iTopSideBearing;
    Int iRightSideBearing;
    Int iBottomSideBearing;
    UInt iAdvanceWidth;
    UInt iAdvanceHeight;
} GPU2DFontGlyphMetrics;

typedef struct _gpu2d_font_glyph_offset {
    Float fAdvanceOffset;
    Float fAscenderOffset;
} GPU2DFontGlyphOffset;

// GPU2DText definitions
enum GPU2DTextMeasuringMode {
    GPU2DTEXT_MEASURING_NATURAL     = DEVICE2DTEXT_MEASURING_NATURAL,
    GPU2DTEXT_MEASURING_GDI_CLASSIC = DEVICE2DTEXT_MEASURING_GDI_CLASSIC,
    GPU2DTEXT_MEASURING_GDI_NATURAL = DEVICE2DTEXT_MEASURING_GDI_NATURAL
};

enum GPU2DTextFlowDirection {
    GPU2DTEXT_FLOWDIRECTION_TOP_BOTTOM = DEVICE2DTEXT_FLOWDIRECTION_TOP_BOTTOM
};

enum GPU2DTextReadingDirection {
    GPU2DTEXT_READINGDIRECTION_LEFT_RIGHT = DEVICE2DTEXT_READINGDIRECTION_LEFT_RIGHT,
    GPU2DTEXT_READINGDIRECTION_RIGHT_LEFT = DEVICE2DTEXT_READINGDIRECTION_RIGHT_LEFT
};

enum GPU2DTextLineSpacingMethod {
    GPU2DTEXT_LINESPACING_DEFAULT = DEVICE2DTEXT_LINESPACING_DEFAULT,
    GPU2DTEXT_LINESPACING_UNIFORM = DEVICE2DTEXT_LINESPACING_UNIFORM
};

enum GPU2DTextAlignment {
    GPU2DTEXT_ALIGNMENT_LEADING  = DEVICE2DTEXT_ALIGNMENT_LEADING,
    GPU2DTEXT_ALIGNMENT_TRAILING = DEVICE2DTEXT_ALIGNMENT_TRAILING,
    GPU2DTEXT_ALIGNMENT_CENTER   = DEVICE2DTEXT_ALIGNMENT_CENTER
};

enum GPU2DTextParagraphAlignment {
    GPU2DTEXT_PARAGRAPHALIGNMENT_NEAR   = DEVICE2DTEXT_PARAGRAPHALIGNMENT_NEAR,
    GPU2DTEXT_PARAGRAPHALIGNMENT_FAR    = DEVICE2DTEXT_PARAGRAPHALIGNMENT_FAR,
    GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER = DEVICE2DTEXT_PARAGRAPHALIGNMENT_CENTER
};

enum GPU2DTextTrimmingGranularity {
    GPU2DTEXT_TRIMMINGGRANULARITY_NONE      = DEVICE2DTEXT_TRIMMINGGRANULARITY_NONE,
    GPU2DTEXT_TRIMMINGGRANULARITY_CHARACTER = DEVICE2DTEXT_TRIMMINGGRANULARITY_CHARACTER,
    GPU2DTEXT_TRIMMINGGRANULARITY_WORD      = DEVICE2DTEXT_TRIMMINGGRANULARITY_WORD
};

enum GPU2DTextWordWrapping {
    GPU2DTEXT_WORDWRAPPING_NONE = DEVICE2DTEXT_WORDWRAPPING_NONE,
    GPU2DTEXT_WORDWRAPPING_WRAP = DEVICE2DTEXT_WORDWRAPPING_WRAP
};

enum GPU2DTextBreakCondition {
    GPU2DTEXT_BREAKCONDITION_NEUTRAL       = DEVICE2DTEXT_BREAKCONDITION_NEUTRAL,
    GPU2DTEXT_BREAKCONDITION_CAN_BREAK     = DEVICE2DTEXT_BREAKCONDITION_CAN_BREAK,
    GPU2DTEXT_BREAKCONDITION_MAY_NOT_BREAK = DEVICE2DTEXT_BREAKCONDITION_MAY_NOT_BREAK,
    GPU2DTEXT_BREAKCONDITION_MUST_BREAK    = DEVICE2DTEXT_BREAKCONDITION_MUST_BREAK
};

typedef struct _gpu2d_text_range {
    UInt iPosition;
    UInt iLength;
} GPU2DTextRange;

typedef struct _gpu2d_text_metrics {
    Float fLeft;
    Float fTop;
    Float fWidth;
    Float fWidthIncludingTrailingWhitespaces;
    Float fHeight;
    Float fLayoutWidth;
    Float fLayoutHeight;
    UInt iMaxBidiReorderingDepth;
    UInt iLineCount;
} GPU2DTextMetrics;

typedef struct _gpu2d_text_overhang_metrics {
    Float fLeft;
    Float fTop;
    Float fRight;
    Float fBottom;
} GPU2DTextOverhangMetrics;

typedef struct _gpu2d_text_line_metrics {
    UInt iLength;
    UInt iTrailingWhitespaceLength;
    UInt iNewlineLength;
    Float fHeight;
    Float fBaseline;
    Int bIsTrimmed; // Bool
} GPU2DTextLineMetrics;

typedef struct _gpu2d_text_cluster_metrics {
    Float fWidth;
    UShort iLength;
    UShort bCanWrapLineAfter :1;
    UShort bIsWhitespace     :1;
    UShort bIsNewline        :1;
    UShort bIsSoftHyphen     :1;
    UShort bIsRightToLeft    :1;
    UShort _Padding          :11;    
} GPU2DTextClusterMetrics;

typedef struct _gpu2d_text_hittest_metrics {
    UInt iPosition; // First character position within the hit region
    UInt iLength;   // number of character positions within the hit region
    Float fLeft;   //
    Float fTop;    // Hit region
    Float fWidth;  // rectangle
    Float fHeight; //
    UInt iBidiLevel; // Unicode - BIDI algorithm for display ordering of bi-directional texts
                     // Gives the position of the character relative to display order rather than logical order
    Int bContainsText; // Bool - Hit region contains text
    Int bIsTrimmed;    // Bool - Text range is trimmed
} GPU2DTextHitTestMetrics;

typedef struct _gpu2d_text_inlineobject_metrics {
    Float fWidth;
    Float fHeight;
    Float fBaseline;
    Bool bSupportSideways;
} GPU2DTextInlineObjectMetrics;

// Prototypes
class GPU2DContext;

class GPU2DFontCollection;
class GPU2DFontFamily;
class GPU2DFont;
class GPU2DFontList;
class GPU2DFontFace;

class GPU2DTextFormat;
class GPU2DTextLayout;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DFontCollection class
class GPU2DFontCollection
{
public:
    GPU2DFontCollection();
    ~GPU2DFontCollection();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind();
    inline Void UnBind();

    // Methods
    inline static UInt GetLocaleNameMaxLength();
    inline static Void GetLocaleName( GChar * outLocaleName );

    inline UInt GetFontFamilyCount() const;
    inline Void GetFontFamily( GPU2DFontFamily * outFontFamily, UInt iIndex ) const;

    inline Bool FindFontFamily( UInt * outIndex, const GChar * strFontFamilyName ) const;

    inline Void EnumerateFontFamilies( GChar ** outFamilyNames, UInt * outNameLengths, UInt iMaxNames, const GChar * strLocaleName = NULL ) const;

private:
    friend class GPU2DFontFamily;
    friend class GPU2DFontList;
    friend class GPU2DTextFormat;
    friend class GPU2DTextLayout;

    // Device data
    Device2DFontCollection m_hDevice2DFontCollection;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DFontFamily class
class GPU2DFontFamily
{
public:
    GPU2DFontFamily();
    ~GPU2DFontFamily();

    // Binding
    inline Bool IsBound() const;

    inline Void UnBind();

    // Methods
    inline Bool GetFamilyName( GChar * outFamilyName, UInt * pNameLength, const GChar * strLocaleName = NULL ) const;

    inline Void GetFontCollection( GPU2DFontCollection * outFontCollection ) const;

    inline UInt GetFontCount() const;
    inline Void GetFont( GPU2DFont * outFont, UInt iIndex ) const;

    inline Void MatchClosestFont( GPU2DFont * outFont, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch ) const;
    inline Void MatchFonts( GPU2DFontList * outFontList, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch ) const;

private:
    friend class GPU2DFontCollection;
    friend class GPU2DFont;

    // Device data
    Device2DFontFamily m_hDevice2DFontFamily;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DFont class
class GPU2DFont
{
public:
    GPU2DFont();
    ~GPU2DFont();

    // Binding
    inline Bool IsBound() const;

    inline Void UnBind();

    // Methods
    inline Bool IsSymbolFont() const;
    inline Bool HasCharacter( UInt iCharacter ) const;

    inline Bool GetFaceName( GChar * outFaceName, UInt * pNameLength, const GChar * strLocaleName = NULL ) const;
    inline Bool GetInfoString( GChar * outInfoString, UInt * pStringLength, GPU2DFontInfoStringID idInfoString, const GChar * strLocaleName = NULL ) const;

    inline GPU2DFontWeight GetWeight() const;
    inline GPU2DFontStyle GetStyle() const;
    inline GPU2DFontStretch GetStretch() const;

    inline GPU2DFontSimulation GetSimulation() const;
    inline Void GetMetrics( GPU2DFontMetrics * outFontMetrics ) const;

    inline Void GetFontFamily( GPU2DFontFamily * outFontFamily ) const;

    inline Void CreateFontFace( GPU2DFontFace * outFontFace ) const;

private:
    friend class GPU2DFontFamily;
    friend class GPU2DFontList;

    // Device data
    Device2DFont m_hDevice2DFont;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DFontList class
class GPU2DFontList
{
public:
    GPU2DFontList();
    ~GPU2DFontList();

    // Binding
    inline Bool IsBound() const;

    inline Void UnBind();

    // Methods
    inline Void GetFontCollection( GPU2DFontCollection * outFontCollection ) const;

    inline UInt GetFontCount() const;
    inline Void GetFont( GPU2DFont * outFont, UInt iIndex ) const;

private:
    friend class GPU2DFontFamily;

    // Device data
    Device2DFontList m_hDevice2DFontList;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DFontFace class
class GPU2DFontFace
{
public:
    GPU2DFontFace();
    ~GPU2DFontFace();

    // Binding
    inline Bool IsBound() const;

    inline Void UnBind();

    // Methods
    inline Bool IsSymbolFont() const;

    inline UInt GetIndex() const;
    inline GPU2DFontFaceType GetType() const;

    inline GPU2DFontSimulation GetSimulation() const;
    inline Void GetMetrics( GPU2DFontMetrics * outFontMetrics ) const;
    inline Void GetGdiCompatibleMetrics( GPU2DFontMetrics * outFontMetrics, Float fSizeDIP, Float fPixelsPerDIP, const GPU2DMatrix32 * pTransform = NULL ) const;

    inline Void GetRecommendedRenderingMode( GPU2DTextRenderingMode * outMode, Float fSizeDIP, Float fPixelsPerDIP, GPU2DTextMeasuringMode iMeasuringMode, const GPU2DTextRenderState * pTextRenderState ) const;

    // Glyphs access
    inline UInt GetGlyphCount() const;
    inline Void GetGlyphIndices( UShort * outGlyphIndices, const UInt * arrCodePointsUSC4, UInt iCodePointCount ) const;

    inline Void GetDesignGlyphMetrics( GPU2DFontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways ) const;
    inline Void GetGdiCompatibleGlyphMetrics( GPU2DFontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways,
                                                                                       Float fSizeDIP, Float fPixelsPerDIP, Bool bUseGdiNatural, const GPU2DMatrix32 * pTransform = NULL ) const;

    inline Void GetGlyphRunOutline( GPU2DPathGeometry * outGeometry, Float fSizeDIP, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Bool bIsRightToLeft,
                                                                     const Float * arrGlyphAdvances = NULL, const GPU2DFontGlyphOffset * arrGlyphOffsets = NULL ) const;

private:
    friend class GPU2DFont;

    // Device data
    Device2DFontFace m_hDevice2DFontFace;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DTextFormat class
class GPU2DTextFormat
{
public:
    GPU2DTextFormat();
    ~GPU2DTextFormat();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind( const GChar * strLocaleName, const GChar * strFontFamilyName, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch, Float fSize, GPU2DFontCollection * pFontCollection = NULL );
    inline Void UnBind();

    // Methods
    inline Void GetFontCollection( GPU2DFontCollection * outFontCollection ) const;

    inline UInt GetLocaleNameLength() const;
    inline Void GetLocaleName( GChar * outLocaleName, UInt iMaxLength ) const;
    inline UInt GetFontFamilyNameLength() const;
    inline Void GetFontFamilyName( GChar * outFontFamilyName, UInt iMaxLength ) const;

    inline GPU2DFontWeight GetFontWeight() const;
    inline GPU2DFontStyle GetFontStyle() const;
    inline GPU2DFontStretch GetFontStretch() const;
    inline Float GetFontSize() const;

    Void GetFontFamily( GPU2DFontFamily * outFontFamily ) const;
    Void GetFont( GPU2DFont * outFont ) const;

    Float GetLineHeight( Bool bIncludeLineGap ) const;

    inline GPU2DTextFlowDirection GetFlowDirection() const;
    inline Void SetFlowDirection( GPU2DTextFlowDirection iFlowDirection );

    inline GPU2DTextReadingDirection GetReadingDirection() const;
    inline Void SetReadingDirection( GPU2DTextReadingDirection iReadingDirection );

    inline GPU2DTextLineSpacingMethod GetLineSpacing( Float * outLineSpacing = NULL, Float * outBaseline = NULL ) const;
    inline Void SetLineSpacing( GPU2DTextLineSpacingMethod iLineSpacingMethod, Float fLineSpacing, Float fBaseline );

    inline GPU2DTextAlignment GetTextAlignment() const;
    inline Void SetTextAlignment( GPU2DTextAlignment iTextAlignment );

    inline GPU2DTextParagraphAlignment GetParagraphAlignment() const;
    inline Void SetParagraphAlignment( GPU2DTextParagraphAlignment iParagraphAlignment );

    inline Float GetIncrementalTabStop() const;
    inline Void SetIncrementalTabStop( Float fIncrementalTabStop );

    inline GPU2DTextTrimmingGranularity GetTrimming( GChar * outDelimiter = NULL, UInt * outDelimiterCount = NULL ) const;
    inline Void SetTrimming( GPU2DTextTrimmingGranularity iTrimmingGranularity, GChar chDelimiter, UInt iDelimiterCount );

    inline GPU2DTextWordWrapping GetWordWrapping() const;
    inline Void SetWordWrapping( GPU2DTextWordWrapping iWordWrapping );

private:
    friend class GPU2DContext;
    friend class GPU2DTextLayout;

    // Device data
    Device2DTextFormat m_hDevice2DTextFormat;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DTextInlineObject class
class GPU2DTextInlineObject
{
public:
    GPU2DTextInlineObject( GPU2DContext * pContext2D );
    virtual ~GPU2DTextInlineObject();

    // Getters
    inline Void GetMetrics( GPU2DTextInlineObjectMetrics * outMetrics ) const;
    inline Void GetOverhangMetrics( GPU2DTextOverhangMetrics * outOverhangMetrics ) const;
    inline GPU2DTextBreakCondition GetBreakConditionBefore() const;
    inline GPU2DTextBreakCondition GetBreakConditionAfter() const;

    // Methods
    virtual Void Draw( const GPU2DPoint * pDstPoint, Bool bIsSideways, Bool bIsRightToleft ) const = 0;

protected:
    // Methods
    inline Void SetMetrics( const GPU2DTextInlineObjectMetrics * pMetrics );
    inline Void SetOverhangMetrics( const GPU2DTextOverhangMetrics * pOverhangMetrics );
    inline Void SetBreakConditionBefore( GPU2DTextBreakCondition iBreakCondition );
    inline Void SetBreakConditionAfter( GPU2DTextBreakCondition iBreakCondition );

    GPU2DContext * m_pContext2D;

private:
    class _TextInlineObject : public Device2DTextInlineObject {
        public:
            _TextInlineObject( Device2DContext * pContext2D, GPU2DTextInlineObject * pParentHandler );
            virtual ~_TextInlineObject();

            inline virtual Void Draw( const Device2DPoint * pDstPoint, Bool bIsSideways, Bool bIsRightToleft ) const;

        protected:
            friend class GPU2DTextLayout;
            GPU2DTextInlineObject * m_pParentHandler;
    };

    friend class _TextInlineObject;
    friend class GPU2DTextLayout;

    // Device data
    _TextInlineObject m_hTextInlineObject;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DTextLayout class
class GPU2DTextLayout
{
public:
    GPU2DTextLayout();
    ~GPU2DTextLayout();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind( GPU2DTextFormat * pTextFormat, Float fMaxWidth, Float fMaxHeight, const GChar * strText, UInt iTextLength = INVALID_OFFSET );
    inline Void UnBind();

    // Methods
    inline Float GetMaxWidth() const;
    inline Void SetMaxWidth( Float fMaxWidth );

    inline Float GetMaxHeight() const;
    inline Void SetMaxHeight( Float fMaxHeight );

    inline Void GetMetrics( GPU2DTextMetrics * outTextMetrics ) const;
    inline Void GetOverhangMetrics( GPU2DTextOverhangMetrics * outOverhangMetrics ) const;

    inline UInt GetLineMetricCount() const;
    inline Void GetLineMetrics( GPU2DTextLineMetrics * outLineMetrics, UInt iMaxLines ) const;

    inline UInt GetClusterMetricCount() const;
    inline Void GetClusterMetrics( GPU2DTextClusterMetrics * outClusterMetrics, UInt iMaxClusters ) const;

        // Computes the minimal width of the layout box
    inline Float ComputeMinWidth() const;

        // Computes the string hit-region at a given pixel location in DIP, relative to layout box's top-left corner
    inline Void HitTestPoint( GPU2DTextHitTestMetrics * outHitTestMetrics, Bool * outIsInside, Bool * outIsTrailingHit, Float fX, Float fY ) const;

        // Computes the hit-region and pixel location in DIP, given a character position and its logical side (leading or trailing)
    inline Void HitTestTextPosition( GPU2DTextHitTestMetrics * outHitTestMetrics, Float * outX, Float * outY, UInt iPosition, Bool bIsTrailingHit ) const;

        // Computes a set of hit-regions for the given range of character positions
    inline UInt HitTestTextRangeCount( const GPU2DTextRange * pRange, Float fOriginX = 0.0f, Float fOriginY = 0.0f ) const;
    inline Void HitTestTextRange( GPU2DTextHitTestMetrics * outHitTestMetrics, UInt iMaxHitTests, const GPU2DTextRange * pRange, Float fOriginX = 0.0f, Float fOriginY = 0.0f ) const;

    // TextRange-specific layout
    inline UInt GetLocaleNameLength( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void GetLocaleName( GChar * outLocaleName, UInt iNameSize, UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetLocaleName( const GChar * strLocaleName, const GPU2DTextRange * pRange );

    inline Void GetFontCollection( GPU2DFontCollection * outFontCollection, UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontCollection( GPU2DFontCollection * pFontCollection, const GPU2DTextRange * pRange );

    inline UInt GetFontFamilyNameLength( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void GetFontFamilyName( GChar * outFamilyName, UInt iNameSize, UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontFamilyName( const GChar * strFontFamilyName, const GPU2DTextRange * pRange );

    inline GPU2DFontWeight GetFontWeight( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontWeight( GPU2DFontWeight iWeight, const GPU2DTextRange * pRange );

    inline GPU2DFontStyle GetFontStyle( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontStyle( GPU2DFontStyle iStyle, const GPU2DTextRange * pRange );

    inline GPU2DFontStretch GetFontStretch( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontStretch( GPU2DFontStretch iStretch, const GPU2DTextRange * pRange );

    inline Float GetFontSize( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetFontSize( Float fSize, const GPU2DTextRange * pRange );

    inline Bool HasUnderline( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetUnderline( Bool bUnderline, const GPU2DTextRange * pRange );

    inline Bool HasStrikethrough( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetStrikethrough( Bool bStrikethrough, const GPU2DTextRange * pRange );

    inline Void GetForegroundBrush( GPU2DSolidColorBrush * outBrush, UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetForegroundBrush( GPU2DSolidColorBrush * pBrush, const GPU2DTextRange * pRange );

    inline GPU2DTextInlineObject * GetInlineObject( UInt iPosition, GPU2DTextRange * outRange = NULL ) const;
    inline Void SetInlineObject( GPU2DTextInlineObject * pInlineObject, const GPU2DTextRange * pRange );

private:
    friend class GPU2DContext;

    // Device data
    Device2DTextLayout m_hDevice2DTextLayout;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DText.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DTEXT_H
