#pragma once
#include "ParticleSystem.h"

class CAttack_P :public CParticleSystem
{
public:
	explicit CAttack_P(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {};
	 ~CAttack_P() override;
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
class CShock_Powder : public CParticleSystem
{
public:
	explicit CShock_Powder(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	~CShock_Powder() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static CShock_Powder* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin, _int _numParticles);

	void Free() override;
};

class CCloud : public CParticleSystem
{
public:
	explicit CCloud(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
	~CCloud() override;

public:
	_int Update_Object(const _float& fTimeDelta) override;
	void Render_Object() override;
	void Reset_Particle(Attribute* _Attribute) override;
	void PreRender_Particle() override;
	void PostRender_Particle() override;

public:
	static CCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin, _int _numParticles);

	void Free() override;
};
//
// class CSpeedBoots : public CParticleSystem
// {
// public:
// 	explicit CSpeedBoots(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
// 	~CSpeedBoots() override;
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
// 	static CSpeedBoots* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin, _int _numParticles);
//
// 	void Free() override;
// };
//
// class CSpeedBoots_Particle : public CParticleSystem
// {
// public:
// 	explicit CSpeedBoots_Particle(LPDIRECT3DDEVICE9 pGraphicDev) : CParticleSystem(pGraphicDev) {}
// 	~CSpeedBoots_Particle() override;
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
// 	static CSpeedBoots_Particle* Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin, _int _numParticles);
//
// 	void Free() override;
// };
//

