#include "stage1.h"

CStage1::CStage1(ifstream& f) :
IStage("Stage1")
{
	load(f);
}

void CStage1::step()
{

	for (auto& ap : actionPoints_)
		ap->step();

	// 多重スクロールとか


}

void CStage1::draw()
{
	for (auto& ap : actionPoints_)
		ap->draw();
	graph::Draw_Graphics(
		0, 0, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(rect.right - rect.left) / 1280.f,
		(rect.bottom - rect.top) / 800.f);

}

void CStage1::init(std::ifstream& f)
{
	
}