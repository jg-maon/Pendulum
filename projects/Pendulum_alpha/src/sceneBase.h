#ifndef DEF_SCENEBASE_H
#define DEF_SCENEBASE_H

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#ifndef DEF_GAMEMANAGER_H
#include "gameManager.h"
#endif

#include <memory>
class IScene
{
public:
	class ShareResource
	{
	public:
		CStageMng* stage;
		ShareResource(){}
		~ShareResource()
		{
			if(stage != nullptr)
			{
				delete stage;
				stage = nullptr;
			}
		}
		void Create()
		{
			stage = new CStageMng();
		}
		void Destroy()
		{
			if(stage != nullptr)
			{
				delete stage;
				stage = nullptr;
			}
		}
		void Update()
		{
			stage->step();
		}

		void Draw()
		{
			stage->draw();
		}

	};
	typedef std::unique_ptr<ShareResource> ShareResPtr; 
protected:
	enum class State		// シーン内部状態
	{
		INNING,		// シーン移り始め(INだとdefineされているので区別)
		MAIN,		// シーンのメイン処理
		OUTING,		// シーン移り終わり(OUTだとdefineされているので区別)
	};
	State state_;
	static ShareResPtr shareRes_;
	int bgmVolum_;		// BGM音量
	//std::string name_;
public:
	IScene():state_(IScene::State::INNING),bgmVolum_(100){}
	//IScene(const std::string& name):name_(name){}
	
	virtual ~IScene() =0 {}

	virtual IScene* step() =0 {return this;}
	virtual void draw() =0 {}
	
	static void CreateShareResource(){ shareRes_->Create(); }
	static void DestroyShareResource(){ shareRes_->Destroy(); }
	static void UpdateShareResource(){ shareRes_->Update(); }

};

#endif