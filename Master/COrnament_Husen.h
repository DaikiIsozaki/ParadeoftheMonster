//////////////////////////////////////////////////
////
////	オブジェクト(風船)クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_HUSEN_H_
#define _CORNAMENT_HUSEN_H_

#include "COrnament.h"

class COrnament_HUSEN : public COrnament
{
public:
	COrnament_HUSEN(ORNAMENT_EMITTER *Emitter);
	~COrnament_HUSEN();

	void Update(void);
	void Draw(void);
	void Finalize(void);
	void Damage(void);
	bool Get_DrawCheck(void) { return false; }
private:
	void Initialize(ORNAMENT_EMITTER *Emitter);
	bool m_angle;
	bool hit;
	float updown;

};
#endif;#pragma once
