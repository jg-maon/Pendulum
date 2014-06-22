#include "stage2.h"

CStage2::CStage2(ifstream& f) :
IStage("Stage2")
{
	load(f);
}

void CStage2::step()
{

	for (auto& ap : actionPoints_)
		ap->step();

	// ���d�X�N���[���Ƃ�


}

void CStage2::draw()
{
	for (auto& ap : actionPoints_)
		ap->draw();
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