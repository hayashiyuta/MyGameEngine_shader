//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

Texture2D		g_toon_texture : register(t1);

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global:register(b0)
{
	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matW;	//���[���h�s��
	float4x4	matNormal;
	float4		diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		ambientColor;
	float4		specularColor;
	bool		isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���

};

cbuffer global:register(b1)
{
	float4		lightPosition;
	float4		eyePosition;
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	//UV���W
	float4 color	: COLOR;	//�F�i���邳�j
	float4 eyev		:POSITION;
	float4 normal	:NORMAL;
};

/*struct PS_IN
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	//UV���W
	float4 eyev		:POSITION1;
	float4 normal	:POSITION2;//�@��   XYZ -> RGB *2-1  
  	float4 light	:POSITION3;         RGB -> X Y Z  +1/2  X Y Z(0,0,1) -> RGB(0.5,0.5,1)
};*/

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;
	normal.w = 0;//���s�ړ��Ȃ��Ń}�b�s���O�����
	pos = pos + normal * 0.05;
	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	//outData.uv = uv;
	outData.uv = uv;
	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);
	outData.normal = normal;

	float4 light = normalize(lightPosition);
	light = normalize(light);

	outData.color = saturate(dot(normal, light));
	float4 posw = mul(pos, matW);
	outData.eyev = eyePosition - posw;


	//�@������]
	/*normal = mul(normal, matW);

	float4 light = float4(1, 0, 0, 0);
	light = normalize(light);
	outData.color = clamp(dot(normal, light), 0, 1);*/

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	/*float4 light = float4(0.0, 2, 0, 1);//�_�����̈ʒu
	light = mul(light, matW);
	float3 LD = inData.pos_ - light;//���̕����x�N�g��
	float len = length(LD);//���̕����x�N�g���𐳋K��
	float4 outColor = { 1, 1, 1, 1 };
	float lightMagnitude = seturate(dot(inData.normal, -normalize(LD)));
	float k = seturate(1.0f / (1.0f + 1.0 * len * len));

	return outColor * (0.8 * k * lightMagnitude * 0.2f);*/

	float4 lightSource = float4(1.0f, 1.0f, 1.0f, 1.0f);//���C�g�̐F�Ɩ��邳 Iin
	float4 ambentSource = ambientColor;//��(�A���r�G���g�W�� Ka)
	//return lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;//float4(1,1,1,1)
	float4 diffuse;
	float4 ambient;
	float4 NL = dot(inData.normal,normalize(lightPosition));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPosition));
	float4 specular = pow(saturate(dot(reflect,normalize(inData.eyev))),8) * specularColor;
	//�g�U����
	float2 uv;
	uv.x = abs(dot(normalize(inData.eyev), inData.normal));
	uv.y = abs(dot(normalize(inData.eyev), inData.normal));
	//float4 nk = 0.1 * step(n1, inData.color) + 0.3 * step(n2, inData.color) + 
				//0.3 * step(n3, inData.color) + 0.4 * step(n4, inData.color);
	float4 t1 = g_toon_texture.Sample(g_sampler, uv);



	uv.x = 0.5;

	if (isTexture == 0) {
		diffuse = lightSource * diffuseColor * t1;
		ambient = lightSource * diffuseColor * ambentSource;
	}
	else {
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * t1;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambentSource;
	}
	//�֊s=�����x�N�g���Ɩʂ̖@���̊p�x��90�x�t��

	return ambient;
	//return t1;
}