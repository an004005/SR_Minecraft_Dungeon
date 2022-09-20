#pragma once
#include "GameObject.h"

struct SkeletalPart
{
	// string strName;
	SkeletalPart* pParent = nullptr;
	// vector<SkeletalPart*> vecChild;

	CVIBuffer* pBuf = nullptr;
	_uint iTexIdx = 0;
	CTexture* pTex = nullptr;
	CTransform* pTrans = nullptr;

	_matrix GetSkeletalWorldMat() const
	{
		_matrix matWorldPart = pTrans->m_matWorld;
		const SkeletalPart* pTmpParent = pParent;
		while (pTmpParent != nullptr)
		{
			matWorldPart = matWorldPart * pTmpParent->pTrans->m_matWorld;
			pTmpParent = pTmpParent->pParent;
		}

		return matWorldPart;
	}
};

class CSkeletalCube : public CGameObject
{
	friend class CImGuiMgr; // for tool
private:
	explicit CSkeletalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkeletalCube() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

	static CSkeletalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	static string s_strRoot;

	SkeletalPart* m_pRootPart = nullptr;
	map<string, SkeletalPart*> m_mapParts;
};
