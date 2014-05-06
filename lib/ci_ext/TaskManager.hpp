#pragma once
#include "../gplib.h"
#include <vector>
#include <functional>
#include <algorithm>

namespace ci_ext
{

class TaskManager final
{
	std::vector<TaskPtr> task_;
	std::vector<TaskPtr> ins_;
	void changeScene(int sceneNo);
	void insert()
	{
		if(!ins_.empty())
		{
			task_.insert(task_.end(), ins_.begin(), ins_.end());
		}
		ins_.clear();
	}

public:
	TaskManager(int sceneNo = 0)
	{ 
		task_.clear();
		ins_.clear();
		changeScene(sceneNo);
	}
	~TaskManager() { }

	template<typename Ptr>
	void insert(Ptr* o)
	{
		ins_.push_back(std::shared_ptr<Ptr>(o));
	}

	void awake()
	{
		for(auto& task: task_)
		{
			if(task->isSleeping())
				task->awake();
		}
	}

	bool run()
	{
		//登録されているオブジェクトを動作させる
		for(auto& task: task_)
		{
			task->awake();
			if(task->isRunning())
				task->step();
		}

		//オブジェクトの消去及び追加
		task_.erase(
			std::remove_if(task_.begin(), task_.end(),
                     std::mem_fn(&Task::isDestroy)),
			task_.end()
		);
		insert();

		//登録されているオブジェクトを描画
		for(auto& task: task_)
		{
			if(task->isDraw())
				task->draw();
		}

		return false;
	}
};

}