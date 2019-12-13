//////////////////////////////////////////////////
////
////	アトラクション(ジェットコースター)クラス
////
//////////////////////////////////////////////////

#ifndef _COASTER_H_
#define _COASTER_H_

#include "CAttraction.h"

class Coaster : public CAttraction
{
public:
	Coaster();
	~Coaster();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Coaster_Create(void);
	void EnemyDamage(void);
	static C3DObj *Get_Coaster(void);
	bool Get_DrawCheck(void) { return m_DrawCheck; }
private:
	bool m_DrawCheck;
	void Initialize();
	int u;	//モデルの角度
	int v;	//モデルの角度
};
#endif;