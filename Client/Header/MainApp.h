#pragma once

#include "Engine_Include.h"
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;
class CTimerMgr;

END

class CMainApp : public CBase
{
private:			// ������, �Ҹ���
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT			Ready_MainApp(void);
	Engine::_int	Update_MainApp(const Engine::_float& fTimeDelta);
	void			LateUpdate_MainApp(void);
	void			Render_MainApp(void);

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);
	HRESULT         Ready_Proto();
private:
	Engine::CGraphicDev*			m_pDeviceClass = nullptr;
	Engine::CManagement*			m_pManagementClass = nullptr;
	Engine::CTimerMgr*				m_pTimerMgr = nullptr;

	LPDIRECT3DDEVICE9				m_pGraphicDev;


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


public:			// ��ü ���� �Լ�
	static		CMainApp*		Create(void);

private:		// ��ü �Ҹ� �Լ�
	virtual		void			Free(void);
};

