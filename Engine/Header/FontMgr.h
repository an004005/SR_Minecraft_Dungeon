#pragma once

#include "Base.h"
#include "Engine_Include.h"
#include "MyFont.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr :public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& pFontTag,	 // Ű��
		const _tchar* pFontType, // ��ü
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void	Render_Font(const wstring& pFontTag,
						const _tchar* pString,
						const _vec2* pPos,
						D3DXCOLOR Color);
private:
	CMyFont*		Find_Font(const wstring& pFontTag);

private:
	map<wstring, CMyFont*>		m_mapFont;


private:
	virtual void	Free(void);
};


END