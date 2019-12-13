//=============================================================================
//	インクルードファイル
//=============================================================================

#include "score_draw.h"

//=============================================================================
//	定数定義
//=============================================================================

#define NUMBER_SIZE (32)
#define SCORE_NUMBER_SIZE (130)
#define SCORE_NUMBER_SIZE_RED (180)
#define SCORE_NUMBER_SIZE_BREAK (225)
#define MP_NUMBER_SIZE (45)
#define MP_NUMBER_SIZE2 (50) //MPの横の数字
//=============================================================================
//	グローバル変数宣言
//=============================================================================

static int score = 0;			//	表示スコア
static int scorecache = 0;		//	追加スコアキャッシュ
static int totalscore = 0;		//	トータルスコア
static bool g_bend = false;


void Score_Initialize(void)
{
	score = 0;
	scorecache = 0;
	totalscore = 0;
	g_bend = false;
}

void Score_Draw(float x, float y, int score, int fig, bool bLeft, bool bZero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}
	if (bZero)
	{
		for (int i = 0; i < fig; i++)
		{

			int number = score % 10;
			score /= 10;
			Number_Draw(x + NUMBER_SIZE*(fig - (i + 1)), y, number);

		}
	}



}


void Score_Draw(float x, float y, int cache, int fig)
{

	int count_stop_score = 1;
	if (cache < 2100)
	{
		scorecache += cache;	//	追加点をキャッシュにプラス
		score = totalscore;		//	表示するスコアを代入

		if (scorecache > 0)		//	キャッシュに点が入ってたら
		{
			score += 10;			//	表示を１プラスして
			scorecache -= 10;		//	キャッシュを１マイナス
		}
		totalscore = score;		//	スコアをトータルに保管
	}
	else
	{
		score = cache;
		totalscore += score;
		//Fade_Start(true, 1, 0, 0, 0);
		//g_bend = true;

	}
	if (g_bend)
	{
		if (!Fade_IsFade())
		{
			Fade_Start(false, 1, 0, 0, 0);
			Scene_Change(SCENE_INDEX_RESULT);
		}
	}

	//	カンスト用
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}

	//	スコア表示	右詰め	０埋め
	for (int j = 0; j < fig; j++)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw(x + NUMBER_SIZE*(fig - (j + 1)), y, number);
	}
}


void Mp_Draw(float x, float y, int score, int fig, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}

	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw(x + NUMBER_SIZE*(fig + 1), y, number);
		if (score != 0)
		{
			Number_Draw(x + NUMBER_SIZE*fig, y, score);
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw(x + NUMBER_SIZE*(fig - (i + 1)), y, number);
		}
	}
}

void Time_Draw(float x, float y, int score, int fig, int color, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}


	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw_Leader(x + SCORE_NUMBER_SIZE*(fig + 1), y, number, color);
		for (int i = 0; i < fig - 1; i++)
		{
			if (score > 0)
			{
				int number = score % 10;
				score /= 10;
				Number_Draw_Leader(x + SCORE_NUMBER_SIZE*(fig - i), y, number, color);
			}
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw_Leader(x + SCORE_NUMBER_SIZE*(fig - (i + 1)), y, number, color);
		}
	}
}


void Time_Draw02(float x, float y, int score, int fig, int color, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}


	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw_Leader_red(x + SCORE_NUMBER_SIZE_RED*(fig + 1), y, number, color);
		for (int i = 0; i < fig - 1; i++)
		{
			if (score > 0)
			{
				int number = score % 10;
				score /= 10;
				Number_Draw_Leader_red(x + SCORE_NUMBER_SIZE_RED*(fig - i), y, number, color);
			}
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw_Leader_red(x + SCORE_NUMBER_SIZE_RED*(fig - (i + 1)), y, number, color);
		}
	}
}

void Num_Draw(float x, float y, int score, int fig, int color, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}


	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw_Break(x + SCORE_NUMBER_SIZE*(fig + 1), y, number, color);
		for (int i = 0; i < fig - 1; i++)
		{
			if (score > 0)
			{
				int number = score % 10;
				score /= 10;
				Number_Draw_Break(x + SCORE_NUMBER_SIZE*(fig - i), y, number, color);
			}
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw_Break(x + SCORE_NUMBER_SIZE*(fig - (i + 1)), y, number, color);
		}
	}
}

void Num_Draw2(float x, float y, int score, int fig, int color, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}


	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw3(x + MP_NUMBER_SIZE*(fig + 1), y, number, color);
		for (int i = 0; i < fig - 1; i++)
		{
			if (score > 0)
			{
				int number = score % 10;
				score /= 10;
				Number_Draw3(x + MP_NUMBER_SIZE*(fig - i), y, number, color);
			}
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw3(x + SCORE_NUMBER_SIZE*(fig - (i + 1)), y, number, color);
		}
	}
}

void Num_Draw3(float x, float y, int score, int fig, int color, bool zero)
{

	int count_stop_score = 1;
	for (int i = 0; i < fig; i++)
	{
		count_stop_score *= 10;
	}
	if (score >= count_stop_score)
	{
		score = count_stop_score - 1;
	}


	if (!zero)
	{
		int number = score % 10;
		score /= 10;
		Number_Draw5(x + MP_NUMBER_SIZE2*(fig + 1), y, number, color);
		for (int i = 0; i < fig - 1; i++)
		{
			if (score > 0)
			{
				int number = score % 10;
				score /= 10;
				Number_Draw5(x + MP_NUMBER_SIZE2*(fig - i), y, number, color);
			}
		}
	}
	else
	{
		for (int i = 0; i < fig; i++)
		{
			int number = score % 10;
			score /= 10;
			Number_Draw5(x + MP_NUMBER_SIZE2*(fig - (i + 1)), y, number, color);
		}
	}
}

void ResultScore_Draw(float x, float y, int score, int fig, int colornum)
{
	for (int i = 0; i < fig; i++)
	{

		int number = score % 10;
		score /= 10;
		Number_Draw_Leader(x + SCORE_NUMBER_SIZE_BREAK*(fig - (i + 1)), y, number, colornum);

	}
}

void ResultScore_Draw02(float x, float y, int score, int fig, int colornum)
{
	for (int i = 0; i < fig; i++)
	{

		int number = score % 10;
		score /= 10;
		Number_Draw_yellow_180(x + SCORE_NUMBER_SIZE_RED*(fig - (i + 1)), y, number, colornum);

	}
}

void ResultScore_Draw(float x, float y, int score, int fig, int texindex, int colornum)
{
	for (int i = 0; i < fig; i++)
	{
		int numbersize = /*CTexture::Texture_GetWidth(texindex, 10);*/100;
		int number = score % 10;
		score /= 10;
		Number_Draw_Select(x + numbersize*(fig - (i + 1)), y, number, texindex, colornum);

	}
}

void ResultScore_Draw02(float x, float y, int score, int fig, int texindex, int colornum)
{
	for (int i = 0; i < fig; i++)
	{
		int numbersize = /*CTexture::Texture_GetWidth(texindex, 10);*/100;
		int number = score % 10;
		score /= 10;
		Number_Draw_Select(x + 200 * (fig - (i + 1)), y, number, texindex, colornum);

	}
}

int Get_TotalScore(void)
{
	return totalscore;
}