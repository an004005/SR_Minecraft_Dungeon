#include "..\..\Header\SkeletalCube.h"

string CSkeletalCube::m_strRoot = "root";

CSkeletalCube::CSkeletalCube(LPDIRECT3DDEVICE9 pGraphicDev): CGameObject(pGraphicDev)
{
}

CSkeletalCube::CSkeletalCube(const CGameObject& rhs): CGameObject(rhs)
{
}

CSkeletalCube::~CSkeletalCube()
{
}

HRESULT CSkeletalCube::Ready_Object()
{
	return CGameObject::Ready_Object();
}

_int CSkeletalCube::Update_Object(const _float& fTimeDelta)
{
	return CGameObject::Update_Object(fTimeDelta);
}

void CSkeletalCube::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CSkeletalCube::Render_Object()
{
	CGameObject::Render_Object();
}

void CSkeletalCube::Free()
{
	CGameObject::Free();
}
