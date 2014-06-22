#include "stage3.h"

CStage3::CStage3(ifstream& f) :
IStage("Stage3")
{
	load(f);
}

void CStage3::step()
{

	for (auto& ap : actionPoints_)
		ap->step();

	// 多重スクロールとか


}

void CStage3::draw()
{
	for (auto& ap : actionPoints_)
		ap->draw();
	graph::Draw_GraphicsLeftTop(
		0, 0, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(cameraRect.right - cameraRect.left) / 1280.f,
		(cameraRect.bottom - cameraRect.top) / 800.f);

}

void CStage3::init(std::ifstream& f)
{
	__super::init(f);
}