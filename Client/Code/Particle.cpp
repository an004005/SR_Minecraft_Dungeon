#include "stdafx.h"
#include "..\Header\Particle.h"
#include "Export_Utility.h"
#include "AbstFactory.h"
#include "SphereEffect.h"
#define			PI			3.141592f

#pragma region

#pragma endregion




#pragma region Particle System

#pragma region Attakc_Basic
CAttack_P::~CAttack_P() = default;

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

	return OBJ_NOEVENT;
}

void CAttack_P::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CAttack_P::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	//m_fSize = _Attribute->_fSize;

	m_fSize = GetRandomFloat(_Attribute->_fSize-0.1f,_Attribute->_fSize+0.1f);
	_vec3 min = _vec3(-3.0f, -3.0f, -3.0f);
	_vec3 max = _vec3(3.0f, 3.0f, 3.0f);

	GetRandomVector(
		&_Attribute->_vVelocity,
		&min,
		&max);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);
	_Attribute->_vVelocity *= 10.f;
	
	
	_Attribute->_fAge = 0.0f;
}

void CAttack_P::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
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
#pragma endregion

C3DBaseTexture::~C3DBaseTexture()
{
}

_int C3DBaseTexture::Update_Object(const _float& fTimeDelta)
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

	return OBJ_NOEVENT;
}

void C3DBaseTexture::Render_Object()
{
	CParticleSystem::Render_Object();
}

void C3DBaseTexture::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;
	_Attribute->_vPosition.y += 4.f;
	_Attribute->_vVelocity = _vec3(0.f, 3.f, 0.f);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	// 터지는 방향 조절
	// _Attribute->_vVelocity.y = GetRandomFloat(0.0f, 1.0f) * 3.f;

	_Attribute->_vVelocity.y += 3.f;

	_Attribute->_fAge = 0.0f;
}

void C3DBaseTexture::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void C3DBaseTexture::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

C3DBaseTexture* C3DBaseTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	C3DBaseTexture* Inst = new C3DBaseTexture(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;

	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void C3DBaseTexture::Free()
{
	CParticleSystem::Free();
}


#pragma region SpeedBoots
CSpeedBoots::~CSpeedBoots()
{
}

_int CSpeedBoots::Update_Object(const _float& fTimeDelta)
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

	return OBJ_NOEVENT;
}

void CSpeedBoots::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CSpeedBoots::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;
	_Attribute->_vPosition.y += 4.f;
	_Attribute->_vVelocity = _vec3(0.f, 3.f, 0.f);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	// 터지는 방향 조절
	// _Attribute->_vVelocity.y = GetRandomFloat(0.0f, 1.0f) * 3.f;

	_Attribute->_vVelocity.y += 3.f;

	_Attribute->_fAge = 0.0f;
}

