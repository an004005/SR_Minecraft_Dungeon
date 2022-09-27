#pragma once
#include "Base.h"

class CSkeletalCube;

/*----------------
 *	Controller
 ----------------*/
class CController : public CBase
{
protected:
	explicit CController();
	virtual ~CController();

public:
	virtual void Update(CSkeletalCube* m_pOwner) PURE;
	virtual void Free() override;
};

/*----------------
 *	CPlayerController
 ----------------*/
class CPlayerController : public CController
{
private:
	CPlayerController();
	virtual ~CPlayerController() override;

public:
	virtual void Update(CSkeletalCube* m_pOwner) override;

public:
	static CPlayerController* Create();
};