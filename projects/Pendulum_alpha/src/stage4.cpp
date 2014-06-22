#include "stage4.h"

CStage4::CStage4(ifstream& f) :
IStage("Stage4")
{
	load(f);
}

void CStage4::step()
{
	__super::step();

	// ���d�X�N���[���Ƃ�


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