void CSpeedBoots::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CSpeedBoots::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CSpeedBoots* CSpeedBoots::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	CSpeedBoots* Inst = new CSpeedBoots(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;

	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CSpeedBoots::Free()
{
	CParticleSystem::Free();
}




#pragma endregion

#pragma region SpeedBoots_Particle

CSpeedBoots_Particle::~CSpeedBoots_Particle()
{
}

_int CSpeedBoots_Particle::Update_Object(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
	{
		if (i->_bIsAlive)
		{
			i->_vPosition += i->_vVelocity * fTimeDelta;
			i->_fAge += fTimeDelta;

			if (_int(i->_fAge) % 2 == 0)
			{
				CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
				pPlayerTransform->Get_Info(INFO_POS, &i->_vPosition);
				i->_vPosition.y += 4.f;
			}

			if (i->_fAge > i->_fLifeTime)
			{
				i->_bIsAlive = false;

			}
		}
	}

	RemoveDeadParticles();
	CParticleSystem::Update_Object(fTimeDelta);

	return OBJ_NOEVENT;
}

void CSpeedBoots_Particle::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CSpeedBoots_Particle::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;

	// if (_Attribute->_fSize <= EPSILON)
	// 	m_fSize = _Attribute->_fSize;
	//
	// CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	// pPlayerTransform->Get_Info(INFO_POS, &_Attribute->_vVelocity);
	//
	// CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	// pPlayerTransform->Get_Info(INFO_POS, &_Attribute->_vPosition);
	// _Attribute->_vPosition.y += 10.f;

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	pPlayerTransform->Get_Info(INFO_POS, &_Attribute->_vPosition);

	_Attribute->_vPosition.y += 4.f;

	_vec3 min = _vec3(0.0f, -1.0f, 0.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	// m_tPosin.x = LONG(m_tInfo.fX + cosf(m_fAngle * (PI / 180.f)) * m_fDiagonal);
	// m_tPosin.y = LONG(m_tInfo.fY - sinf(m_fAngle * (PI / 180.f)) * m_fDiagonal);

	m_fRotAngle += GetRandomFloat(5.f, 10.f);

	_Attribute->_vVelocity.x += _Attribute->_vVelocity.x +  cosf(m_fRotAngle * (PI / 180.f));
	_Attribute->_vVelocity.y -= (_Attribute->_vVelocity.y) +   sinf(m_fRotAngle * (PI / 180.f));
	// _Attribute->_vVelocity.z = 0.f;

	_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(45.f));
	D3DXVec3TransformNormal(&_Attribute->_vVelocity, &_Attribute->_vVelocity, &matRot);

	// GetRandomVector(
	// 	&_Attribute->_vVelocity,
	// 	&min,
	// 	&max);
	
	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	_Attribute->_vVelocity *= m_fSpeed;
	// CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	// pPlayerTransform->Get_Info(INFO_POS, &_Attribute>_vVelocity);
	// _Attribute->_vPosition.y += 4.f;
	
	

	// _Attribute->_vPosition.x += _Attribute->_vPosition.x +  m_fSpeed * cosf(m_fRotAngle);
	//_Attribute->_vPosition.y -= _Attribute->_vPosition.y - m_fSpeed * sinf(m_fRotAngle);

	// _Attribute->_vVelocity *= m_fSpeed;
	// _Attribute->_vVelocity.x *= -5.f;
	//
	// _Attribute->_vVelocity.z *= -5.f;
	// _Attribute->_vVelocity.y *= -2.f;



	// 터지는 방향 조절
	// _Attribute->_vVelocity.y = GetRandomFloat(0.0f, 1.0f) * 3.f;


	_Attribute->_fAge = 0.0f;
}

void CSpeedBoots_Particle::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CSpeedBoots_Particle::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CSpeedBoots_Particle* CSpeedBoots_Particle::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	CSpeedBoots_Particle* Inst = new CSpeedBoots_Particle(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;
	Inst->m_fSpeed = 5.f;
	Inst->m_fRotAngle = 0.f;
	Inst->m_fRotSpeed = 10.f;
	Inst->m_fDiagonal = 0.f;
	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CSpeedBoots_Particle::Free()
{
	CParticleSystem::Free();
}

#pragma endregion



CFireWork_Fuze::~CFireWork_Fuze()
{
}

_int CFireWork_Fuze::Update_Object(const _float& fTimeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
	{
		if (i->_bIsAlive)
		{
			i->_vPosition += i->_vVelocity * fTimeDelta;
			i->_fAge += fTimeDelta;
			// CEffectFactory::Create<CLava_Particle>("Lava_Particle", L"Lava_Particle");

			if (i->_fAge > i->_fLifeTime)
			{
				vDeadPos = i->_vPosition;

				_float fColor = GetRandomFloat(0.0f, 1.0f);
				_float fBlend = GetRandomFloat(0.0f, 0.5f);
				_ULonglong tick = GetTickCount64();

				if(tick % 3 == 0)
					Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(vDeadPos, 0.5f, D3DXCOLOR(fColor, fBlend, 0, 0), 256, 0.5f);
				else if(tick % 3 == 1)
					Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(vDeadPos, 0.5f, D3DXCOLOR(0, fColor, fBlend, 0), 256, 0.5f);
				else if (tick % 3 == 2)
					Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(vDeadPos, 0.5f, D3DXCOLOR(fBlend, 0, fColor, 0), 256, 0.5f);
				else
					Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(vDeadPos, 0.5f, D3DXCOLOR(fColor, fBlend, fBlend, fColor), 256, 0.5f);

				 CEffectFactory::Create<CUVCircle>("Firwork_Circle", L"Firwork_Circle");

				i->_bIsAlive = false;
			}
		}
	}

	RemoveDeadParticles();
	CParticleSystem::Update_Object(fTimeDelta);


	return OBJ_NOEVENT;
}

void CFireWork_Fuze::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CFireWork_Fuze::Reset_Particle(Attribute* _Attribute)
{
	_Attribute->_bIsAlive = true;
	m_fSize = _Attribute->_fSize;

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	pPlayerTransform->Get_Info(INFO_LOOK, &_Attribute->_vVelocity);

	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	_Attribute->_vVelocity *= 20.f;
	_Attribute->_fAge = 0.0f;
}

void CFireWork_Fuze::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFireWork_Fuze::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CFireWork_Fuze* CFireWork_Fuze::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	CFireWork_Fuze* Inst = new CFireWork_Fuze(pGraphicDev);

	Inst->m_dwVtxBf_Size = 2048;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 512;
	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CFireWork_Fuze::Free()
{
	CParticleSystem::Free();
}


CFireWork::~CFireWork()
{
}

_int CFireWork::Update_Object(const _float& fTimeDelta)
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

	return OBJ_NOEVENT;
}

