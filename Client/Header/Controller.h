#pragma once
#include "Base.h"

class CSkeletalCube;
class CPlayer;
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

protected:
	_uint m_iTick = 0;
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

/*----------------
 *	CGeomancerController
 ----------------*/
class CGeomancerController : public CController
{
private:
	CGeomancerController();
	virtual ~CGeomancerController() override;

public:
	virtual void Update(CSkeletalCube* m_pOwner) override;

public:
	static CGeomancerController* Create();

private:
};