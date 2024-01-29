#include<assert.h>
#include "Fbx.h"
#include"Direct3D.h"
#include"Camera.h"
#include"Texture.h"

using namespace Direct3D;
using namespace Camera;

const XMFLOAT4 Light = {3.0f,1.0f,1.0f,1.0f};

Fbx::Fbx():pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), polygonCount_(0),pMaterialList_(nullptr), materialCount_(0)
{

}

HRESULT Fbx::Load(std::string fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = mesh->GetPolygonCount();	//ポリゴンの数
	materialCount_ = pNode->GetMaterialCount();

	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//引数のfileNameからディレクトリ部分を取得
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	

	//カレントディレクトリ変更
	SetCurrentDirectory(dir);


	InitVertex(mesh);		//頂点バッファ準備
	InitIndex(mesh);		//インデックスバッファ準備
	IntConstantBuffer();	//コンスタントバッファ準備
	InitMaterial(pNode);

	//カレントディレクトリを元に戻す
	SetCurrentDirectory(defaultCurrentDir);

	//マネージャ解放
	pFbxManager->Destroy();
	pToonTex_ = new Texture;
	pToonTex_->Load("Assets\\toon2.png");
	return S_OK;
}

//頂点バッファ準備
void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//頂点情報を入れる配列
	VERTEX* vertices = new VERTEX[vertexCount_];
	
	//全ポリゴン
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//頂点のUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//頂点の法線
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}
	FbxGeometryElementTangent* t = mesh->GetElementTangent(0);
	//タンジェント取得
	for (int i = 0; i < polygonCount_; i++)
	{
		//mesh->GetElementTangentCount();//0
		FbxVector4 tangent{ 0,0,0,0 };
		int startsIndex = mesh->GetPolygonVertexIndex(i);
		if (t) 
		{
			tangent = t->GetDirectArray().GetAt(startsIndex).mData;
			
		}
		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertices()[startsIndex + j];
			vertices[index].tangent
				= { (float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f };
		}
		

	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;//sizeof(vertices[0]) * vertexCount_
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	

	Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
}

//インデックスバッファ準備
void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	index_Count_ = vector<int>(materialCount_);

	//int* index = new int[polygonCount_ * 3];

	vector<int> index(polygonCount_ * 3);//ポリゴン数*3=全頂点分用意

	

	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;

		//全ポリゴン
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3頂点分
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}

			}
		}
		index_Count_[i] = count;

		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;//sizeof(index)
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
	}
}



void Fbx::IntConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	//materialCount_ = pNode->GetMaterialCount();
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		pMaterialList_[i].pTexture = nullptr;

		//マテリアルの色
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);//i番目のマテリアル情報を取得
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;
		FbxDouble3  diffuse = pPhong->Diffuse;
		FbxDouble3  ambient = pPhong->Ambient;
		//FbxDouble3  specular = pPhong->Specular;

		
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterialList_[i].specular = XMFLOAT4(0, 0, 0, 0);//ハイライトは黒
		pMaterialList_[i].shininess = 1;

		//Mayaで指定したのがフォンシェーダーだったら
		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			//Mayaで指定したSpecularColorの情報
			FbxDouble3 specular = pPhong->Specular;
			pMaterialList_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);

			FbxDouble shininess = pPhong->Shininess;
			pMaterialList_[i].shininess = (float)shininess;
		}

		//テクスチャ情報
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
		//テクスチャの数数
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//テクスチャあり
		if (fileTextureCount == true)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//ファイル名+拡張だけにする
			char name[_MAX_FNAME];	//ファイル名
			char ext[_MAX_EXT];	//拡張子
			_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
			wsprintf(name, "%s%s", name, ext);

			//ファイルからテクスチャ作成
			pMaterialList_[i].pTexture = new Texture;
			HRESULT hr = pMaterialList_[i].pTexture->Load(name);
			//assert(hr == S_OK);
		}
		else {
			pMaterialList_[i].pTexture = nullptr;
		}


		//ノーマルテクスチャ
		//テクスチャ情報
		lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
		//テクスチャの数数
		fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//テクスチャあり
		if (fileTextureCount == true)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//ファイル名+拡張だけにする
			char name[_MAX_FNAME];	//ファイル名
			char ext[_MAX_EXT];	//拡張子
			_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
			wsprintf(name, "%s%s", name, ext);

			//ファイルからテクスチャ作成
			pMaterialList_[i].pNormalTexture = new Texture;
			HRESULT hr = pMaterialList_[i].pNormalTexture->Load(name);
			//assert(hr == S_OK);
		}
		else {
			pMaterialList_[i].pNormalTexture = nullptr;
		}
	}
}

void    Fbx::Draw(Transform& transform)
{
	if (state_ == RENDER_DIRLIGHT)
		Direct3D::SetShader(SHADER_NORMALMAP);
	else
		Direct3D::SetShader(SHADER_3D);
	transform.Calclation();//トランスフォームを計算
	
	//ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	//Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	//ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	//Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
	for (int i = 0; i < materialCount_; i++)
	{
		CONSTANT_BUFFER cb;

		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matNormal = XMMatrixTranspose(transform.GetWorldMatrix());
		cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());

		cb.diffuseColor = pMaterialList_[i].diffuse;
		cb.ambientColor = pMaterialList_[i].ambient;
		cb.specularColor = pMaterialList_[i].specular;
		cb.shininess = pMaterialList_[i].shininess;
		//cb.lightPosition = Light;
		//XMStoreFloat4(&cb.eyePos, Camera::GetCamPosition());
		cb.hasTextured = pMaterialList_[i].pTexture != nullptr;
		cb.hasNormalMap = pMaterialList_[i].pNormalTexture != nullptr;

		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る


		Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開
		

		//頂点バッファ
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		// インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用


		if (pMaterialList_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
		}
		if (pMaterialList_[i].pNormalTexture)
		{
			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pNormalTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(1, 1, &pSRV);
		}

		//ID3D11ShaderResourceView* pSRVToon = pToonTex_->GetSRV();
		//Direct3D::pContext_->PSSetShaderResources(1, 1, &pSRVToon);

		Direct3D::pContext_->DrawIndexed(index_Count_[i], 0, 0);//polygonCount_ * 3
	}
	/*for (int j = 0; j < 2; j++)
	{
		Direct3D::SetShader(SHADER_3D);
	}*/
}

void    Fbx::Release()
{
	//SAFE_RELEASE(pConstantBuffer_);
	//SAFE_RELEASE(pIndexBuffer_);
	//SAFE_RELEASE(pVertexBuffer_);
}