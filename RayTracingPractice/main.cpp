#include <Dxlib.h>
#include <math.h>

const int ScreenX = 800;
const int ScreenY = 600;

/// 計算用
// float2つの塊（色情報とか）
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

// float3つの塊（色情報とか）
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

	// 大きさを返す
	float Magnitude()
	{
		return sqrt(x*x + y * y + z * z);
	}
	// 正規化したベクトルを返す
	float3 Normalized() 
	{
		float mag = Magnitude();
		return float3(x / mag, y / mag, z / mag);
	};
};

// float4つの塊
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


// float2同士の計算
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

// float3同士の計算
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
///内積を返す
float Dot(const float3& a, const float3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
};


/// オブジェクト類の当たり判定
// 球
struct Sphere
{
	float rad;// 半径
	float3 pos;// 座標
	Sphere() { rad = 100; pos = float3(); };
	Sphere(float rad) { this->rad = rad; pos = float3(); };
	Sphere(float rad,float3 pos) { this->rad = rad; this->pos = pos; };
};


// 球との当たり判定、当たった場所の色を返す。
bool isHitSphere(float3& retCol, const float3& eye, const float3& ray, const Sphere& sph)
{
	float3 toCenter = sph.pos - eye;// 視点から球体中心までのベクトルを出す

	float CdotR = Dot(toCenter, ray);// 視点から球体中心までのベクトルと視線ベクトルの内積を取得

	float3 vecR = ray * (toCenter * ray);

	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	float3 line = toCenter - vecR;

	// 中心からの垂線が半径より小さければ当たっている
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
			// 中心原点のスクリーン座標を取得
			float3 screenPos(x, y, 0);

			// 返り値用
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
	const float3 eye(0,0,300);// 視点
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