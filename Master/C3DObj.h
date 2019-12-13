//////////////////////////////////////////////////
////
////	3Dオブジェクトクラス
////
//////////////////////////////////////////////////

#ifndef _3DOBJ_H_
#define _3DOBJ_H_
#include <crtdbg.h>

#include "CGameObj.h"
#include <d3dx9.h>
#include "CSkinAnimation.h"
#include "BSphere.h"

class C3DObj :virtual public CGameObj
{
public:
	C3DObj();
	~C3DObj();

	C3DObj(int type);
	typedef enum
	{
		A_BUTTON,
		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		L_BUTTON,
		R_BUTTON,
		BACK_BUTTON,
		START_BUTTON,
	} CONTROLLER;
	typedef enum
	{
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// エネミー
		TYPE_ATTRACTION,// アトラクション
		TYPE_COASTER,	// ジェットコースター
		TYPE_POPCORN,	// ポップコーン
		TYPE_STADBY,    // 建設中
		TYPE_ORNAMENT,	// 障害物
		TYPE_etc,		// その他

		TYPE_MAX

	} OBJTYPE;

	typedef enum {
		MODELL_PLAYER,
		MODELL_CUP_TABLE,
		MODELL_ENBAN,
		MODELL_HASIRA,
		MODELL_WHEEL,
		MODELL_COASTER,
		MODELL_CUP_BLUE,
		MODELL_CUP_YELLOW,
		MODELL_CUP_GREEN,
		MODELL_POPCORN,
		MODELL_STANDBY,
		MODELL_LAMP,
		MODELL_GOMIBAKO,
		MODELL_BENCH,
		MODELL_HUNSUI,
		MODELL_KANRANSYA_DODAI,
		MODELL_HUSEN,
		MODELL_MERI_DODAI,
		MODELL_MERI_UMA,
		MODELL_KANRANSYA,
		MODELL_GASUBONBE,
		MODELL_POTETO,
	}ModelFileData;
	enum ANIME_MODEL
	{
		MODELL_ANIME_PLAYER,
		MODELL_ANIME_SMALL,
		MODELL_ANIME_MIDDLE,
		MODELL_ANIME_SPECIAL,
		MODELL_ANIME_BIG,
		ANIME_MODEL_MAX,//アニメモデル最大数
	}AnimeModelFileData;;

	virtual void Finalize(void) ;
	virtual void Update(void) = 0;	//	更新
	virtual void Draw(void) = 0;	//	描画

	

	D3DXMATRIX Get_mtxWorld(void) { return m_mtxWorld; }				//	ワールド変換用行列取得
	D3DXMATRIX Get_mtxTranslation(void) { return m_mtxTranslation; }	//	移動用行列取得

	static void FinalizeAll();	// 全オブジェクト初期化
	static void UpdateAll();	// 全オブジェクト更新
	static void DrawAll();		// 全オブジェクト描画
	static void DeleteAll();	// 全オブジェクト削除
	void C3DObj_delete(void);	// オブジェクト削除	
	static C3DObj *Get(int nIdx);					// インスタンス取得
	int Get_3DObjIndex() { return m_3DObjIndex; }	// ワークインデックス取得
	int Get_3DObjType() { return m_3DObjType; }		// 種類取得
	int Get_Angle(void) { return (int)m_Angle; }	//	角度取得	
	int Get_Hp(void) { return m_Hp; }				//	HP取得
	int Get_Mp(void) { return m_Mp; }				//	MP取得
	int Get_Attck(void) { return m_Attack; }		//	MP取得
	int Get_MpStock(void) { return m_MpStock; }		//	MPストック取得
	int Get_SummonsNum(void) { return m_SummonsNum; } // 選択中アトラクション取得 
	int Get_CoolTime(int summonstype) { return m_CoolTime[summonstype]; }
	bool Get_AttacFlag(void) { return m_AttakFlag; }
	int Get_OrnamentType(void) { return m_OrnamentType; }
	bool Get_DamageFlag(void) { return m_DamageFlag; }
	static char* Get_AnimeFileName(int index) { return ANIME_MODEL_FILES[index].filename; }
	static void TitleFlag_Change(bool change) { m_TitleFlag = change; }

	static bool boRenderSphere;//当たり判定を可視化するかどうか？ true 可視化
	//	モデル読み込み
	static HRESULT InitModelLoad();
	//モデル情報取得
	THING* C3DObj::GetAnimeModel(void);
	//THING GetAnimeModel(int index);
	THING_NORMAL GetNormalModel(int index);
	//THING_NORMAL GetNormal(int index);
	THING_NORMAL GetNormalModel(void);
	//	終了処理
	static void Model_Finalize(void);
	static void Model_Finalize(int index);
	//当たり判定
	//static void HitCheck(void);
	//衝突検出 ノーマルモデル対ノーマルモデル
	static bool Collision_NomalVSNormal(THING_NORMAL*, THING_NORMAL*);
	//衝突検出 アニメモデル対ノーマルモデル
	static bool Collision_AnimeVSNormal(THING*, THING_NORMAL*);
	//衝突検出 アニメモデル対アニメモデル
	static bool Collision_AnimeVSAnime(THING*, THING*);

