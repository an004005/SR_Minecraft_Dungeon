#ifndef Engine_Template_h__
#define Engine_Template_h__

#include "Engine_Typedef.h"
#include <functional>
#include "Base.h"

namespace Engine
{
	template <typename T> // Ŭ���� �����͵��� ����
	DWORD Safe_Release(T& pointer)
	{
		DWORD dwRefCnt = 0;

		if (NULL != pointer)
		{
			dwRefCnt = pointer->Release();

			if (dwRefCnt == 0)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destory(T& pointer)
	{
		if (NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

	template <typename T> // ���� �ڷ���, ����ü ������ ����
	void Safe_Delete(T& pointer)
	{
		if (NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T> // ���� �迭�� �����ϴ� �뵵
	void Safe_Delete_Array(T& pointer)
	{
		if (NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////
	/////////////////////////////////Functor �Լ���ü//////////////////////////

	//class CTag_Finder
	//{
	//public:
	//	explicit CTag_Finder(const _tchar* pTag)
	//		: m_pTargetTag(pTag)
	//	{
	//	}
	//	~CTag_Finder() {		}
	//public:
	//	template<typename T>
	//	bool operator()(const T& pair)
	//	{
	//		if (0 == lstrcmpW(m_pTargetTag, pair.first))
	//		{
	//			return true;
	//		}

	//		return false;
	//	}

	//private:
	//	const _tchar*		m_pTargetTag = nullptr;
	//};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void)
		{
		}

		~CDeleteObj(void)
		{
		}

	public: // operator
		template <typename T>
		void operator ()(T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// ���������̳� ������
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void)
		{
		}

		~CDeleteMap(void)
		{
		}

	public: // operator	
		template <typename T>
		void operator ()(T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};


	template <typename... Args>
	class BaseDelegater
	{
		using CallBack = std::function<void(Args ...)>;

	public:
		~BaseDelegater()
		{
			m_vecBinds.clear();
			m_vecBinds.shrink_to_fit();
		}

		void broadcast(Args&&... args)
		{
			for (auto itr = m_vecBinds.begin(); itr != m_vecBinds.end();)
			{
				if (itr->first->GetRef() == 0) // ref == 0�̶�� ���� �� ���������͸� �ش� ref�� ������ �ִٴ� ��
				{
					Safe_Release(itr->first);
					itr = m_vecBinds.erase(itr);
				}
				else
				{
					itr->second(args...);
					++itr;
				}
			}
		}

		template <typename T>
		void bind(T* obj, void (T::*memFunc)(Args ...))
		{
			NULL_CHECK(dynamic_cast<CBase*>(obj));

			CallBack callback = [obj, memFunc](Args ... args)
			{
				(obj->*memFunc)(args...);
			};

			obj->AddRef();
			m_vecBinds.emplace_back(std::make_pair(obj, callback));
		}

		template <typename T>
		void unbind(T* obj)
		{
			auto removedItr = remove_if(m_vecBinds.begin(), m_vecBinds.end(), [&obj](std::pair<void*, CallBack> e)
			{
				return e.first == obj;
			});
			m_vecBinds.erase(removedItr, m_vecBinds.end());
		}

	private:
		std::vector<std::pair<CBase*, CallBack>> m_vecBinds;
	};
}


#endif // Engine_Template_h__
