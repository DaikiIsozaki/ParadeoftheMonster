//////////////////////////////////////////////////
////
////	エネミー(ビッグ)クラス
////
//////////////////////////////////////////////////

#ifndef _CENEMY_BIG_H_
#define _CENEMY_BIG_H_

#include "CEnemy.h"

#define BIG_SPEED (0.05f)

class CEnemy_Big : public CEnemy
{
public:
	CEnemy_Big(ENEMY_EMITTER *Emitter);
	~CEnemy_Big();

	void GameBegin(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);
	
private:

	void Initialize(ENEMY_EMITTER *Emitter);

	int m_Movetime = 0;

	bool m_AttackCheck = false;
	int m_AttackTime = 0;

	void Big_Move(void);
	void Big_Attack(void);

	bool m_JumpFlag = false;
};
#endif;