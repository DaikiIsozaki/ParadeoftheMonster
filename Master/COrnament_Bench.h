//////////////////////////////////////////////////
////
////	オブジェクト(ベンチ)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_BENCH_H_
#define _CORNAMENT_BENCH_H_

#include "COrnament.h"

class COrnament_BENCH : public COrnament
{
public:
	COrnament_BENCH(ORNAMENT_EMITTER *Emitter);
	~COrnament_BENCH();

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