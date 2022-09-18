#pragma once
#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	RETURN = VK_RETURN,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	E_END
};

enum
{
	KEY_TYPE_COUNT = static_cast<_int>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<_int>(KEY_STATE::E_END),
};

class ENGINE_DLL CInputMgr : public CBase
{
	DECLARE_SINGLETON(CInputMgr)

private:
	explicit CInputMgr();
	virtual ~CInputMgr();

public:
	void Ready_Input(HWND hwnd);
	void Update_Input();

	// ������ ���� ��
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// �� ó�� ������ ��
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// �� ó�� ������ ���� ��
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

	void SetAxisMode(bool bAxisMode);
	_float GetVerticalAxis() const { return _fVertical; }
	_float GetHorizontalAxis() const { return _fHorizontal; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<_byte>(key)]; }

private:
	HWND _hwnd;
	vector<KEY_STATE> _states;

	bool _bAxisMode;
	RECT _rtClient;
	POINT _ptCenter;
	_float _fVertical;
	_float _fHorizontal;

public:
	virtual void Free(void) override;
};
END
