#pragma once
#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CTargetTexture : public CComponent
{
private:
	explicit CTargetTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTargetTexture(const CTargetTexture& rhs);
	virtual ~CTargetTexture() override;

public:
	HRESULT		Ready_Texture();
	void		Set_Texture(const _uint& iIndex = 0);		// �ؽ�ó �׸��� ���� �Լ�
	void		Set_Texture(class CShader* pShaderCom, D3DXHANDLE hHandle, const _uint& iIndex = 0);		// �ؽ�ó �׸��� ���� �Լ�

	void AddRender(CGameObject* pObj) { m_RenderList.push_back(pObj);}
	void RenderOnSurface();
private:
	IDirect3DTexture9*		m_pTexture;
	ID3DXRenderToSurface* m_pRender2Surface;
	list<CGameObject*> m_RenderList;

public:
	static	CTargetTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;
	virtual void Free() override;
};

END

