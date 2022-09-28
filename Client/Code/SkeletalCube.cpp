#include "stdafx.h"
#include "SkeletalCube.h"
#include "CubeAnimMgr.h"
#include "GameUtilMgr.h"

string CSkeletalCube::s_strRoot = "root";

CSkeletalCube::CSkeletalCube(LPDIRECT3DDEVICE9 pGraphicDev): CGameObject(pGraphicDev)
{
}

CSkeletalCube::~CSkeletalCube()
{
}

HRESULT CSkeletalCube::Ready_Object()
{
	CComponent* pComponent = nullptr;

	// root
	m_pRootPart = new SkeletalPart;
	// ��Ʈ �κ� �������� ����
	pComponent = m_pRootPart->pTrans = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({L"Proto_TransformCom_root", pComponent});
	m_pRootPart->strName = s_strRoot;
	m_mapParts.insert({s_strRoot, m_pRootPart});
	m_pRootPart->strTransProto = L"Proto_TransformCom";
	m_pRootPart->strTransCom = L"Proto_TransformCom_root";

	return S_OK;
}

_int CSkeletalCube::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	AnimFrameConsume(fTimeDelta);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CSkeletalCube::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CSkeletalCube::Render_Object()
{
	// // m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
 //   m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
 //   m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 //   m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
 //   m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
 //   m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
 //   m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pRootPart->matParents = m_pRootPart->pTrans->m_matWorld;
	for (const auto& child : m_pRootPart->vecChild)
	{
		RenderObjectRecur(child);
	}

   // m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
   // m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSkeletalCube::RenderObjectRecur(SkeletalPart* pPart)
{
	const _matrix matPartWorld = pPart->GetWorldMat();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPartWorld);
	if (pPart->pTex)
		pPart->pTex->Set_Texture(pPart->iTexIdx);
	if (pPart->pBuf)
		pPart->pBuf->Render_Buffer();

	for (const auto& child : pPart->vecChild)
		RenderObjectRecur(child);
}

void CSkeletalCube::Free()
{
	CGameObject::Free();
	for_each(m_mapParts.begin(), m_mapParts.end(), [](pair<string, SkeletalPart*> Pair)
	{
		Pair.second->vecChild.clear();
		delete Pair.second;
	});
}

_bool CSkeletalCube::AddSkeletalPart(const string& strPart, const string& strParent, const wstring& strBuf,
                                     const wstring& strTex,
                                     const _uint iTexNum)
{
	const auto itr_dup_check = m_mapParts.find(strPart);
	if (itr_dup_check != m_mapParts.end())
	{
		MSG_BOX("strPart is already exist");
		return false;
	}

	auto itr_parent = m_mapParts.find(strParent);
	if (itr_parent == m_mapParts.end())
	{
		MSG_BOX("Fail to Find strParent");
		return false;
	}

	SkeletalPart* pPart = new SkeletalPart;
	CComponent* pComponent = nullptr;
	wstring wstrPart;
	wstrPart.assign(strPart.begin(), strPart.end());

	pComponent = pPart->pBuf = dynamic_cast<CVIBuffer*>(Clone_Proto(strBuf.c_str()));
	NULL_CHECK_RETURN(pComponent, false);
	pPart->strBufCom = strBuf + L"_" + wstrPart;
	m_mapComponent[ID_STATIC].insert({pPart->strBufCom.c_str(), pComponent});
	pPart->strBufProto = strBuf;

	pComponent = pPart->pTex = dynamic_cast<CTexture*>(Clone_Proto(strTex.c_str()));
	NULL_CHECK_RETURN(pComponent, false);
	pPart->strTexCom = strTex + L"_" + wstrPart;
	m_mapComponent[ID_STATIC].insert({pPart->strTexCom.c_str(), pComponent});
	pPart->strTexProto = strTex;


	pComponent = pPart->pTrans = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, false);
	pPart->strTransCom = L"Proto_TransformCom_" + wstrPart;
	m_mapComponent[ID_STATIC].insert({pPart->strTransCom.c_str(), pComponent});
	pPart->strTransProto = L"Proto_TransformCom";

	pPart->iTexIdx = iTexNum;
	pPart->pParent = (*itr_parent).second;
	pPart->strName = strPart;
	m_mapParts.insert({strPart, pPart});
	pPart->pParent->vecChild.push_back(pPart);

	return true;
}

