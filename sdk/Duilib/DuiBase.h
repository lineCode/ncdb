#pragma once
#include "DuiHeader.h"
#include "DuiCommon.h"
//#include "WndShadow.h"


class CDuiBase;


struct tagWNDMSG
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	HRESULT hr;
};

struct tagMove
{
	UINT message;
	WPARAM empty;
	short x;
	short  y;
	HRESULT hr;
};

struct tagSize
{
	UINT message;
	WPARAM nResizeType;
	short width;
	short  height;
	HRESULT hr;
};

struct tagLButtonDown
{
	UINT message;
	WPARAM vitualKeys;
	short x;
	short  y;
	HRESULT hr;
};

typedef void ( CDuiBase::*PI_PMSG)(tagWNDMSG& tag);
typedef void ( CDuiBase::*PI_PMSGV)();
typedef void ( CDuiBase::*PI_PMSG_MOVE)(tagMove& tag);
typedef void ( CDuiBase::*PI_PMSG_SIZE)(tagSize& tag);
typedef void ( CDuiBase::*PI_PMSG_LBUTTONDOWN)(tagLButtonDown& tag);

//#define PI_ON_MESSAGE(MsgID, Func) {MsgID, 0, 0, 0, (PI_PMSG)Func},
#define PI_ON_MESSAGE(MsgID, Func) {MsgID, 0, 0, 0, (PI_PMSG)(Func)},
#define PI_ON_WM_MOVE() 			{ WM_MOVE, 0, 0, 0, (PI_PMSG)(PI_PMSG_MOVE)(&ThisClass::PiOnMove) },
#define PI_ON_WM_SIZE() 			{ WM_SIZE, 0, 0, 0, (PI_PMSG)(PI_PMSG_SIZE)(&ThisClass::PiOnSize) },
#define PI_ON_WM_LBUTTONDOWN(Func)	{ WM_LBUTTONDOWN, 0, 0, 0, (PI_PMSG)(PI_PMSG_LBUTTONDOWN)(&ThisClass::PiOnLButtonDown) },


struct PIMSGMAP_ENTRY
{
	UINT nMessage;   // windows message
	UINT nCode;      // control code or WM_NOTIFY code
	UINT nID;        // control ID (or 0 for windows messages)
	UINT nLastID;    // used for entries specifying a range of control id's
	//UINT_PTR nSig;       // signature type (action) or pointer to message #
	PI_PMSG pfn;    // routine to call (or special value)
};

struct PIMSGMAP
{
	const PIMSGMAP* (PASCAL* pfnGetBaseMap)();
	const PIMSGMAP_ENTRY* lpEntries;
};



#define PI_DESC_MESSAGE_MAP \
	static const PIMSGMAP* PASCAL GetPiThisMessageMap(); \
	virtual const PIMSGMAP* GetPiMessageMap() const;

#define PI_BEGIN_MESSAGE_MAP(theClass, baseClass) \
	const PIMSGMAP* theClass::GetPiMessageMap() const \
{ return GetPiThisMessageMap(); } \
	const PIMSGMAP* PASCAL theClass::GetPiThisMessageMap() \
{ \
	typedef theClass ThisClass;						   \
	typedef baseClass TheBaseClass;					   \
	static const PIMSGMAP_ENTRY _pi_messageEntries[] =  \
{

#define PI_End_MESSAGE_MAP() \
{0, 0, 0, 0, (PI_PMSG)0 } \
}; \
	static const PIMSGMAP pimessageMap = \
{ &TheBaseClass::GetPiThisMessageMap, &_pi_messageEntries[0] }; \
	return &pimessageMap; \
}

class CDuiBase :
	public WindowImplBase
{
public:
	CDuiBase(HWND hParent = NULL);
	~CDuiBase(void);
public:
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
#ifdef M_DUI_TYPE_STD
	virtual UILIB_RESOURCETYPE GetResourceType() const;
#endif
	virtual LPCTSTR GetResourceID() const;

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleMessageChild(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static const PIMSGMAP* PASCAL GetPiThisMessageMap();
	virtual const PIMSGMAP* GetPiMessageMap() const;
	/*------------------------------------------------------------
			fun:	窗口即将显示时调用, 虚函数
			ret:	
			param: bShow, 是否在显示窗口
			remark:	
	------------------------------------------------------------*/
	virtual void OnShow(BOOL bShow, int nState);
	operator HWND();
	bool IsWindow();
	bool CreateModalShow(UINT* pRet = NULL);
	bool CreateChild(UINT* pRet = NULL);
	bool SetDuiWindowText(LPCTSTR szCt, LPCTSTR szText);
	CDuiString GetDuiWindowText(LPCTSTR szCt);
	bool SetDuiWindowVisible(LPCTSTR szCt, bool bShow);
	bool SetDuiWindowBkImage(LPCTSTR szCt, LPCTSTR szPic);
	bool SetDuiCtPos(LPCTSTR szCt, const DuiLib::CDuiSize& pt);
	void SetWndowSize(SIZE szSize);
	void MoveWindow(int x, int y);
	CControlUI* FindControl(LPCTSTR szCt);
	/*------------------------------------------------------------
			fun:	显示指定容器里的一组控件
			ret:	
			param: szContainter:容器名, lstFilter:要显示的控件
			remark:	其他所有控件隐藏起来
	------------------------------------------------------------*/
	bool ShowFilter( LPCTSTR szContainter, const LST_STRING& lstFilter );
	int CenterPosX( ARR_CONTROL& lstCt, int nSpace, int nAreaWidth);
	int CenterPosX( LPCTSTR szControlName, int nSpace, int nAreaWidth);
	int CenterPosX( ARR_CONTROL_NAME& lstCt, int nSpace, int nAreaWidth);
	SIZE GetWindowSize();
protected:
	CDuiString				m_SkinFolder;			
	CDuiString				m_SkinFile;			
	CDuiString				m_WndClassName;			
	CDuiString				m_strResID;		
#ifdef M_DUI_TYPE_STD
	UILIB_RESOURCETYPE		m_ResType;
#endif
private:
	//CWndShadow				m_WndShadow;	//阴影边框
	bool					m_bCenterToParent;	//相对于父窗口居中
	HWND					m_hParent;
};

