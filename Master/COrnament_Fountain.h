//////////////////////////////////////////////////
////
////	オブジェクト(噴水)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_FOUNTAIN_H_
#define _CORNAMENT_FOUNTAIN_H_

#include "COrnament.h"

class COrnament_Fountain : public COrnament
{
public:
	COrnament_Fountain(ORNAMENT_EMITTER *Emitter);
	~COrnament_Fountain();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Damage(void);
	bool Get_DrawCheck(void) { return false; }
private:
	void Initialize(ORNAMENT_EMITTER *Emitter);
	int m_DrawCount;
};
#endif;