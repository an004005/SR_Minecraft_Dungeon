#include "..\..\Header\CollisionCom.h"
#include "Collider.h"
#include "Transform.h"

CCollisionCom::CCollisionCom()
{
}

CCollisionCom::CCollisionCom(const CCollisionCom& rhs): CComponent(rhs)
{
}

CCollisionCom::~CCollisionCom()
{
}

_int CCollisionCom::Update_Component(const _float& fTimeDelta)
{
	if (m_bStop == false)
		CCollider::GetInstance()->Add_CollisionCom(this);
	return 0;
}

void CCollisionCom::LateUpdate_Component()
{
	CComponent::LateUpdate_Component();
}

CComponent* CCollisionCom::Clone()
{
	return new CCollisionCom(*this);
}

void CCollisionCom::Free()
{
	CComponent::Free();
}

CCollisionCom* CCollisionCom::Create()
{
	return new CCollisionCom();
}

void CCollisionCom::CollisionDynamic(CCollisionCom* pOther)
{
}

void CCollisionCom::CollisionStatic(const _vec3& vCenter, _float fRadius)
{
	_vec3& vPos = m_pOwnerTrans->m_vInfo[INFO_POS];

	_vec3 vDiff = vPos - vCenter;
	_float fMoved = m_fRadius + fRadius - D3DXVec3Length(&vDiff);

	D3DXVec3Normalize(&vDiff, &vDiff);

	vPos += fMoved * vDiff;
}

