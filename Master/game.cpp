
//=============================================================================
//	インクルードファイル
//=============================================================================
#define NOMINMAX

#include "CLight.h"
#include "CCamera.h"
#include "game.h"
#include "CMeshField.h"
#include "CMeshField_Cylinder.h"
#include "CMesh_SkyDome.h"
#include "Cplayer.h"
#include "billboard.h"
#include "CEnemy.h"
#include "fade.h"
#include "scene.h"
#include "CUserInterface.h"
#include "CAttraction.h"
#include "common.h"
#include "input.h"
#include "exp.h"
#include "shadow.h"
#include "COrnament.h"
#include "sound.h"
#include <crtdbg.h>
#include "Score_total.h"
#include "fade.h"
//=============================================================================
//	定数定義
//=============================================================================
#define FADE_WAIT (255.0f)
#define FADE_SPEED (8)//4の倍数


//=============================================================================
//	グローバル宣言
//=============================================================================
static bool g_bend;				//	フェードインアウトフラグ
static bool g_gameend;
static C2DObj *m_fade;

static int FaDe_counterX;

static float fade_in_u;
static float fade_in_v;
static float fade_u;
static float fade_v;
static float fade_wait;
static bool Fadraw;
static bool fade_ioFlg;
static bool Fwait_flg;
//=============================================================================
//	初期化処理
//=============================================================================
void Game_Initialize(void)
{
	g_bend = false;
	g_gameend = false;
	
	CCamera::Camera_Create();
	fade_ioFlg = false;
	fade_wait = 0;
	Fadraw = false;
	Fwait_flg = false;
	/*C3DObj::InitModelLoad();

	CPlayer::PlayerCreate();				//	プレイヤー生成		
	CLight::Light_Create();					//	ライト生成
	CCamera::Camera_Create();				//	カメラ生成

	CMeshField::MeshField_Create(CTexture::TEX_KUSA_RENGA, 120.0f, 2, 2);							//	地面生成
	CMeshField_Cylinder::MeshField_Cylinder_Create(CTexture::TEX_MOUNTAIN, 10.0f, FIELDSIZE, 20, 1, false);	//	外カベ生成
	CMesh_SkyDome::Mesh_SkyDome_Create(CTexture::TEX_SKY, 2.0f, SKYSIZE, 40, 20);					//	空生成
	CUserInterFace::UICreate();				//	UI生成
	CEnemy::Create();						//	敵生成
	COrnament::Create();					//	オブジェクト生成*/
	//Shadow_Initialize();
	PlaySound(MAIN_BGM);
	Fadraw_Set(true, true);
}

//=============================================================================
//	終了処理
//=============================================================================

void Game_Finalize(void)
{
	/*C3DObj::DeleteAll();			//	3Dオブジェクト全消去
	CGameObj::DeleteAll2D();			//	2Dオブジェクト全消去

	C3DObj::Model_Finalize();*/
	CTexture::Texture_Release(CTexture::TEX_SCREENSHOT);
	CTexture::Texture_Load(CTexture::TEX_SCREENSHOT);
	KoCount_Set(CPlayer::Get_KoCount());
	ScorePer_Set(CEnemy::Get_EnemyPer());
	CEnemy::Reset_EnemyEnableNum();
	C3DObj::FinalizeAll();
	CCamera::Dalete_Camera();
	CGameObj::FrameCountReset();	//	フレームカウントリセット
	_CrtDumpMemoryLeaks();
}

//=============================================================================
//	更新処理
//=============================================================================

void Game_Updata(void)
{
	C3DObj::UpdateAll();	//	3Dオブジェクト更新
	CGameObj::UpdateAll();	//	2Dオブジェクト更新
	//COrnament::Create();    //  オブジェクト生成
	
	/*if (Keyboard_IsPress(DIK_1))
	{
		//Fadraw_Set(true, false);
	}*/
	if (Fadraw == true)
	{
		if (fade_ioFlg == false)
		{
			FaDe_counterX++;
			if (Fwait_flg == false)
			{
				if (FaDe_counterX == FADE_SPEED)
				{
					FaDe_counterX = 0;
					fade_in_v++;
					fade_in_u = 0;
				}
				if (FaDe_counterX % (FADE_SPEED / 4) == 0)
				{
					fade_in_u++;
				}
				if (fade_in_v == 6.0f && fade_in_u > 3)
				{
					Fwait_flg = true;
					Fade_Start(false, 1, 0, 0, 0);
					StopSound(MAIN_BGM);
					Scene_Change(SCTNE_INDEX_SCORE);
					PlaySound(GAME_END_SE);
				}
			}

			if (Fwait_flg == true)
			{
				fade_wait++;
				if (fade_wait > 255)
				{
					Fadraw = false;
					Fwait_flg = false;
					fade_wait = 0;
				}
			}
		}
		if (fade_ioFlg == true)
		{
			fade_wait++;
			if (fade_wait > FADE_WAIT)
			{
				FaDe_counterX++;
				if (FaDe_counterX == FADE_SPEED)
				{
					FaDe_counterX = 0;
					fade_v++;
					fade_u = 0;
				}
				if (FaDe_counterX % (FADE_SPEED / 4) == 0)
				{
					fade_u++;
				}
				if (fade_v > 6.0f)
				{
					FaDe_counterX = 0;
					fade_wait = 0;
					//fade_ioFlg = false;
					Fadraw = false;
				}
			}
		}
	}
	if (Fadraw == false)
	{
		fade_in_u = 0.0f;
		fade_in_v = 0.0f;
		fade_u = 0.0f;
		fade_v = 0.0f;
	}


	if (g_gameend)		//	FraemCountがGAMEENDになるまでカウントUP　なったら終了
	{
		if (!g_bend)
		{
			Fadraw_Set(true, false);
			//Fade_Start(true, 1, 0, 0, 0);
			g_bend = true;
		}
		else
		{
			if (!Fade_IsFade())
			{
				//Fade_Start(false, 1, 0, 0, 0);
				
				/*StopSound(MAIN_BGM);
				Scene_Change(SCTNE_INDEX_SCORE);
				PlaySound(GAME_END_SE);*/
			}
		}
	}
	else
	{
		CGameObj::FrameCountUp();
		if (GAMEEND <= CGameObj::Get_FraemCount())
		{
			g_gameend = true;
		}
	}
	Exp_Update();


	
}

//=============================================================================
//	描画処理
//=============================================================================

void Game_Draw(void)
{
	C3DObj::DrawAll();		//	3Dオブジェクト描画
	CGameObj::DrawAll();	//	2Dオブジェクト描画
	Exp_Draw();
	Shadow_Draw();
	if (Fadraw == true)
	{
		if (fade_ioFlg == false)
		{
			m_fade->Sprite_Draw(CTexture::TEX_EFFECT_FADE_IN, 0.0f, 0.0f, (int)(fade_in_u * 1920), (int)(fade_in_v * 1016), 1920, 1016);
		}
		if (fade_ioFlg == true)
		{
			m_fade->Sprite_Draw(CTexture::TEX_EFFECT_FADE, 0.0f, 0.0f, (int)(fade_u * 1920), (int)(fade_v * 1016), 1920, 1016);
		}
	}
	//C3DObj::HitCheck();
}


void Game_End(void)
{
	g_gameend = true;
}

bool Get_Gameend(void)
{
	return g_gameend;
}

void Fadraw_end(void)
{
	if (fade_v > 6.0f)
	{
		Fadraw = false;
	}
}

void Fadraw_Set(bool type, bool index)
{
	Fadraw = type;
	fade_ioFlg = index;
}