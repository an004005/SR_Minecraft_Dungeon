#pragma once
#include "GameObject.h"

struct SkeletalPart
{
	// info
	string strName;
	SkeletalPart* pParent = nullptr;
	vector<SkeletalPart*> vecChild;
	_matrix matLocal;

	// components
	CVIBuffer* pBuf = nullptr;
	wstring strBufCom;
	wstring strBufProto;

	_uint iTexIdx = 0;
	CTexture* pTex = nullptr;
	wstring strTexCom;
	wstring strTexProto;

	CTransform* pTrans = nullptr;
	wstring strTransCom;
	wstring strTransProto;

	SkeletalPart() { D3DXMatrixIdentity(&matLocal); }

	// todo 같은 부모의 world 공유로 최적화
	_matrix GetParentsWorldMat() const 
	{
		_matrix matParentsWorld;
		D3DXMatrixIdentity(&matParentsWorld);
		const SkeletalPart* pTmpParent = pParent;
		while (pTmpParent != nullptr)
		{
			matParentsWorld = matParentsWorld * pTmpParent->pTrans->m_matWorld;
			pTmpParent = pTmpParent->pParent;
		}

		return matParentsWorld;
	}

	_matrix GetWorldMat() const
	{
		return matLocal *  pTrans->m_matWorld * GetParentsWorldMat();
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

	_bool AddSkeletalPart(const string& strPart, const string& strParent, const wstring& strBuf, const wstring& strTex, const _uint iTexNum);
	_bool DeleteSkeletalPart(const string& strPart);
	static CSkeletalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath = L"");

	void Load(wstring wstrPath);
	void Save(wstring wstrPath);

private:
	void SaveRecursive(HANDLE hFile, SkeletalPart* pPart);
	void DeleteRecursive(const string& strPart);


private:
	static string s_strRoot;

	SkeletalPart* m_pRootPart = nullptr;
	map<string, SkeletalPart*> m_mapParts;
};
