#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE	 { MODE_FULL, MODE_WIN };
	
// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum INFOID { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATIONID { ROT_X, ROT_Y, ROT_Z, ROT_END};

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y,DIMS_Z,  DIMS_END }; 

	//Maptool enums
	enum CubeFace {	FACE_LOOK,	FACE_BACK,	FACE_LEFT,	FACE_RIGHT,	FACE_UP, FACE_DOWN,	FACE_END};

	enum PICKOPTION {  PICK_CUBE, PICK_PLANT, PICK_DELETE, PICK_END};
	enum CUBETYPE { TYPE_LAND, TYPE_COLLISION, TYPE_DECO, TYPE_TEX, TYPE_END };
	/////////////////////////////////////////////////////////////////

	enum LAYERID
	{
		LAYER_ENV, // 지형, 맵
		LAYER_GAMEOBJ, // 기타 GameObject
		LAYER_ENEMY, // 적
		LAYER_PLAYER, // 플레이어
		LAYER_UI, // ui
		LAYER_EFFECT, // effect
		LAYER_BULLET,
		LAYER_END
	};

}

#endif // Engine_Enum_h__




