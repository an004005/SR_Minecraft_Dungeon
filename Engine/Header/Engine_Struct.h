#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTex
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;
	} VTXTEX;
	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort			_0;
		_ushort			_1;
		_ushort			_2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	typedef struct tagVertexCubeTex
	{
		_vec3		vPos;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);		// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT �� 3���� ũ�⸸ŭ�̸�, ��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ�ó UV ���� �������� �� �� �ִµ� ���� 0��° ���� ���� �����ϰڴٴ� �ǹ��̴�.


	typedef struct tagVertexCubeTex2
	{
		_vec3		vPos;
		_vec2		vTexUV;

	}VTXCUBE2;

	const _ulong		FVF_CUBE2 = D3DFVF_XYZ | D3DFVF_TEX1;

	struct VertexCubeNormal
	{
		_vec3 vPos;
		_vec3 vNormal;
		_vec3 vTexUV;
	};
	const _ulong		FVF_CUBE_NOR = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);		// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT �� 3���� ũ�⸸ŭ�̸�, ��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ�ó UV ���� �������� �� �� �ִµ� ���� 0��° ���� ���� �����ϰڴٴ� �ǹ��̴�.



	//MapImGuiTool
	typedef struct maptool
	{
		_int	iTexIdx;
		_int	iPlantIdx;
		_int	iCubeCount;
		_int	iPickingOption;
		_float	fHeight;
		_int	iCubeType;
		_bool	bRendState;

	}MapTool;
#pragma region Particle Struct
	

	

	struct Attribute
	{
		Attribute()
		{
			_fLifeTime = 0.0f;
			_fAge = 0.0f;
			_bIsAlive = true;
		}

		_vec3		_vPosition;	//	���� �����̽� ���� ��ƼŬ ��ġ 
		_vec3		_vVelocity;  //  ��ƼŬ�� �ӵ�, ������ �ʴ� �̵������� ���  
		_vec3		_vAcceleration;  // ��ƼŬ�� ����, ������ �ʴ� �̵������� ���
		_float		_fLifeTime;     // ��ƼŬ�� �Ҹ��� �� ���� �����Ǵ� �ð�
		_float      _fAge;          // ��ƼŬ�� ���� ����
		D3DXCOLOR   _color;        // ��ƼŬ�� ����
		D3DXCOLOR   _colorFade;    // ��ƼŬ�� �÷��� �ð��� �帧�� ���� ����ϴ� ���
		_bool       _bIsAlive;	   // ��ƼŬ�� ������ ��� True, �Ҹ��� ��� False
		_float		_fSize;
		_int		_iType;
	};
#pragma endregion



#pragma region Attack_Range
	enum ATTACKCIRCLETYPE
	{
		READY_CIRCLE = 0,
		ATTACK_CIRCLE
	};
	typedef struct AttackRangeOption
	{
		_float _fAcc;
		_float _fMaxAcc;

		_float _fLifeTime;
		_float _fTime;

		_vec3 _vMinSize;
		_vec3 _vMaxSize;

		_uint _iTotalFrame;
		_uint _iNextFrame;
		_vec3 _vLerpScale;

		ATTACKCIRCLETYPE _eRangeType;
	}ATKRNGOPTION;


#pragma endregion
}



#endif // Engine_Struct_h__
