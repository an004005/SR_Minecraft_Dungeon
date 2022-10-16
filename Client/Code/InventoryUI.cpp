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
		wstrItemName[0] = L"����";
		wstrItemName[1] = L"���� ���� 60";
		/*wstrItemName[3] = L"������ ȿ������ �����, ��ο��� ���뿡 �ִ�\n���ں��� �����ڵ��� ��� ����մϴ�.";*/
		wstrItemName[2] = L"������ ȿ������ �����,\n�ȼ��� �������� ���� ������ ���˴ϴ�.";
	}
		break;
	case 8:
	{
		//crossbow
		wstrItemName[0] = L"�����";
		wstrItemName[1] = L"���Ÿ� ���� 20";
		wstrItemName[2] = L"�ܼ������� ������ ���� �ִ� �����Դϴ�.\n�������� ������ ȭ�� �浹ó����\n�Ǹ��� �ߴٰ� �������ϴ�.";
	}
		break;
	case 9:
	{
		//galive
		wstrItemName[0] = L"�۷��̺�";
		wstrItemName[1] = L"�ٰŸ� ���� 35";
		wstrItemName[2] = L"�ȼ������� �پ ������ �� �Ѹ��� �ÿ�ȸ \n������ ġ���ϴ� �ǹ̷� �ϻ�� �����Դϴ�.\n�� �۷��̺�� �ڵ��� ���� ���� �ֽ��ϴ�.";
	}
		break;
	case 10:
	{
		//sword
		wstrItemName[0] = L"��";
		wstrItemName[1] = L"�ٰŸ� ���� 25";
		wstrItemName[2] = L"ưư�ϰ� �ϰ� �� �� �ִ� ���Դϴ�.";
	}
		break;
	case 11:
	{
		//boots
		wstrItemName[0] = L"�ż��� ����";
		wstrItemName[1] = L"���ӽð� 10";
		wstrItemName[2] = L"ȿ���ο��� ���� �ż��� �������� �����ϰ�\n�ϴ� �����Դϴ�. ����ó�� ��� �� ��Ȯ����\n���� �����մϴ�. ";
	}
		break;
	case 13:
	{
		//firework
		wstrItemName[0] = L"���� ȭ��";
		wstrItemName[1] = L"���Ÿ� ���� 50";
		wstrItemName[2] = L"��ƼŬ �̻��� �־����ϴ�. �Ҹ� �ֳ���?";
		
	}
		break;
	case 14:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
		break;
	case 15:
	{
		//shockpowder
		wstrItemName[0] = L"������ ��";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	case 16:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	case 17:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	case 18:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	case 19:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	case 20:
	{
		//shockpowder
		wstrItemName[0] = L"��� ����";
		wstrItemName[1] = L"���� ���ӽð� 2��";
		wstrItemName[2] = L"��� ����� ������ ���������� ���� �������\n���� �������� �����Դϴ�.";
	}
	break;
	default:
		MSG_BOX("input nonTextIndex");
		break;
	}
}