void CFireWork::Render_Object()
{
	CParticleSystem::Render_Object();
}

void CFireWork::Reset_Particle(Attribute* _Attribute)
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

		_Attribute->_vVelocity *= 10.f;

	_Attribute->_fAge = 0.0f;
}

void CFireWork::PreRender_Particle()
{
	CParticleSystem::PreRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFireWork::PostRender_Particle()
{
	CParticleSystem::PostRender_Particle();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CFireWork* CFireWork::Create(LPDIRECT3DDEVICE9 pGraphicDev, LPCWSTR _TexFileName)
{
	CFireWork* Inst = new CFireWork(pGraphicDev);
	Inst->Reset_tmp();
	Inst->m_dwVtxBf_Size = 4096 * 2;
	Inst->m_dwVtxBf_Offset = 0;
	Inst->m_dwVtxBf_BatchSize = 1024 * 2;
	if (FAILED(Inst->Ready_Object(_TexFileName)))
	{
		return nullptr;
	}
	return Inst;
}

void CFireWork::Free()
{
	CParticleSystem::Free();
}





#pragma region UVAnimation
#pragma region ShockPowder

CShock_Powder::CShock_Powder(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	
}

CShock_Powder::~CShock_Powder()
{
}

HRESULT CShock_Powder::Ready_Object()
{
	static _vec3 vv{ 0.f,0.f,0.f };
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_RcShaderCom", L"Proto_RcShaderCom",ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom",L"Proto_TransformCom",ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Electric", L"Proto_Electric", ID_STATIC);
	m_pBufferCom->Set_TextureOption(_uint(CGameUtilMgr::GetRandomFloat(10.f, 15.f)), 4, 4);


	m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	
	m_pTransCom->Set_Pos(pPos.x + CGameUtilMgr::GetRandomFloat(-3.f, 3.f),
		pPos.y + 0.5f, pPos.z + CGameUtilMgr::GetRandomFloat(-3.f, 3.f));
	IM_LOG("%f, %f", m_pTransCom->m_vInfo[INFO_POS].x,  m_pTransCom->m_vInfo[INFO_POS].z);
	//m_pTransCom->Set_Scale(_float(rand() % 2)  + 1, _float(rand() % 2) + 1, _float(rand() % 2 ) + 1);

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CShock_Powder::Update_Object(const _float& fTimeDelta)
{
	if (m_fCurTime > m_fTime)
		return OBJ_DEAD;
	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);
	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	m_pTransCom->Set_Scale(CGameUtilMgr::GetRandomFloat(0.7f,2.5f), CGameUtilMgr::GetRandomFloat(0.7f, 2.5f), CGameUtilMgr::GetRandomFloat(0.7f, 2.5f));

	_float fY = CGameUtilMgr::GetRandomFloat(0.f, 2.f);
	m_pTransCom->Rotation(ROT_Y, D3DXToRadian(fY * fTimeDelta));

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CShock_Powder::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CShock_Powder::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CShock_Powder::PreRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CShock_Powder::PostRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CShock_Powder* CShock_Powder::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShock_Powder* Inst = new CShock_Powder(pGraphicDev);

	if (FAILED(Inst->Ready_Object()))
	{
		return nullptr;
	}
	return Inst;
}

void CShock_Powder::Free()
{
	CGameObject::Free();
}


#pragma endregion

#pragma region UVCircle
CUVCircle::~CUVCircle()
{
}

HRESULT CUVCircle::Ready_Object(_float _size, CIRCLETYPE _type)
{
	static _vec3 vv{ 0.f,0.f,0.f };
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_ShaderCircleCom", L"Proto_RcShaderCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Circle", L"Proto_Circle", ID_STATIC);
	m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());

	if (_type == SHOCK)
	{
		m_pBufferCom->Set_TextureOption(2, 4, 2);
		CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayerTransform->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 0.5f, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
	}
	else if (_type == FIREWORK)
	{
		m_pBufferCom->Set_TextureOption(3, 4, 2);
		_vec3 vPos = Get_GameObject<CFireWork_Fuze>(LAYER_EFFECT, L"FireWork_Fuze")->Get_FuzePos();
		m_pTransCom->Set_Pos(vPos.x, vPos.y + 0.5f, vPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
	}
	else if (_type == CREEPER)
	{
		// 크리퍼 포인터로 바꿔야함
		m_pBufferCom->Set_TextureOption(7, 4, 2);
		// m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y + 0.5f, m_vPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
	}
	else if (_type == GOLEM)
	{
		//골렘 포인터로 바꿔야함 손 위치 받아오기 
		m_pBufferCom->Set_TextureOption(3, 4, 2);
		CTransform*	pGolem = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pGolem->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 0.5f, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
	}

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CUVCircle::Update_Object(const _float& fTimeDelta)
{
	if (m_pBufferCom->Check_TextureCnt(3,1))
		return OBJ_DEAD;
	
	CGameObject::Update_Object(fTimeDelta);
	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;

}

void CUVCircle::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CUVCircle::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CUVCircle::PreRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CUVCircle::PostRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CUVCircle* CUVCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev,_float _size, CIRCLETYPE _type)
{
	CUVCircle* Inst = new CUVCircle(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size, _type)))
	{
		return nullptr;
	}
	return Inst;
}