	bool C3DObj::Collision_AnimeVSNormalCapsule(THING*, THING_NORMAL*);
	float C3DObj::GetSqDistancePoint2Segment(THING* pThingA, THING_NORMAL* pThingB);
	void HitCheck(void);

	virtual bool Get_DrawCheck(void) = 0;
	void DamageFlag_Change(void);	//	ダメージフラグチェンジ
	void Position_Keep(D3DXMATRIX mtxT);
	
	static int Get_Score(void) { return m_TotalScore; }	//	スコア取得
	static void Reset_Score(void) { m_TotalScore = 0; }	//　スコアリセット
	static bool GetW_coaster(void) { return GetWCos; }
	static bool *GetCosterModeStandby(void) { return &g_CosterModeStandby; }//コースターを呼んでるとき; }//コースターを呼んでるとき
protected:
	//THING *Thing_Anime_model;//アニメモデル情報
	//static THING Thing_Anime[];//読み込むモデルの最大数+1

	THING_NORMAL Thing_Normal_model;//通常モデル情報

	D3DXMATRIX m_mtxWorld;			//	ワールド変換用行列
	D3DXMATRIX m_mtxTranslation;	//	移動行列
	D3DXMATRIX m_mtxRotation;		//	移動行列
	D3DXMATRIX m_mtxScaling;		//	移動行列
	D3DXMATRIX m_mtxKeepTranslation;	//	移動保持行列
	D3DXMATRIX m_mtxInit;			//	初期位置行列
	float m_Angle;			//	角度
	
	int m_MpStock;			//	MPストック
	int m_Hp;				//	HP
	int m_Mp;				//	MP
	int m_Attack;			//	攻撃
	int shadow_number;      //  影の番号
	bool m_DamageFlag;		//	ダメージフラグ
	bool m_AttakFlag;		//	アタックフラグ
	int m_SummonsNum;				//	召喚するアトラクションの番号

	D3DXVECTOR3 m_PosKeep;	//	ポジション保持
	//  0 coffee
	//  1 fall
	//  2 wheel
	//  3 coaster
	//  4 popcorn
	int m_CoolTime[5] = {0};	//	クールタイム
	// 描画処理
	void DrawDX_Anime(D3DXMATRIX mtxWorld, int type, THING* pThing);
	void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing);
	void C3DObj::DrawDX_NormalAdd(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position);
	void C3DObj::DrawDX_NormalAddScale(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position, D3DXVECTOR3 scale);
	void C3DObj::DrawDX_NormalCapsule(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position,float rotation);
	
	void Animation_Change(int index, float speed);
	LPD3DXANIMATIONSET pAnimSet[10];//選択したモデルに10個までのアニメーションをセット
	D3DXTRACK_DESC TrackDesc;
	SKIN_MESH SkinMesh;



	
	//static THING Thing[];//読み込むモデルの最大数+1
	THING Thing;//読み込むモデルの最大数+1

	

	void Add_Mp(int mp);	//	MP加算
	void Add_Hp(void);
	void Add_Score(int score) { m_TotalScore += score; }	//	スコア加算
	void Attraction_Delete(void);
	int m_OrnamentType;
	
	int m_AnimationType;
	static bool m_TitleFlag;
private:
	
	//	モデルデータの構造体
	typedef struct
	{
		char filename[256];

	}MaterialFileData;
	//	モデルデータの構造体
	typedef struct
	{
		char filename[256];

	}MaterialFileData2;
	//	使いたいモデルの数だけ書く
	static MaterialFileData NORMAL_MODEL_FILES[];
	static MaterialFileData2 ANIME_MODEL_FILES[];
	static int MODEL_FILES_MAX;	//	テクスチャ構造体総数
	static int ANIME_MODEL_FILES_MAX;	//	テクスチャ構造体総数
	static THING_NORMAL Thing_Normal[];//読み込むモデルの最大数+1
	static HRESULT InitThing(THING_NORMAL *pThing, LPSTR szXFileName);//ノーマルモデルの読み込み
	int m_3DObjIndex;		//	3Dオブジェクトインデックス
	int m_3DObjType;		//	3Dオブジェクトタイプ
	static int m_3DObjNum;	//	ゲームオブジェクト総数
	static int m_TotalScore;

	static C3DObj *p3DObj[MAX_GAMEOBJ];


	static bool VFCulling(D3DXVECTOR3* pPosition);
	

	static LPD3DXMESH m_pD3DXMesh[];			//	メッシュを受け取る変数
	static DWORD m_dwNumMaterials[];			//	マテリアルの総数	描画する回数
	static LPD3DXBUFFER m_pD3DXMtrBuffer[];		//	マテリアル情報を受け取る変数
	static LPDIRECT3DTEXTURE9 *m_pTexures[];	//　
	static D3DMATERIAL9 *m_pd3dMaterials[];		//　


	static bool GetWCos;
	static bool g_CosterModeStandby;
	#define MODEL_MAX (9)

	
	
	
	
	FLOAT fAnimTime = 0.0f;
	BOOL boPlayAnim = true;
	//float fAnimTimeHold;
};

#endif // !1
