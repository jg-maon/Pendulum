
#include "../../../lib/gplib.h"
#include "gameover.h"

using namespace gplib;


struct GameOver
{
	float count;
	bool	isOver;
	float	changeTime;
};

static GameOver gameover_;

//ゲームオーバー時に切り替わる時間を指定する。
//デフォルトは3秒
//任意変更の場合はStartGameOverで行うこと
void gameover::SetChangeTime(float time)
{
	gameover_.changeTime = time;
}

//終了処理開始
//ゲームオーバー成立時に呼び出し
void gameover::StartGameOver()
{
	gameover_.isOver = true;
	gameover_.count = 0.f;
	gameover_.changeTime = 1.5f;
}

//終了処理初期化
void gameover::InitGameOver()
{
	gameover_.isOver = false;
	gameover_.count = 0.f;
	gameover_.changeTime = 3.0f;
}

//終了処理判別
//キャラクタの移動抑止などに
bool gameover::isGameOver()
{
	return gameover_.isOver;
}

//終了時ステージ移行処理
//StartGameOver後に一定時間で画面切り替えを実行する。
void gameover::CheckToChangeScreen(int next)
{
	if (gameover_.isOver == true){
		gameover_.count += system::ONEFRAME_TIME;//60分の１sec
		debug::SToMNC(0, 100, "GameOverCount:%0.2f", gameover_.count);
		if (gameover_.count >= gameover_.changeTime){
			gplib::stage::ChangeStage(next);
		}
	}
}
