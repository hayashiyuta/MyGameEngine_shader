//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	matrix g_matWorld;		// ���_���W�ϊ��s��
	matrix g_matTexture;	// �e�N�X�`�����W�ϊ��s��
	float4 g_vecColor;		// �e�N�X�`�������F
	float  g_scroll;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	//UV���W
	//float4 color	: COLOR;	//�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)//, float4 normal : NORMAL
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�s���������
	//���[���h���W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, g_matWorld);
	outData.uv = mul(uv, g_matTexture);

	//�@������]
	/*normal = mul(normal, matW);

	float4 light = float4(-1, 0.5, -0.7, 0);
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1*/

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	//float output = floor(g_texture.Sample(g_sampler, inData.uv)*8.0)/8;
	float4 output;
	float2 tmpUV = inData.uv;
	
	tmpUV.x += g_scroll;
	output = g_vecColor * g_texture.Sample(g_sampler, tmpUV);

	return output;
}