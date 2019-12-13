/*#define NOMINMAX
#include <windows.h>
#undef min
#undef max*/

// アニメーションループX
int Animation_UpdateX(int FrameCount, int WaitFrame, int MaxPattern, int tw, int WidthMaxPattern)
{
	int DrawPattern = (FrameCount / WaitFrame) % MaxPattern;

	int BiginX;

	return BiginX = tw * (DrawPattern % WidthMaxPattern);

}
// アニメーションループY
int Animation_UpdateY(int FrameCount, int WaitFrame, int MaxPattern, int th, int WidthMaxPattern)
{
	int DrawPattern = (FrameCount / WaitFrame) % MaxPattern;

	int BiginY;

	return BiginY = th * (DrawPattern / WidthMaxPattern);

}
// アニメーションループ無しX
/*int Animation_UpdateX2(int FrameCount, int WaitFrame, int MaxPattern, int tw, int WidthMaxPattern)
{
	int DrawPattern = min((FrameCount / WaitFrame) , MaxPattern-1);

	int BiginX;

	return BiginX = tw * (DrawPattern % WidthMaxPattern);

}
// アニメーションループ無しY
int Animation_UpdateY2(int FrameCount, int WaitFrame, int MaxPattern, int th, int WidthMaxPattern)
{
	int DrawPattern = min((FrameCount / WaitFrame) , MaxPattern-1);

	int BiginY;

	return BiginY = th * (DrawPattern / WidthMaxPattern);

}*/
//	何パターン目を再生するか

//	(サイクルの場合)
//	表示パターン = (フレームカウンタ / 待ちフレーム数) % 最大パターン数

//	(サイクルしない場合)
//	表示パターン = MIN((フレームカウンタ / 待ちフレーム数), 最大パターン数 - 1)


//	テクスチャ―の切り取り座標x = パターンの幅 * (表示パターン目 % 横最大パターン数)
//	テクスチャ―の切り取り座標y = パターンの高さ * (表示パターン目 / 横最大パターン数)