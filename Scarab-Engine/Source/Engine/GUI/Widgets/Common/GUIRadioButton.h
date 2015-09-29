/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIRadioButton.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Radio buttons
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
#ifndef SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIRADIOBUTTON_H
#define SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIRADIOBUTTON_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"

#include "../../GUIWidget.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class GUIRadioButtonModel;

/////////////////////////////////////////////////////////////////////////////////
// The GUIRadioButtonGroup class
class GUIRadioButtonGroup
{
public:
	GUIRadioButtonGroup();
	~GUIRadioButtonGroup();

    // Getters
    inline UInt GetModelCount() const;
    inline GUIRadioButtonModel * GetModel( UInt iModel ) const;

    inline Bool IsTicked( UInt iModel ) const;
    inline UInt GetTickedIndex() const;
    inline GUIRadioButtonModel * GetTickedModel() const;

private:
    friend class GUIRadioButtonModel;

	UInt m_iTicked;
	Array<GUIRadioButtonModel*> m_arrModels;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIRadioButtonModel class
class GUIRadioButtonModel : public GUIWidgetModel
{
public:
    GUIRadioButtonModel( GUIRadioButtonGroup * pGroup );
    virtual ~GUIRadioButtonModel();

    // Model
    inline GUIRadioButtonGroup * GetGroup() const;

    inline Bool IsCleared() const;
    inline Bool IsTicked() const;

	Void SetTicked();

    // Controller
    virtual Void OnClick( const Point2 & ptLocalPos, KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnStateChange();

protected:
    Bool m_bTicked;
    UInt m_iGroupIndex;
    GUIRadioButtonGroup * m_pGroup;
};

/////////////////////////////////////////////////////////////////////////////////
// The GUIRadioButton class
class GUIRadioButton : public GUIWidget
{
public:
    GUIRadioButton( GUIRadioButtonModel * pModel, GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName );
    virtual ~GUIRadioButton();

    // Type
	inline virtual GUIWidgetType GetType() const;

protected:
    // Drawing interface
    virtual Void _Draw( const Rectangle2 & rectClient );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GUIRadioButton.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_GUI_WIDGETS_COMMON_GUIRADIOBUTTON_H

