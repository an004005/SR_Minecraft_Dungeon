#pragma once
#include "ParticleSystem.h"

#pragma region Particle System
class CAttack_P :public CParticleSystem
{
public:
	explicit CAttack_P(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {};
	virtual ~CAttack_P() override;
public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static CAttack_P* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;
};


// class CTrajectory : public CParticleSystem
// {
// public:
// 	explicit CTrajectory(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
// 	~CTrajectory() override;
//
// public:
// 	_int Update_Object(const _float& fTimeDelta) override;
// 	void Render_Object() override;
// 	void Reset_Particle(Attribute* _Attribute) override;
// 	void PreRender_Particle() override;
// 	void PostRender_Particle() override;
//
// 	HRESULT		SetUp_Material(void);
// public:
// 	static CTrajectory* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin, _int _numParticles);
//
// 	void Free() override;
// };
//


class CSpeedBoots : public CParticleSystem
{
public:
	explicit CSpeedBoots(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	virtual ~CSpeedBoots() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static CSpeedBoots* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;
};

class C3DBaseTexture : public CParticleSystem
{
public:
	explicit C3DBaseTexture(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	virtual ~C3DBaseTexture() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static C3DBaseTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;
};

class CSpeedBoots_Particle : public CParticleSystem
{
public:
	explicit CSpeedBoots_Particle(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	virtual ~CSpeedBoots_Particle() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static CSpeedBoots_Particle* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;
};

class CFireWork_Fuze : public CParticleSystem
{
public:
	explicit CFireWork_Fuze(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	virtual ~CFireWork_Fuze() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

	const _vec3& Get_FuzePos(void) { return vDeadPos; }

public:
	static CFireWork_Fuze* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;
public:
	_vec3 vDeadPos;

};

class CFireWork : public CParticleSystem
{
public:
	explicit CFireWork(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	virtual ~CFireWork() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;
	void Reset_tmp(void) { tmp = 1; }
public:
	static CFireWork* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName);

	void Free() override;

private:
	_uint tmp = 1;
};



#pragma endregion



#pragma region UVAnimation(RcShader)
class CShock_Powder : public CGameObject
{
public:
	explicit CShock_Powder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShock_Powder() override;

public:
	virtual HRESULT Ready_Object() override;
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void LateUpdate_Object() override;
	void PreRender_Particle();
	void PostRender_Particle();

public:
	static CShock_Powder* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CRcShader*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTexture = nullptr;
	void Free() override;

private:
	_float m_fTime = 1.f;
	_float m_fCurTime = 0.f;
};

enum CIRCLETYPE
{
	SHOCK = 0,
	FIREWORK,
	CREEPER,
	GOLEM
};
class CUVCircle : public CGameObject
{
	
public:
	explicit CUVCircle(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev) {}
	~CUVCircle() override;

public:
	virtual HRESULT Ready_Object(_float _size, CIRCLETYPE _type);
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void LateUpdate_Object() override;
	void PreRender_Particle();
	void PostRender_Particle();

public:
	static CUVCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, CIRCLETYPE _type);
	CRcShader*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTexture = nullptr;
	void Free() override;


private:
	_float m_fTime;
	_float m_fCurTime;
	_vec3	m_vVelocity;

	CFireWork_Fuze* m_pFuze;
};

enum CLOUDTYPE
{
	WALK=0,
	ROLL,
	SHOCKPOWDER,
	CREEPEREX
};


class CCloud : public CGameObject
{
public:
	explicit CCloud(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev) {}
	~CCloud() override;

public:
	virtual HRESULT Ready_Object(_float _size, CLOUDTYPE _type) ;
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void LateUpdate_Object() override;
	void PreRender_Particle();
	void PostRender_Particle();

public:
	static CCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, CLOUDTYPE _type);
	CRcShader*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTexture = nullptr;
	void Free() override;

private:
	_float m_fTime;
	_float m_fCurTime;
	_vec3	m_vVelocity;
	_float m_fSpeed;
};
#pragma endregion

