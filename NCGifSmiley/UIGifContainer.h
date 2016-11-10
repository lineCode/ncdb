#ifndef __UIGIFCONTAINER_H__
#define __UIGIFCONTAINER_H__

#pragma once
#define MOUSE_IN_TIMER       UIMSG_USER+100
class CGifContainerUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGifContainerUI)
public:
	CGifContainerUI();
	void MouseInTimer(UINT_PTR idEvent);
	void DoEvent(TEventUI& event);
	void	DoInit() override;
protected:
	//bool mouseInFlag;
	CDuiPoint point;
};
#endif // __UIGIFCONTAINER_H__
