#ifndef _GAME_H_
#define _GAME_H_



void Game_Initialize(void);
void Game_Finalize(void);
void Game_Updata(void);
void Game_Draw(void);

void Fadraw_Set(bool type, bool index);
void Fadraw_end(void);
#endif //_GAME_H_