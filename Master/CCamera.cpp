//////////////////////////////////////////////////
////
////	カメラクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CCamera.h"
#include "Cplayer.h"
#include "common.h"
#include "input.h"
#include "gamepad.h"
#include "debug_font.h"
#include "CCoaster_ON.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================


#define ASPECT ((float)WINDOW_WIDTH / WINDOW_HIGHT)
#define ATLENGTH (14.0f)
#define COSLENGTH (0.5f)
#define CAMERA_SPEED (0.00f)


//=============================================================================
//	静的変数
//=============================================================================

D3DXVECTOR3 CCamera::m_Right;
D3DXMATRIX CCamera::m_mtxView;
CCamera *CCamera::m_pCamera;
bool CCamera::m_Vibration;
int CCamera::m_VibCount;
float CCamera::angle = 0.0;
bool CCamera::m_AngleCheck = false;

static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;

bool r = false;
bool l = false;
//=============================================================================
//	生成
//=============================================================================
CCamera::CCamera() :CGameObj(CGameObj::TYPE_CAMERA)
{
	Camera_Initialize();
}



//=============================================================================
//	破棄
//=============================================================================
CCamera::~CCamera()
{
	//Camera_Finalize();
}



//	初期化
void CCamera::Camera_Initialize(void)
{
	m_CameraPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AtLength = ATLENGTH;
	Cos_AtLength = COSLENGTH;
	Cos_angle = D3DXVECTOR3(0.0f, 0.0f, 3.0f);

	m_Front = D3DXVECTOR3(0.0f, -0.4f, 1.0f);
	D3DXVec3Normalize(&m_Front, &m_Front); //　長さを1にする　正規化
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&m_Right, &m_Front, &m_Up);	// 外積
	D3DXVec3Normalize(&m_Right, &m_Right); //　長さを1にする　正規化
	D3DXVec3Cross(&m_Up, &m_Right, &m_Front);
	D3DXVec3Normalize(&m_Up, &m_Up); //　長さを1にする　正規化
	m_AngleCheck = false;
	angle = -90.0;
	m_Angle = 2.0;
	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	m_Vibration = false;
	m_VibCount = 0;
}

//	終了処理
void CCamera::Finalize(void)
{
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
	}
}



//	カメラクラス作成
CCamera* CCamera::Camera_Create(void)
{
	m_pCamera = new CCamera;
	return m_pCamera;
}

