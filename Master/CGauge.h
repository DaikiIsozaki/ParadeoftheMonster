//////////////////////////////////////////////////
////
////	アトラクション進行ゲージクラス
////
//////////////////////////////////////////////////

#ifndef _CGAUGE_H_
#define _CGAUGE_H_

#include "CAttraction.h"

class CGAUGE : public CAttraction
{
public:
	CGAUGE(D3DXMATRIX mtxWorld);
	~CGAUGE();

	void Update(void);
	void Draw(void);
	void GaugeDraw(void);
	void UpdateGaugeDraw(void);//更新ゲージ表示
	void WhiteGaugeDraw(void);//更新ゲージ表示
	void Finalize(void);
	static void CGAUGE_Create(D3DXMATRIX m_mtxWorld);
	static C3DObj *Get_CGAUGE(void);
	bool Get_DrawCheck(void) { return m_DrawCheck; }

	typedef struct
	{
		D3DXVECTOR3 pos;	//	座標x,y,z
		D3DXVECTOR3 normal; //	法線ベクトル 座標の後 長さは1.0
		D3DCOLOR color;		//  色
		D3DXVECTOR2 uv;		//  テクスチャ

	}BillBoardVertex3D;
	BillBoardVertex3D g_bb[4];
private:
	bool m_DrawCheck;
	void Initialize(D3DXMATRIX m_mtxWorld);

	float bar_width;
};
#endif;