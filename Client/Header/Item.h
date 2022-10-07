
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
	//static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;



};
