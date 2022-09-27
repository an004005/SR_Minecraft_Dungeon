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

	_matrix matParents;

	SkeletalPart() { D3DXMatrixIdentity(&matLocal); D3DXMatrixIdentity(&matParents); }

	_matrix GetWorldMat()
	{
		matParents = pTrans->m_matWorld * pParent->matParents;
		return matLocal *  matParents;
	}
};

// _float �ð��� �´� scale, rot, pos ����
struct TransFrame
{
	_float fTime = 0.f;
	_vec3 vScale;
	D3DXQUATERNION qRot;
	_vec3 vPos;

	TransFrame(){}
	TransFrame(_float fTime, _vec3 vScale, D3DXQUATERNION qRot, _vec3 vPos)
		: fTime(fTime), vScale(vScale), qRot(qRot), vPos(vPos) {}
};

struct CubeAnimFrame
{
	_bool bLoop = false;			// �ݺ� ����
	_bool bCancen = false;			// �߰� ��� ���� ����
	_float fTotalTime = 0.f;		// �� ��� �ð�
	// part�̸�,     part�� �ð��� transform����
	map<string, vector<TransFrame>> mapFrame; // (float�������� ���Ļ��¿��� �Ѵ�.)

	static CubeAnimFrame Load(const wstring& wstrPath);
	void Save(const wstring& wstrPath);
	void SortFrame(const string& strPart);
};

class CSkeletalCube : public CGameObject
{
	friend class CImGuiMgr; // for tool
protected:
	explicit CSkeletalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkeletalCube() override;

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void RenderObjectRecur(SkeletalPart* pPart);
	virtual void Free() override;


	_bool AddSkeletalPart(const string& strPart, const string& strParent, const wstring& strBuf, const wstring& strTex, const _uint iTexNum);
	_bool DeleteSkeletalPart(const string& strPart);
	static CSkeletalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath = L"");

	virtual void PlayAnimationOnce(const string& strAnim);
	virtual void PlayAnimationOnce(CubeAnimFrame* frame);

	void LoadSkeletal(wstring wstrPath);
	void SaveSkeletal(wstring wstrPath);


private:
	virtual void SaveRecursive(HANDLE hFile, SkeletalPart* pPart);
	virtual void DeleteRecursive(const string& strPart);
	static void TransFrameLerp(_matrix& matOut, const TransFrame& PrevFrame, const TransFrame& NextFrame, const _float fS);
	virtual void AnimFrameConsume(_float fTimeDelta);


protected:
	static string s_strRoot;

	// buffer, tex, trans com
	SkeletalPart* m_pRootPart = nullptr;
	map<string, SkeletalPart*> m_mapParts;

	// animation com
	CubeAnimFrame* m_pCurAnim = nullptr;
	CubeAnimFrame* m_pIdleAnim = nullptr;
	_float fAccTime = 0.f;	  // �ִϸ��̼� ���� �ð�
	_bool m_bStopAnim = false;
};
