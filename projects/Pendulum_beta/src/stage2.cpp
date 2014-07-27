#include "stage2.h"

#include "gameManager.h"

CStage2::CStage2(std::ifstream& f) :
IStage("Stage2")
{
	load(f, 0);
}

void CStage2::step()
{
	__super::step();

	// 多重スクロールとか


}

void CStage2::draw()
{
	__super::draw();

}

void CStage2::init(std::ifstream& f)
{
	__super::init(f);
}

bool CStage2::UpdateClearAnnounce()
{
	return true;
}

bool CStage2::UpdateNormal()
{
	return false;
}

bool CStage2::UpdateBoss()
{
	return false;
}
