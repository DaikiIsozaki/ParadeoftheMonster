//////////////////////////////////////////////////
////
////	アトラクション(ポップコーン)クラス
////
//////////////////////////////////////////////////

#ifndef _POPCORN_H_
#define _POPCORN_H_

#include "CAttraction.h"

class Popcorn : public CAttraction
{
public:
	Popcorn(D3DXMATRIX mtxWorld);
	~Popcorn();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void PopcornDamage(void);
	bool Get_DrawCheck(void) { return false; }
	static C3DObj* Get_Popcorn(int index);
private:
	void Initialize(D3DXMATRIX mtxWorld);
	bool PlayerCheck(void);
	int CoolTime;//クールタイム
	int m_DrawCount;
	bool B_CoolTime;					//クールタイムのブール
	bool m_DrawCheck;

};
#endif;