#ifndef DEF_SCOREMNG_H
#define DEF_SCOREMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

#ifndef DEF_CHARBASE_H
#include "charBase.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif

#include <string>

/*
	@brief	�X�R�A����Ǘ��N���X
			�Q�[�����ł̃X�R�A�\���⃉���L���O�̍쐬���s��
*/
class CScoreMng : public Base
{
public:
	struct LoadInfo
	{
		std::vector<std::string> labels;
		std::vector<int> scores;
	};
	struct BonusScore
	{
		std::string label;		// �{�[�i�X��
		int score;				// �X�R�A��
	};
private:
	enum
	{
		RANK_NUM = 50,		// �����L���O�o�^��
	};
	int score_;								// ���݂̃X�R�A

	int dispScore_;							// �\���p�X�R�A
	common::DispState scoreState_;			// �X�R�A�\�����
	//mymath::Vec3f drawStartPos_;			// �X�R�A�\���������W
	charabase::CharBase scoreObj_;			// �X�R�A�x�[�X
	float scoreAnimTime_;					// �X�R�A�\���A�j���[�V�����p
	std::vector<BonusScore> bonusScores_;	// �{�[�i�X


	struct Ranking		// �����L���O�o�^���
	{
		std::string name;		// ���O
		int score;				// �X�R�A

		static const char SPACE = '@';	// �󔒕����̒u��

		// ��v�f
		static Ranking empty()
		{
			Ranking tmp = { "NONAME", 0 };
			return tmp;
		}
		// �t�@�C�����o��
		friend std::ifstream& operator>>(std::ifstream& f, Ranking& rank)
		{ 
			f >> rank.name >> rank.score;
			rank.name = common::StrReplace(rank.name, std::to_string(SPACE), "");
			return f;
		}
		friend std::ofstream& operator<<(std::ofstream& f, const Ranking& rank)
		{
			f << rank.name << "\t" << rank.score << std::endl;
			return f;
		}
		// sort�p
		bool operator<(const Ranking& rank) const
		{
			return score < rank.score;
		}
		bool operator>(const Ranking& rank) const
		{
			return score > rank.score;
		}
	};
	std::vector<Ranking> ranking_;			// �����L���O


private:

public:
	/*
		@brief	�������A�����L���O���ǂݍ���
		@return	�Ȃ�
	*/
	CScoreMng();
	/*
		@brief	������
		@return	�Ȃ�
	*/
	void start() override;

	/*
		@brief	�X�V
		@return	�Ȃ�
	*/
	void step() override;
	/*
		@brief	�X�R�A�̕`��
		@return	�Ȃ�
	*/
	void draw() override;
	
	/*
		@brief	�X�R�A�`��n�_�̐ݒ�
		@param	[in]	pos	�`��n�_
		@return	�Ȃ�
	*/
	void SetDrawPos(const mymath::Vec3f& pos);

	/*
		@brief	�X�R�A���Z
		@param	[in]	add	���Z��
		@return	�Ȃ�
	*/
	void AddScore(int add);
	
	/*
		@brief	�X�R�A�㏑��
		@param	[in]	s	�X�R�A
		@return	�Ȃ�
	*/
	void score(int s);

	/*
		@brief	�X�R�A�̎擾
		@return	�Ȃ�
	*/
	int score() const;


	/*
		@brief	�X�R�A���Z�A�j���[�V�����̏I�����f
		@return	�X�R�A���Z�A�j���[�V�������I�����Ă��邩
		@retval	true	�A�j���[�V�����I��
		@retval	false	�A�j���[�V������
	*/
	bool isStopAddAnimation() const;

	/*
		@brief	�G�l�~�[�}�l�[�W�����{�[�i�X�X�R�A�v�Z
		@param	[in]	em	�G�l�~�[�}�l�[�W��
		@return	�Ȃ�
	*/
	void CalcBonusScore(const std::weak_ptr<CEnemyMng>& em);

	/*
		@brief	�{�[�i�X�X�R�A�v�Z
		@return	�Ȃ�
	*/
	void CalcBonusScore();

	/*
		@brief	�{�[�i�X�X�R�A�擾
		@return	�{�[�i�X�X�R�A
	*/
	const std::vector<BonusScore>& getBonusScore() const;

	/*
		@brief	���݂̃X�R�A���g�������L���O�o�^
		@param	[in]	name	�����L���O�o�^��
		@return	�Ȃ�
	*/
	void RegisterRanking(const std::string& name);

	/*
		@brief	�����̃X�R�A�������N�C�����邩�ǂ���
		@attension		����X�R�A�̏ꍇ�����N�C�����Ȃ�
		@param	[in]	score	�X�R�A
		@return	�����N�C�����邩
		@retval	�����N�C������
		@retval	�����N�C�����Ȃ�
	*/
	bool isRankin(int score) const;
};

#endif