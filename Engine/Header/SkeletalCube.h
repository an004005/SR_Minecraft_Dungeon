#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;
class CTransform;

struct SkeletalPart
{
	string strName;
	SkeletalPart* pParent = nullptr;
	vector<SkeletalPart*> vecChild;

	CVIBuffer* pBuf = nullptr;
	_uint iTexIdx = 0;
	CTexture* pTex = nullptr;
	CTransform* pTrans = nullptr;
};

class CSkeletalCube : public CGameObject
{
private:
	explicit CSkeletalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkeletalCube(const CGameObject& rhs);
	virtual ~CSkeletalCube() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Free() override;

private:
	static string m_strRoot;
	map<string, SkeletalPart*> m_mapParts;
};
END