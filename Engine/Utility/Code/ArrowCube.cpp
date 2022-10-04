#include "..\..\Header\ArrowCube.h"


CArrowCube::CArrowCube(LPDIRECT3DDEVICE9 pGraphicDev): CVIBuffer(pGraphicDev)
{
}

CArrowCube::CArrowCube(const CArrowCube& rhs): CVIBuffer(rhs)
{
}

CArrowCube::~CArrowCube()
{
}

HRESULT CArrowCube::Ready_Buffer()
{
	m_dwVtxCnt = 5 * 24;
	m_dwTriCnt = 5 * 12;
	m_dwVtxSize = sizeof(VTXCUBE2);
	m_dwFVF = FVF_CUBE2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	static _float w2 = 0.5f;
	static _float h2 = 0.5f;
	static _float d2 = 0.5f;
	static VTXCUBE2 tVtxCubeDefault[FACE_END][4]
	{
		{
			{{-w2, -h2, +d2}, {}},
			{{+w2, -h2, +d2}, {}},
			{{+w2, +h2, +d2}, {}},
			{{-w2, +h2, +d2}, {}}
		}, // look
		{
			{{-w2, -h2, -d2}, {}},
			{{-w2, +h2, -d2}, {}},
			{{+w2, +h2, -d2}, {}},
			{{+w2, -h2, -d2}, {}}
		}, // back
		{
			{{-w2, -h2, +d2}, {}},
			{{-w2, +h2, +d2}, {}},
			{{-w2, +h2, -d2}, {}},
			{{-w2, -h2, -d2}, {}}
		}, // left
		{
			{{+w2, -h2, -d2}, {}},
			{{+w2, +h2, -d2}, {}},
			{{+w2, +h2, +d2}, {}},
			{{+w2, -h2, +d2}, {}}
		}, // right
		{
			{{-w2, +h2, -d2}, {}},
			{{-w2, +h2, +d2}, {}},
			{{+w2, +h2, +d2}, {}},
			{{+w2, +h2, -d2}, {}}
		}, // up
		{
			{{-w2, -h2, -d2}, {}},
			{{+w2, -h2, -d2}, {}},
			{{+w2, -h2, +d2}, {}},
			{{-w2, -h2, +d2}, {}}
		}, // down
	};




	CVIBuffer::Ready_Buffer();


	return S_OK;
}

void CArrowCube::Render_Buffer()
{
	CVIBuffer::Render_Buffer();



	
}

CComponent* CArrowCube::Clone()
{
	_CRASH("Dont clone this");
	return nullptr;
}

void CArrowCube::Free()
{
	CVIBuffer::Free();
}
