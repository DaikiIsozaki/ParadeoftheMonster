//////////////////////////////////////////////////
////
////	エネミー(スモール)クラス
////
//////////////////////////////////////////////////

#ifndef _CENEMY_SMALL_H_
#define _CENEMY_SMALL_H_

#include "CEnemy.h"

class CEnemy_Small: public CEnemy
{
public:
	CEnemy_Small(ENEMY_EMITTER *Emitter);
	~CEnemy_Small();

	void GameBegin(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);

private:

	void Initialize(ENEMY_EMITTER *Emitter);
	
	int m_Movetime = 0;

	bool m_AttackCheck = false;
	int m_AttackTime = 0;

	void Small_Move(void);
	void Small_Attack(void);
};
#endif;