//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

Texture2D		normalTex: register(t1);

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
	int		hasTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
	int		hasNormalMap;

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
	float4 pos		: SV_POSITION;	//�ʒu
	float2 uv		: TEXCOORD;	//UV���W
	float4 eyev		: POSITION;
	float4 Neyev	: POSITION1;
	float4 normal	: POSITION2;
	float4 light	: POSITION3;
	float4 color	: POSITION4;
};

/*struct PS_IN
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	//UV���W
	float4 eyev		:POSITION1;
	float4 normal	:POSITION2;
	float4 light	:POSITION3;
};*/

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = (float2)uv;

	float3 binormal = cross(normal, tangent);

	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);//�@���x�N�g�������[�J�����W�ɕϊ�
	outData.normal = normal;

	tangent.w = 0;
	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);//�ڐ��x�N�g�������[�J�����W�ɕϊ�

	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);//�]�@���x�N�g�������[�J�����W�ɕϊ�

	float4 posw = mul(pos, matW);
	outData.Neyev.x = dot(outData.eyev, tangent);//�ڐ��Ԃ̎����x�N�g��
	outData.Neyev.y = dot(outData.eyev, binormal);
	outData.Neyev.z = dot(outData.eyev, normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPosition);
	light = normalize(light);

	outData.color = mul(light, normal);

	outData.light.x = dot(light, tangent);//�ڐ��Ԃ̌����x�N�g��
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);
	outData.light.w = 0;
	
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

	return diffuse + ambient;
	//return t1;
}