_bool CSkeletalCube::DeleteSkeletalPart(const string& strPart)
{
	auto itr_part = m_mapParts.find(strPart);
	if (itr_part == m_mapParts.end())
	{
		MSG_BOX("Fail to Find strPart");
		return false;
	}
	if (strPart == s_strRoot)
	{
		MSG_BOX("Do not delete root");
		return false;
	}

	SkeletalPart* pToDelete = (*itr_part).second;
	SkeletalPart* pParent = pToDelete->pParent;
	pParent->vecChild.erase(
		remove_if(pParent->vecChild.begin(), pParent->vecChild.end(),
		          [strPart](const SkeletalPart* pChild)
		          {
			          if (pChild->strName == strPart)
				          return true;
			          return false;
		          }),
		pParent->vecChild.end());

	DeleteRecursive(strPart);

	return true;
}

CSkeletalCube* CSkeletalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrPath)
{
	CSkeletalCube* pInstance = new CSkeletalCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	if (!wstrPath.empty())
		pInstance->LoadSkeletal(wstrPath);

	return pInstance;
}

void CSkeletalCube::AnimFrameConsume(_float fTimeDelta)
{
#ifdef _DEBUG
	if (m_bStopAnim)  // for imgui
		return;
#endif

	if (m_pCurAnim == nullptr) return;

	fAccTime += fTimeDelta;
	if (m_pCurAnim->fTotalTime < fAccTime)
	{
		fAccTime = 0.f;
		if (m_pCurAnim->bLoop == false) // loop �� �ƴϸ� �� �����ϰ� loop�� ����
		{
			// m_pCurAnim = m_pAnimInst->GetCurrentLoopAnim();
			m_pCurAnim = m_pIdleAnim;
			m_pCurAnim->bLoop = true;
			return;
		}
	}

	for (const auto& Part : m_mapParts)
	{
		auto itr_frame = m_pCurAnim->mapFrame.find(Part.second->strName);
		if (itr_frame == m_pCurAnim->mapFrame.end())
			continue;

		vector<TransFrame>& vecTransFrame = itr_frame->second;

		const TransFrame* pPrevFrame = nullptr;
		const TransFrame* pNextFrame = nullptr;
		
		for (auto& frame : vecTransFrame)
		{
			const _float fTime = frame.fTime;
			if (fTime < fAccTime)
			{
				pPrevFrame = &frame;
			}
			else if (fTime > fAccTime)
			{
				pNextFrame = &frame;
				break;
			}
		}
		if (pPrevFrame == nullptr)
			continue;
		if (pNextFrame == nullptr)
		{
			CGameUtilMgr::MatWorldCompose(
				OUT Part.second->pTrans->m_matWorld,
				pPrevFrame->vScale,
				pPrevFrame->qRot,
				pPrevFrame->vPos);
			continue;
		}
		
		const _float fS = (fAccTime - pPrevFrame->fTime) / (pNextFrame->fTime - pPrevFrame->fTime);
		TransFrameLerp(
			OUT Part.second->pTrans->m_matWorld, 
			*pPrevFrame, 
			*pNextFrame, 
			fS);
	}
}

void CSkeletalCube::PlayAnimationOnce(const string& strAnim)
{
	fAccTime = 0.f;
	m_pCurAnim = CCubeAnimMgr::GetInstance()->GetAnim(strAnim);
	m_pCurAnim->bLoop = false;
	// std::sort(m_pCurAnim..begin(), vecFrame.end());
}

void CSkeletalCube::PlayAnimationOnce(CubeAnimFrame* frame)
{
	fAccTime = 0.f;
	m_pCurAnim = frame;
	m_pCurAnim->bLoop = false;
}

