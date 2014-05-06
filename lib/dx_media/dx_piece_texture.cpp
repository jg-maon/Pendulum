
//**************************************************************************
//部分テクスチャ構造体
//基本型　：　DxTexture
//派生型　：　なし
//**************************************************************************

#include "dx_piece_texture.h"

//コンストラクタ
DxPieceTexture::DxPieceTexture() : DxTexture()
{
	//表示領域の初期化
	SetRect(&this->tex_pos,0,0,0,0);
	//テクスチャ中心座標の初期化
	this->tex_center = DxVec2(0,0);
	//カラー情報の初期化
	this->color = D3DCOLOR_ARGB(255,255,255,255);
}
