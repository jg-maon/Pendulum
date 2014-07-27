#include "stage4.h"


#include "gameManager.h"

CStage4::CStage4(std::ifstream& f) :
IStage("Stage4")
{
	load(f, 0);
}

void CStage4::step()
{
	__super::step();

	// 多重スクロールとか


}

void CStage4::draw()
{
	__super::draw();

}

void CStage4::init(std::ifstream& f)
{
	__super::init(f);
}

bool CStage4::UpdateClearAnnounce()
{
	return true;
}

bool CStage4::UpdateNormal()
{
	return false;
}

bool CStage4::UpdateBoss()
{
	return false;
}