void CUVCircle::Free()
{
	CGameObject::Free();
}

#pragma endregion

#pragma region UV Cloud
CCloud::~CCloud() = default;

HRESULT CCloud::Ready_Object(_float _size, CLOUDTYPE _type)
{
	
	m_pTexture = Add_Component<CTexture>(L"Proto_Cloud", L"Proto_Cloud_Cloud", ID_STATIC);

	

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);

	if (_type == WALK)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_WalkCloudCom", L"Proto_WalkCloudCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
		
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
		m_pBufferCom->Set_TextureOption(5, 4, 2);
		CTransform*	pPlayer = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayer->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 0.5f;
		m_fCurTime = 0.f;
		m_fSpeed = 0.75f;

		_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

		CGameUtilMgr::GetRandomVector(
			&m_vVelocity,
			&min,
			&max);

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;

	}
	else if (_type == ROLL)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_RollCloudCom", L"Proto_RollCloudCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransFormCom_CloudEffect", ID_DYNAMIC);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
		m_pBufferCom->Set_TextureOption(15, 4, 2);
		CTransform*	pPlayer = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayer->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 0.5f, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 1.5f;
		m_fCurTime = 0.f;
		m_fSpeed = 0.f;

		_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

		CGameUtilMgr::GetRandomVector(
			&m_vVelocity,
			&min,
			&max);

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
	}
	else if (_type == SHOCKPOWDER)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_ShockPowderCloudCom", L"Proto_ShockPowderCloudCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransFormCom_CloudEffect", ID_DYNAMIC);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
		m_pBufferCom->Set_TextureOption(20, 4, 2);
		CTransform*	pPlayer = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayer->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 0.5f, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 1.8f;
		m_fCurTime = 0.f;
		m_fSpeed = 3.5f;

		_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

		CGameUtilMgr::GetRandomVector(
			&m_vVelocity,
			&min,
			&max);

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
		m_vVelocity.y = 0.f;
	}
	else if (_type == CREEPEREX)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_CreeperExCloudCom", L"Proto_CreeperExCloudCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
		// 크리퍼 포인터로 바꿔야함
		m_pBufferCom->Set_TextureOption(_uint(CGameUtilMgr::GetRandomFloat(10.f,15.f)), 4, 2);

		// m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y + 0.5f, m_vPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = CGameUtilMgr::GetRandomFloat(0.5f, 2.f);
		m_fTime = 1.2f;
		m_fCurTime = 0.f;
		m_fSpeed = 3.5f;

		_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

		CGameUtilMgr::GetRandomVector(
			&m_vVelocity,
			&min,
			&max);

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
	}
	else if (_type == DECAL)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_WalkCloudCom", L"Proto_WalkCloudCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
		// m_pBufferCom->Set_TextureOption(_uint(CGameUtilMgr::GetRandomFloat(7.f, 15.f)), 4, 2);
		m_pBufferCom->Set_TextureOption(5, 4, 2);

		CTransform*	pPlayer = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		_vec3 pLook;
		pPlayer->Get_Info(INFO_POS, &pPos);
		pPlayer->Get_Info(INFO_LOOK, &pLook);

		m_pTransCom->m_vInfo[INFO_POS] = pPos + pLook * 2.f;
		// m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 0.7f;
		m_fCurTime = 0.f;
		m_fSpeed = 3.5f;

		_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

		CGameUtilMgr::GetRandomVector(
			&m_vVelocity,
			&min,
			&max);

		m_vVelocity.x += pLook.x;
		m_vVelocity.y += pLook.y;

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
		// m_vVelocity.x = 0.f;
		// m_vVelocity.z = 0.f;

	}
	// m_pTransCom->Set_Pos(pPos.x,
	// 	pPos.y, pPos.z);
	// IM_LOG("%f, %f", m_pTransCom->m_vInfo[INFO_POS].x, m_pTransCom->m_vInfo[INFO_POS].z);

	// _vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
	// _vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	// CGameUtilMgr::GetRandomVector(
	// 	&m_vVelocity,
	// 	&min,
	// 	&max);
	D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

