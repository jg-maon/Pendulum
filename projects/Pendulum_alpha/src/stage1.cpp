#include "stage1.h"

CStage1::CStage1(ifstream& f) :
IStage("Stage1")
{
	load(f);
	LoadClear(f, goalArea_);
}

void CStage1::step()
{
	__super::step();
	// 多重スクロールとか


}

void CStage1::draw()
{
	__super::draw();

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



void CStage1::LoadClear(std::ifstream& f, mymath::ShapefPtr& area)
{
	if (!common::FindChunk(common::SeekSet(f), "#StageClear"))
	{
		debug::BToM("CStage1::LoadGoal #StageClear not found");
		return;
	}
	std::string label;
	f >> label;
	if (label == "Goal")
	{
		// ゴールポイント
		std::vector<float> info;
		std::getline(f, label);
		std::stringstream ss(label);
		while (!ss.eof())
		{
			float val;
			ss >> val;
			info.push_back(val);
		}
		if (info.size() == 3)
		{
			// X Y Radius
			area = mymath::ShapefPtr(new mymath::Circlef(info[0], info[1], 0.5f, info[2]));
			auto& circle = std::dynamic_pointer_cast<mymath::Circlef>(area);
			goalObj_.pos = circle->center;
			goalObj_.size = static_cast<int>(circle->radius);
		}
		else if (info.size() == 4)
		{
			// X Y W H
			area = mymath::ShapefPtr(new mymath::Rectf(info[0], info[1], info[2], info[3]));
			auto& rect = std::dynamic_pointer_cast<mymath::Rectf>(area);
			goalObj_.size.x = rect->right - rect->left;
			goalObj_.size.y = rect->bottom - rect->top;
			goalObj_.pos.x = rect->left + goalObj_.HalfWidth();
			goalObj_.pos.y = rect->top + goalObj_.HalfHeight();
		}
	}
	else if (label == "Annihilation")
	{
		// 殲滅型
	}
}