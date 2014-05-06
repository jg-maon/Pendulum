#ifndef DEF_FADE_H
#define DEF_FADE_H


class CFade
{
	static unsigned char r_;			// フェード色
	static unsigned char g_;			// フェード色
	static unsigned char b_;			// フェード色
	static float a_;			// alpha値
	static bool fadeInOn_;		// フェードイン中
	static bool fadeOutOn_;	// フェードアウト中
public:

	/*
		@brief	色変更
		@param	[in]	r	赤
		@param	[in]	g	緑
		@param	[in]	b	青
		@return	なし
	*/
	static void ChangeColor(unsigned char r, unsigned char g, unsigned char b);

	/*
		@brief	フェードイン開始
		@return	なし
	*/
	static void StartFadeIn();

	/*
		@brief	フェードアウト開始
		@return	なし
	*/
	static void StartFadeOut();

	/*
		@brief	フェードイン
		@param	[in]	speed	速度(1フレームの変位)
		@return	フェードインが完了したか
		@retval	true	フェードイン完了
		@retval	false	フェードイン中
	*/
	static bool FadeIn(float speed);
	
	/*
		@brief	フェードアウト
		@param	[in]	speed	速度(1フレームの変位)
		@return	フェードアウトが完了したか
		@retval	true	フェードアウト完了
		@retval	false	フェードアウト中
	*/
	static bool FadeOut(float speed);

	/*
		@brief	フェードイン・アウト描画
		@attention	毎フレーム呼び続ける
		@return	なし
	*/
	static void draw();
	
	/*
		@brief	フェードイン中か
		@return	フェードイン中か
		@retval	true	フェードイン中
		@retval	false	フェードイン中でない
	*/
	static bool IsFadeInning();
	/*
		@brief	フェードアウト中か
		@return	フェードアウト中か
		@retval	true	フェードアウト中
		@retval	false	フェードアウト中でない
	*/
	static bool IsFadeOuting();

	/*
		@brief	フェードイン・アウトどちらか実行中か
		@return	実行中か
		@retval	true	フェード実行中
		@retval	false	フェード実行中でない
	*/
	static bool IsFading();

};


#endif