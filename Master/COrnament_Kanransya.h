//////////////////////////////////////////////////
////
////	オブジェクト(観覧車	)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_KANRANSYA_H_
#define _CORNAMENT_KANRANSYA_H_

#include "COrnament.h"

class COrnament_KANRANSYA : public COrnament
{
public:
	COrnament_KANRANSYA(ORNAMENT_EMITTER *Emitter);
	~COrnament_KANRANSYA();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Damage(void);
	bool Get_DrawCheck(void) { return false; }
private:
	void Initialize(ORNAMENT_EMITTER *Emitter);
	void Ornament_Damage_Kanransya(float flyinghigh);
	void Ornament_Flying_Kanransya(float speed);
	void Death_Kanransya(void);
	bool m_OrnamentFlying;
	bool m_OrnamentFlyingDown;
	int m_FlyingCount;
	D3DXVECTOR3 m_FlyingMove;
	D3DXVECTOR3 m_FlyingMove2;
	float angle;
	THING_NORMAL kanransya;
	D3DXMATRIX m_mtxWorld2;
	D3DXMATRIX trans2;
	D3DXMATRIX rotation2;
	D3DXMATRIX rotation3;
	D3DXMATRIX scale2;
};
#endif;#pragma once
