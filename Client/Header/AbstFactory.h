#pragma once
#include <functional>

class CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T, typename... Args>
	static T* Create(Args... args)
	{
		return T::Create(std::forward<Args>(args)...);
	}

	// read factory after graphic dev inited
	static void Ready_Factories(LPDIRECT3DDEVICE9 pGraphicDev);
	static void ReleaseGraphicDev() { Safe_Release(s_pGraphicDev); }

protected:
	static LPDIRECT3DDEVICE9 s_pGraphicDev;
};

class CPlayerFactory : CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T>
	static T* Create(const string& strFactoryTag, const wstring& wstrObjTag, const _matrix& matWorld)
	{
		T* pCasted = dynamic_cast<T*>(s_mapPlayerSpawner.find(strFactoryTag)->second());
		_ASSERT_CRASH(pCasted != nullptr);
		Engine::AddGameObject(LAYER_PLAYER, wstrObjTag, pCasted);

		CTransform* pTrans = pCasted->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC);
		pTrans->Set_WorldDecompose(matWorld);

		return pCasted;
	}

	static void Ready_PlayerFactory();

private:
	static map<string, std::function<CGameObject*()>> s_mapPlayerSpawner;
};

class CEnemyFactory : CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T>
	static T* Create(const string& strFactoryTag, const wstring& wstrObjTag, const _matrix& matWorld)
	{
		T* pCasted = dynamic_cast<T*>(s_mapEnemySpawner.find(strFactoryTag)->second());
		_ASSERT_CRASH(pCasted != nullptr);
		Engine::AddGameObject(LAYER_ENEMY, wstrObjTag, pCasted);

		CTransform* pTrans = pCasted->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC);
		pTrans->Set_WorldDecompose(matWorld);

		return pCasted;
	}

	static void Ready_EnemyFactory();

private:
	static map<string, std::function<CGameObject*()>> s_mapEnemySpawner;
	
};

class CEffectFactory : CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T>
	static T* Create(const string& strFactoryTag, const wstring& wstrObjTag, const _matrix& matWorld)
	{
		T* pCasted = dynamic_cast<T*>(s_mapEffectSpawner.find(strFactoryTag)->second());
		_ASSERT_CRASH(pCasted != nullptr);
		Engine::AddGameObject(LAYER_EFFECT, wstrObjTag, pCasted);

		// CTransform* pTrans = pCasted->Get_Component<CTransform>(L"Proto_TransformCom_root", ID_DYNAMIC);
		// pTrans->Set_WorldDecompose(matWorld);

		return pCasted;
	}

	static void Ready_EffectFactory();

private:
	static map<string, std::function<CGameObject*()>> s_mapEffectSpawner;
};

class CEnvFactory : CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T>
	static T* Create(const string& strFactoryTag, const wstring& wstrObjTag)
	{
		T* pCasted = dynamic_cast<T*>(s_mapEnvSpawner.find(strFactoryTag)->second());
		_ASSERT_CRASH(pCasted != nullptr);
		Engine::AddGameObject(LAYER_ENV, wstrObjTag, pCasted);

		return pCasted;
	}

	static void Ready_EnvFactory();

private:
	static map<string, std::function<CGameObject*()>> s_mapEnvSpawner;
};

class CBulletFactory : CAbstFactory
{
	friend class CImGuiMgr;
public:
	template<typename T>
	static T* Create(const string& strFactoryTag, const wstring& wstrObjTag, const _matrix& matWorld)
	{
		T* pCasted = dynamic_cast<T*>(s_mapBulletSpawner.find(strFactoryTag)->second());
		_ASSERT_CRASH(pCasted != nullptr);
		Engine::AddGameObject(LAYER_BULLET, wstrObjTag, pCasted);

		return pCasted;
	}

	static void Ready_BulletFactory();

private:
	static map<string, std::function<CGameObject*()>> s_mapBulletSpawner;

};