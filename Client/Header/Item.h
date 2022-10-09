
#include "GameObject.h"

class CItem :
	public CGameObject
{
public:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItem();

public:
	virtual HRESULT Ready_Object();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	void SetState(ITEMSTATE eState) { m_eItemState = eState; }
	void SetVelocity(_vec3 vLook, _float fPower)
	{
		_float fAngle = _float(rand() % 140 - 70);
		D3DXVec3Normalize(&vLook, &vLook);
		vLook.x = cosf(D3DXToRadian(fAngle)) * vLook.x - sinf(D3DXToRadian(fAngle)) * vLook.z;
		vLook.z = sinf(D3DXToRadian(fAngle)) * vLook.x + cosf(D3DXToRadian(fAngle)) * vLook.z;

		m_vVelocity = vLook;	
		m_fPower = fPower;
	
	}
	ITEMSTATE GetItemState() { return m_eItemState; }

protected:
	ITEMSTATE	m_eItemState;
	_vec3 m_vVelocity;
	_float m_fPower;
	_float m_fTime;
	CCollisionCom* m_pColl = nullptr;

	virtual void Parabola(_vec3 & vPos,const _float& fHeight,const _float& fTimeDelta);
};
