#ifndef DEF_STAGEBASE_HPP
#define DEF_STAGEBASE_HPP

#ifndef DFE_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif




#include <vector>
#include <string>
#include <sstream>
#include <fstream>




/*
	@brief	�X�e�[�W���N���X
*/
class IStage : public Base
{
public:
	enum class Phase	// �X�e�[�W�t�F�[�Y
	{
		CLEAR_ANNOUNCE,	// �N���A�����\��
		NORMAL,			// �G���X�e�[�W
		BOSS,			// �{�X�X�e�[�W
		RESULT,			// ���U���g��ʂ�
	};
	enum class ClearType	// �N���A����
	{
		GOAL,			// �S�[��
		ANNIHILATION,	// �r��
	};
protected:
	std::string bgm_;							// BGM

	IStage::Phase phase_;						// �X�e�[�W�t�F�[�Y
	IStage::ClearType clearType_;				// �N���A����
	mymath::Recti cameraRect_;					// �J�����̉��͈�
	mymath::Recti stageRect_;					// �X�e�[�W�̑傫��
	std::vector<std::string> backgroundIMG_;	// �w�i�摜
	std::vector<ActPtPtr> actionPoints_;

public:
	const std::string& bgm;

	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
	const mymath::Recti& cameraRect;


private:
#pragma region private methods

	/*
		@brief		�X�e�[�W�V�X�e�����̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadEnv(std::ifstream& f)
	{
		//--------------------------------------
		// �X�e�[�W
		if (common::FindChunk(common::SeekSet(f), "#Bgm"))
		{
			f >> bgm_;
		}
		return f.eof();
	}

	/*
		@brief		�X�e�[�W�T�C�Y,�J�������͈͂̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadRect(std::ifstream& f)
	{
		//--------------------------------------
		// �X�e�[�W
		if (common::FindChunk(common::SeekSet(f), "#Left"))
		{
			f >> stageRect_.left;
		}
		if (common::FindChunk(common::SeekSet(f), "#Top"))
		{
			f >> stageRect_.top;
		}
		if (common::FindChunk(common::SeekSet(f), "#Right"))
		{
			f >> stageRect_.right;
		}
		if (common::FindChunk(common::SeekSet(f), "#Bottom"))
		{
			f >> stageRect_.bottom;
		}
		//--------------------------------------
		// �J����
		if (common::FindChunk(common::SeekSet(f), "#CameraLeft"))
		{
			f >> cameraRect_.left;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraTop"))
		{
			f >> cameraRect_.top;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraRight"))
		{
			f >> cameraRect_.right;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraBottom"))
		{
			f >> cameraRect_.bottom;
		}
		return f.eof();
	}
	/*
		@brief		�v���C���[�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadPlayer(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#Player"))
		{
			// �v���C���[���W���擾
			float pos[2];	// [0]:x [1]:y
			for (auto& p : pos)
			{
				std::string label;
				f >> label;
				p = static_cast<float>(std::atof(label.c_str()));
			}
			// �v���C���[�I�u�W�F�N�g��ǉ�
			auto& pl = gm()->GetObj(typeid(CPlayer));
			std::shared_ptr<CPlayer> player;
			if (!pl)
			{
				player = std::shared_ptr<CPlayer>(new CPlayer(pos[0], pos[1]));
				gm()->AddObject(player);
			}
			else
			{
				// ���d�o�^�h�~
				player = std::dynamic_pointer_cast<CPlayer>(pl);
				player->init(pos[0], pos[1], player->obj().pos.z);
			}
#ifdef DEF_GM_PTR
			gm()->SetPlayerPtr(player);
#endif
			
		}

		return f.eof();
	}
	/*
		@brief		�G���̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadEnemies(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#Enemy"))
		{
			std::string tableFile;
			f >> tableFile;
			// �v���C���[�I�u�W�F�N�g��ǉ�
			auto& pEm = gm()->GetObj(typeid(CEnemyMng));
			std::shared_ptr<CEnemyMng> em;
			if (!pEm)
			{
				// EnemyMng���Ȃ��ꍇ�A�V�K�ɒǉ�����
				em = std::shared_ptr<CEnemyMng>(new CEnemyMng());
				gm()->AddObject(em);
			}
			else
			{
				// ���d�o�^�h�~
				em = std::dynamic_pointer_cast<CEnemyMng>(pEm);
			}
			em->LoadEnemyTable(tableFile);
#ifdef DEF_GM_PTR
			gm()->SetEnemyMngPtr(em);
#endif
		}
		return f.eof();
	}

	/*
		@brief		ActionCircle�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadActionCircles(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#ActionCircle"))
		{
			std::string label;
			f >> label;
			if (label != "{" || f.eof()) return f.eof();
			while (!f.eof())
			{
				std::vector<float> info;
				for (int i = 0; i<3; ++i)
				{
					f >> label;
					if (label == "}" || f.eof())return f.eof();
					info.push_back(static_cast<float>(std::atof(label.c_str())));
				}
				if (info.size() == 3)
				{
					actionPoints_.push_back(ActPtPtr(
						new CActionCircle(info[0], info[1], info[2])
						));
				}
			}
		}
		return f.eof();
	}
	/*
		@brief		ActionPolygon�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadActionPolygons(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#ActionPolygon"))
		{
			std::string label;
			f >> label;
			if (label != "{" || f.eof()) return f.eof();
			while (!f.eof())
			{
				f >> label;
				if (label == "}")break;
				int num = std::atoi(label.c_str());
				std::vector<mymath::Vec3f> info(num);
				for (int i = 0; i<num; ++i)
				{
					mymath::Vec3f pos;
					f >> label;
					if (label == "}" || f.eof()) return f.eof();
					pos.x = static_cast<float>(std::atof(label.c_str()));
					f >> label;
					if (label == "}" || f.eof()) return f.eof();
					pos.y = static_cast<float>(std::atof(label.c_str()));
					pos.z = 0.5f;
					info[i] = pos;
				}
				//info.push_back(info[0]);	// �H�ɂ���
				actionPoints_.push_back(ActPtPtr(
					new CActionPolygon(info)));

			}
		}
		return f.eof();
	}

#pragma endregion	// private methods

protected:
#pragma region protected methods
	/*
		@brief	�e��t�@�C�����[�h����
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	void load(std::ifstream& f)
	{
		/*
		// ��ɓo�^����Ă���I�u�W�F�N�g�������Ă���ǂݍ���
		auto& objs = gm()->GetObjects("Player,Action", ',');
		for (auto& obj : objs)
			obj->kill();
		LoadPlayer(f);
		LoadEnemies(f);
		//*/

