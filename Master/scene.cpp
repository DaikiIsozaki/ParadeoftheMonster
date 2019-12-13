//=============================================================================
//	インクルードファイル
//=============================================================================
#include"scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "Score_total.h"
#include "manual.h"
//=============================================================================
//	グローバル変数宣言
//=============================================================================

static SCENE_INDEX g_SceneIndex;
static SCENE_INDEX g_SceneNextIndex;

typedef void(*SceneFunc)(void);	//呪文

static const SceneFunc Initialize[] = {
	Title_Initialize,
	Manual_Init,
	Game_Initialize,
	Score_total_Initialize,
	Result_Initialize
	//	関数のポインタは関数名のみ
};

static const SceneFunc Finalize[] = {
	Title_Finalize,
	Manual_Fainalize,
	Game_Finalize,
	Score_total_Finalize,
	Result_Finalize
};

static const SceneFunc Update[] = {
	Title_Update,
	Manual_Update,
	Game_Updata,
	Score_total_Update,
	Result_Update
};

static const SceneFunc Draw[] = {
	Title_Draw,
	Manual_Draw,
	Game_Draw,
	Score_total_Draw,
	Result_Draw
};

//=============================================================================
//	初期化処理
//=============================================================================

void Scene_Initialize(SCENE_INDEX index)
{
	g_SceneNextIndex = g_SceneIndex = index;

	Initialize[g_SceneIndex]();
	//	Initializeのg_SceneIndex番目を実行

}

//=============================================================================
//	終了処理
//=============================================================================

void Scene_Finalize(void)
{
	Finalize[g_SceneIndex]();
}

//=============================================================================
//	更新処理
//=============================================================================

void Scene_Update(void)
{
	Update[g_SceneIndex]();
}

//=============================================================================
//	描画処理
//=============================================================================

void Scene_Draw(void)
{
	Draw[g_SceneIndex]();
}

//=============================================================================
//	関数定義
//=============================================================================

// シーンチェック
void Scene_check(void)
{
	if (g_SceneIndex != g_SceneNextIndex)
	{
		Scene_Finalize();
		Scene_Initialize(g_SceneNextIndex);
	}
}

// シーンチェンジ
void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}


