//////////////////////////////////////////////////
////
////	オブジェクト(メリーゴーランド)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_MERI_H_
#define _CORNAMENT_MERI_H_

#include "COrnament.h"

class COrnament_MERI : public COrnament
{
public:
	COrnament_MERI(ORNAMENT_EMITTER *Emitter);
	~COrnament_MERI();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Damage(void);
	bool Get_DrawCheck(void) { return false; }
private:
	void Initialize(ORNAMENT_EMITTER *Emitter);
	void Ornament_Damage_Meri(float flyinghigh);
	void Ornament_Flying_Meri(float speed);
	void Death_Meri(void);
	float angle;
	bool m_OrnamentFlying;
	bool m_OrnamentFlyingDown;
	int m_FlyingCount;
	D3DXVECTOR3 m_FlyingMove;
	D3DXVECTOR3 m_FlyingMove2;
	THING_NORMAL uma;
	D3DXMATRIX m_mtxWorld2;
	D3DXMATRIX trans2;
	D3DXMATRIX rotation2;
	D3DXMATRIX rotation3;
	D3DXMATRIX scale2;
};
#endif;#pragma once
#pragma once