void CCloud::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

CCloud* CCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, CLOUDTYPE _type)
{
	CCloud* Inst = new CCloud(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size, _type)))
	{
		return nullptr;
	}
	return Inst;
}

_int CCloud::Update_Object(const _float& fTimeDelta)
{
	if (m_fCurTime > m_fTime)
		return OBJ_DEAD;
	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;
	
	_vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];
	// 삼각함수 적용해보기
	// m_vVelocity.y = GetRandomFloat(0.5f, 1.0f) * 1.f;
	vPos += m_vVelocity * fTimeDelta * m_fSpeed;
	
	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CCloud::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}



void CCloud::PreRender_Particle()
{
	// m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CCloud::PostRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CCloud::Free()
{
	CGameObject::Free();
}

#pragma endregion

#pragma region Crack


CCrack::~CCrack()
{
}

HRESULT CCrack::Ready_Object(_float _size, CRACKTYPE _type)
{
	static _vec3 vv{ 0.f,0.f,0.f };


	//m_pTransCom->Set_Scale(_float(rand() % 2)  + 1, _float(rand() % 2) + 1, _float(rand() % 2 ) + 1);
	//


	if (_type == GOLEM_SPIT_CRACK)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_CrackCom", L"Proto_CrackCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
		m_pTexture = Add_Component<CTexture>(L"Proto_Crack", L"Proto_Crack", ID_STATIC);
		m_pBufferCom->Set_TextureOption(15, 2, 2);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
		CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayerTransform->Get_Info(INFO_POS, &pPos);
		
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 4.f, pPos.z);
		IM_LOG("%f, %f", m_pTransCom->m_vInfo[INFO_POS].x, m_pTransCom->m_vInfo[INFO_POS].z);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_iCrackType = GOLEM_SPIT_CRACK;
	}

	else if (_type == EXE_DECAL)
	{
		m_pBufferCom = Add_Component<CRcShader>(L"Proto_Crack_DecalCom", L"Proto_Crack_DecalCom", ID_STATIC);
		m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
		m_pTexture = Add_Component<CTexture>(L"Proto_Crack", L"Proto_Crack", ID_STATIC);
		m_pBufferCom->Set_TextureOption(15, 2, 2);
		m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
		CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		_vec3 pLook;
		pPlayerTransform->Get_Info(INFO_POS, &pPos);
		pPlayerTransform->Get_Info(INFO_LOOK, &pLook);
		m_pTransCom->m_vInfo[INFO_POS] = pPos + pLook * 4.f;
		m_pTransCom->m_vInfo[INFO_POS].y = pPos.y + 0.3f;

		// m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);
	
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 0.6f;
		m_fCurTime = 0.f;
		m_iCrackType = EXE_DECAL;

	}

	// 열거체 받아서 추가 폭발 설정하기
	m_fCurTime = 0;
	m_fTime = 1.0f;
	m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CCrack::Update_Object(const _float& fTimeDelta)
{
	if (m_fCurTime > m_fTime)
	{
		if (m_iCrackType == GOLEM_SPIT_CRACK)
		{
			CEffectFactory::Create<CSphereEffect>("Golem_Melee_M", L"Golem_Melee_M", m_pTransCom->m_vInfo[INFO_POS]);
			CEffectFactory::Create<CSphereEffect>("Golem_Melee_Shpere_M", L"Golem_Melee_Shpere_M", m_pTransCom->m_vInfo[INFO_POS]);
			Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(m_pTransCom->m_vInfo[INFO_POS], 0.3f, D3DXCOLOR(0.8f, 0.25f, 0.1f, 1.f), 5, 0.5f);
		}
		return OBJ_DEAD;
	}
	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);
	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	//m_pTransCom->Set_Scale(CGameUtilMgr::GetRandomFloat(0.7f, 2.5f), CGameUtilMgr::GetRandomFloat(0.7f, 2.5f), CGameUtilMgr::GetRandomFloat(0.7f, 2.5f));

	//_float fY = CGameUtilMgr::GetRandomFloat(0.f, 2.f);
	//m_pTransCom->Rotation(ROT_Y, D3DXToRadian(fY * fTimeDelta));

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CCrack::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CCrack::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CCrack::PreRender_Particle()
{
}

