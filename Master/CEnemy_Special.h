//////////////////////////////////////////////////
////
////	エネミー(スペシャル)クラス
////
//////////////////////////////////////////////////

#ifndef _CENEMY_SPECIAL_H_
#define _CENEMY_SPECIAL_H_

#include "CEnemy.h"


class CEnemy_Special : public CEnemy
{
public:
	CEnemy_Special(ENEMY_EMITTER *Emitter);
	~CEnemy_Special();

	void GameBegin(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);
	
private:
	
	void Initialize(ENEMY_EMITTER *Emitter);

	int m_Movetime = 0;

	bool m_AttackCheck = false;
	int m_AttackTime = 0;

	void Special_Move(void);
	void Special_Attack(void);
	bool Chase_Attraction(void);
	bool m_Stop = false;
};
#endif;

