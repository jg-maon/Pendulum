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
protected:
	mymath::Recti cameraRect_;					// �J�����̉��͈�
	mymath::Recti stageRect_;					// �X�e�[�W�̑傫��
	std::vector<std::string> backgroundIMG_;	// �w�i�摜
	std::vector<ActPtPtr> actionPoints_;

public:
	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
	const mymath::Recti& cameraRect;


private:
#pragma region private methods
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
		// �X�e�[�W
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
			InsertObject(ObjPtr(new CPlayer(pos[0], pos[1])));
			
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
			auto& ems = gm()->GetObjects("EnemyMng");
			ObjPtr em;
			if (ems.empty())
			{
				// EnemyMng���Ȃ��ꍇ�A�V�K�ɒǉ�����
				em = ObjPtr(new CEnemyMng());
				gm()->AddObject(em);
			}
			else
			{
				em = ems[0];
			}
			std::dynamic_pointer_cast<CEnemyMng>(em)->LoadEnemyTable(tableFile);
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
		// ��ɓo�^����Ă���I�u�W�F�N�g�������Ă���ǂݍ���
		auto& objs = gm()->GetObjects("Player,Action", ',');
		for (auto& obj : objs)
			obj->kill();


		LoadRect(f);
		LoadPlayer(f);
		LoadEnemies(f);
		LoadActionCircles(f);
		LoadActionPolygons(f);

	}
#pragma endregion	// protected methods
public:
#pragma region public methods
	IStage(const std::string& name) :
		Base(name)
		, actionPoints(actionPoints_)
		, rect(stageRect_)
		, cameraRect(cameraRect_)
	{
		

	}
	virtual void step()override = 0{}
	virtual void draw()override = 0{}

	/*
		@brief	�X�e�[�W�ǂݍ���(������)
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	virtual void init(std::ifstream& f)
	{
		// ��ɓo�^����Ă���I�u�W�F�N�g�������Ă���ǂݍ���
		auto& objs = gm()->GetObjects("Player", ',');
		for (auto& obj : objs)
			obj->kill();

		LoadPlayer(f);
		LoadEnemies(f);
	}

#pragma endregion	// public methods
};

#endif