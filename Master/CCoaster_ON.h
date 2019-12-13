//////////////////////////////////////////////////
////
////	アトラクション(ジェットコースター登場)クラス
////
//////////////////////////////////////////////////

#ifndef _COASTER_ON_H_
#define _COASTER_ON_H_

#include "CAttraction.h"
#define COSTERON_ENDTIME (30)	//コースタースタート終了時間
class CoasterON : public CAttraction
{
public:
	CoasterON();
	~CoasterON();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Coaster_Create(void);
	static C3DObj *Get_Coaster(void);
	bool Get_DrawCheck(void) { return m_DrawCheck; }
private:
	bool m_DrawCheck;
	void Initialize();
	int u;	//モデルの角度
	int v;	//モデルの角度
	float angle;
	D3DXVECTOR3 coaster_position;
	D3DXMATRIX mtxR;
	D3DXVECTOR3 m_front;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_right;
};
#endif;