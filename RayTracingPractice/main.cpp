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


/// �I�u�W�F�N�g��
// ��
struct Sphere
{
	float rad;// ���a
	float3 pos;// ���W
	Sphere() { rad = 100; pos = float3(); };
	Sphere(float rad) { this->rad = rad; pos = float3(); };
	Sphere(float rad,float3 pos) { this->rad = rad; this->pos = pos; };
};
// ��
struct Plane
{
	float offset;// �����ǂ̂��炢�����Ă���(����ł���)�̂�
	float3 normal;// �@��(���K���ς�)
	Plane() { offset = 0; normal = float3(0,1,0); };
	Plane(float offset) { this->offset = offset; normal = float3(0,1,0); };
	Plane(float offset, float3 normal) { this->offset = offset; this->normal = normal; };
};


// ���Ƃ̓����蔻��A���������ꏊ�̐F��Ԃ��B
bool isHitSphere(float3& retCol, const float3& eye, const float3& ray, const float3& light,const Sphere& sph)
{
	// ���̂̒��S�_�܂ł̃x�N�g��
	float3 toCenter(sph.pos - eye);
	// ���̂ƃ��C�x�N�g���̐ڐ��̃x�N�g�����Z�o
	float3 RayVec(ray*Dot(toCenter,ray));

	float3 line = toCenter - RayVec;

	if (sph.rad >(toCenter- RayVec).Magnitude())
	{
		float cPos = sqrt(sph.rad * sph.rad - line.Magnitude() * line.Magnitude());

		float distance = RayVec.Magnitude() - cPos;

		float3 hitPos = eye + ray * distance;// �����x�N�g���Ǝ��_�Ƌ������瓖�����Ă�����W���擾

		// �@�����o���Đ��K������
		float3 normal = sph.pos- hitPos;
		normal = normal.Normalized();

		float shadow = Dot(light, normal);

		shadow = max(min(shadow,1),0);
		retCol = float3(1,1,1)*shadow;
		return true;
	}

	return false;
}

// ���Ƃ̓����蔻��A���������ꏊ�̐F��Ԃ��B
bool isHitPlane(float3& retCol, const float3& eye, const float3& ray, const float3& light, const Plane& plane)
{
	return false;
}

// ���C�g���[�V���O�{��
void RayTracing(const float3& eye, const Sphere& sph,const Plane& plane)
{
	for (int y = 0; y < ScreenY; ++y)
	{
		for (int x = 0; x < ScreenX; ++x)
		{
			// ���S���_�̃X�N���[�����W���擾
			float3 screenPos(x- ScreenX/2, -y+ ScreenY/2, 0);
			float3 light(1,-1,-1);
			light = light.Normalized();

			// �Ԃ�l�p
			float3 color;
			float3 ray = screenPos - eye;
			ray = ray.Normalized();

			// �����蔻��̎擾
			if (!isHitSphere(color, eye, ray, light, sph) && !isHitPlane(color, eye, ray, light, plane))
			{
				// ���C���������ĂȂ������Ƃ��͊D�F�ɂ���
				color = float3(0.25,0.25,0.25);
			}

			DrawPixel(x, y, GetColor(color.x * 0xff, color.y * 0xff, color.z * 0xff));
		}
	}
}

int main()
{
	SetGraphMode(ScreenX, ScreenY, 32);
	ChangeWindowMode(true);
	SetDrawScreen(DX_SCREEN_BACK);
	DxLib_Init();

	const float3 eye(0,0,300);// ���_
	const Sphere sph(100,float3(0,0,0));// ����
	const Plane plane;// ��

	// ���s���[�v
	while (!ProcessMessage()&&!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClearDrawScreen();
		RayTracing(eye, sph, plane);
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}