void CSkeletalCube::LoadSkeletal(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Fail to Load SkeletalCube");
		return;
	}

	for (auto& child : m_pRootPart->vecChild)
	{
		string strChild = child->strName;
		DeleteRecursive(strChild);
	}
	m_pRootPart->vecChild.clear();


	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	size_t iPartCnt = 0;

	ReadFile(hFile, &iPartCnt, sizeof(size_t), &dwByte, nullptr);

	for (size_t i = 0; i < iPartCnt; ++i)
	{
		string strParent;
		string strPart;
		wstring wstrBufProto;
		wstring wstrTexProto;

		_uint iTexNum;
		_matrix matLocal;
		_matrix matWorld;

		// read parent
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		char szParent[128]{};
		ReadFile(hFile, szParent, dwStrByte, &dwByte, nullptr);
		strParent = szParent;

		// read part name
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		char szPart[128]{};
		ReadFile(hFile, szPart, dwStrByte, &dwByte, nullptr);
		strPart = szPart;

		// read buf proto
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		_tchar wszBufProto[128]{};
		ReadFile(hFile, wszBufProto, dwStrByte, &dwByte, nullptr);
		wstrBufProto = wszBufProto;

		// read tex proto
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		_tchar wszTexProto[128]{};
		ReadFile(hFile, wszTexProto, dwStrByte, &dwByte, nullptr);
		wstrTexProto = wszTexProto;

		// read tex num
		ReadFile(hFile, &iTexNum, sizeof(_uint), &dwByte, nullptr);

		AddSkeletalPart(strPart, strParent, wstrBufProto, wstrTexProto, iTexNum);

		// local, world mat
		ReadFile(hFile, &matLocal, sizeof(_matrix), &dwByte, nullptr);
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);
		
		const auto itr_part = m_mapParts.find(strPart);
		if (itr_part != m_mapParts.end())
		{
			(*itr_part).second->matLocal = matLocal;
			(*itr_part).second->pTrans->m_matWorld = matWorld;
		}
	}

	CloseHandle(hFile);
}

void CSkeletalCube::SaveSkeletal(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Fail to Save SkeletalCube");
		return;
	}
	DWORD dwByte = 0;

	size_t iPartCnt = m_mapParts.size() - 1; // except root

	WriteFile(hFile, &iPartCnt, sizeof(size_t), &dwByte, nullptr);

	for (const auto& child : m_pRootPart->vecChild)
	{
		SaveRecursive(hFile, child);
	}

	CloseHandle(hFile);
}

void CSkeletalCube::SaveRecursive(HANDLE hFile, SkeletalPart* pPart)
{
	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	// parent name
	dwStrByte = (DWORD)pPart->pParent->strName.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, pPart->pParent->strName.c_str(), dwStrByte, &dwByte, nullptr);

	// part name
	dwStrByte = (DWORD)pPart->strName.size();
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, pPart->strName.c_str(), dwStrByte, &dwByte, nullptr);

	// Buf proto name
	dwStrByte = (DWORD)pPart->strBufProto.size() * sizeof(_tchar);
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, pPart->strBufProto.c_str(), dwStrByte, &dwByte, nullptr);

	// Tex proto name, idx
	dwStrByte = (DWORD)pPart->strTexProto.size() * sizeof(_tchar);
	WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
	WriteFile(hFile, pPart->strTexProto.c_str(), dwStrByte, &dwByte, nullptr);
	WriteFile(hFile, &pPart->iTexIdx, sizeof(_uint), &dwByte, nullptr);

	// mat local
	WriteFile(hFile, &pPart->matLocal, sizeof(_matrix), &dwByte, nullptr);

	// trans world
	WriteFile(hFile, &pPart->pTrans->m_matWorld, sizeof(_matrix), &dwByte, nullptr);

	for (const auto& child : pPart->vecChild)
	{
		SaveRecursive(hFile, child);
	}
}

void CSkeletalCube::DeleteRecursive(const string& strPart)
{
	auto itr_part = m_mapParts.find(strPart);
	if (itr_part == m_mapParts.end())
	{
		MSG_BOX("Fail to Find strPart");
		return;
	}
	SkeletalPart* pToDelete = (*itr_part).second;

	for (auto& child : pToDelete->vecChild)
	{
		string strChild = child->strName;
		DeleteRecursive(strChild);
	}
	pToDelete->vecChild.clear();

	pToDelete->pBuf->Release();
	pToDelete->pTex->Release();
	pToDelete->pTrans->Release();

	auto itr = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pToDelete->strBufCom.c_str()));
	m_mapComponent[ID_STATIC].erase(itr);
	auto itr2 = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pToDelete->strTexCom.c_str()));
	m_mapComponent[ID_STATIC].erase(itr2);
	auto itr3 = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pToDelete->strTransCom.c_str()));
	m_mapComponent[ID_STATIC].erase(itr3);

	m_mapParts.erase(strPart);
	delete pToDelete;
}

