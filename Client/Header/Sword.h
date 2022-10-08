#pragma once
#include "EquipItem.h"

class CSword :
	public CEquipItem
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
	virtual _int Attack() override;
	virtual void Equipment(SkeletalPart* pSkeletalPart) override;
	virtual void Collision() override;

};

