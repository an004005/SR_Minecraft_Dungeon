#include "stdafx.h"
#include "..\Header\Particle.h"

#pragma region Circle
CAttack_P::~CAttack_P()
{
	Free();
}

_int CAttack_P::Update_Object(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
	{
		if (i->_bIsAlive)
		{
			i->_vPosition += i->_vVelocity * fTimeDelta;
			i->_fAge += fTimeDelta;

			if (i->_fAge > i->_fLifeTime)
			{
				i->_bIsAlive = false;
				
			}
		}
	}

	RemoveDeadParticles();
	CParticleSystem::Update_Object(fTimeDelta);

	// if (IsEmpty())
	// 	return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CAttack_P::Render_Object()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &MAT_I);
	CParticleSystem::Render_Object();
}

void CAttack_P::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;
	_vec3 min = _vec3(-3.0f, 0.0f, -3.0f);
	_vec3 max = _vec3(3.0f, 1.0f, 3.0f);

	GetRandomVector(
		&_Attribute->_vVelocity,
		&min,
		&max);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);
	_Attribute->_vVelocity *= 20.f;

	_Attribute->_vVelocity.y =GetRandomFloat(0.0f, 1.0f) * -2.f;
	_Attribute->_fAge = 0.0f;
}

void CAttack_P::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CAttack_P::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CAttack_P* CAttack_P::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	CAttack_P* Inst = new CAttack_P(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;

	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CAttack_P::Free()
{
	CParticleSystem::Free();
}





CShock_Powder::~CShock_Powder()
{
	Free();
}

_int CShock_Powder::Update_Object(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
	{
		if (i->_bIsAlive)
		{
			i->_vPosition += i->_vVelocity * fTimeDelta;
			i->_fAge += fTimeDelta;

			if (i->_fAge > i->_fLifeTime)
			{
				i->_bIsAlive = false;

			}
		}
	}

	RemoveDeadParticles();
	CParticleSystem::Update_Object(fTimeDelta);

	// if (IsEmpty())
	// 	return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CShock_Powder::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CShock_Powder::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;
	_vec3 min = _vec3(-1.0f, 0.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	GetRandomVector(
		&_Attribute->_vVelocity,
		&min,
		&max);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	_Attribute->_color = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	_Attribute->_fAge = 0.0f;
}

void CShock_Powder::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CShock_Powder::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CShock_Powder* CShock_Powder::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin,
	_int _numParticles)
{
	CShock_Powder* Inst = new CShock_Powder(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;

	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CShock_Powder::Free()
{
	CParticleSystem::Free();
}


#pragma region Trajectory

// CTrajectory::~CTrajectory()
// {
// }
//
// _int CTrajectory::Update_Object(const _float& fTimeDelta)
// {
// 	return CParticleSystem::Update_Object(fTimeDelta);
// }
//
// void CTrajectory::Render_Object()
// {
// 	CParticleSystem::Render_Object();
// }
//
// void CTrajectory::Reset_Particle(Attribute* _Attribute)
// {
// }
//
// void CTrajectory::PreRender_Particle()
// {
// 	CParticleSystem::PreRender_Particle();
// }
//
// void CTrajectory::PostRender_Particle()
// {
// 	CParticleSystem::PostRender_Particle();
// }
//
// HRESULT CTrajectory::SetUp_Material()
// {
// }
//
// CTrajectory* CTrajectory::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName, _vec3* _origin,
// 	_int _numParticles)
// {
// }
//
// void CTrajectory::Free()
// {
// 	CParticleSystem::Free();
// }
// #pragma endregion
//
//
//
