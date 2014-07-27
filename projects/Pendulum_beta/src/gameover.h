#pragma once
#define DEF_GAMEOVER_H

namespace gameover
{
	void SetChangeTime(float time);
	void StartGameOver();
	void InitGameOver();
	bool isGameOver();
	void CheckToChangeScreen(int next);
}

