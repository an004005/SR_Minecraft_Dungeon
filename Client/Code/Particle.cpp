#include "stdafx.h"
#include "..\Header\Particle.h"

#include "AbstFactory.h"

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

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	pPlayerTransform->Get_Info(INFO_LOOK, &_Attribute->_vVelocity);

	_vec3 min = _vec3(0.0f, -1.0f, 0.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	GetRandomVector(
		&_Attribute->_vVelocity,
		&min,
		&max);


	D3DXVec3Normalize(
		&_Attribute->_vVelocity,
		&_Attribute->_vVelocity);

	
	_Attribute->_vVelocity.x *= -5.f;
	
	_Attribute->_vVelocity.z *= -5.f;
	_Attribute->_vVelocity.y *= -2.f;



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

			if (i->_fAge > i->_fLifeTime)
			{
				vDeadPos = i->_vPosition;

				_float fColor = GetRandomFloat(0.0f, 1.0f);
				_float fBlend = GetRandomFloat(0.0f, 0.5f);
				ULONG tick = GetTickCount64();

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

		_Attribute->_vVelocity *= 15.f;

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
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom",L"Proto_TransFormCom_ShockEffect",ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Electric", L"Proto_Electric", ID_STATIC);
	m_pBufferCom->Set_TextureOption(GetRandomFloat(10, 15), 4, 4);


	m_pTransCom->Rotation(ROT_X, D3DXToDegree(90.f));

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);
	
	m_pTransCom->Set_Pos(pPos.x + GetRandomFloat(-3.f, 3.f),
		pPos.y + 0.5f, pPos.z + GetRandomFloat(-3.f, 3.f));
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

	m_pTransCom->Set_Scale(_float(rand() % 2) + 0.5f, _float(rand() % 2) + 0.5f, _float(rand() % 2) + 0.5f);

	_float fY = 1.0f;
	m_pTransCom->Rotation(ROT_Y, D3DXToDegree(fY * fTimeDelta));

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
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransFormCom_ShockEffect", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Circle", L"Proto_Circle", ID_STATIC);
	m_pTransCom->Rotation(ROT_X, D3DXToDegree(90.f));

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture());

	if (_type == SHOCK)
	{
		m_pBufferCom->Set_TextureOption(1, 4, 2);
		CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
		_vec3 pPos;
		pPlayerTransform->Get_Info(INFO_POS, &pPos);
		m_pTransCom->Set_Pos(pPos.x, pPos.y + 0.5f, pPos.z);
		m_pTransCom->Set_Scale(_size, _size, _size);
	}
	else
	{
		m_pBufferCom->Set_TextureOption(3, 4, 2);
		_vec3 vPos = Get_GameObject<CFireWork_Fuze>(LAYER_EFFECT, L"FireWork_Fuze")->Get_FuzePos();
		m_pTransCom->Set_Pos(vPos.x, vPos.y + 0.5f, vPos.z);
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

HRESULT CCloud::Ready_Object()
{
	static _vec3 vv{ 0.f,0.f,0.f };
	m_pBufferCom = Add_Component<CRcShader>(L"Proto_ShaderCloudCom", L"Proto_ShaderCloudCom", ID_STATIC);
	m_pTransCom = Add_Component<CTransform>(L"Proto_TransformCom", L"Proto_TransFormCom_CloudEffect", ID_DYNAMIC);
	m_pTexture = Add_Component<CTexture>(L"Proto_Cloud", L"Proto_Cloud_Cloud", ID_STATIC);
	m_pBufferCom->Set_TextureOption(15, 4, 2);

	m_pBufferCom->Set_Texture(m_pTexture->GetDXTexture(0));
	m_pTransCom->Rotation(ROT_X, D3DXToDegree(90.f));

	CTransform*	pPlayerTransform = Engine::Get_Component<CTransform>(LAYER_PLAYER, L"Player", L"Proto_TransformCom_root", ID_DYNAMIC);
	_vec3 pPos;
	pPlayerTransform->Get_Info(INFO_POS, &pPos);


	m_pTransCom->Set_Pos(pPos.x,
		pPos.y, pPos.z);
	IM_LOG("%f, %f", m_pTransCom->m_vInfo[INFO_POS].x, m_pTransCom->m_vInfo[INFO_POS].z);

	_vec3 min = _vec3(-1.0f, 1.0f, -1.0f);
	_vec3 max = _vec3(1.0f, 1.0f, 1.0f);

	GetRandomVector(
		&m_vVelocity,
		&min,
		&max);

	D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);

	m_pTransCom->Update_Component(0.f);
	return S_OK;
}

void CCloud::LateUpdate_Object()
{
	CGameObject::LateUpdate_Object();
}

CCloud* CCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCloud* Inst = new CCloud(pGraphicDev);

	if (FAILED(Inst->Ready_Object()))
	{
		return nullptr;
	}
	return Inst;
}

_int CCloud::Update_Object(const _float& fTimeDelta)
{
	if (m_pBufferCom->Check_TextureCnt(3, 1))
		return OBJ_DEAD;

	CGameObject::Update_Object(fTimeDelta);
	m_pBufferCom->m_matWorld = m_pTransCom->m_matWorld;
	
	_vec3& vPos = m_pTransCom->m_vInfo[INFO_POS];

	m_vVelocity.y = GetRandomFloat(0.0f, 1.0f) * 0.5f;
	
	vPos += m_vVelocity * fTimeDelta * 5;
	
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
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
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
#pragma endregion



