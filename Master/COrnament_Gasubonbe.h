//////////////////////////////////////////////////
////
////	オブジェクト(ガスボンベ)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_GASUBONBE_H_
#define _CORNAMENT_GASUBONBE_H_

#include "COrnament.h"

class COrnament_Gasubonbe : public COrnament
{
public:
	COrnament_Gasubonbe(ORNAMENT_EMITTER *Emitter);
	~COrnament_Gasubonbe();

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