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

	// ���d�X�N���[���Ƃ�


}

void CStage1::draw()
{
	for (auto& ap : actionPoints_)
		ap->draw();
	graph::Draw_GraphicsLeftTop(
		cameraRect_.left, cameraRect_.top, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(cameraRect_.right - cameraRect_.left) / 1280.f,
		(cameraRect_.bottom - cameraRect_.top) / 800.f);

}

void CStage1::init(std::ifstream& f)
{
	__super::init(f);

}