void CCrack::PostRender_Particle()
{
}

CCrack* CCrack::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, CRACKTYPE _type)
{
	CCrack* Inst = new CCrack(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size,_type)))
	{
		return nullptr;
	}
	return Inst;
}

void CCrack::Free()
{
	CGameObject::Free();
}
#pragma endregion




#pragma region GolemSpit
CGolemSpit::~CGolemSpit()
{
	// Get_GameObject<CFireWork>(LAYER_EFFECT, L"FireWork")->Add_Particle(m_pTransCom->m_vInfo[INFO_POS], 0.3f, D3DXCOLOR(1.f, 1.f, 0.2f, 0), 256, 0.4f);
	// CEffectFactory::Create<CUVCircle>("Creeper_Explosion", L"Creeper_Explosion", m_pTransCom->m_vInfo[INFO_POS]);
	// Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(m_pTransCom->m_vInfo[INFO_POS], 0.3f, RED, 30, 0.5f);
	// for (int i = 0; i < 5; i++)
	// {
	// 	CEffectFactory::Create<CCloud>("Creeper_Cloud", L"Creeper_Cloud", m_pTransCom->m_vInfo[INFO_POS]);
	// }
}

HRESULT CGolemSpit::Ready_Object(_float _size)
{
	m_pTexture = Add_Component<CTexture>(L"Proto_Spit", L"Proto_Spit", ID_STATIC);
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_SpitCom", L"Proto_SpitCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));

	

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);

	m_pTransCom->m_vInfo[INFO_POS].y += 5.f;

		m_pTransCom->Rotation(ROT_X, D3DXToRadian(45.f));
		m_pBufferCom->Set_TextureOption(0, 0, 0);
		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fTime = 0.5f;
		m_fCurTime = 0.f;
		m_fSpeed = 20.f;

	
		pPlayerTransform->Get_Info(INFO_LOOK, &m_vVelocity);
		// _vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
		// _vec3 max = _vec3(1.0f, 1.0f, 1.0f);
		//
		// CGameUtilMgr::GetRandomVector(
		// 	&m_vVelocity,
		// 	&min,
		// 	&max);

		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
		// m_vVelocity.x = 0.f;
		// m_vVelocity.z = 0.f;
		
		m_fSpeed = 1.5f;
		m_pTransCom->Update_Component(0.f);
		return S_OK;
}

_int CGolemSpit::Update_Object(const _float& fTimeDelta)
{

	if (m_fCurTime > m_fTime)
	{
		CEffectFactory::Create<CSphereEffect>("Golem_Melee_M", L"Golem_Melee_M", m_pTransCom->m_vInfo[INFO_POS]);
		CEffectFactory::Create<CSphereEffect>("Golem_Spit_Sphere", L"Golem_Spit_Sphere", m_pTransCom->m_vInfo[INFO_POS]);
		CEffectFactory::Create<CUVCircle>("Creeper_Explosion", L"Creeper_Explosion", m_pTransCom->m_vInfo[INFO_POS]);
		Get_GameObject<CAttack_P>(LAYER_EFFECT, L"Attack_Basic")->Add_Particle(m_pTransCom->m_vInfo[INFO_POS], 0.3f, D3DXCOLOR(0.8f,0.25f,0.1f,1.f), 5, 0.5f);
		for (int i = 0; i < 5; i++)
		{
			CEffectFactory::Create<CCloud>("Creeper_Cloud", L"Creeper_Cloud", m_pTransCom->m_vInfo[INFO_POS]);
		}
		return OBJ_DEAD;
	}
	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	_vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];
	// 삼각함수 적용해보기
	m_vVelocity *= CGameUtilMgr::GetRandomFloat(0.5f, 1.0f) * 1.f;

	m_pTransCom->m_vAngle.y += D3DXToRadian(60.f) * fTimeDelta;



	// vPos.x += m_vVelocity.x * fTimeDelta * m_fSpeed;
	// vPos.y -= m_vVelocity.y * fTimeDelta * m_fSpeed ;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CGolemSpit::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CGolemSpit::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CGolemSpit::PreRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

}

void CGolemSpit::PostRender_Particle()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);

}

