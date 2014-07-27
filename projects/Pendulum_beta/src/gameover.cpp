
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

//�Q�[���I�[�o�[���ɐ؂�ւ�鎞�Ԃ��w�肷��B
//�f�t�H���g��3�b
//�C�ӕύX�̏ꍇ��StartGameOver�ōs������
void gameover::SetChangeTime(float time)
{
	gameover_.changeTime = time;
}

//�I�������J�n
//�Q�[���I�[�o�[�������ɌĂяo��
void gameover::StartGameOver()
{
	gameover_.isOver = true;
	gameover_.count = 0.f;
	gameover_.changeTime = 1.5f;
}

//�I������������
void gameover::InitGameOver()
{
	gameover_.isOver = false;
	gameover_.count = 0.f;
	gameover_.changeTime = 3.0f;
}

//�I����������
//�L�����N�^�̈ړ��}�~�Ȃǂ�
bool gameover::isGameOver()
{
	return gameover_.isOver;
}

//�I�����X�e�[�W�ڍs����
//StartGameOver��Ɉ�莞�Ԃŉ�ʐ؂�ւ������s����B
void gameover::CheckToChangeScreen(int next)
{
	if (gameover_.isOver == true){
		gameover_.count += system::ONEFRAME_TIME;//60���̂Psec
		debug::SToMNC(0, 100, "GameOverCount:%0.2f", gameover_.count);
		if (gameover_.count >= gameover_.changeTime){
			gplib::stage::ChangeStage(next);
		}
	}
}
