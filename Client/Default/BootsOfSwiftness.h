#pragma once
#include "EquipItem.h"
class CBootsOfSwiftness :
	public CEquipItem
{
private:
	explicit CBootsOfSwiftness(LPDIRECT3DDEVICE9 pGraphicDev);
	~CBootsOfSwiftness();

public:
	//gameobj function
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	static CBootsOfSwiftness* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	// equipitem function
	virtual void Equipment(SkeletalPart* pSkeletalPart) override;
};

