#include "stage3.h"

#include "gameManager.h"

CStage3::CStage3(std::ifstream& f) :
IStage("Stage3")
{
	load(f, 0);
}

void CStage3::step()
{
	__super::step();

	// 多重スクロールとか


}

void CStage3::draw()
{
	__super::draw();

}

void CStage3::init(std::ifstream& f)
{
	__super::init(f);
}

bool CStage3::UpdateClearAnnounce()
{
	return true;
}

bool CStage3::UpdateNormal()
{
	return false;
}

bool CStage3::UpdateBoss()
{
	return false;
}
