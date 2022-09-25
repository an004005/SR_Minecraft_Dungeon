#include "stdafx.h"
#include "ImSequencerImpl.h"


void CImSequencerImpl::AddTransFrame(const int iCurrentFrame, const SkeletalPart* pPart)
{
	if (pPart == nullptr)
	{
		IM_LOG("Warning : Select part first");
		return;
	}

	auto itrPartName = find(m_vecPartName.begin(), m_vecPartName.end(), pPart->strName);
	if (itrPartName == m_vecPartName.end())
	{
		IM_LOG("Warning : Selecte part is not include in Animation");
		return;
	}

	auto itrFrame = m_CubeAnim.mapFrame.find(*itrPartName);
	if (itrFrame == m_CubeAnim.mapFrame.end())
	{
		IM_LOG("Warning : Selecte part is not include in Animation");
		return;
	}
	_float fTime = Frame2Sec(iCurrentFrame);
	_vec3 vScale;
	D3DXQUATERNION qRot;
	_vec3 vPos;

	D3DXMatrixDecompose(&vScale, &qRot, &vPos, &pPart->pTrans->m_matWorld);
	itrFrame->second.push_back({fTime, vScale, qRot, vPos});
}

void CImSequencerImpl::AddTransFrameRecur(const int iCurrentFrame, const SkeletalPart* pPart)
{
	AddTransFrame(iCurrentFrame, pPart);
	for (auto& child : pPart->vecChild)
	{
		AddTransFrameRecur(iCurrentFrame, child);
	}
}
