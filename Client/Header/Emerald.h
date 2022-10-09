#pragma once
#include "ConsumeItem.h"

class CEmerald : public CConsumeItem
{
public:
	explicit CEmerald(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEmerald();

public:
	//gameobj function
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	static CEmerald* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	//���޶��尡 ���������� �÷��̾ �Ծ����� �Ѵ�.
	_bool OnGround() { return m_bIdle; }
	void	GoToPlayer() { m_bGotoPlayer = true; }
private:
	virtual void Parabola(_vec3 & vPos, const _float& fHeight, const _float& fTimeDelta) override;
	_bool m_bGotoPlayer = false;
};