CGolemSpit* CGolemSpit::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size)
{
	CGolemSpit* Inst = new CGolemSpit(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size)))
	{
		return nullptr;
	}
	return Inst;
}

void CGolemSpit::Free()
{
	CGameObject::Free();
}


#pragma endregion

CStun::~CStun()
{
}

HRESULT CStun::Ready_Object(_float _size)
{
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_StunCom", L"Proto_StunCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Stun", L"Proto_Stun", ID_STATIC);
	m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
	m_pBufferCom->Set_TextureOption(0,0,0);

	
	m_pTransCom->Set_Scale(_size, _size, _size);
	m_fSpeed = 6.f;
	m_fTime = 2.f;
	m_fCurTime = 0.f;
	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CStun::Update_Object(const _float& fTimeDelta)
{
	if (m_fCurTime >= m_fTime)
		return OBJ_DEAD;

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	m_pTransCom->Set_Pos(pPos.x, pPos.y + 3.f, pPos.z);

	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->m_vAngle.y += D3DXToRadian(40.f) * fTimeDelta * m_fSpeed;

	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CStun::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CStun::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CStun::PreRender_Particle()
{
}

void CStun::PostRender_Particle()
{
}

CStun* CStun::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size)
{
	CStun* Inst = new CStun(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size)))
	{
		return nullptr;
	}
	return Inst;
}

void CStun::Free()
{
	CGameObject::Free();
}




#pragma endregion 

#pragma region Lava_Particle

CLava_Particle::~CLava_Particle()
{
}

HRESULT CLava_Particle::Ready_Object(_float _size, LAVATYPE _type)
{
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_Lava_ParticleCom", L"Proto_Lava_ParticleCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Lava_Particle", L"Proto_Lava_Particle", ID_STATIC);
	// m_pTransCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
	m_pBufferCom->Set_TextureOption(6, 4, 2);

	if (_type == FALLINLAVA)
	{
		CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayerTransform->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);

		m_pTransCom->Set_Scale(_size, _size, _size);
		m_fSpeed = 3.f;
		m_fTime = 0.6f;
		m_fCurTime = 0.f;
	}
	else if (_type == FUZEPARTICLE)
	{
		// CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		// _vec3 pPos;
		// pPlayerTransform->Get_Info(INFO_POS, &pPos);
		// m_pTransCom->Set_Pos(pPos.x, pPos.y, pPos.z);
		//
		// m_pTransCom->Set_Scale(_size, _size, _size);
		// m_fSpeed = 3.f;
		// m_fTime = 0.6f;
		// m_fCurTime = 0.f;
	}
	
	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CLava_Particle::Update_Object(const _float& fTimeDelta)
{
	if (m_fCurTime >= m_fTime)
		return OBJ_DEAD;

	
	m_fCurTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->m_vAngle.y += D3DXToRadian(CGameUtilMgr::GetRandomFloat(-40.f,60.f)) * fTimeDelta * m_fSpeed;

	m_pTransCom->m_vInfo[INFO_POS].x += 5.f * fTimeDelta * m_fSpeed;

	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CLava_Particle::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();

}

void CLava_Particle::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CLava_Particle::PreRender_Particle()
{
}

void CLava_Particle::PostRender_Particle()
{
}

CLava_Particle* CLava_Particle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, LAVATYPE _type)
{
	CLava_Particle* Inst = new CLava_Particle(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size, _type)))
	{
		return nullptr;
	}
	return Inst;
}

void CLava_Particle::Free()
{
	CGameObject::Free();
}

#pragma endregion


#pragma region HealCircle


CHealCircle::~CHealCircle()
{
}

HRESULT CHealCircle::Ready_Object(_float _size, _float _rad)
{
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_HealCom", L"Proto_HealCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Heal", L"Proto_Heal", ID_STATIC);
	m_pTransCom->Rotation(ROT_X, D3DXToRadian(_rad));
	// m_pTransCom->Rotation(ROT_Z, D3DXToRadian(30.f));

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
	m_pBufferCom->Set_TextureOption(0, 0, 0);

	// CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	// _vec3 pPos;
	// pPlayerTransform->Get_Info(INFO_POS, &pPos);
	// m_pTransCom->Set_Pos(pPos.x, pPos.y+1.5f, pPos.z);

	m_pTransCom->Set_Scale(_size, _size, _size);
	m_fSpeed = 4.f;
	m_fTime = 1.2f;
	m_fCurTime = 0.f;
	m_pTransCom->Update_Component(0.f);

	return S_OK;
}

