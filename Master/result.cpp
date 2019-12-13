//=============================================================================
//	インクルードファイル
//=============================================================================

#include "input.h"
#include "fade.h"
#include "C2DObj.h"
#include "scene.h"
#include <stdio.h>
#include "score_draw.h"
#include "common.h"
#include "gamepad.h"
#include "sound.h"
#include "C3DObj.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	グローバル変数宣言
//=============================================================================

static bool g_bend;		//	フェードインアウトフラグ
static int count = 0;
static int DrawCount = 0;
FILE *rank_pt;
//static int g_ranking[6] = {};
typedef struct {
	int score;
	bool ScoreCheck;
}RANKING;

RANKING g_ranking[7] = {};

void Ranking(void);
void Check(void);

C2DObj *presult;
C2DObj *presult2;
static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;
//=============================================================================
//	初期化処理
//=============================================================================

void Result_Initialize(void)
{
//	PlaySound(SOUND_LABEL_ED);
	presult = new C2DObj;
	presult2 = new C2DObj;
	g_bend = false;
	g_bend = false;
	count = 0;
	DrawCount = 0;
	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	for (int i = 0;i < 7;i++)
	{
		g_ranking[i].score = 0;
		g_ranking[i].ScoreCheck = false;
	}
	g_ranking[6].score = C3DObj::Get_Score();

	Ranking();
	Check();
}

//=============================================================================
//	終了処理
//=============================================================================

void Result_Finalize(void)
{
	C3DObj::Reset_Score();
	delete presult2;
	delete presult;
	StopSound();
}

//=============================================================================
//	更新処理
//=============================================================================

void Result_Update(void)
{
	//コントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	//	スペースでタイトル画面へ
	if (!g_bend)
	{
		if (Keyboard_IsTrigger(DIK_SPACE) || js.rgbButtons[0])
		{
			PlaySound(ENTER_SE);
			//PlaySound(SOUND_LABEL_SELECT);
			Fade_Start(true, 3, 0, 0, 0);
			g_bend = true;
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			StopSound(RESULT_BGM);
			Fade_Start(false, 3, 0, 0, 0);
			Scene_Change(SCENE_INDEX_TITLE);
		}
	}
	count++;
}

//=============================================================================
//	描画処理
//=============================================================================

void Result_Draw(void)
{
	presult->m_Sprite_Draw(CTexture::TEX_RESULT, 0, 0, 0, 0, presult->Texture_GetWidth(4, 1), presult->Texture_GetHeight(4, 1));

	for (int i = 0;i < 5;i++)
	{
		if (g_ranking[i].ScoreCheck)
		{
			if ((count % 60) > 25)
			{
				//Score_Draw(WINDOW_WIDTH/2, WINDOW_HIGHT / 2 + 50 * i, g_ranking[i].score, 5, 0, 1);
				ResultScore_Draw(WINDOW_WIDTH / 2 - 230, WINDOW_HIGHT / 2 - 190 + 120 * i, g_ranking[i].score, 5, CTexture::TEX_RANKING_NUM, 0);
			}
		}
		else
		{
			ResultScore_Draw(WINDOW_WIDTH / 2 - 230, WINDOW_HIGHT / 2 - 190 + 120 * i, g_ranking[i].score, 5, CTexture::TEX_RANKING_NUM, 0);
		}


	}
	ResultScore_Draw(WINDOW_WIDTH / 1.4f, WINDOW_HIGHT / 2 - 190 + 220, g_ranking[6].score, 5, CTexture::TEX_RANKING_NUM, 1);
	if ((count % 60) > 25)
	{
		//presult2->Sprite_Draw(CTexture::TEX_SPACE, 450, WINDOW_HIGHT - 90, 0, CTexture::Texture_GetHeight(CTexture::TEX_SPACE, 3) * 0, CTexture::Texture_GetWidth(CTexture::TEX_SPACE, 1), CTexture::Texture_GetHeight(CTexture::TEX_SPACE, 3));
	}
}


void Ranking(void)
{
	rank_pt = fopen("ranking.txt", "a");
	fprintf(rank_pt, "\n%d\n", g_ranking[6].score);
	fclose(rank_pt);

	rank_pt = fopen("ranking.txt", "r");
	while (fscanf(rank_pt, "%d", &g_ranking[5].score) != EOF)
	{
		for (int i = 0;i < 5;i++)
		{
			if (g_ranking[5].score >= g_ranking[i].score)
			{
				for (int k = 4;k >= i;k--)
				{
					if (k > i)
					{
						g_ranking[k].score = g_ranking[k - 1].score;
					}
					else
					{
						g_ranking[k].score = g_ranking[5].score;
					}
				}
				break;
			}
		}
	}
	fclose(rank_pt);
}


void Check(void)
{
	for (int i = 0;i < 6;i++)
	{
		if (g_ranking[i].score == g_ranking[6].score)
		{
			g_ranking[i].ScoreCheck = true;
			break;
		}
	}
}