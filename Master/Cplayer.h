#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "C3DObj.h"
#include <d3dx9.h>

#define PLAYER_MAX (2)
#define MP_MAX (30)
#define HP_MAX (50)
//アトラクション待機時間
#define COOLTIME_COFFEE   (300.0f)
#define COOLTIME_FALL	  (400.0f)
#define COOLTIME_WHEEL	 (1800.0f)
#define COOLTIME_COASTER (1200.0f)
#define COOLTIME_POPCORN    (100.0f)
//アトラクションMP
#define COFFEE_MP (1)
#define FALL_MP (2)
#define POPCORN_MP (1)
#define WHEEL_MP (3)
#define COASTER_MP (3)

typedef enum
{
	SUMMONS_COFFEE,
	SUMMONS_FALL,
	SUMMONS_POPCORN,
	SUMMONS_WHEEL,
	SUMMONS_COASTER,

	SUMMONS_MAX,
}SUMMONS_TYPE;

class CPlayer :public C3DObj
{

public:

	CPlayer();
	~CPlayer();

	void GameBegin(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);	//	終了処理

	static CPlayer *PlayerCreate(void);
	static CPlayer *Get_Player(int IndexPlayer) { return m_pPlayer[IndexPlayer]; }
	//D3DXMATRIX Get_mtxkeep(void) { return m_mtxKeepTranslation; }
	static int Get_KoCount(void) { return m_KO_Count; }
	static void Add_KoCount(void) { m_KO_Count++; }
	static void Reset_KoCount(void) { m_KO_Count = 0; }
	static C3DObj *Get_Player(void);
	bool Get_DrawCheck(void) { return m_DrawCheck; }
	static bool GetCoaster_Enable(void) { return g_CosterMode; };
	
protected:


	bool m_DrawCheck;			//	描画フラグ
private:
	void Summons_Attraction(void);	//	アトラクション召喚
	void Initialize(void);	//	初期化
	void Player_Move(void);			//	移動処理
	void Player_GamepadMove(void);			//	移動処理
	void AngleChange(int index);	//	方向変換
	void ControllerAngleChange(int index);	//	コントローラーの方向変換
	void Player_Camera(void);		//	カメラ処理
	void Wall_Check(void);			//	カベ判定
	void Player_Damage(void);		//	プレイヤーダメージ
	void Cool_Time(void);			//	クールタイム処理
	void Mp_Add(void);				//	MP増加処理
	bool JoyDevice_IsTrigger(int nKey);//コントローラーのトリガー処理
	void Ornament_Check(void);
	int m_Direction;				//	方向
	bool m_MoveCheck;				//	移動フラグ
	int m_PadDirection;
	enum
	{
		DIRE_UP,		//　上
		DIRE_RIGHT,		//	右
		DIRE_DOWN,		//　下
		DIRE_LEFT,		//　左
		DIRE_UP_RIGHT,	//	右上
		DIRE_UP_LEFT,	//	左上
		DIRE_DOWN_RIGHT,//	右下
		DIRE_DOWN_LEFT,	//	左下

		DIRE_N,

	};//　方向


	static CPlayer *m_pPlayer[PLAYER_MAX];
	static int m_PlayerNum;
	int m_PlayerIndex;
	static int m_KO_Count;

	static bool g_CosterMode;

	int m_DrawCount;
	bool right_trigger = false;
	bool left_trigger = false;
	bool right_on;//右スティック入力確認
	
};



#endif // !

