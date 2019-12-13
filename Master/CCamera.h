//////////////////////////////////////////////////
////
////	カメラクラス
////
//////////////////////////////////////////////////

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CGameObj.h"
#define NYAA (0.1f)
#define FUAA (150.0f)

class CCamera :public CGameObj
{
public:
	CCamera();
	~CCamera();


	void Update(void);
	void Draw(void);
	void Finalize(void);		//	終了処理
	static D3DXVECTOR3 Get_RightVec(void) { return m_Right; }	//	ライトベクトル取得	
	static const D3DXMATRIX Get_ViewMtx(void) { return m_mtxView; }		//	ビューマトリクス取得
	static CCamera* Camera_Create(void);	//	カメラクラス作成

	void VibrationCtrl(void);
											//	デバッグ用
	void DebugDraw(void);
	static CCamera* Get_CCamera(void) { return m_pCamera; }

	static bool Get_CameraAngleCheck(void) { return m_AngleCheck; }
	static float Get_Angle(void) { return angle; }
	static void Dalete_Camera(void);
protected:


private:
	void Camera_Initialize(void);	//	初期化

	D3DXVECTOR3 m_CameraPos;	//	カメラ座標
	D3DXVECTOR3 m_at;		//	見てる場所	注視点
	float m_AtLength = 0.0; // atまでの長さ
	float Cos_AtLength = 0.0;
	D3DXVECTOR3 m_Front;		//	フロントベクトル
	static D3DXVECTOR3 m_Right;	//	ライトベクトル
	D3DXVECTOR3 m_Up;			//	アップベクトル

	float m_Angle;	//	回転角度
	D3DXVECTOR3 Cos_angle;
	static float angle;
	static D3DXMATRIX m_mtxView;	//	ビュー変換用行列

	static CCamera *m_pCamera;

	static bool m_AngleCheck;
	static bool m_Vibration;//画面振動するか否か
	static int m_VibCount;
};

#endif // !1
