//=============================================================================
//	インクルードファイル
//=============================================================================

#include "input.h"
#include "fade.h"
#include "C2DObj.h"
#include "scene.h"
#include "game.h"
#include "score_draw.h"
#include "CUserInterface.h"
#include "Cplayer.h"
#include "CTexture.h"
#include "sound.h"
#include "gamepad.h"
#include "CEnemy.h"
#include "sound.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	グローバル変数宣言
//=============================================================================
static int FPS_mCounter;
static bool g_bend;			//	フェードインアウトフラグ
static bool Clear_flg;		//	クリア判定
static float Percent;		//	制圧率
static float Enemy_Percent;
static float Mark_Percent;
static int score_counter;	//	秒数
static int score_total;		//	合計
static float score_per;		//	加算用
static float score_up;		//	加算用
static float Mark_per;
static int Ko_Count;
static int total;
static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;
C2DObj *pScore_total;
//=============================================================================
//	初期化処理
//=============================================================================

void Score_total_Initialize(void)
{
	PlaySound(RESULT_BGM);
	//C2DObj::Texture_Release(CTexture::TEX_SCREENSHOT);	//	テクスチャ破棄
	//C2DObj::Texture_Load(CTexture::TEX_SCREENSHOT);	//	テクスチャ読み込み
	//コントローラー情報取得
	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	pScore_total = new C2DObj;
	FPS_mCounter = 0;
	g_bend = false;
	Clear_flg = false;
	//score_per = 0;
	score_up = 0;
	Mark_per = 0;
	score_counter = 0;
	score_total = 0;
	total = 0;
	Percent = 0.0f;
	Enemy_Percent = 0.0f;
	Mark_Percent = 0;
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);	//	αテスト有効
}

//=============================================================================
//	終了処理
//=============================================================================

void Score_total_Finalize(void)
{
	delete pScore_total;
}

//=============================================================================
//	更新処理
//=============================================================================

void Score_total_Update(void)
{
	//コントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	//スコア画面でカウンタースタート
	score_counter++;



	//ゲージの長さに対する制圧率
	Enemy_Percent = 762 * score_per;
	Percent = 100 * score_per;
	//score_total = CUserInterFace::Get_UIScore();

	//ゲームオーバー
	if (Percent < 70)
	{
		Clear_flg = false;
	}
	//ゲームクリアー
	if (Percent >= 70)
	{
		Clear_flg = true;
	}

	//	スペースでタイトル画面へ
	if (!g_bend)
	{
		if (Keyboard_IsTrigger(DIK_SPACE) && score_counter >= 600 || js.rgbButtons[0] && score_counter >= 600 || js.rgbButtons[0] && score_counter >= 600)
		{
			Fade_Start(true, 3, 0, 0, 0);
			g_bend = true;
			score_counter = 0;
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			PlaySound(ENTER_SE);
			Fade_Start(false, 3, 0, 0, 0);
			Scene_Change(SCENE_INDEX_RESULT);
		}
	}
}

//=============================================================================
//	描画処理
//=============================================================================

