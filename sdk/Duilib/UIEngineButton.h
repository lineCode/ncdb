#pragma once

class CEngineButton :
	public CButtonUI
{
public:
	CEngineButton(void);
	~CEngineButton(void);
	 LPCTSTR GetClass() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	ENUM_ENGINE GetButtomSimulatorType();
private:
	ENUM_ENGINE m_eSimulatorType;
};
