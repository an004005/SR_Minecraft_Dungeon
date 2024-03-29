#include "..\..\Header\FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr()
{
	AddFontResourceEx(L"../Bin/Resource/Font/Minecraft.ttf", FR_PRIVATE, NULL);
	AddFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Bold.ttf", FR_PRIVATE, NULL);
	AddFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Light.ttf", FR_PRIVATE, NULL);
	AddFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Regular.ttf", FR_PRIVATE, NULL);
}


CFontMgr::~CFontMgr()
{
	Free();
	RemoveFontResourceEx(L"../Bin/Resource/Font/Minecraft.ttf", FR_PRIVATE, NULL);
	RemoveFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Bold.ttf", FR_PRIVATE, NULL);
	RemoveFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Light.ttf", FR_PRIVATE, NULL);
	RemoveFontResourceEx(L"../Bin/Resource/Font/KimjungchulGothic-Regular.ttf", FR_PRIVATE, NULL);
}

HRESULT CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pFontTag, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	CMyFont*	pMyFont = Find_Font(pFontTag);

	if (nullptr != pMyFont)
		return E_FAIL;

	pMyFont = CMyFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pMyFont, E_FAIL);

	m_mapFont.insert({ pFontTag, pMyFont });

	return S_OK;
}

void CFontMgr::Render_Font(const wstring& pFontTag, const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	CMyFont*	pMyFont = Find_Font(pFontTag);
	NULL_CHECK(pMyFont);

	pMyFont->Render_Font(pString, pPos, Color);
}

CMyFont * CFontMgr::Find_Font(const wstring& pFontTag)
{
	// auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));
	auto iter = m_mapFont.find(pFontTag);
	if (iter == m_mapFont.end())
		return nullptr;
	
	return iter->second;
}

void CFontMgr::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}