void Score_total_Draw(void)
{

	pScore_total->m_Sprite_Draw(CTexture::TEX_SCREENSHOT, 0, 0, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_SCREENSHOT, 1), pScore_total->Texture_GetHeight(CTexture::TEX_SCREENSHOT, 1));

	//撃破数
	//文字
	pScore_total->m_Sprite_Draw(CTexture::TEX_RESULT_BREAK, 100, 340, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_RESULT_BREAK, 1), pScore_total->Texture_GetHeight(CTexture::TEX_RESULT_BREAK, 1));
	//値
	if (score_counter >= 100)
	{
		ResultScore_Draw02(500, 330, Ko_Count, 3, CTexture::TEX_UI_GEKIHA, 0);
		if (score_counter == 100)
		{
			PlaySound(SCOREHYOUJI_SE);
		}
	}


	// 線
	//文字
	pScore_total->m_Sprite_Draw(CTexture::TEX_RESULT_BOU, 50, 710, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_RESULT_BOU, 1), pScore_total->Texture_GetHeight(CTexture::TEX_RESULT_BOU, 1));

	//制圧率
	//文字
	pScore_total->m_Sprite_Draw(CTexture::TEX_RESULT_PER, 100, 510, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_RESULT_PER, 1), pScore_total->Texture_GetHeight(CTexture::TEX_RESULT_PER, 1));
	if (score_counter >= 200)
	{
		////ゲージ
		//pScore_total->m_Sprite_Draw(CTexture::TEX_PER_GAUGE01, 530, 465, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_PER_GAUGE01, 1), pScore_total->Texture_GetHeight(CTexture::TEX_PER_GAUGE01, 1));
		//pScore_total->m_Sprite_Draw(CTexture::TEX_PER_GAUGE02, 530, 465, 0, 0, score_per * 2, pScore_total->Texture_GetHeight(CTexture::TEX_PER_GAUGE02, 1));
		////指標
		//pScore_total->m_Sprite_Draw(CTexture::TEX_BER, 510 + score_per * 2, 440, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_BER, 1), pScore_total->Texture_GetHeight(CTexture::TEX_BER, 1));


		//ゲージ
		pScore_total->Sprite_Draw(CTexture::TEX_MP3, 631, 570, 0, 0, 791, 54, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		pScore_total->Sprite_Draw(CTexture::TEX_HP3, 631, 570, 0, 0, score_up, 54, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		pScore_total->Sprite_Draw(CTexture::TEX_UI_BER, 500, 537, 0, 0, 1024, 172, 0.0f, 0.0f, 1.0f, 0.8f, 0.0f);
		//指標					
		pScore_total->Sprite_Draw(CTexture::TEX_UI_MEMORI, 614 + 532, 550, 0, 0, 43, 38, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		pScore_total->Sprite_Draw(CTexture::TEX_UI_MEMORI, 614 + 532, 610, 0, 0, 43, 38, 43 / 2, 38 / 2, 1.0f, 1.0f, D3DXToRadian(180));

		//値
		ResultScore_Draw02(1540, 500, (int)Percent, 2, 0);

		if (score_counter == 200)
		{
			PlaySound(BARUP_SE);
		}
		//指標とゲージ動かしてるとこ
		if (score_up <= Enemy_Percent)
		{
			score_up += 3;
		}


	}






	//合計
	//文字
	pScore_total->m_Sprite_Draw(CTexture::TEX_RESULT_TOTAL, 140, 770, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_RESULT_TOTAL, 1), pScore_total->Texture_GetHeight(CTexture::TEX_RESULT_TOTAL, 1));

	if (score_counter >= 450)
	{
		if (Clear_flg == false)
		{
			//値
			if (score_counter == 450)
			{
				PlaySound(SCOREHYOUJI_SE);
			}
			ResultScore_Draw02(600, 770, score_total, 5, 0);
			//スコアまで
			if (score_total < C3DObj::Get_Score())
			{
				score_total += 100;
			}
		}
		if (Clear_flg == true)
		{

			if (score_counter >= 500)
			{
				if (score_counter == 500)
				{
					PlaySound(CLEARBONUS_SE);
				}
				//クリアボーナス用
				pScore_total->m_Sprite_Draw(CTexture::TEX_CLEAR_BONUS, 1460, 700, 0, 0, pScore_total->Texture_GetWidth(CTexture::TEX_CLEAR_BONUS, 1), pScore_total->Texture_GetHeight(CTexture::TEX_CLEAR_BONUS, 1));
				//2倍までプラス
				if (score_total < C3DObj::Get_Score())
				{
					score_total += 100;
				}

			}

			//値
			if (score_counter == 450)
			{
				PlaySound(SCOREHYOUJI_SE);
			}
			ResultScore_Draw02(600, 760, C3DObj::Get_Score() + score_total, 5, 0);
		}
	}

}

void ScorePer_Set(float score)
{
	score_per = score;
}

void KoCount_Set(int count)
{
	Ko_Count = count;
}