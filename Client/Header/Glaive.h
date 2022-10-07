#pragma once
#include "EquipItem.h"
class CGlaive :
	public CEquipItem
{
private:
	explicit CGlaive(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGlaive();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	static CGlaive* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
	virtual _int Attack() override;
	virtual void Equipment(SkeletalPart* pSkeletalPart) override;


};

