#ifndef DEF_SCENEBASE_HPP
#define DEF_SCENEBASE_HPP


#include <memory>
class IScene
{
protected:
	enum class State		// シーン内部状態
	{
		INNING,		// シーン移り始め(INだとdefineされているので区別)
		MAIN,		// シーンのメイン処理
		OUTING,		// シーン移り終わり(OUTだとdefineされているので区別)
	};
	State state_;
	int bgmVolum_;		// BGM音量
	//std::string name_;
public:
	IScene():state_(IScene::State::INNING),bgmVolum_(100){}
	//IScene(const std::string& name):name_(name){}
	
	virtual ~IScene() =0 {}

	virtual IScene* step() =0 {return this;}
	virtual void draw() =0 {}
	
};

#endif