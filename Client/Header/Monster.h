#pragma once
#include "SkeletalCube.h"

class CStatComponent;

class CMonster : public CSkeletalCube
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;

	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrPath);
	virtual void Free() override;

protected:
	CCollisionCom* m_pColl = nullptr;
	CStatComponent* m_pStat = nullptr;


};

