/*
//=============================================================================
//	インクルードファイル
//=============================================================================
#define NOMINMAX

#include "CLight.h"
#include "CCamera.h"
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
#include "exp.h"
#include "shadow.h"
#include "COrnament.h"
#include "sound.h"
#include <crtdbg.h>
//=============================================================================
//	定数定義
//=============================================================================
#define FADE_WAIT (255.0f)
#define FADE_SPEED (8)//4の倍数

//=============================================================================
//	グローバル宣言
//=============================================================================
C2DObj *m_fade;

int FaDe_counterX;

float fade_in_u;
float fade_in_v;
float fade_u;
float fade_v;
float fade_wait;
bool Fadraw;
bool fade_ioFlg;
bool Fwait_flg;
//=============================================================================
//	初期化処理
//=============================================================================
void Fadraw_Initialize(void)
{
	fade_ioFlg = false;
	fade_wait = 0;
	Fadraw = false;
	Fwait_flg = false;
}

//=============================================================================
//	終了処理
//=============================================================================

void Fadraw_Finalize(void)
{
	delete m_fade;
}

//=============================================================================
//	更新処理
//=============================================================================

void Fadraw_Updata(void)
{
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

}

//=============================================================================
//	描画処理
//=============================================================================

void Fadraw_Draw(void)
{
	if (Fadraw == true)
	{
		if (fade_ioFlg == false)
		{
			m_fade->Sprite_Draw(CTexture::TEX_EFFECT_FADE_IN, 0, 0, fade_in_u * 1920, fade_in_v * 1016, 1920, 1016);
		}
		if (fade_ioFlg == true)
		{
			m_fade->Sprite_Draw(CTexture::TEX_EFFECT_FADE, 0, 0, fade_u * 1920, fade_v * 1016, 1920, 1016);
		}
	}
	//C3DObj::HitCheck();
}



void Fadraw_Set(bool type, bool index)
{
	Fadraw = type;
	fade_ioFlg = index;
}*/