#include "sceneNameInput.h"
#include "define.h"


#include "scoreMng.h"

//======================================
// CSceneNameInput methods
#pragma region NameInput

//定数
const mymath::Vec3f CSceneNameInput::KEYBOARD_POS = { 800.f, 400.f, 0.5f };
const mymath::Vec3f CSceneNameInput::CHANGE_POS = { 887.5f, 340.f, 0.5f };
const mymath::Vec3f CSceneNameInput::DISPLAY_POS = { 840.f, 100.f, 0.5f };
const float CSceneNameInput::FLASHING_TIME = 0.5f;
const std::string CSceneNameInput::KEYBOARD_NAME[3] = { "img_kbletter", "img_kbselect", "img_kbbutton" };
const std::string CSceneNameInput::CHANGE_NAME[3] = { "img_chgletter", "img_chgselect", "img_chgbutton" };
const std::string CSceneNameInput::DISPLAY_NAME[2] = { "img_kbletter", "img_display" };
const std::string CSceneNameInput::KEYBOARD_MARKDATA[4] = { "01234!#", "56789$%", "&()><_?", "[]^{} " };


// コンストラクタ
CSceneNameInput::CSceneNameInput() :
IScene("SceneNameInput", "img_nameInput", "bgm_nameInput")
{
}
CSceneNameInput::~CSceneNameInput()
{
}
void CSceneNameInput::start()
{
	__super::start();

	state_ = State::INPUT;
	wordcount_ = 0;			//文字数の初期化
	name_ = "";				//名前の初期化

	int col = 0;

	for (auto &keyBtn : keyButtons)
	{
		int row = 0;

		for (auto & kb : keyBtn)
		{
			//大文字小文字切り替え
			if (col == 3 && row == 5)
			{
				kb.init(
					KEYBOARD_POS,
					kbData::KB_WIDTH, kbData::KB_HEIGHT,
					BtnAttribute::CHANGE_UP_LOW,
					KEYBOARD_NAME,
					col, row);
				
			}
			//デリートボタン
			else if (col == 3 && row == 6)
			{
				kb.init(
					KEYBOARD_POS,
					kbData::KB_WIDTH, kbData::KB_HEIGHT,
					BtnAttribute::KEYBOARD_DELETE,
					KEYBOARD_NAME,
					col, row);
			}
			else{

				kb.init(
					KEYBOARD_POS,
					kbData::KB_WIDTH, kbData::KB_HEIGHT,
					BtnAttribute::KEYBOARD_DATA,
					KEYBOARD_NAME,
					col, row);
			}
			++row;
		}
		++col;
	}

	//文字変更ボタン
	changeMark[0].init(
		CHANGE_POS,
		chgData::CHG_WIDTH, chgData::CHG_HEIGHT,
		BtnAttribute::CHANGE_LETTER,
		CHANGE_NAME,
		0, 0);

	//記号変更ボタン
	changeMark[1].init(
		CHANGE_POS,
		chgData::CHG_WIDTH, chgData::CHG_HEIGHT,
		BtnAttribute::CHANGE_MARK,
		CHANGE_NAME,
		0, 1);

	int row = 0;
	for (auto & disp : display)
	{
		disp.init(DISPLAY_NAME, row);
		++row;
	}

	DisplayReflesh();

}
void CSceneNameInput::draw()
{
	font::Draw_FontTextNC(100, 100, 0.5f, "名前登録(右ボタン：完了)", ARGB(255, 0, 0, 0), 0);
	//font::Draw_FontTextNC(200, 300, 0.5f, name_, ARGB(255,0,0,0), 0);

	//bg_.drawNC(charabase::CharBase::MODE::LeftTop);
	__super::draw();

	for (auto &keyBtn : keyButtons)
	{
		for (auto & kb : keyBtn)
		{
			kb.draw();
		}

	}

	for (auto& cMark : changeMark)
	{
		cMark.draw();
	}

	for (auto & disp : display)
	{
		disp.draw();
	}
}
bool CSceneNameInput::update()
{
	switch (state_)
	{
	case CSceneNameInput::State::INPUT:
		stepInput();
		if (input::CheckPush(input::KEY_MOUSE_RBTN))				//かつ、スコアがハイスコアの時
		{
			if (!name_.empty())
				state_ = State::END;
		}
		break;
	case CSceneNameInput::State::CHECK:
		break;
	case CSceneNameInput::State::END:
		return true;
	}
	return false;
}
int CSceneNameInput::NextScene() const
{
	// ランキング登録
	gm()->scoreMng()->RegisterRanking(name_);
	return CSceneMng::Scene::TITLE;
}
void CSceneNameInput::IsRelease(const CButton& cb_)
{
	auto att = cb_.getAtt();
	switch (att)
	{
	case CSceneNameInput::BtnAttribute::NONE:

		break;

	case CSceneNameInput::BtnAttribute::KEYBOARD_DATA:
		
		if (wordcount_ >= NAME_LEN) return;	//規定文字以上のとき追加しない

		name_ += cb_.getChar();
		display[wordcount_].LoadLetter(cb_.obj());
		++wordcount_;

		DisplayReflesh();
		
		break;

	case CSceneNameInput::BtnAttribute::KEYBOARD_DELETE:

		if (wordcount_ <= 0) return;		//入力文字が0なら動作しない

		--wordcount_;
		name_.erase(wordcount_);

		DisplayReflesh();

		break;

	// CHANGE_UP_LOW、CHANGE_MARK、CHANGE_LETTERの処理
	default:

		int col = 0;
		for (auto &keyBtn : keyButtons)
		{
			int row = 0;

			for (auto & kb : keyBtn)
			{
				auto kbAtt = kb.getAtt();
				// 大文字小文字の変換
				if (att == BtnAttribute::CHANGE_UP_LOW)
				{
					if (kbAtt != BtnAttribute::KEYBOARD_DATA)
					{
						break;
					}
					kb.ChangeCapital();
					kb.ChangeLetterUpLow();
				}
				//文字へ変換
				else if (att == BtnAttribute::CHANGE_LETTER)
				{
					if (col == 3 && row == 5)
					{
						kb.setAtt(BtnAttribute::CHANGE_UP_LOW);
						kb.UpLowInit();
						return;
					}

					if (kbAtt == BtnAttribute::KEYBOARD_DATA)
					{
						kb.ChangeLetter(col, row);
					}

				}
				//記号へ変換
				else if (att == BtnAttribute::CHANGE_MARK)
				{
					if (kbAtt == BtnAttribute::CHANGE_UP_LOW)
					{
						kb.setAtt(BtnAttribute::KEYBOARD_DATA);
					}
					if (kbAtt == BtnAttribute::KEYBOARD_DELETE)
					{
						break;
					}

					kb.ChangeMark(col, row);
	
				}
				++row;
			}
			++col;
		}


		break;
	}

}
void CSceneNameInput::stepInput()
{
	

	for (auto &keyBtn : keyButtons)
	{
		for (auto & kb : keyBtn)
		{
			kb.step();
			if (kb.isReleased())
			{
				IsRelease(kb);
			}
		}
	}

	for (auto & chgMk : changeMark)
	{
		chgMk.step();
		if (chgMk.isReleased())
		{
			IsRelease(chgMk);
		}
	}

	for (auto & disp : display)
	{
		disp.step();
	}


	//================================================================
	//以下デバッグ
	//================================================================
	//if (input::CheckPull(input::KEY_BTN1))
	//{
	//	keyButtons[0][0].pushState_ = CButton::BtnState::RELEASE;
	//	if (keyButtons[0][0].isReleased())
	//	{
	//		IsRelease(keyButtons[0][0]);
	//	}
	//}
	//if (input::CheckPull(input::KEY_BTN2))
	//{
	//	keyButtons[2][3].pushState_ = CButton::BtnState::RELEASE;
	//	if (keyButtons[2][3].isReleased())
	//	{
	//		IsRelease(keyButtons[2][3]);
	//	}
	//}
	//if (input::CheckPull(input::KEY_SPACE))
	//{
	//	keyButtons[3][6].pushState_ = CButton::BtnState::RELEASE;
	//	if (keyButtons[3][6].isReleased())
	//	{
	//		IsRelease(keyButtons[3][6]);
	//	} 
	//}
	//if (input::CheckPull(input::KEY_UP))
	//{
	//	keyButtons[3][5].pushState_ = CButton::BtnState::RELEASE;
	//	if (keyButtons[3][5].isReleased())
	//	{
	//		IsRelease(keyButtons[3][5]);
	//	}
	//}
	//if (input::CheckPull(input::KEY_LEFT))
	//{
	//	changeMark[0].pushState_ = CButton::BtnState::RELEASE;
	//	if (changeMark[0].isReleased())
	//	{
	//		IsRelease(changeMark[0]);
	//	}
	//}
	//if (input::CheckPull(input::KEY_RIGHT))
	//{
	//	changeMark[1].pushState_ = CButton::BtnState::RELEASE;
	//	if (changeMark[1].isReleased())
	//	{
	//		IsRelease(changeMark[1]);
	//	}
	//}
	//================================================================
}
void CSceneNameInput::DisplayReflesh()
{
	int count = 0;
	for (auto & disp : display)
	{
		if (count < wordcount_)
		{
			disp.ChangeState(CDisplay::DispState::DISP);
		}
		else if (count == wordcount_)
		{
			disp.ChangeState(CDisplay::DispState::FLASH);
		}
		else
		{
			disp.ChangeState(CDisplay::DispState::NONE);
		}
		++count;
	}
}
/*
bool CSceneNameInput::InputNextKey()
{
	return input::CheckPush(input::KEY_BTN0);
}
//*/
#pragma endregion
// CSceneNameInput methods
//======================================


