//////////////////////////////////////////////////
////
////	障害物クラス
////
//////////////////////////////////////////////////

#ifndef _CORNAMENT_H_
#define _CORNAMENT_H_

#include "C3DObj.h"

#define ORNAMENT_MAX (100)
#define ORNAMENT_WAIT_TIME (15)//無敵時間

class COrnament :virtual public C3DObj
{
public:
	
	enum
	{
		TYPE_ALL = 0,

		TYPE_LAMP,		//	街灯
		TYPE_DUSTBOX,	//	ゴミ箱
		TYPE_BENCH,		//	ベンチ
		TYPE_FOUNTAIN,	//	噴水
		TYPE_KANRANSYA_DODAI,	//観覧車の土台	
		TYPE_HUSEN,				//風船
		TYPE_MERI,				//メリーゴーランド
		TYPE_GASUBONBE,         //ガスボンベ
		TYPE_POTETO,			//ごっつぽてと像
		TYPE_MAX
	};
	
	COrnament();
	COrnament::COrnament(int OrnamentType);
	~COrnament();
	void Ornament_Finalize(int Index);


	static void Create(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Damage(void) = 0;

	static int Get_OrnamentNum(int OrnamType){ return m_OrnamentNum[OrnamType]; }
	static int Get_OrnamentIndex(int EnemyType) { return m_OrnamentNum[EnemyType] - 1; }
	int Get_Score(void) { return m_Score; }
	int Get_Type(void) { return m_Type; }
	void OrnamentDamage(void);
	static int Get_OrnamentMaxNum(void) { return m_ORNAMENT_MAX; }
	static C3DObj *Get_Ornament(int index);
	bool Get_DrawCheck(void) { return m_DrawCheck; }

	static C3DObj *Get_Map_Ornament(int Index);
protected:
	typedef struct {
		int CreateFrame;		//	生成フレーム
		int Type;				//	オブジェクトタイプ
		D3DXVECTOR3 InitPos;	//	初期位置
		int InitDirection;		//	初期向き
		bool CreateCheck;		//	生成フラグ
		D3DXVECTOR3 scale;      //  大きさ
	}ORNAMENT_EMITTER;
	int m_OrnamentIndex;
	int m_Score;				//	スコア
	int m_Direction;			//	向き
	float m_DirectionAngle;		//	向き変更角度
	static COrnament *m_pOrnament[ORNAMENT_MAX];
	static C3DObj *Get_AllOrnament(int index);
	bool m_DrawCheck;			//	描画フラグ

	void Ornament_Damage(float flyinghigh);
	void Ornament_Flying(float speed);
	void COrnament::Death(void);
private:
	int m_Type;			// 種類
	static int m_OrnamentNum[TYPE_MAX];

	static ORNAMENT_EMITTER m_OrnamentEmitter[];
	static int m_ORNAMENT_MAX;

	bool m_OrnamentFlying = false;
	bool m_OrnamentFlyingDown = false;
	int m_FlyingCount = 0;
	D3DXVECTOR3 m_FlyingMove;
};

#endif // !1
#pragma once
