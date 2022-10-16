#include "stdafx.h"
#include "..\Header\InventoryUI.h"
#include "AbstFactory.h"
#include "Inventory.h"
/*------------------------
* BackGround
------------------------*/
CInventoryUI::CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CInventoryUI::~CInventoryUI()
{
}

HRESULT CInventoryUI::Ready_Object()
{

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_InventoryUI_Texture", L"Proto_InventoryUI_Texture", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	m_iRenderPriority = 8;
	return S_OK;
}

_int CInventoryUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bClose)
		return OBJ_NOEVENT;

	CUI::Update_Object(fTimeDelta);
	
	return OBJ_NOEVENT;
}

void CInventoryUI::Render_Object()
{
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();

	/*_vec2 vPos = _vec2(m_matIonUIWorld._41, m_matIonUIWorld._22);

	if (m_iTexNum != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matIonUIWorld);
		m_pTextureCom->Set_Texture(m_iTexNum);
		m_pBufferCom->Render_Buffer();

		SetTexture(m_iTexNum);
		
		
		Render_Font(L"Gothic_Regular30", wstrItemName[0].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.38f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		Render_Font(L"Gothic_Bold20", wstrItemName[1].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.48f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		Render_Font(L"Gothic_Bold20", wstrItemName[2].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.55f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}*/


}

void CInventoryUI::Free()
{
	CUI::Free();
}

CInventoryUI * CInventoryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CInventoryUI* pInstance = new CInventoryUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventoryUI::SetTexture(_int iTexNum)
{
	switch (iTexNum)
	{
	case 7:
	{
		//axe
		wstrItemName[0] = L"도끼";
		wstrItemName[1] = L"근접 피해 60";
		/*wstrItemName[3] = L"도끼는 효율적인 무기로, 우민왕의 군대에 있는\n무자비한 변명자들이 즐겨 사용합니다.";*/
		wstrItemName[2] = L"도끼는 효율적인 무기로,\n안선생 조원들의 정훈 교육에 사용됩니다.";
	}
		break;
	case 8:
	{
		//crossbow
		wstrItemName[0] = L"보우건";
		wstrItemName[1] = L"원거리 피해 20";
		wstrItemName[2] = L"단순하지만 균형이 잡혀 있는 무기입니다.\n조원들은 조장의 화살 충돌처리에\n실망을 했다고 전해집니다.";
	}
		break;
	case 9:
	{
		//galive
		wstrItemName[0] = L"글레이브";
		wstrItemName[1] = L"근거리 피해 35";
		wstrItemName[2] = L"안선생님의 뛰어난 조원들 중 한명에게 시연회 \n정복을 치하하는 의미로 하사된 무기입니다.\n이 글레이브는 코딩의 신이 깃들어 있습니다.";
	}
		break;
	case 10:
	{
		//sword
		wstrItemName[0] = L"검";
		wstrItemName[1] = L"근거리 피해 25";
		wstrItemName[2] = L"튼튼하고 믿고 쓸 수 있는 검입니다.";
	}
		break;
	case 11:
	{
		//boots
		wstrItemName[0] = L"신속의 부츠";
		wstrItemName[1] = L"지속시간 10";
		wstrItemName[2] = L"효과부여를 통해 신속한 움직임을 가능하게\n하는 부츠입니다. 지금처럼 모든 게 불확실한\n때에 유용합니다. ";
	}
		break;
	case 13:
	{
		//firework
		wstrItemName[0] = L"폭죽 화살";
		wstrItemName[1] = L"원거리 피해 50";
		wstrItemName[2] = L"파티클 이뻐서 넣었습니다. 불만 있나요?";
		
	}
		break;
	case 14:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
		break;
	case 15:
	{
		//shockpowder
		wstrItemName[0] = L"레이저 룬";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	case 16:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	case 17:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	case 18:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	case 19:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	case 20:
	{
		//shockpowder
		wstrItemName[0] = L"충격 가루";
		wstrItemName[1] = L"마비 지속시간 2초";
		wstrItemName[2] = L"충격 가루는 빠르게 빠져나가고 싶은 사람들을\n위한 믿음직한 도구입니다.";
	}
	break;
	default:
		MSG_BOX("input nonTextIndex");
		break;
	}
}

