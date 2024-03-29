#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
	// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum INFOID { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATIONID { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	//Maptool enums
	enum CubeFace { FACE_LOOK, FACE_BACK, FACE_LEFT, FACE_RIGHT, FACE_UP, FACE_DOWN, FACE_END };

	enum PICKOPTION { PICK_CUBE, PICK_PLANT, PICK_DELETE, PICK_END };

	enum CUBETYPE { TYPE_LAND, TYPE_COLLISION, TYPE_DECO, TYPE_TEX, TYPE_END };


	//ITEM

	enum ITEMTYPE { IT_MELEE, IT_RANGE, IT_LEGACY1, IT_LEGACY2, IT_LEGACY3, IT_RUNE, IT_END };
	
	enum ITEMSTATE { IS_TAKE, IS_DROP, IS_EQUIP, IS_END};

	enum ITEMEFFECT { IE_MONEY, IE_HEAL, IE_ARROW, IE_POSION, IE_END};

	// Object

	enum OBJTYPE { BIRD_WHITE, BIRD_BROWN, BIRD_END };
	/////////////////////////////////////////////////////////////////

	enum LAYERID
	{
		LAYER_ENV,
		// 지형, 맵
		LAYER_GAMEOBJ,
		// 기타 GameObject
		LAYER_ENEMY,
		// 적
		LAYER_PLAYER,
		// 플레이어
		LAYER_UI,
		// ui
		LAYER_EFFECT,
		// effect
		LAYER_BULLET,
		//item
		LAYER_ITEM,
		LAYER_END
	};


	enum COLLISION_TYPE
	{
		COLL_PLAYER,
		COLL_ENEMY,
		COLL_PLAYER_BULLET,
		COLL_ENEMY_BULLET,
		COLL_WALL,
		COLL_END
	};

	enum BLOCKING_TYPE
	{
		PUSH_EACH_OTHER,
		PUSH_OTHER_ONLY,
		CUSTOM_EVENT,
		BLOCKING_END // ignore
	};

	enum ArrowType
	{
		ARROW_NORMAL,
		ARROW_FIREWORK,
		ARROW_END
	};

}

#endif // Engine_Enum_h__
