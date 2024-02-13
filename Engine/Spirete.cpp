#include "Spirete.h"
#include "Camera.h"
#include <DirectXMath.h>


const int WINDOW_WIDTH = 800;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���

Spirete::Spirete() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), 
pConstantBuffer_(nullptr), pTexture_(nullptr),scrollVal(0)
{
}

Spirete::~Spirete()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

HRESULT Spirete::Initialize()
{
	
	InitVertexData();
	
	
	HRESULT hr = CreateVertexBuffer();
	if (FAILED(hr))
	{
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	
	InitIndexData();
	
	hr = CreateIndexBuffer();
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;

	}

	hr = CreateConstantBuffer();
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;

	}

	hr = LoadTexture();
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�e�N�X�`���̃��[�h�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;

	}

	return S_OK;
}

void Spirete::Draw(Transform& transform)
{
	
	Direct3D::SetShader(SHADER_2D);

	SetBufferToPipeline();

	PassDataTcCB(transform.GetWorldMatrix());

	Direct3D::pContext_->DrawIndexed((UINT)indexNum, (UINT)0, 0);
}

void Spirete::Draw(Transform& transform, RECT rect, float alpha)
{
	scrollVal = 0.0f;
	scrollVal += 0.01f;

	//���낢��ݒ�
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::SetDepthBafferWriteEnable(false);
	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// �p�����[�^�̎󂯓n��
	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;

	transform.Calclation();//�g�����X�t�H�[�����v�Z

	//�\������T�C�Y�ɍ��킹��
	XMMATRIX cut = XMMatrixScaling((float)rect.right, (float)rect.bottom, 1);

	//��ʂɍ��킹��
	XMMATRIX view = XMMatrixScaling(1.0f / Direct3D::screenSize.cx, 1.0f / Direct3D::screenSize.cy, 1.0f);

	//�ŏI�I�ȍs��
	XMMATRIX world = cut * transform.matScale_ * transform.matRotate_ * view * transform.matTranslate_;
	cb.world = XMMatrixTranspose(world);

	// �e�N�X�`�����W�ϊ��s���n��
	XMMATRIX mTexTrans = XMMatrixTranslation((float)rect.left / (float)pTexture_->GetTextureSize().x,
		(float)rect.top / (float)pTexture_->GetTextureSize().y, 0.0f);
	XMMATRIX mTexScale = XMMatrixScaling((float)rect.right / (float)pTexture_->GetTextureSize().x,
		(float)rect.bottom / (float)pTexture_->GetTextureSize().y, 1.0f);
	XMMATRIX mTexel = mTexScale * mTexTrans;
	cb.uvTrans = XMMatrixTranspose(mTexel);

	cb.color = XMFLOAT4(1, 1, 1, alpha);

	cb.scroll = scrollVal;

	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃��\�[�X�A�N�Z�X���ꎞ�~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// ���\�[�X�֒l�𑗂�


	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	// GPU����̃��\�[�X�A�N�Z�X���ĊJ

	SetBufferToPipeline();

	//�|���S�����b�V����`�悷��
	Direct3D::pContext_->DrawIndexed(indexNum, 0, 0);

	Direct3D::SetShader(SHADER_TYPE::SHADER_3D);

	Direct3D::SetDepthBafferWriteEnable(true);
}


void Spirete::Release()
{
	SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pTexture_);

	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);

	//pConstantBuffer_->Release();
	//pIndexBuffer_->Release();
	//pVertexBuffer_->Release();
}


void Spirete::InitVertexData()//���_���̏���
{


	// ���_���
	vertices_ =
	{
		{XMVectorSet(400 / (WINDOW_WIDTH / 2.0f) - 1.0f,  -300 / (WINDOW_HEIGHT / 2.0f) + 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },	// �l�p�`�̒��_�i����j
		{XMVectorSet(800 / (WINDOW_WIDTH / 2.0f) - 1.0f, -300  / (WINDOW_HEIGHT / 2.0f) + 1.0f, 0.0f, 0.0f),XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },	// �l�p�`�̒��_�i�E��j
		{XMVectorSet(800 / (WINDOW_WIDTH / 2.0f) - 1.0f, -600  / (WINDOW_HEIGHT / 2.0f) + 1.0f, 0.0f, 0.0f),XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },	// �l�p�`�̒��_�i�E���j
		{XMVectorSet(400 / (WINDOW_WIDTH / 2.0f) - 1.0f, -600  / (WINDOW_HEIGHT / 2.0f) + 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	// �l�p�`�̒��_�i�����j	
		//XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f),	// �l�p�`�̒��_
	};
	vertexNum = vertices_.size();
}

HRESULT Spirete::CreateVertexBuffer()//���_�o�b�t�@���쐬
{
	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices_[0])* vertexNum;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = &vertices_[0];
	HRESULT hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}



void Spirete::InitIndexData()//�C���f�b�N�X���̏���
{
	//�C���f�b�N�X���
	index_ = { 0,2,3, 0,1,2 };

	indexNum = index_.size();

}

HRESULT Spirete::CreateIndexBuffer()//�C���f�b�N�X�o�b�t�@���쐬
{
	// �C���f�b�N�X�o�b�t�@�𐶐�����
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index_[0]) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &index_[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	HRESULT hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;

	}
	return S_OK;
}

HRESULT Spirete::CreateConstantBuffer()//�R���X�^���g�o�b�t�@�쐬
{
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;

	}
	return S_OK;
}

HRESULT Spirete::LoadTexture()//�e�N�X�`�������[�h
{
	pTexture_ = new Texture;
	pTexture_->Load("Assets\\Lenna.jpg");

	return S_OK;
}

void Spirete::PassDataTcCB(DirectX::XMMATRIX worldMatrix)//�R���X�^���g�o�b�t�@�Ɋe�����n��
{
	//�R���X�^���g�o�b�t�@�ɓn�����

	CONSTANT_BUFFER cb;
	D3D11_MAPPED_SUBRESOURCE pdata;
	//cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.world = XMMatrixTranspose(worldMatrix);
	cb.scroll = scrollVal;
	

	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);



	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//�ĊJ
}

void Spirete::SetBufferToPipeline()//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
{
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

	Direct3D::pContext_->DrawIndexed(indexNum, 0, 0);
}

