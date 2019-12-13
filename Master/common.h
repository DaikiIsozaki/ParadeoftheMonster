#ifndef _COMMON_H_
#define _COMMON_H_

#define WINDOW_TOP		(40.0f)	// スクリーン上限
#define WINDOW_WIDTH	(1920.0f)	// スクリーン幅
#define WINDOW_HIGHT	(1016.0f)	// スクリーン高さ
#define WINDOW_UNDER	(WINDOW_HIGHT - 50.0f)	//	地面の高さ
#define WINDOW_PLAYER_UNDER (WINDOW_HIGHT - 97.0f)	//	プレイヤーから見た地面の高さ
#define GAMEEND (5660)
#define FIELDSIZE (90.0f)
#define SYLINDERSIZE (55.0f)
#define SKYSIZE (80.0f)

void Game_End(void);
bool Get_Gameend(void);
#endif //_COMMON_H_