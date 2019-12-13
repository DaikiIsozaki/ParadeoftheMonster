#ifndef _SCORE_DRAW_H_
#define _SCORE_DRAW_H_

#include "number.h"
#include "fade.h"
#include "scene.h"

void Time_Draw(float x, float y, int score, int fig, int color, bool zero);
void Time_Draw02(float x, float y, int score, int fig, int color, bool zero);
void Score_Draw(float x, float y, int score, int fig, bool bLeft, bool bZero);
void Score_Draw(float x, float y, int cache, int fig);
void Num_Draw2(float x, float y, int score, int fig, int color, bool zero);
void Num_Draw3(float x, float y, int score, int fig, int color, bool zero);
void Score_Initialize(void);
void ResultScore_Draw(float x, float y, int score, int fig, int colornum);
void ResultScore_Draw02(float x, float y, int score, int fig, int colornum);
void ResultScore_Draw(float x, float y, int score, int fig, int texindex, int colornum);
void ResultScore_Draw02(float x, float y, int score, int fig, int texindex, int colornum);
void Mp_Draw(float x, float y, int score, int fig, bool zero);
int Get_TotalScore(void);

void Num_Draw(float x, float y, int score, int fig, int color, bool zero);
#endif