		LoadEnv(f);
		LoadRect(f);
		LoadActionCircles(f);
		LoadActionPolygons(f);
	}

	/*
		@brief	�N���A�����\���X�V����
		@return	�N���A�����\���I��
		@retval	true	�N���A�����\���I��
		@retval	false	�N���A�����\���p��
	*/
	virtual bool UpdateClearAnnounce() = 0{ return true; };

	/*
		@brief	�G���X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateNormal() = 0{ return true; }

	/*
		@brief	�{�X�X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateBoss() = 0{ return true; }

#pragma endregion	// protected methods
public:
#pragma region public methods
	IStage(const std::string& name) :
		Base(name)
		, phase_(IStage::Phase::CLEAR_ANNOUNCE)
		, bgm(bgm_)
		, actionPoints(actionPoints_)
		, rect(stageRect_)
		, cameraRect(cameraRect_)
	{
		

	}
	/*
		@brief	�X�e�[�W���ʍX�V����
		@return	�Ȃ�
	*/
	virtual void step() override
	{
		//*
		switch (phase_)
		{
		case IStage::Phase::CLEAR_ANNOUNCE:
			if (UpdateClearAnnounce())
			{
				phase_ = IStage::Phase::NORMAL;
			}
			/*
			switch (clearType_)
			{
			case IStage::ClearType::GOAL:
				break;
			case IStage::ClearType::ANNIHILATION:
				break;
			default:
				break;
			}
			//*/
			break;
		case IStage::Phase::NORMAL:
			for (auto& ap : actionPoints_)
				ap->step();
			if (UpdateNormal())
			{
				phase_ = IStage::Phase::BOSS;
			}
			break;
		case IStage::Phase::BOSS:
			for (auto& ap : actionPoints_)
				ap->step();
			if (UpdateBoss())
			{
				phase_ = IStage::Phase::RESULT;
			}
			break;
		}
		if (input::CheckPush(input::KEY_BTN0))
		{
			phase_ = IStage::Phase::RESULT;
		}
	}
	/*
		@brief	�X�e�[�W���ʕ`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() override
	{
		for (auto& ap : actionPoints_)
			ap->draw();
	}

	/*
		@brief	�X�e�[�W�ǂݍ���(������)
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	virtual void init(std::ifstream& f)
	{
		/*
		// ��ɓo�^����Ă���I�u�W�F�N�g�������Ă���ǂݍ���
		auto& objs = gm()->GetObjects("Player");
		for (auto& obj : objs)
			obj->kill();
		//*/
		phase_ = IStage::Phase::CLEAR_ANNOUNCE;
		LoadPlayer(f);
		LoadEnemies(f);



	}

	/*
		@brief	�X�e�[�W���I���������擾
		@return	�X�e�[�W�I���t���O
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�v���C�p��
	*/
	bool isEndStage() const
	{
		return phase_ == IStage::Phase::RESULT;
	}

#pragma endregion	// public methods
};

#endif