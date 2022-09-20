#include "stdafx.h"
#include "SkeletalCube.h"


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

	pComponent = m_pRootPart->pTrans = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom_root", pComponent });
	m_mapParts.insert({s_strRoot, m_pRootPart});

	// body
	SkeletalPart* body = new SkeletalPart;

	pComponent = body->pBuf = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom_body", pComponent });
	
	pComponent = body->pTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MinecraftCubeTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexture_body", pComponent });
	              
	pComponent = body->pTrans = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom_body", pComponent });
	body->iTexIdx = 10;
	body->pParent = m_pRootPart;
	m_mapParts.insert({"body", body});

	// leg l
	SkeletalPart* leg_l = new SkeletalPart;

	pComponent = leg_l->pBuf = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom_leg_l", pComponent });
	
	pComponent = leg_l->pTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_MinecraftCubeTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexture_leg_l", pComponent });
	              
	pComponent = leg_l->pTrans = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom_leg_l", pComponent });
	leg_l->iTexIdx = 1;
	leg_l->pParent = m_pRootPart;
	m_mapParts.insert({"leg_l", leg_l});

	return S_OK;
}

_int CSkeletalCube::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CSkeletalCube::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CSkeletalCube::Render_Object()
{
	for(const auto& Pair : m_mapParts)
	{
		SkeletalPart* pPart = Pair.second;
		if (pPart->pBuf != nullptr && pPart->pTex != nullptr)
		{
			_matrix matPartWorld = pPart->GetSkeletalWorldMat();
			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPartWorld);
			pPart->pTex->Set_Texture(pPart->iTexIdx);
			pPart->pBuf->Render_Buffer();
		}
	}
}

void CSkeletalCube::Free()
{
	CGameObject::Free();
	for_each(m_mapParts.begin(), m_mapParts.end(), [](pair<string, SkeletalPart*> Pair)
	{
		// Pair.second->vecChild.clear();
		delete Pair.second;
	});
}

CSkeletalCube* CSkeletalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletalCube*	pInstance = new CSkeletalCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
