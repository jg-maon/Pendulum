#include "stage4.h"


#include "gameManager.h"

CStage4::CStage4(std::ifstream& f) :
IStage("Stage4")
{
	load(f);
}

void CStage4::step()
{
	__super::step();

	// 多重スクロールとか


}

void CStage4::draw()
{
	__super::draw();
	graph::Draw_GraphicsLeftTop(
		0, 0, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(cameraRect.right - cameraRect.left) / 1280.f,
		(cameraRect.bottom - cameraRect.top) / 800.f);

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
