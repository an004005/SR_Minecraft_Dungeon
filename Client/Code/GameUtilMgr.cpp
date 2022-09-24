#include "stdafx.h"
#include "GameUtilMgr.h"


void CGameUtilMgr::WorldMatrixLerp(_matrix& matOut, const _matrix& matPrev, const _matrix& matNext, const _float fS)
{
	_vec3 vPrevPos, vNextPos; // pos
	_vec3 vPrevScale, vNextScale; // scale
	D3DXQUATERNION qPrevQuat, qNextQuat; // rot
	D3DXMatrixDecompose(&vPrevScale, &qPrevQuat, &vPrevPos, &matPrev);
	D3DXMatrixDecompose(&vNextScale, &qNextQuat, &vNextPos, &matNext);
	D3DXQuaternionNormalize(&qPrevQuat, &qPrevQuat);
	D3DXQuaternionNormalize(&qNextQuat, &qNextQuat);

	_vec3 vLerpPos = vPrevPos;
	_vec3 vLerpScale = vPrevScale;
	D3DXQUATERNION qLerpQuat = qPrevQuat;
	
	if (Vec3Cmp(vPrevPos, vNextPos) == false)
		D3DXVec3Lerp(&vLerpPos, &vPrevPos, &vNextPos, fS);

	if (Vec3Cmp(vPrevScale, vNextScale) == false)
		D3DXVec3Lerp(&vLerpScale, &vPrevScale, &vNextScale, fS);

	if (QuatCmp(qPrevQuat, qNextQuat) == false)
		D3DXQuaternionSlerp(&qLerpQuat, &qPrevQuat, &qNextQuat, fS);

	_matrix matScale, matRot, matTrans;
	D3DXMatrixScaling(&matScale, vLerpScale.x, vLerpScale.y, vLerpScale.z);
	D3DXMatrixRotationQuaternion(&matRot, &qLerpQuat);
	D3DXMatrixTranslation(&matTrans, vLerpPos.x, vLerpPos.y, vLerpPos.z);

	matOut = matScale * matRot * matTrans;
}

void CGameUtilMgr::QuatToPitchYawRoll(const D3DXQUATERNION& q, _float& pitch, _float& yaw, _float& roll)
{
	// to radian version
	_float sqw = q.w * q.w;
	_float sqx = q.x * q.x;
	_float sqy = q.y * q.y;
	_float sqz = q.z * q.z;

	pitch = asinf(2.f * (q.w * q.x - q.y * q.z));
	yaw = atan2f(2.f * (q.x * q.z + q.w * q.y), (-sqx - sqy + sqz + sqw));
	roll = atan2f(2.f * (q.x * q.y + q.w * q.z), (-sqx + sqy - sqz + sqw));
}

bool CGameUtilMgr::Vec3Cmp(const _vec3& v1, const _vec3& v2, _float fEpsilon)
{
	return fabs(v1.x - v2.x) < fEpsilon
		&& fabs(v1.y - v2.y) < fEpsilon
		&& fabs(v1.z - v2.z) < fEpsilon;
}

bool CGameUtilMgr::QuatCmp(const D3DXQUATERNION& q1, const D3DXQUATERNION& q2, _float fEpsilon)
{
	return fabs(q1.x - q2.x) < fEpsilon
		&& fabs(q1.y - q2.y) < fEpsilon
		&& fabs(q1.z - q2.z) < fEpsilon
		&& fabs(q1.w - q2.w) < fEpsilon;
}

bool CGameUtilMgr::MatCmp(const _matrix& m1, const _matrix& m2, _float fEpsilon)
{
	for (_uint i = 0; i < 4; ++i)
	{
		for (_uint j = 0; j < 4; ++j)
		{
			if (fabs(m1(i, j) - m2(i, j)) >= fEpsilon)
			{
				return false;
			}
		}
	}

	return true;
}

void CGameUtilMgr::MatWorldCompose(_matrix& matOut, const _vec3& vScale, const D3DXQUATERNION& qRot,
	const _vec3& vPos)
{
	_matrix matScale;
	D3DXMatrixIdentity(&matOut);
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationQuaternion(&matOut, &qRot);

	matOut = matScale * matOut;

	matOut._41 = vPos.x;
	matOut._42 = vPos.y;
	matOut._43 = vPos.z;
}
