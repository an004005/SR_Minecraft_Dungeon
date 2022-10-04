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
	_vec3& vPos = m_pOwnerTrans->m_vInfo[INFO_POS];
	_vec3 vCollPos = GetCollPos();

	_vec3& vPosOther = pOther->GetTransform()->m_vInfo[INFO_POS];
	_vec3 vCollPosOther = pOther->GetCollPos();

	_vec3 vToMe = vCollPos - vCollPosOther;
	_float fMoved = (m_fRadius + pOther->GetRadius() - D3DXVec3Length(&vToMe)) * 0.5f;

	D3DXVec3Normalize(&vToMe, &vToMe);

	vPos += fMoved * vToMe;
	vPosOther += -fMoved * vToMe;
}

void CCollisionCom::CollisionStatic(const _vec3& vCenter, _float fRadius)
{
	_vec3& vPos = m_pOwnerTrans->m_vInfo[INFO_POS];
	_vec3 vCollPos = GetCollPos();

	_vec3 vDiff = vCollPos - vCenter;
	_float fMoved = m_fRadius + fRadius - D3DXVec3Length(&vDiff);

	D3DXVec3Normalize(&vDiff, &vDiff);

	vPos += fMoved * vDiff;
}

