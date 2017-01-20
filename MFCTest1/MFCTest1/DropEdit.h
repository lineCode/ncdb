#pragma once


// CDropEdit

class CDropEdit : public CEdit
{
	DECLARE_DYNAMIC(CDropEdit)

public:
	CDropEdit();
	virtual ~CDropEdit();

public:

	virtual BOOL Register();

	// Generated message map functions

protected:

	COleDropTargetEx m_dropEx; virtual BOOL OnDrop(WPARAM pDropInfoClass, LPARAM lParm);

	virtual DROPEFFECT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);

	virtual DROPEFFECT OnDragOver(WPARAM pDropInfoClass, LPARAM lParm);
protected:
	DECLARE_MESSAGE_MAP()
};


