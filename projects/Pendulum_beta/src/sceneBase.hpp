#ifndef DEF_SCENEBASE_HPP
#define DEF_SCENEBASE_HPP

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_FADE_H
#include "fade.h"
#endif



#include <string>

#include <memory>


class IScene : public Base
{
public:
	enum class State		// �V�[���������
	{
		INNING,		// �V�[���ڂ�n��(IN����define����Ă���̂ŋ��)
		MAIN,		// �V�[���̃��C������
		OUTING,		// �V�[���ڂ�I���(OUT����define����Ă���̂ŋ��)
		END,		// �V�[���I��
	};
private:
	const float FADE_IN_TIME;			// �t�F�[�h�C���ɂ����鎞��
	const float FADE_OUT_TIME;			// �t�F�[�h�A�E�g�ɂ����鎞��

protected:
	static bool isSoftReset_;			// �\�t�g���Z�b�g������ ���̐錾��sceneMng.cpp��
	static float resetTime_;			// �\�t�g���Z�b�g�p ���̐錾��sceneMng.cpp��

	static int bgmVolume_;				// BGM����

	enum
	{
		SOFT_RESET_TIME = 30,			// �\�t�g���Z�b�g����[unit:/10sec]
	};

	const std::string BACK_RESNAME;		// �w�i
	std::string bgmResname_;			// BGM����

	static State state_;

	//std::string name_;

protected:

	/*
		@brief	�\�t�g���Z�b�g(JEC���S�܂Ŗ߂�)
		@return	�\�t�g���Z�b�g���邩
		@retval	true	�\�t�g���Z�b�g����
		@retval	false	�\�t�g���Z�b�g���Ȃ�
	*/
	bool SoftReset()
	{
		// ���E�{�^�������n�߂Ń��Z�b�g
		if (input::CheckPush(input::KEY_MOUSE_LBTN) || input::CheckPush(input::KEY_MOUSE_RBTN))
		{
			resetTime_ = 0.f;
		}
		// 3�{�^���������ςȂ��ŃJ�E���g����
		if (input::CheckPress(input::KEY_MOUSE_LBTN) 
			&&
			input::CheckPress(input::KEY_MOUSE_RBTN)
			&&
			input::CheckPress(input::KEY_MOUSE_MBTN))
		{
			resetTime_ += system::FrameTime;
			if (resetTime_ >= SOFT_RESET_TIME / 10.f)
			{
				// ���S��ʂ�
				isSoftReset_ = true;
				return true;
			}
		}
		return false;
	}


	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	virtual bool update() = 0;


public:
	/*
		@brief	�V�[������
		@param	[in]	name		���O
		@param	[in]	back		�w�i�摜�Ǘ���
		@param	[in]	bgm			BGM�Ǘ���
		@param	[in]	fadeInTime	�t�F�[�h�C���ɂ����鎞��(�f�t�H���g0.3�b)
		@param	[in]	fadeOutTime	�t�F�[�h�A�E�g�ɂ����鎞��(�f�t�H���g0.3�b)
	*/
	IScene(const std::string& name, const std::string& back, const std::string& bgm, float fadeInTime = 0.3f, float fadeOutTime = 0.3f) :
		Base(name)
		, BACK_RESNAME(back)
		, bgmResname_(bgm)
		, FADE_IN_TIME(fadeInTime)
		, FADE_OUT_TIME(fadeOutTime)
	{
		//start();
	}
	/*
		@brief	��V�[������
		@param	[in]	name		���O
	*/
	IScene(const std::string& name) :
		Base(name)
		, BACK_RESNAME("")
		, bgmResname_("")
		, FADE_IN_TIME(0.3f)
		, FADE_OUT_TIME(0.3f)
	{}
	//IScene(const std::string& name):name_(name){}
	
	virtual ~IScene() =0 {}

	virtual void step() override
	{
		switch (state_)
		{
		case IScene::State::INNING:
			if (CFade::FadeIn(255.f / FADE_IN_TIME*system::FrameTime))
			{
				state_ = State::MAIN;
			}
			break;
		case IScene::State::MAIN:
			if (SoftReset() || update())
			{
				// �t�F�[�h�A�E�g�J�n
				CFade::StartFadeOut();
				state_ = State::OUTING;
			}
			break;
		case IScene::State::OUTING:
			//--------------------------------------
			// BGM�t�F�[�h�A�E�g
			bgmVolume_ -= static_cast<int>(100.f / FADE_OUT_TIME * system::FrameTime);
			if (bgmVolume_ <= 0)
				bgmVolume_ = 0;
			if (!bgmResname_.empty())
				bgm::DShow_VolumeControl(bgmResname_, bgmVolume_);
			//--------------------------------------
			if (CFade::FadeOut(255.f / FADE_OUT_TIME * system::FrameTime))
			{
				state_ = State::END;
			}
			break;
		case IScene::State::END:
			break;
		}
	}

	virtual void draw()
	{
		SIZE size = graph::Draw_GetImageSize2(BACK_RESNAME);
		graph::Draw_GraphicsLeftTopNC(
			0, 0, 1.f,
			BACK_RESNAME,
			0, 0,
			size.cx, size.cy,
			0, nullptr,
			static_cast<float>(system::WINW) / size.cx,
			static_cast<float>(system::WINH) / size.cy);
	}
	/*
		@brief		�V�[���̏�����
		@attention	�h����ŕK��__supter::start()���ĂԂ���
		@return		�Ȃ�
	*/
	virtual void start() = 0
	{
		__super::start();
		isSoftReset_ = false;
		state_ = (IScene::State::INNING);
		bgmVolume_ = (100);
		if (!bgmResname_.empty())
		{
			bgm::DShow_SetStartPos(bgmResname_);
			bgm::DShow_VolumeControl(bgmResname_, bgmVolume_);
			bgm::DShow_Play(bgmResname_);
		}
		CFade::StartFadeIn();
	}
	
	/*
		@brief	���̃V�[���ɐ؂�ւ��u�ԂɌĂ΂��
		@return	���̃V�[��
	*/
	virtual int NextScene() const = 0;

	/*
		@brief	�V�[����ς��邩
		@return	�V�[����ς��邩
		@retval	true	�V�[����ς���
		@retval	false	�V�[����ς��Ȃ�
	*/
	static bool isChangeScene()
	{
		return state_ == State::END;
	}
	
	/*
		@brief	�\�t�g���Z�b�g���邩
		@return	�\�t�g���Z�b�g���邩
		@retval	true	�\�t�g���Z�b�g����
		@retval	false	�\�t�g���Z�b�g���Ȃ�
	*/
	static bool isSoftReset()
	{
		return isSoftReset_;
	}
	
};
typedef std::shared_ptr<IScene> ScenePtr;

#endif