_int CHealCircle::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_fCurTime >= m_fTime)
	{
		return OBJ_DEAD;
	}

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	m_pTransCom->m_vInfo[INFO_POS].x = pPos.x;
	m_pTransCom->m_vInfo[INFO_POS].z = pPos.z;


	// Set_Pos(pPos.x, pPos.y + 1.5f, pPos.z);

	m_pTransCom->m_vInfo[INFO_POS].y += fTimeDelta * 10.f;

	// m_pTransCom->m_vScale *= 61.5f * fTimeDelta;

	if(m_pTransCom->m_vInfo[INFO_POS].y >= pPos.y + 3.f)
	{
		m_pTransCom->m_vInfo[INFO_POS].y = pPos.y;
	}

	m_fCurTime += fTimeDelta;
	

	// m_pTransCom->m_vAngle.y += D3DXToRadian(50.f) * fTimeDelta * m_fSpeed;

	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

void CHealCircle::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CHealCircle::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CHealCircle::PreRender_Particle()
{
}

void CHealCircle::PostRender_Particle()
{
}

CHealCircle* CHealCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size, _float _rad)
{
	CHealCircle* Inst = new CHealCircle(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size, _rad)))
	{
		return nullptr;
	}
	return Inst;
}

void CHealCircle::Free()
{
	CGameObject::Free();
}

#pragma endregion 

#pragma region HeartParticle

CHeartParticle::~CHeartParticle()
{
}

HRESULT CHeartParticle::Ready_Object(_float _size)
{
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_HeartParticleCom", L"Proto_HeartParticleCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransformCom", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Heal_Particle", L"Proto_Heal_Particle", ID_STATIC);
	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());
	m_pBufferCom->Set_TextureOption(_uint(CGameUtilMgr::GetRandomFloat(15.f,20.f)), 4, 1);

	m_pTransCom->Rotation(ROT_Y, D3DXToRadian(45.f));
	// pPlayerTransform->Get_Info(INFO_POS, &m_vOrigin);
	tmp = CGameUtilMgr::GetRandomFloat(-1.5f, 1.5f);
	desk = CGameUtilMgr::GetRandomFloat(-1.5f, 1.5f);
	m_pTransCom->Set_Scale(_size, _size, _size);
	m_fSpeed = CGameUtilMgr::GetRandomFloat(3.f, 7.f);
	m_fTime = 1.5f;
	m_fCurTime = 0.f;

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

_int CHeartParticle::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);



	if (m_fCurTime >= m_fTime)
		return OBJ_DEAD;


	m_fCurTime += fTimeDelta;
	if(_int(fTimeDelta)% 2 == 0)
		m_pTransCom->m_vScale *= fTimeDelta * 58.5f;
	// else 
	// 	m_pTransCom->m_vScale *= fTimeDelta * 61.f;

	m_pTransCom->m_vAngle.y += D3DXToRadian(CGameUtilMgr::GetRandomFloat(-40.f, 60.f)) * fTimeDelta * m_fSpeed;

	// m_pTransCom->m_vInfo[INFO_POS].x += acosf(D3DXToRadian(20.f)) * fTimeDelta * 10.f;
	// m_pTransCom->m_vInfo[INFO_POS].y += sinf(D3DXToRadian(20.f)) * fTimeDelta * 10.f;
	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	// m_pTransCom->Set_Pos(pPos.x + CGameUtilMgr::GetRandomFloat(-1.f, 1.f), pPos.y, pPos.z + CGameUtilMgr::GetRandomFloat(-1.f, 1.f));



	m_pTransCom->m_vInfo[INFO_POS].x = pPos.x +tmp;
	m_pTransCom->m_vInfo[INFO_POS].z = pPos.z + desk;

	// 	+ CGameUtilMgr::GetRandomFloat(-1.f, 1.f);
	// +CGameUtilMgr::GetRandomFloat(-1.f, 1.f);
	m_pTransCom->m_vInfo[INFO_POS].y += fTimeDelta * m_fSpeed;


	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;

	Add_RenderGroup(RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CHeartParticle::Render_Object()
{
	CGameObject::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CHeartParticle::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

void CHeartParticle::PreRender_Particle()
{
}

void CHeartParticle::PostRender_Particle()
{
}

CHeartParticle* CHeartParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _size)
{
	CHeartParticle* Inst = new CHeartParticle(pGraphicDev);

	if (FAILED(Inst->Ready_Object(_size)))
	{
		return nullptr;
	}
	return Inst;
}

void CHeartParticle::Free()
{
	CGameObject::Free();
}
#pragma endregion


#pragma endregion



