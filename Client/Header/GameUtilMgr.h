#pragma once
class CGameUtilMgr
{
public:
	static void WorldMatrixLerp(_matrix& matOut, const _matrix& matPrev, const _matrix& matNext, const _float fS);
	static void QuatToPitchYawRoll(const D3DXQUATERNION& q, _float& pitch, _float& yaw, _float& roll);
	static bool Vec3Cmp(const _vec3& v1, const _vec3& v2, _float fEpsilon = 0.000001f);
	static bool QuatCmp(const D3DXQUATERNION& q1, const D3DXQUATERNION& q2, _float fEpsilon = 0.000001f);
	static bool MatCmp(const _matrix& m1, const _matrix& m2, _float fEpsilon = 0.000001f);
	static void MatWorldCompose(_matrix& matOut, const _vec3& vScale, const D3DXQUATERNION& qRot, const _vec3& vPos);
};

