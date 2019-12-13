//////////////////////////////////////////////////
////
////	タイトル観覧車クラス
////
//////////////////////////////////////////////////

#ifndef _TITLEKANRANSYA_H_
#define _TITLEKANRANSYA_H_

#include "CAttraction.h"

class CTitleKanransya : public CAttraction
{
public:
	CTitleKanransya(D3DXVECTOR3 posisiton);
	~CTitleKanransya();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	bool Get_DrawCheck(void) { return m_DrawCheck; }
	void EnemyDamage(void);
private:
	void Initialize(D3DXVECTOR3 posisiton);
	int m_AttractionIndex;


	bool ferris_flg;				 //移動と壁激突のチェンジ
	int ferris_counter;				 //観覧車停止
	float rotate_ferris;			//観覧車の角度
	float angle;
	bool m_DrawCheck;
	D3DXMATRIX mtxR;
	D3DXMATRIX m_mtxRotationY;		//	移動行列
	D3DXMATRIX m_mtxRotationYY;
	D3DXVECTOR3 move;
	D3DXVECTOR3 Wheel_position;
	D3DXVECTOR3 m_front;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_right;
};
#endif;