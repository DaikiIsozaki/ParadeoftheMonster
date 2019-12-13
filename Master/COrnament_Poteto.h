//////////////////////////////////////////////////
////
////	オブジェクト(ポテト像)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_POTETO_H_
#define _CORNAMENT_POTETO_H_

#include "COrnament.h"

class COrnament_Poteto : public COrnament
{
public:
	COrnament_Poteto(ORNAMENT_EMITTER *Emitter);
	~COrnament_Poteto();

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