//======================================
// CSceneNameInput CButton methods
#pragma region CButton
CSceneNameInput::CButton::CButton() :
IObject("End_Button")
{
	
}
CSceneNameInput::CButton::~CButton()
{
}
void CSceneNameInput::CButton::init(const mymath::Vec3f& pos, int w, int h, BtnAttribute atr, const string resnames[3], int col, int row)
{
	obj_.pos = { pos.x + row * (w + INTERVAL_W), pos.y + col * (h + INTERVAL_H), pos.z };
	btn_.pos = { pos.x + row * (w + INTERVAL_W), pos.y + col * (h + INTERVAL_H), pos.z + 0.1f };
	select_.pos = { pos.x + row * (w + INTERVAL_W), pos.y + col * (h + INTERVAL_H), pos.z - 0.1f };

	obj_.size.x = btn_.size.x = w;
	obj_.size.y = btn_.size.y = h;

	select_.size.x = w + OFFSET_W;
	select_.size.y = h + OFFSET_H;

	attribute_ = atr;

	obj_.resname = resnames[0];
	select_.resname = resnames[1];
	btn_.resname = resnames[2];

	obj_.SetUse(true);
	select_.SetUse(false);
	btn_.SetUse(true);

	pushState_ = BtnState::NONE;


	switch (atr)
	{
	case CSceneNameInput::BtnAttribute::KEYBOARD_DATA:

		ChangeLetter(col,row);
		obj_.src.x = obj_.size.x * (row + (col * 7));

		break;

	case CSceneNameInput::BtnAttribute::KEYBOARD_DELETE:

		obj_.src.x = 26 * obj_.size.x;
		obj_.src.y = 1 * obj_.size.y;

		break;

	case CSceneNameInput::BtnAttribute::CHANGE_UP_LOW:

		UpLowInit();

		break;

	default:

		obj_.src.x = obj_.size.x * row;
		break;

	}
	

}
void CSceneNameInput::CButton::step()
{
	CheckState();

	if (attribute_ == BtnAttribute::CHANGE_UP_LOW)
	{
		obj_.SetUse(true);
	}
	switch (pushState_)
	{
	case CSceneNameInput::CButton::BtnState::NONE:

		select_.SetUse(false);
		btn_.src.x = 0;
		break;

	case CSceneNameInput::CButton::BtnState::SELECT:


		break;
	case CSceneNameInput::CButton::BtnState::PRESS:

		btn_.src.x = btn_.size.x;

		break;
	case CSceneNameInput::CButton::BtnState::RELEASE:

		btn_.src.x = 0;
		break;
	}
}
void CSceneNameInput::CButton::draw()
{
	if (obj_.CheckUse())
		obj_.draw2NC();

	if (select_.CheckUse()) 
		select_.draw2NC();
	
	if (btn_.CheckUse())
		btn_.draw2NC();

}
bool CSceneNameInput::CButton::CheckSelect()
{
	const mymath::Vec3f& p = gm()->GetCursorPosNC();
/*
	mymath::Rectf rt = { obj_.pos.x - obj_.size.x / 2.f, obj_.pos.y - obj_.size.y / 2.f,
		obj_.pos.x + obj_.size.x / 2.f, obj_.pos.y + obj_.size.y / 2.f };
//*/
	const mymath::Rectf& rt = obj_.GetRect();

	if (rt.Contains(p))
	{
		return true;
	}

	return false;
	
}
/*
bool CSceneNameInput::CButton::CheckRelease()
{
	return input::CheckPull(input::KEY_MOUSE_LBTN);
}
bool CSceneNameInput::CButton::CheckPress()
{
	return input::CheckPress(input::KEY_MOUSE_LBTN);
}
//*/
void CSceneNameInput::CButton::CheckState()
{
	if (attribute_ == BtnAttribute::NONE)
	{
		pushState_ = BtnState::NONE;
		obj_.SetUse(false);
	}
	else if (CheckSelect())
	{
		select_.SetUse(true);

		if (input::CheckPress(input::KEY_MOUSE_LBTN))
		{
			pushState_ = BtnState::PRESS;
		}
		else if (input::CheckPull(input::KEY_MOUSE_LBTN))
		{
			pushState_ = BtnState::RELEASE;
		}
		else
		{
			pushState_ = BtnState::SELECT;
		}
	}
	else
	{
		pushState_ = BtnState::NONE;
	}
}
void CSceneNameInput::CButton::UpLowInit()
{
	
	obj_.src.x = 26 * obj_.size.x;
	obj_.src.y = 0 * obj_.size.y;
}
void CSceneNameInput::CButton::ChangeLetterUpLow()
{
	if (isCapital_)
	{
		obj_.src.y = 0;
	}
	else
	{
		obj_.src.y = obj_.size.y;
	}
	charData_ = charData_ ^ 32;

}
void CSceneNameInput::CButton::ChangeLetter(int col, int row)
{
	charData_ = 'A' + (row + (col * 7));
	
	obj_.src.y = 0;
	isCapital_ = true;
}
void CSceneNameInput::CButton::ChangeMark(int col, int row)
{
	charData_ = KEYBOARD_MARKDATA[col][row];
	obj_.src.y = obj_.size.y * 2;
	isCapital_ = true;
}
bool CSceneNameInput::CButton::isReleased()
{
	if (pushState_ == BtnState::RELEASE)
	{
		return true;
	}
	return false;
}