void CSkeletalCube::TransFrameLerp(_matrix& matOut, const TransFrame& PrevFrame, const TransFrame& NextFrame,
	const _float fS)
{
	const _vec3 vPrevScale = PrevFrame.vScale;
	const D3DXQUATERNION qPrevQuat = PrevFrame.qRot;
	const _vec3 vPrevPos = PrevFrame.vPos;

	const _vec3 vNextScale = NextFrame.vScale;
	const D3DXQUATERNION qNextQuat = NextFrame.qRot;
	const _vec3 vNextPos = NextFrame.vPos;

	_vec3 vLerpScale = vPrevScale;
	D3DXQUATERNION qLerpQuat = qPrevQuat;
	_vec3 vLerpPos = vPrevPos;

	D3DXVec3Lerp(&vLerpScale, &vPrevScale, &vNextScale, fS);
	D3DXQuaternionSlerp(&qLerpQuat, &qPrevQuat, &qNextQuat, fS);
	D3DXVec3Lerp(&vLerpPos, &vPrevPos, &vNextPos, fS);

	CGameUtilMgr::MatWorldCompose(matOut, vLerpScale, qLerpQuat, vLerpPos);
}

/*----------------------
 *     CubeAnimFrame
 -----------------------*/
CubeAnimFrame CubeAnimFrame::Load(const wstring& wstrPath)
{
	CubeAnimFrame tmp;

	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Fail to Load CubeAnimFrame");
		return tmp;
	}

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;
	size_t mapSize;

	string strPartName;
	size_t vecSize;
	TransFrame tFrame;

	ReadFile(hFile, &tmp.bLoop, sizeof(_bool), &dwByte, nullptr);
	ReadFile(hFile, &tmp.fTotalTime, sizeof(_float), &dwByte, nullptr);

	ReadFile(hFile, &mapSize, sizeof(size_t), &dwByte, nullptr);

	for (size_t i = 0; i < mapSize; ++i)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		char szPartName[128]{};
		ReadFile(hFile, szPartName, dwStrByte, &dwByte, nullptr);
		strPartName = szPartName;

		vector<TransFrame> vecFrameTmp;
		ReadFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

		for (size_t j = 0; j < vecSize; ++j)
		{
			ReadFile(hFile, &tFrame.fTime, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &tFrame.vScale, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &tFrame.qRot, sizeof(D3DXQUATERNION), &dwByte, nullptr);
			ReadFile(hFile, &tFrame.vPos, sizeof(_vec3), &dwByte, nullptr);
			vecFrameTmp.push_back(tFrame);
		}

		tmp.mapFrame.insert({strPartName, vecFrameTmp});
	}

	CloseHandle(hFile);

	return tmp;
}

void CubeAnimFrame::Save(const wstring& wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Fail to Save CubeAnimFrame");
		return;
	}

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	WriteFile(hFile, &bLoop, sizeof(_bool), &dwByte, nullptr);
	WriteFile(hFile, &fTotalTime, sizeof(_float), &dwByte, nullptr);

	size_t mapSize = mapFrame.size();
	WriteFile(hFile, &mapSize, sizeof(size_t), &dwByte, nullptr);

	for (const auto& frame : mapFrame)
	{
		// target part name
		dwStrByte = (DWORD)frame.first.size();
		WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, frame.first.c_str(), dwStrByte, &dwByte, nullptr);

		size_t vecSize = frame.second.size();
		WriteFile(hFile, &vecSize, sizeof(size_t), &dwByte, nullptr);

		for (const auto& trans : frame.second)
		{
			WriteFile(hFile, &trans.fTime, sizeof(_float), &dwByte, nullptr);
			WriteFile(hFile, &trans.vScale, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &trans.qRot, sizeof(D3DXQUATERNION), &dwByte, nullptr);
			WriteFile(hFile, &trans.vPos, sizeof(_vec3), &dwByte, nullptr);
		}
	}
	
	CloseHandle(hFile);
}


void CubeAnimFrame::SortFrame(const string& strPart)
{
	auto& vecFrame = mapFrame.find(strPart)->second;
	sort(vecFrame.begin(), vecFrame.end(), [](const TransFrame& a, const TransFrame& b)
	{
		return a.fTime < b.fTime;
	});

	sort(vecEvent.begin(), vecEvent.end());
}