//	更新
void CCamera::Update(void)
{
	C3DObj *playerget = CPlayer::Get_Player();	//	プレイヤー取得
	float u;
	u = (float)(playerget->Get_Angle());
	//コントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	D3DXMATRIX at;

	C3DObj *pPlayer = CPlayer::Get_Player();
	bool *standby = C3DObj::GetCosterModeStandby();
	at = pPlayer->Get_mtxWorld();

	m_at.x = at._41;
	m_at.y = at._42 + 2.3f;
	m_at.z = at._43;

	m_AngleCheck = false;
	if (!*standby && C3DObj::GetW_coaster() == false)
	{
		if ((Keyboard_IsPress(DIK_RIGHT)) && !r || js.lRx >= 6)
		{
			//	注視点回転
			D3DXMATRIX mtxRotation;
			D3DXMatrixRotationY(&mtxRotation, D3DXToRadian(m_Angle));
			//D3DXMatrixRotationAxis(&mtxRotation, &m_Up, D3DXToRadian(2));
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation); // 第2引数を第3引数で行列変換し第1引数に入れる
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);
			D3DXVec3TransformNormal(&m_Up, &m_Up, &mtxRotation);
			m_AngleCheck = true;
			angle += m_Angle;

		}
		if (Keyboard_IsRelease(DIK_RIGHT))
		{
			m_AngleCheck = false;
		}

		if ((Keyboard_IsPress(DIK_LEFT)) && !l || js.lRx <= -6)
		{
			//	注視点回転
			D3DXMATRIX mtxRotation;
			D3DXMatrixRotationY(&mtxRotation, D3DXToRadian(-m_Angle));
			//D3DXMatrixRotationAxis(&mtxRotation, &m_Up, D3DXToRadian(2));
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation); // 第2引数を第3引数で行列変換し第1引数に入れる
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);
			D3DXVec3TransformNormal(&m_Up, &m_Up, &mtxRotation);
			m_AngleCheck = true;
			angle -= m_Angle;

		}
		if (Keyboard_IsRelease(DIK_RIGHT))
		{
			m_AngleCheck = false;
		}
	}
	else
	{
		if ((Keyboard_IsPress(DIK_RIGHT)) && !r || js.lX >= 6)
		{
			//	注視点回転
			D3DXMATRIX mtxRotation;
			D3DXMatrixRotationY(&mtxRotation, D3DXToRadian(m_Angle));
			//D3DXMatrixRotationAxis(&mtxRotation, &m_Up, D3DXToRadian(2));
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation); // 第2引数を第3引数で行列変換し第1引数に入れる
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);
			D3DXVec3TransformNormal(&m_Up, &m_Up, &mtxRotation);
			m_AngleCheck = true;
			angle += m_Angle;

		}
		if (Keyboard_IsRelease(DIK_RIGHT))
		{
			m_AngleCheck = false;
		}

		if ((Keyboard_IsPress(DIK_LEFT)) && !l || js.lX <= -6)
		{
			//	注視点回転
			D3DXMATRIX mtxRotation;
			D3DXMatrixRotationY(&mtxRotation, D3DXToRadian(-m_Angle));
			//D3DXMatrixRotationAxis(&mtxRotation, &m_Up, D3DXToRadian(2));
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation); // 第2引数を第3引数で行列変換し第1引数に入れる
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);
			D3DXVec3TransformNormal(&m_Up, &m_Up, &mtxRotation);
			m_AngleCheck = true;
			angle -= m_Angle;

		}
		if (Keyboard_IsRelease(DIK_RIGHT))
		{
			m_AngleCheck = false;
		}
	}
	if ((Keyboard_IsTrigger(DIK_J)))
	{
		m_Vibration = true;
	}
	/*if ((Keyboard_IsRelease(DIK_J)))
	{
	m_Vibration = false;
	m_VibCount = 0;
	}*/
	//m_CameraPos = m_at - m_Front * m_AtLength;
	if (C3DObj::GetW_coaster() == false)
	{
		//VibrationCtrl();
		m_CameraPos = m_at - m_Front * m_AtLength;
	}
	if (C3DObj::GetW_coaster() == true)
	{

		m_CameraPos = m_at - m_Front * Cos_AtLength;
	}

	//WallCheck();

	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);		//  上方向

											// ビュー座標変換
	D3DXMatrixLookAtLH(&m_mtxView, &m_CameraPos, &m_at, &m_Up);

	// ビュー座標変換行列設定
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);


	// プロジェクション座標変換行列
	D3DXMATRIX mtxProjection;
	// プロジェクション座標変換
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), ASPECT, NYAA, FUAA);

	/*
	1：行列アドレス
	2：画角 視野角の半分
	3：アスペクト比
	4：near
	5：far
	*/
	// プロジェクション座標変換行列設定
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);



}

//	描画
void CCamera::Draw(void)
{

}

//	デバッグ描画
void  CCamera::DebugDraw(void)
{
	DebugFont_Draw(400, 10, "%f\n,%f\n,%f\n,", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
}

void CCamera::VibrationCtrl(void)
{
	m_VibCount++;
	if (m_VibCount < 40)
	{
		static int coss = 300;
		if (!m_Vibration)
		{
			m_CameraPos = m_at - m_Front * m_AtLength;
		}
		else
		{
			//m_at.x = (float)((3 ^ (-coss / 100))*cos(coss / 50)*cos(coss / 1));
			m_at.y = (float)((3 ^ (-coss / 100))*cos(coss / 50)*cos(coss / 1));
			m_at.z = (float)((3 ^ (-coss / 100))*cos(coss / 50)*cos(coss / 1));
			//battle_windowY += (float)((3 ^ (-coss / 100))*cos(coss / 50)*cos(coss / 1));
			coss -= 1;
		}
		if (coss < 100)
		{
			coss = 300;
		}
	}
	if (m_VibCount >= 40)
	{
		m_Vibration = false;
		m_VibCount = 0;
	}
}


void CCamera::Dalete_Camera(void)
{
	CGameObj *pcamera = CGameObj::Get(CGameObj::TYPE_CAMERA);
	if (pcamera)
	{
		delete pcamera;
	}
}









