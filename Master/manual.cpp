///////////////////////////////////////////////////////////////////////////////////////
//
//			操作説明もろもろ manual
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//
//		ヘッダ読み込み
//
///////////////////////////////////////////////////////////////////////////////////////

#include "manual.h"
#include "C2DObj.h"
#include "input.h"
#include "gamepad.h"
#include "CTexture.h"
#include "fade.h"
#include "sound.h"
#include "scene.h"
#include "game.h"
/////////////////////////////////////////////////////////////////
//
//				マクロ
//
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
//
//				グローバル変数
//
/////////////////////////////////////////////////////////////////
static bool g_bend = false;	//	フェードインアウトフラグ
int ManualPage;		// 現在のページ
int M_alpha;
int FPS_mCounter;
static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;

void Manual_Init(void)
{
	g_bend = false;
	ManualPage = 1;
	FPS_mCounter = 0;
	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	PlaySound(CHUTO_BGM);
}

void Manual_Update(void)
{
	//コントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	// 次のページへ 箱コンの操作をここに
	if (!g_bend)
	{
		if (ManualPage == 1)
		{
			if (Keyboard_IsTrigger(DIK_D) && FPS_mCounter >= 30 || Keyboard_IsTrigger(DIK_SPACE) && FPS_mCounter >= 30 || js.lY >= -2 && js.lY <= 2 && js.lX == 6 && FPS_mCounter >= 30 || js.rgbButtons[0] && FPS_mCounter >= 30) // 押したら次のページへ
			{
				ManualPage = 2;
				FPS_mCounter = 0;
			}
		}
		if (ManualPage == 2)
		{
			// 前のページにもどる 箱コンの操作をここに
			if (Keyboard_IsTrigger(DIK_A) && FPS_mCounter >= 30 || js.lY >= -2 && js.lY <= 2 && js.lX == -6 && FPS_mCounter >= 30 || js.rgbButtons[1] && FPS_mCounter >= 30)
			{
				ManualPage = 1;
				FPS_mCounter = 0;
			}
			// 次のページへ 箱コンの操作をここに
			if (Keyboard_IsTrigger(DIK_D) && FPS_mCounter >= 30 || Keyboard_IsTrigger(DIK_SPACE) && FPS_mCounter >= 30 || js.lY >= -2 && js.lY <= 2 && js.lX == 6 && FPS_mCounter >= 30 || js.rgbButtons[0] && FPS_mCounter >= 30)
			{
				ManualPage = 3;
				FPS_mCounter = 0;
			}
		}
	}
	if (ManualPage == 3)
	{
		// 前のページにもどる 箱コンの操作をここに
		if (!g_bend)
		{
			if (Keyboard_IsTrigger(DIK_A) && FPS_mCounter >= 30 || js.lY >= -2 && js.lY <= 2 && js.lX == -6 && FPS_mCounter >= 30 || js.rgbButtons[1] && FPS_mCounter >= 30)
			{
				ManualPage = 2;
				FPS_mCounter = 0;
			}
		}
		if (!g_bend)
		{
			if (Keyboard_IsTrigger(DIK_D) && FPS_mCounter >= 30 || Keyboard_IsTrigger(DIK_SPACE) && FPS_mCounter >= 30 || js.lY >= -2 && js.lY <= 2 && js.lX == 6 && FPS_mCounter >= 30 || js.rgbButtons[0] && FPS_mCounter >= 30)
			{
				StopSound(CHUTO_BGM);
				PlaySound(START_SE);
				Fade_Start(true, 1, 0, 0, 0);
				g_bend = true;
			}
		}
		else
		{
			if (!Fade_IsFade())
			{
				StopSound(TITLE_BGM);
				Fade_Start(false, 1, 0, 0, 0);
				Scene_Change(SCENE_INDEX::SCENE_INDEX_GAME);
			}
		}
	}
}

void Manual_Draw(void)
{

	FPS_mCounter += 1;

	// 1ページ目
	if (ManualPage == 1)
	{
		C2DObj::SpriteDraw(CTexture::TEX_CHUTO01, 0, 0);
		C2DObj::SpriteDraw(CTexture::TEX_ABUTTON, 1630, 780);
	}
	// 2ページ目
	if (ManualPage == 2)
	{
		C2DObj::SpriteDraw(CTexture::TEX_CHUTO02, 0, 0);
		C2DObj::SpriteDraw(CTexture::TEX_ABUTTON, 1630, 780);
	}
	// 3ページ目
	if (ManualPage == 3)
	{
		C2DObj::SpriteDraw(CTexture::TEX_CHUTO03, 0, 0);
		C2DObj::SpriteDraw(CTexture::TEX_ABUTTON, 1630, 780);
	}

}

void Manual_Fainalize(void)
{

}