#pragma once
#include "Rune.h"
class CLaserShotRune : public CRune
{
private:
	explicit CLaserShotRune(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaserShotRune() override;

public:
	static CLaserShotRune* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

