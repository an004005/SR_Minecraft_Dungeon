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
	m_pTransCom->Set_Scale(WINCX, WINCY, 0.f);
	m_iRenderPriority = 9;
	return S_OK;
}

_int CInventoryUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bClose)
		return OBJ_NOEVENT;

	CUI::Update_Object(fTimeDelta);
	

	if (m_bWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = _vec3(WINCX * 0.37f, WINCY * 0.29f, 0.f);
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 0.1f;
		vScale.y *= 0.15f;
		CGameUtilMgr::MatWorldComposeEuler(m_matIonUIWorld, vScale, vAngle, vPos);	
		m_bWorldSet = true;
	}
	

	return OBJ_NOEVENT;
}

void CInventoryUI::Render_Object()
{
	//CUI::Render_Object();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	if (m_iTexNum != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matIonUIWorld);
		m_pTextureCom->Set_Texture(m_iTexNum);
		m_pBufferCom->Render_Buffer();

		SetTexture(m_iTexNum);
		
		_vec2 vPos = _vec2(m_matIonUIWorld._41, m_matIonUIWorld._22);
		Render_Font(L"Gothic_Regular30", wstrItemName[0].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.38f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		Render_Font(L"Gothic_Bold20", wstrItemName[1].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.48f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		Render_Font(L"Gothic_Bold20", wstrItemName[2].c_str(),
			&_vec2(vPos.x + WINCX * 0.28f, -vPos.y + WINCY *0.55f),
			D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
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
	default:
		MSG_BOX("input nonTextIndex");
		break;
	}
}

/*------------------------
* ItemSpace
------------------------*/
CItemSpaceUI::CItemSpaceUI(LPDIRECT3DDEVICE9 pGraphicDev) : CInventoryUI(pGraphicDev)
{
}

CItemSpaceUI::~CItemSpaceUI()
{
}

HRESULT CItemSpaceUI::Ready_Object()
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.f, 1.f);

	m_pBufferCom = Add_Component<CRcTex>(L"Proto_RcTexCom", L"Proto_RcTexCom", ID_STATIC);
	m_pTextureCom = Add_Component<CTexture>(L"Proto_InventoryUI_Texture", L"Proto_InventoryUI_Texture", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);

	

	m_iRenderPriority = 8;
	return S_OK;
}

_int CItemSpaceUI::Update_Object(const _float & fTimeDelta)
{
	if (m_bWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 0.8f;
		vScale.y *= 0.8f;
		CGameUtilMgr::MatWorldComposeEuler(m_matIconWorld, vScale, vAngle, vPos);

		m_bWorldSet = true;
	}

	if (m_MouseCollFrameWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 1.1f;
		vScale.y *= 1.1f;
		CGameUtilMgr::MatWorldComposeEuler(m_matMouseCollFrameWorld, vScale, vAngle, vPos);

		m_MouseCollFrameWorldSet = true;
	}

	if (m_CurClickFrameWorldSet == false)
	{
		_vec3 vScale = m_pTransCom->m_vScale;
		_vec3 vPos = m_pTransCom->m_vInfo[INFO_POS];
		_vec3 vAngle = m_pTransCom->m_vAngle;
		vScale.x *= 1.1f;
		vScale.y *= 1.1f;
		CGameUtilMgr::MatWorldComposeEuler(m_matCurClickFrameWorld, vScale, vAngle, vPos);

		m_CurClickFrameWorldSet = true;
	}

	
	if (m_bClose)
		return OBJ_NOEVENT;


	CUI::Update_Object(fTimeDelta);
	return OBJ_NOEVENT;
}

void CItemSpaceUI::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &CGameUtilMgr::s_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	switch (m_eRenderType)
	{
	case SLOT_BASE:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iBaseSlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_WEAPON:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iWeaponSlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_LEGACY:
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
		m_pTextureCom->Set_Texture(m_iLegacySlot);
		m_pBufferCom->Render_Buffer();
	}
	break;
	case SLOT_TYPE_END:
		break;
	}

	if (m_iIconTexNum != 0)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matIconWorld);
		m_pTextureCom->Set_Texture(m_iIconTexNum);
		m_pBufferCom->Render_Buffer();
		m_iIconTexNum = 0;
	}

	if (m_bMouseCollFrame)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matMouseCollFrameWorld);
		m_pTextureCom->Set_Texture(4);
		m_pBufferCom->Render_Buffer();
		m_bMouseCollFrame = false;
	}

	if (m_bCurClickFrame)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCurClickFrameWorld);
		m_pTextureCom->Set_Texture(5);
		m_pBufferCom->Render_Buffer();
	}


}

void CItemSpaceUI::Free()
{
	
	CInventoryUI::Free();
}

CItemSpaceUI * CItemSpaceUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint iTexNum)
{
	CItemSpaceUI* pInstance = new CItemSpaceUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}