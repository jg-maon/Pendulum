#include "stage2.h"

#include "gameManager.h"

CStage2::CStage2(std::ifstream& f) :
IStage("Stage2")
{
	load(f);
}

void CStage2::step()
{
	__super::step();

	// 多重スクロールとか


}

void CStage2::draw()
{
	__super::draw();
	graph::Draw_GraphicsLeftTop(
		0, 0, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(cameraRect.right - cameraRect.left) / 1280.f,
		(cameraRect.bottom - cameraRect.top) / 800.f);

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
