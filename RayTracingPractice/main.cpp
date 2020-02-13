#include <Dxlib.h>
#include <math.h>

const int ScreenX = 800;
const int ScreenY = 600;

/// �v�Z�p
// float2�̉�i�F���Ƃ��j
struct float2
{
	float x;
	float y;
	float2() {
		x = 0; y = 0;
	};
	float2(float x, float y) {
		this->x = x;
		this->y = y;
	};
	float2(float xy) {
		this->x = xy;
		this->y = xy;
	};
};

// float3�̉�i�F���Ƃ��j
struct float3
{
	float x;
	float y;
	float z;
	float3() {
		x = 0; y = 0; z = 0;
	};
	float3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};
	float3(float xyz) {
		this->x = xyz;
		this->y = xyz;
		this->z = xyz;
	};

	// �傫����Ԃ�
	float Magnitude()
	{
		return sqrt(x*x + y * y + z * z);
	}
	// ���K�������x�N�g����Ԃ�
	float3 Normalized() 
	{
		float mag = Magnitude();
		return float3(x / mag, y / mag, z / mag);
	};
};

// float4�̉�
struct float4
{
	float x;
	float y;
	float z;
	float w;
	float4() {
		x = 0; y = 0; z = 0; w = 0;
	};
	float4(float x, float y, float z, float w) {
		this->x = x; 
		this->y = y;
		this->z = z;
		this->w = w;
	};
	float4(float3 xyz, float w) {
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
		this->w = w;
	};
	float4(float xyzw) {
		this->x = xyzw;
		this->y = xyzw;
		this->z = xyzw;
		this->w = xyzw;
	};
};


// float2���m�̌v�Z
float2 operator+(const float2& a, const float2& b)
{
	float2 ret = float2(a.x + b.x, a.y + b.y);
	return ret;
};
float2 operator-(const float2& a, const float2& b)
{
	float2 ret = float2(a.x - b.x, a.y - b.y);
	return ret;
};

// float3���m�̌v�Z
float3 operator+(const float3& a, const float3& b)
{
	float3 ret = float3(a.x + b.x, a.y + b.y, a.z + b.z);
	return ret;
};
float3 operator-(const float3& a, const float3& b)
{
	float3 ret = float3(a.x - b.x, a.y - b.y, a.z - b.z);
	return ret;
};
float3 operator*(const float3& a, const float3& b)
{
	float3 ret = float3(a.x * b.x, a.y * b.y, a.z * b.z);
	return ret;
};
///���ς�Ԃ�
float Dot(const float3& a, const float3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
};


/// �I�u�W�F�N�g�ނ̓����蔻��
// ��
struct Sphere
{
	float rad;// ���a
	float3 pos;// ���W
	Sphere() { rad = 100; pos = float3(); };
	Sphere(float rad) { this->rad = rad; pos = float3(); };
	Sphere(float rad,float3 pos) { this->rad = rad; this->pos = pos; };
};


// ���Ƃ̓����蔻��A���������ꏊ�̐F��Ԃ��B
bool isHitSphere(float3& retCol, const float3& eye, const float3& ray, const Sphere& sph)
{
	float3 toCenter = sph.pos - eye;// ���_���狅�̒��S�܂ł̃x�N�g�����o��

	float CdotR = Dot(toCenter, ray);// ���_���狅�̒��S�܂ł̃x�N�g���Ǝ����x�N�g���̓��ς��擾

	float3 vecR = ray * (toCenter * ray);

	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	float3 line = toCenter - vecR;

	// ���S����̐��������a��菬������Γ������Ă���
	if (line.Magnitude() <= sph.rad)
	{
		retCol = { 1,1,1 };
		return true;
	}
	return false;
}

void RayTracing(const float3& eye, const Sphere& sph)
{
	for (int y = 0; y < ScreenY; ++y)
	{
		for (int x = 0; x < ScreenX; ++x)
		{
			// ���S���_�̃X�N���[�����W���擾
			float3 screenPos(x, y, 0);

			// �Ԃ�l�p
			float3 color;
			float3 ray = screenPos - eye;
			ray = ray.Normalized();

			isHitSphere(color, eye, ray, sph);

			DrawPixel(x, y, GetColor(color.x, color.y, color.z));
		}
	}
}

int main()
{
	SetGraphMode(ScreenX, ScreenY,64);
	ChangeWindowMode(true);
	DxLib_Init();
	const float3 eye(0,0,300);// ���_
	const Sphere sph(100,float3(0,0,0));
	while (!ProcessMessage()&&!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClearDrawScreen();
		RayTracing(eye, sph);
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}