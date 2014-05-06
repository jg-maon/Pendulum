

#pragma once

#include "dx.h"

#include "media_manager.h"
#include "dx_font.h"
#include "dx_graphics3d.h"
#include	<unordered_map>

class DX3DMESHMANAGER
{
private:
	//描画管理クラス
	Dx_Graphics3D *g;
	//メディア管理クラス
	MediaManager *m_manager;	//アニメ付メッシュ
	class AnimMesh{
		public:
		DxAnimeMesh	*mesh;
		Dx_AnimeAuxiliary *anime;
		string			filename_;
		AnimMesh(MediaManager *manager,const string& fileName,const string& path)
		{
			mesh = manager->LoadAnimeMeshFromX(fileName,path);
			anime = new Dx_AnimeAuxiliary(mesh->pAnimController);
			filename_ = fileName;
		}
		~AnimMesh()
		{
			
		}
		void del()
		{
			SAFE_DELETE(mesh);
		}
	};
	class Mesh{
	public:
		DxMesh		*mesh;
		string		filename_;
		Mesh(MediaManager *manager,const string& fileName,const string& path)
		{
			mesh = manager->LoadMeshFromX(fileName,path);
			filename_ = fileName;
		}
		~Mesh()
		{
			
		}
		void del()
		{
			SAFE_DELETE(mesh);
		}
	};
	
	unordered_map<string,Mesh*>		DxMeshTable;
	unordered_map<string,AnimMesh*>	DxAnimMeshTable;
public:
	enum MESHTYPE{
		MESH = 0,
		ANIMMESH = 1,
	};
	Dx_Graphics3D* getG();

	DX3DMESHMANAGER();
	~DX3DMESHMANAGER();

	void loadMesh(const string& handleName,const string& fileName,const string& path,MESHTYPE type = MESHTYPE::MESH);
	void drawMesh(DxVec3 pos,const string& handleName,DxVec3 angle,D3DCOLOR color = ARGB(255,255,255,255));
	void drawAnimMesh(DxVec3 pos,const string& handleName,DxVec3 angle,D3DCOLOR color = ARGB(255,255,255,255));
	void ChangeAnim(const string& handleName,int nowMotion);
	void step(const string& handleName,float spd);
	float nowTrack(const string& handleName,int nowMotion);
};



