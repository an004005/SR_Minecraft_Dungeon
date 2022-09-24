#include "stdafx.h"
#include "CubeAnimationInstance.h"
#include "SkeletalCube.h"

CCubeAnimationInstance::CCubeAnimationInstance() = default;

CCubeAnimationInstance::CCubeAnimationInstance(const CCubeAnimationInstance& rhs)
	: CComponent(rhs)
{
}

CCubeAnimationInstance::~CCubeAnimationInstance()
{
}

_int CCubeAnimationInstance::Update_Component(const _float& fTimeDelta)
{
	return CComponent::Update_Component(fTimeDelta);
}

void CCubeAnimationInstance::LateUpdate_Component()
{
	CComponent::LateUpdate_Component();
}

CComponent* CCubeAnimationInstance::Clone()
{
	return new CCubeAnimationInstance(*this);
}

void CCubeAnimationInstance::Free()
{
	CComponent::Free();
}
