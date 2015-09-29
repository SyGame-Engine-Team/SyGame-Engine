/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SkillBookView.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SkillBookView
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBOOKVIEW_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBOOKVIEW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Gameplay/Character.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HUD_SKILLBOOKVIEW_MAX_PAGESIZE 32
#define HUD_SKILLBOOKVIEW_MAX_PAGES    16

/////////////////////////////////////////////////////////////////////////////////
// The SkillBookViewModel class
class SkillBookViewModel : public GUIWidgetModel
{
public:
    SkillBookViewModel( BaseCharacter * pCharacter );
    virtual ~SkillBookViewModel();

    // Model
    inline UInt GetPageSize() const;
    inline UInt GetPageCount() const;
    inline UInt GetCurrentPage() const;

    inline UInt GetSkillCount( UInt iPage ) const;
    inline SkillInstance * GetSkill( UInt iPage, UInt iIndex ) const;

    Void UpdateSkillPages();

    // Controller
    Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStartDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );
	virtual Void OnStopDrag( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

protected:
    // Helper
    UInt _PickSlot( const Point2 & ptLocalPos ) const;

    BaseCharacter * m_pCharacter;

    typedef struct _skill_page {
        UInt iSkillCount;
        SkillInstance * arrSkills[HUD_SKILLBOOKVIEW_MAX_PAGESIZE];
    } _SkillPage;

    UInt m_iPageSize;
    UInt m_iPageCount;
    UInt m_iCurrentPage;

    _SkillPage m_arrSkillPages[HUD_SKILLBOOKVIEW_MAX_PAGES];
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillBookView class
class SkillBookView : public GUIWidget
{
public:
    SkillBookView( SkillBookViewModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~SkillBookView();

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SkillBookView.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_INTERFACE_SKILLBOOKVIEW_H

