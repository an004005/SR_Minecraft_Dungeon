#include "stdafx.h"
#include "..\Header\SoundMgr.h"
#include <io.h>

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	Ready_SoundMgr();
	m_fMasterVolume = 0.8f;
	m_fMaxListenDist = 45.f;
}

CSoundMgr::~CSoundMgr()
{
	Free();
}

void CSoundMgr::PlaySound(const wstring& pSoundKey, const _vec3& vSoundPos, float fVolume)
{
	fVolume *= m_fMasterVolume;

	const _vec3 vDiff = vSoundPos - m_vListenerPos;
	const _float fDistance = D3DXVec3Length(&vDiff);
	if (fDistance > m_fMaxListenDist) // �ʹ� �ִ�.
	{
		return;
	}

	map<TCHAR*, FMOD_SOUND*>::iterator iter; 

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), 
		[&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey.c_str(), iter.first);
	});
	
	if (iter == m_mapSound.end())
		return;

	_float fMinVolume = FLT_MAX;
	_uint iUseChan = 0;
	for (_uint i = 0; i < MAX_FREE_CHANNEL; ++i)
	{
		FMOD_BOOL bPlay = false;
		FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay);
		if (bPlay)
		{
			_float fChanVolume = 0.f;
			FMOD_Channel_GetVolume(m_pChannelArr[i], &fChanVolume);
			if (fChanVolume < fMinVolume)
			{
				fMinVolume = fChanVolume;
				iUseChan = i;
			}
		}
		else
		{
			iUseChan = i;
			break;
		}
	}

	fVolume = fVolume *  (1.f - (fDistance / m_fMaxListenDist));
	
	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[iUseChan]);

	FMOD_Channel_SetVolume(m_pChannelArr[iUseChan], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySoundRandom(const vector<wstring>& vecSoundKey, const _vec3& vSoundPos, float fVolume)
{
	PlaySound(vecSoundKey[rand() % vecSoundKey.size()], vSoundPos, fVolume);
}

void CSoundMgr::PlaySoundChannel(const wstring& pSoundKey, const _vec3& vSoundPos, CHANNELID eID, float fVolume)
{
	fVolume *= m_fMasterVolume;

	const _vec3 vDiff = vSoundPos - m_vListenerPos;
	const _float fDistance = D3DXVec3Length(&vDiff);
	if (fDistance > m_fMaxListenDist) // �ʹ� �ִ�.
	{
		return;
	}

	map<TCHAR*, FMOD_SOUND*>::iterator iter; 

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), 
		[&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey.c_str(), iter.first);
	});
	
	if (iter == m_mapSound.end())
		return;

	fVolume = fVolume *  (1.f - (fDistance / m_fMaxListenDist));

	FMOD_Channel_Stop(m_pChannelArr[eID]);

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySoundRandomChannel(const vector<wstring>& vecSoundKey, const _vec3& vSoundPos, CHANNELID eID,
	float fVolume)
{
	PlaySoundChannel(vecSoundKey[rand() % vecSoundKey.size()], vSoundPos, eID, fVolume);
}

// void CSoundMgr::PlaySoundRandom(float fVolume, const _vec3& vSoundPos int iNum, TCHAR* ...)
// {
// 	va_list VA_LIST;
// 	va_start(VA_LIST, iNum);
//
// 	int iIndex = rand() % iNum;
// 	for (int i = 0; i < iNum; ++i)
// 	{
// 		TCHAR* key = va_arg(VA_LIST, TCHAR*);
// 		if (i == iIndex)
// 			PlaySound(key, fVolume);
// 	}
//
// 	va_end(VA_LIST);
// }

void CSoundMgr::PlayBGM(const wstring& pSoundKey, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey.c_str(), iter.first);
	});
	
	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[SOUND_BGM]);
	
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_BOOL bPlay = false;
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);
	if (bPlay)
		FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::SetChannelPause(CHANNELID eID, bool bPause)
{
	FMOD_Channel_SetPaused(m_pChannelArr[eID], bPause);
}

void CSoundMgr::Update_Listener(LAYERID eID, const wstring& wstrListernTag)
{
	m_vListenerPos = Engine::Get_Component<CTransform>(eID, wstrListernTag, L"Proto_TransformCom", ID_DYNAMIC)
		->m_vInfo[INFO_POS];
}

void CSoundMgr::Ready_SoundMgr()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile(); 
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	intptr_t handle = _findfirst("../Bin/Resource/Sound/*.*", &fd);

	if (handle == -1L)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Resource/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		// ../Sound/"
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}

void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

