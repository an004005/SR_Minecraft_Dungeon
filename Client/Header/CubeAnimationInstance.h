#pragma once
#include "Component.h"

struct CubeAnimFrame;

class CCubeAnimationInstance : public Engine::CComponent
{
private:
	explicit CCubeAnimationInstance();
	explicit CCubeAnimationInstance(const CCubeAnimationInstance& rhs);
	virtual ~CCubeAnimationInstance() override;

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual CComponent* Clone() override;
	virtual void Free() override;

	CubeAnimFrame* GetCurrentLoopAnim() const { return m_pLoopAnim; }


private:
	CGameObject* m_pOwner = nullptr;
	CubeAnimFrame* m_pLoopAnim = nullptr;
};

