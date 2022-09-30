#pragma once

#include "Component.h"
#include "Engine_Include.h"
#include "ProtoMgr.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :	public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const wstring& pComponentTag, COMPONENTID eID);
	template<typename T>
	T* Add_Component(const wstring& pProtoTag, const wstring& pComponentTag, COMPONENTID eID)
	{
		// get proto check
		CComponent* pComponent = CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
		if (pComponent == nullptr)
		{
			_ASSERT_CRASH("insert component fail : pProtoTag is not exist");
		}

		// cast check
		T* pT = dynamic_cast<T*>(pComponent);
		if (pT == nullptr)
		{
			_ASSERT_CRASH("insert component fail : Fail to cast");
		}

		// insert check(duplicated)
		if (false == m_mapComponent[eID].insert({pComponentTag, pComponent}).second)
		{
			_ASSERT_CRASH("insert component fail : pComponentTag is duplicated");
		}

		return pT;
	}

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	CComponent*		Find_Component(const wstring& pComponentTag, COMPONENTID eID);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<wstring, CComponent*>		m_mapComponent[ID_END];


public:
	virtual void	Free(void);
};

END