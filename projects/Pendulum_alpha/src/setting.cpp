#include "setting.h"

#include "gameManager.h"

extern CGameManager* gm;

namespace setting
{

// OnCreate���Ă΂��setting����
void OnCreateSetup()
{

}

int GetFontID(const std::string& resname)
{
	return gm->fm().GetFontID(resname);
}


}	// namespace setting