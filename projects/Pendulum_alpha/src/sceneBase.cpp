#include "sceneBase.h"
std::unique_ptr<IScene::ShareResource> IScene::shareRes_ = std::unique_ptr<IScene::ShareResource>(new IScene::ShareResource());