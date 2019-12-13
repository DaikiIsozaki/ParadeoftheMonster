//=============================================================================
//	インクルードファイル
//=============================================================================

#include "input.h"
#include "fade.h"
#include "C2DObj.h"
#include "scene.h"
#include "gamepad.h"
#include "sound.h"
#include "C3DObj.h"
#include "common.h"
#include "COrnament.h"
#include "CMeshField.h"
#include "CMeshField_Cylinder.h"
#include "CMesh_SkyDome.h"
#include "Cplayer.h"
#include "debug_font.h"
#include "CLight.h"
#include <crtdbg.h>
#include "CUserInterface.h"
#include "Titlekanransya.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	グローバル変数宣言
//=============================================================================

static bool g_bend = false;	//	フェードインアウトフラグ
							//コントローラーに使う変数
static float f;
static float f2;
static int counter;
static int a;
static int b;

static D3DXVECTOR3 g_Camera_Eye;
static D3DXVECTOR3 g_Camera_At;
static D3DXVECTOR3 g_Camera_Up;

static D3DXVECTOR3 front;		//カメラの前向きベクトル、必ず長さ1、単位ベクトル
static D3DXVECTOR3 right;
static D3DXVECTOR3 up;

static D3DXMATRIX mtxR;

static D3DXMATRIX mtxView;

static float at_lenght;	//atまでの距離
static float speed;

static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;
C2DObj *ptitle;
//=============================================================================
//	初期化処理
//=============================================================================

void Title_Initialize(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	counter = 0;
	ptitle = new C2DObj;
	g_bend = false;
	//コントローラー情報取得
	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	PlaySound(TITLE_BGM);
	g_Camera_Eye = D3DXVECTOR3(0.0f, 30.0f, -50.0f); //eye カメラ座標
	g_Camera_At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 //at  注視点・・・自分が注目している視点
	g_Camera_Up = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	 //up  カメラの上方向ベクトル

	front = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&front, &front);

	//frontできたあと
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXVec3Cross(&right, &up, &front);	//外積は順番が肝心
	D3DXVec3Normalize(&right, &right);	//正規化,丸め誤差防止

	D3DXVec3Cross(&up, &front, &right);
	D3DXVec3Normalize(&up, &up);

	at_lenght = 40.0f;
	speed = 0.001f;

	a = 0;
	b = 0;
	/*C3DObj::InitModelLoad();
	CPlayer::PlayerCreate();				//	プレイヤー生成
	CLight::Light_Create();					//	ライト生成
	CMeshField::MeshField_Create(CTexture::TEX_KUSA_RENGA, 120.0f, 2, 2);							//	地面生成																									//CMeshField_Cylinder::MeshField_Cylinder_Create(CTexture::TEX_FLORR, 10.0f, SYLINDERSIZE, 20, 1,true);	//	内カベ生成
	CMeshField_Cylinder::MeshField_Cylinder_Create(CTexture::TEX_MOUNTAIN, 10.0f, FIELDSIZE, 20, 1, false);	//	外カベ生成
	CMesh_SkyDome::Mesh_SkyDome_Create(CTexture::TEX_SKY, 2.0f, SKYSIZE, 40, 20);
	COrnament::Create();*/

	CPlayer::TitleFlag_Change(true);
	CUserInterFace::TitleFlag_Change(true);
	//CTitleKanransya *m_pAttraction = new CTitleKanransya(D3DXVECTOR3(60.0f, 0.0f, 30.0f));

}

//=============================================================================
//	終了処理
//=============================================================================

void Title_Finalize(void)
{
	counter = 0;
	delete ptitle;
	/*C3DObj::DeleteAll();			//	3Dオブジェクト全消去
	CGameObj::DeleteAll2D();
	C3DObj::Model_Finalize();*/
	C3DObj::TitleFlag_Change(false);
	CUserInterFace::TitleFlag_Change(false);
	CPlayer::Reset_KoCount();
	_CrtDumpMemoryLeaks();

}

//=============================================================================
//	更新処理
//=============================================================================

void Title_Update(void)
{
	//コントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	//	スペースまたはAボタンでゲーム画面へ
	if (!g_bend)
	{
		if (Keyboard_IsTrigger(DIK_SPACE) || js.rgbButtons[0] & 0x80)
		{
			PlaySound(START_SE);
			Fade_Start(true, 3, 0, 0, 0);
			g_bend = true;
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			StopSound(TITLE_BGM);
			Fade_Start(false, 3, 0, 0, 0);
			Scene_Change(SCENE_INDEX_CHUTO);
		}
	}
	g_Camera_At = (front * at_lenght) + g_Camera_Eye;

	D3DXMatrixLookAtLH(&mtxView, &g_Camera_Eye, &g_Camera_At, &up);

	D3DXMATRIX mtxProjection;

	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)WINDOW_WIDTH / WINDOW_HIGHT, 0.1f, 200.0f);	//Fov = 画角(視野角),Aspect = アスペクト比, near, far

	C3DObj::m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);
	C3DObj::m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	b++;

	f = (float)(speed * cos(D3DXToRadian(a)));
	f2 = (float)(speed * sin(D3DXToRadian(a)));
	if (b > 5)
	{
		a += 1;
		g_Camera_Eye.x += f;
		g_Camera_Eye.z += f2;
		g_Camera_Eye -= g_Camera_At;

		D3DXMatrixRotationY(&mtxR, D3DXToRadian(-0.1f));//カメラ旋回速度
		D3DXVec3TransformNormal(&right, &right, &mtxR);		//今のところ必要ない
		D3DXVec3TransformNormal(&front, &front, &mtxR);
		D3DXVec3TransformNormal(&up, &up, &mtxR);
	}
	C3DObj::UpdateAll();	//	3Dオブジェクト更新
	CGameObj::UpdateAll();	//	2Dオブジェクト更新
	counter++;
}

//=============================================================================
//	描画処理
//=============================================================================

void Title_Draw(void)
{

	C3DObj::DrawAll();		//	3Dオブジェクト描画
	CGameObj::DrawAll();	//	2Dオブジェクト描画
	if (counter <= 60)
	{
		ptitle->Sprite_Draw(CTexture::TEX_PUSHBUTTON, 400, 700, 0, 0, ptitle->Texture_GetWidth(CTexture::TEX_PUSHBUTTON, 1), ptitle->Texture_GetHeight(CTexture::TEX_PUSHBUTTON, 1));
	}
	else if (counter == 120)
	{
		counter = 0;
	}
	ptitle->Sprite_Draw(CTexture::TEX_TITLELOGO, 400, 30, 0, 0, ptitle->Texture_GetWidth(CTexture::TEX_TITLELOGO, 1), ptitle->Texture_GetHeight(CTexture::TEX_TITLELOGO, 1));
	//テクスチャが出ない＆カメラ回転がおかしい？
	//	DebugFont_Draw(300, 50, "X = %ld , Y= %ld", js.lX, js.lY);
}


