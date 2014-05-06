#pragma once

#include <memory>
#include "DrawObj.hpp"
#include "Vec3.hpp"


namespace ci_ext
{

class TaskManager;

class Task
{
public:
	enum class Group
	{
		system = 1 << 0,
		all = -1
	};

	enum class TaskStatus
	{
		null,
		run,
		sleep,
		destroy,
	};

	enum class DrawStatus
	{
		null,
		on,
		off,
		sleep
	};

private:
	static TaskManager* p_;
	Group group_;
	TaskStatus status_;
	DrawStatus drawStatus_;
	int framecount_;

public:
	TaskManager* p() const 
	{
		return p_; 
	}

	Task(Group group = Group::all,
		TaskStatus status = TaskStatus::run,
		DrawStatus draw = DrawStatus::on)
		: 
		group_(group),
		status_(status),
		drawStatus_(draw),
		framecount_(0)
	{	
	}

	Task(TaskManager* p, Group group = Group::all,
		TaskStatus status = TaskStatus::run,
		DrawStatus draw = DrawStatus::on)
		:
		group_(group),
		status_(status),
		drawStatus_(draw),
		framecount_(0)
	{
		p_ = p;
	}
	virtual ~Task() {}
	bool isDestroy() const
	{
		return (status_ == TaskStatus::destroy);
	}
	bool isRunning() const
	{
		return (status_ == TaskStatus::run);
	}
	bool isDraw() const
	{
		return (drawStatus_ == DrawStatus::on);
	}
	bool isSleeping() const
	{
		return (status_ == TaskStatus::sleep);
	}
	void awake()
	{
		if(isSleeping())
		{
			if(--framecount_ < 0) status_ = TaskStatus::run;
		}
	}
	void kill() 
	{
		status_ = TaskStatus::destroy; 
	}
	virtual void draw() = 0;
	virtual void step() = 0;
};
typedef std::shared_ptr<Task> TaskPtr;

//TaskManagerのポインタを登録するために使うダミークラス
class TaskFirst : public Task
{
public:
	TaskFirst(ci_ext::TaskManager* g) : 
		Task(g) {}
	void step() {}
	void draw() {}
};

class TaskDrawable : public Task
{
protected:
	DrawObj<float> location_;

public:
	TaskDrawable(
		const Vec3<float>& pos, const Vec3<float>& scale, const IMGOBJ img,
		const Vec3<float>& src, const Vec3<float>& srcSize,
		const Color& color, const float degree) 
		:
		location_(pos, scale, img, src, srcSize, color, degree)
	{
	}
	virtual void step() = 0;
	virtual void draw() 
	{
		location_.draw();
	}
};

}