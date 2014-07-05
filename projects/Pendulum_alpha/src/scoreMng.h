#ifndef DEF_SCOREMNG_H
#define DEF_SCOREMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#include <string>

/*
	@brief	�X�R�A����Ǘ��N���X
			�Q�[�����ł̃X�R�A�\���⃉���L���O�̍쐬���s��
*/
class CScoreMng : public Base
{
private:
	enum
	{
		RANK_NUM = 50,		// �����L���O�o�^��
	};

	int score_;			// ���݂̃X�R�A

	struct Ranking		// �����L���O�o�^���
	{
		std::string name;		// ���O
		int score;				// �X�R�A


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
	std::vector<Ranking> ranking_;		// �����L���O

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
	void init();
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
		@brief	���݂̃X�R�A���g�������L���O�o�^
		@param	[in]	name	�����L���O�o�^��
		@return	�Ȃ�
	*/
	void RegisterRanking(const std::string& name);


};

#endif