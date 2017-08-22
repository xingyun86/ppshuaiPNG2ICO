//////////////////////////////////////////////
// CustomComboBox.h

#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H


#include "resource.h"

class CCustomComboBox : public CComboBoxEx
{
public:
	CCustomComboBox() {}
	virtual ~CCustomComboBox() {}
	CEdit* GetEdit() const { return const_cast<CEdit*>(&m_Edit); }
    void AddUsedtoList(CString &strText);
protected:
	virtual void OnAttach()
	{
		m_Edit.Attach(GetEditCtrl());
	}
	virtual void PreCreate(CREATESTRUCT& cs);

private:
	CEdit m_Edit;
};


#endif //CUSTOMCOMBOBOX_H
