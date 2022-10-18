#include "stdafx.h"
#include "..\Header\Logo.h"
#include "AbstFactory.h"
#include "DynamicCamera.h"
#include "UI.h"
#include "EditBox.h"
#include "StaticCamera.h"
#include "TerrainCubeMap.h"
#include "ObjectStoreMgr.h"
#include "SkeletalCube.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	m_pCamMove = CSkeletalCube::Create(m_pGraphicDev, L"../Bin/Resource/SkeletalCube/Object/logocam.cube");
	m_arrLayer[LAYER_ENV]->Add_GameObject(L"CamMove", m_pCamMove);


	m_pCamMove->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS] = m_vCamSelectPos + _vec3{0.f, 10.f, 0.f};
	// m_pCamMove->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vAngle.y = D3DXToDegree(90.f);

	CStaticCamera* pcam = CEnvFactory::Create< CStaticCamera>("StaticCamera", L"StaticCamera");
	pcam->SetSkeletalFollowMode(m_pCamMove->Get_SkeletalPart("body"));

	CGameObject*		pGameObject = nullptr;

	// pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	// NULL_CHECK_RETURN(pGameObject, E_FAIL);
	// FAILED_CHECK_RETURN(m_arrLayer[LAYER_GAMEOBJ]->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	pGameObject = CTerrainCubeMap::Create(m_pGraphicDev, L"../Bin/Resource/Map/selectCharacter.map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_arrLayer[LAYER_ENV]->Add_GameObject(L"TerrainCubeMap", pGameObject), E_FAIL);

	m_pLogo = CUIFactory::Create<CUI>("DefaultUI", L"UI", 26, WINCX / 2, 200, WINCX * 0.8, WINCY * 0.3);
	m_pEdit = CUIFactory::Create<CEditBox>("EditBox", L"EditBox", 26, WINCX / 2, 500, WINCX * 0.3, WINCY * 0.07f);
	m_pEdit->SetText(L"Gothic_Regular25", L"이름을 입력하세요.", _vec2{ - 100.f, -60.f}, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pButton = CUIFactory::Create<CUI>("DefaultUI", L"DefaultUI", 27, WINCX / 2, 600, 100, 50);
	m_pButton->SetText(L"Gothic_Bold30", L"확인", _vec2{-30.f, -15.f}, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pButton->SetButton([this]()
	{
		static _bool bDoor = true;
		if (bDoor == false) return;
		CObjectStoreMgr::GetInstance()->SetPlayerName(m_pEdit->GetInputs());

		eState = MOVETO_PLAYER;
		m_fLogoTo = _vec3{0.f, 1000.f, 0.f} + m_pLogo->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		m_fEditTo = _vec3{0.f, 1000.f, 0.f} + m_pEdit->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		m_fButtonTo = _vec3{0.f, 1000.f, 0.f} + m_pButton->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
		bDoor = false;
	});

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	switch (eState)
	{
		case INPUT_NAME: 
			break;
		case MOVETO_PLAYER:
			{
				_vec3& vLogoPos = m_pLogo->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3& vEditPos = m_pEdit->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				_vec3& vButtonPos = m_pButton->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];

				if (!CGameUtilMgr::Vec3Cmp(vLogoPos, m_fLogoTo))
				{
					D3DXVec3Lerp(&vLogoPos, &vLogoPos, &m_fLogoTo, 0.008f);
					D3DXVec3Lerp(&vEditPos, &vEditPos, &m_fEditTo, 0.008f);
					D3DXVec3Lerp(&vButtonPos, &vButtonPos, &m_fButtonTo, 0.008f);
				}

				_vec3& vCamPos = m_pCamMove->Get_Component<CTransform>(L"Proto_TransformCom", ID_DYNAMIC)->m_vInfo[INFO_POS];
				if (!CGameUtilMgr::Vec3Cmp(m_vCamSelectPos, vCamPos))
				{
					D3DXVec3Lerp(&vCamPos, &vCamPos, &m_vCamSelectPos, 0.05f);
				}
				else
				{
					eState = SELECT_PLAYER;
				}
			}
			break;
		case SELECT_PLAYER: 
			break;
		case LOGO_STATE_END: 
			break;
		default: ;
	}

	return CScene::Update_Scene(fTimeDelta);
}

void CLogo::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	CScene::Render_Scene();
}

void CLogo::Confirm()
{
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
}
