/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DText.cpp
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
// Includes
#include "GPU2DText.h"

#include "../GPU2DContext.h"

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontCollection implementation
GPU2DFontCollection::GPU2DFontCollection():
    m_hDevice2DFontCollection()
{
    // nothing to do
}
GPU2DFontCollection::~GPU2DFontCollection()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontFamily implementation
GPU2DFontFamily::GPU2DFontFamily():
    m_hDevice2DFontFamily()
{
    // nothing to do
}
GPU2DFontFamily::~GPU2DFontFamily()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFont implementation
GPU2DFont::GPU2DFont():
    m_hDevice2DFont()
{
    // nothing to do
}
GPU2DFont::~GPU2DFont()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontList implementation
GPU2DFontList::GPU2DFontList():
    m_hDevice2DFontList()
{
    // nothing to do
}
GPU2DFontList::~GPU2DFontList()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontFace implementation
GPU2DFontFace::GPU2DFontFace():
    m_hDevice2DFontFace()
{
    // nothing to do
}
GPU2DFontFace::~GPU2DFontFace()
{
    if ( IsBound() )
        UnBind();
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextFormat implementation
GPU2DTextFormat::GPU2DTextFormat():
    m_hDevice2DTextFormat()
{
    // nothing to do
}
GPU2DTextFormat::~GPU2DTextFormat()
{
    if ( IsBound() )
        UnBind();
}

Void GPU2DTextFormat::GetFontFamily( GPU2DFontFamily * outFontFamily ) const
{
    Assert( IsBound() );

    GChar strFontFamilyName[GPU2DFONTFAMILY_NAME_MAX_LENGTH + 1];
    GetFontFamilyName( strFontFamilyName, GPU2DFONTFAMILY_NAME_MAX_LENGTH );

    GPU2DFontCollection hFontCollection;
    GetFontCollection( &hFontCollection );

    UInt iFontFamilyIndex = INVALID_OFFSET;
    Bool bFound = hFontCollection.FindFontFamily( &iFontFamilyIndex, strFontFamilyName );
    Assert( bFound );

    hFontCollection.GetFontFamily( outFontFamily, iFontFamilyIndex );
}
Void GPU2DTextFormat::GetFont( GPU2DFont * outFont ) const
{
    Assert( IsBound() );

    GPU2DFontFamily hFontFamily;
    GetFontFamily( &hFontFamily );

    hFontFamily.MatchClosestFont( outFont, GetFontWeight(), GetFontStyle(), GetFontStretch() );
}

Float GPU2DTextFormat::GetLineHeight( Bool bIncludeLineGap ) const
{
    Assert( IsBound() );

    GPU2DFont hFont;
    GetFont( &hFont );

    GPU2DFontMetrics hFontMetrics;
    hFont.GetMetrics( &hFontMetrics );

    Scalar fA = (Scalar)( hFontMetrics.iAscent + hFontMetrics.iDescent + (bIncludeLineGap ? hFontMetrics.iLineGap : 0) );
    Scalar fB = ( GetFontSize() / (Scalar)(hFontMetrics.iDesignUnitsPerEM) );
    return ( fA * fB );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextInlineObject implementation
GPU2DTextInlineObject::GPU2DTextInlineObject( GPU2DContext * pContext2D ):
    m_hTextInlineObject( &(pContext2D->m_hDevice2DContext), this )
{
    m_pContext2D = pContext2D;
}
GPU2DTextInlineObject::~GPU2DTextInlineObject()
{
    // nothing to do
}

GPU2DTextInlineObject::_TextInlineObject::_TextInlineObject( Device2DContext * pContext2D, GPU2DTextInlineObject * pParentHandler ):
    Device2DTextInlineObject( pContext2D )
{
    m_pParentHandler = pParentHandler;
}
GPU2DTextInlineObject::_TextInlineObject::~_TextInlineObject()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextLayout implementation
GPU2DTextLayout::GPU2DTextLayout():
    m_hDevice2DTextLayout()
{
    // nothing to do
}
GPU2DTextLayout::~GPU2DTextLayout()
{
    if ( IsBound() )
        UnBind();
}