void CSceneNameInput::CButton::ChangeCapital()
{
	isCapital_ ^= 1;
}

char CSceneNameInput::CButton::getChar() const
{
	return charData_;
}

CSceneNameInput::BtnAttribute CSceneNameInput::CButton::getAtt() const
{
	return attribute_;
}

void CSceneNameInput::CButton::setAtt(CSceneNameInput::BtnAttribute att)
{
	attribute_ = att;
}

#pragma endregion
// CSceneNameInput CButton methods
//======================================


//======================================
// CSceneNameInput CDisplay methods
#pragma region CDisplay
CSceneNameInput::CDisplay::CDisplay() :
IObject("End_Display")
{

}
CSceneNameInput::CDisplay::~CDisplay()
{
}
void CSceneNameInput::CDisplay::StepFlashing()
{
	phaseTime_ += time::Time_GetOneFrameCount();

	if (phaseTime_ >= FLASHING_TIME)
	{
		flash_.CheckUse() ? flash_.SetUse(false) : flash_.SetUse(true);
		phaseTime_ = 0.f;
	}
}
void CSceneNameInput::CDisplay::ChangeState(DispState ds_)
{
	dispState_ = ds_;

	switch (ds_)
	{
	case CSceneNameInput::CDisplay::DispState::NONE:

		obj_.SetUse(false);
		flash_.SetUse(false);
		break;
	case CSceneNameInput::CDisplay::DispState::FLASH:

		obj_.SetUse(false);
		flash_.SetUse(true);
		break;
	case CSceneNameInput::CDisplay::DispState::DISP:

		obj_.SetUse(true);
		flash_.SetUse(false);
		break;
	}

}
void CSceneNameInput::CDisplay::LoadLetter(const charabase::CharBase& cb_)
{
	obj_.src = cb_.src;

}
void CSceneNameInput::CDisplay::init(const string resnames[2], int row)
{
	obj_.pos = flash_.pos = DISPLAY_POS;

	obj_.size.x = flash_.size.x = DispSize::DISP_WIDTH;
	obj_.size.y = flash_.size.y = DispSize::DISP_HEIGHT;

	obj_.pos.x += obj_.size.x * row;
	flash_.pos.x += flash_.size.x * row;

	obj_.resname = DISPLAY_NAME[0];
	flash_.resname = DISPLAY_NAME[1];
	
	phaseTime_ = 0.f;

}
void CSceneNameInput::CDisplay::step()
{
	switch (dispState_)
	{
	case CSceneNameInput::CDisplay::DispState::NONE:

		break;
	case CSceneNameInput::CDisplay::DispState::FLASH:
		StepFlashing();
		
		break;
	case CSceneNameInput::CDisplay::DispState::DISP:
		  
		break;
	}
}
void CSceneNameInput::CDisplay::draw()
{
	if (obj_.CheckUse())
		obj_.draw2NC();

	if (flash_.CheckUse())
		flash_.draw2NC();

}


#pragma endregion
// CSceneNameInput CDisplay methods
//======================================