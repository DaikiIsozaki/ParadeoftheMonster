//////////////////////////////////////////////////
////
////	エネミー(ミドル)クラス
////
//////////////////////////////////////////////////

#ifndef _CENEMY_MIDDLE_H_
#define _CENEMY_MIDDLE_H_

#include "CEnemy.h"
#define MIDDLE_SPEED (0.05f)

class CEnemy_Middle : public CEnemy
{
public:
	CEnemy_Middle(ENEMY_EMITTER *Emitter);
	~CEnemy_Middle();

	void GameBegin(void);
	void Update(void);
	void Draw(void);
	void Finalize(void);

private:
	void Initialize(ENEMY_EMITTER *Emitter);

	int m_Movetime = 0;

	bool m_AttackCheck = false;
	int m_AttackTime = 0;

	void Middle_Move(void);
	void Middle_Attack(void);
};
#endif;
