#ifndef DEF_GAMESTATUS_H
#define DEF_GAMESTATUS_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

/*
	@brief	���C���Q�[���̏�Ԃ��Ǘ�
*/
class CGameStatus : public Base
{
private:




public:
	CGameStatus();
	~CGameStatus();
	
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() override;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() override;

	/*
		@brief	�q�b�g�X�g�b�v�J�n
		@return	�Ȃ�
	*/
	void StartHitStop();

};

CGameStatus::CGameStatus():
Base("GameStatus")
{
}

CGameStatus::~CGameStatus()
{
}

void CGameStatus::step()
{

}

void CGameStatus::draw()
{

}

void CGameStatus::StartHitStop()
{

}


#endif