#define NOMINMAX
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdint>
#include <imgui.h>
#include <math.h>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_02_07";

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
};

struct Vector4 {
    float x, y, z, w;
};

struct Matrix4x4 {
    float m[4][4];

    // ベクトルとの乗算
    Vector4 Multiply(const Vector4& v) const
    {
        Vector4 result;
        result.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
        result.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
        result.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
        result.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;
        return result;
    }
};

struct Segment {
    Vector3 origin, diff;
};

struct AABB {
    Vector3 min; //!< 最小点
    Vector3 max; //!< 最大点
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
    Vector3 result = {};

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
    Vector3 result = {};

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
    Vector3 result = {};

    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;

    return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 長さ（ノルム）
float Length(const Vector3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// 正規化
Vector3 Normalize(const Vector3& v)
{
    Vector3 result = {};

    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;

    return result;
}

// 4.逆行列
Matrix4x4 Inverse(const Matrix4x4& m)
{
    Matrix4x4 result = {};

    float det = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

    if (det == 0.0f) {
        return result;
    }

    float invDet = 1.0f / det;

    result.m[0][0] = invDet * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);

    result.m[0][1] = invDet * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);

    result.m[0][2] = invDet * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);

    result.m[0][3] = invDet * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

    result.m[1][0] = invDet * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);

    result.m[1][1] = invDet * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);

    result.m[1][2] = invDet * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);

    result.m[1][3] = invDet * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

    result.m[2][0] = invDet * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);

    result.m[2][1] = invDet * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);

    result.m[2][2] = invDet * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);

    result.m[2][3] = invDet * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

    result.m[3][0] = invDet * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);

    result.m[3][1] = invDet * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);

    result.m[3][2] = invDet * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);

    result.m[3][3] = invDet * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

    return result;
}

// 3.行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result = {};

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {

            result.m[row][column] = 0;

            for (int k = 0; k < 4; k++) {
                result.m[row][column] += m1.m[row][k] * m2.m[k][column];
            }
        }
    }

    return result;
}

// 1.平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
    Matrix4x4 result = {};

    result = {
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        translate.x,
        translate.y,
        translate.z,
        1.0f,
    };

    return result;
};

// 2.拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
    Matrix4x4 result = {};

    result = {
        scale.x,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        scale.y,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        scale.z,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
    };

    return result;
};

// 3.座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
    Vector3 result;
    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
    float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
    assert(w != 0.0f);

    result.x /= w;
    result.y /= w;
    result.z /= w;

    return result;
};

// 1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
    Matrix4x4 result = {};

    result = {
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        std::cos(radian),
        std::sin(radian),
        0.0f,
        0.0f,
        std::sin(-radian),
        std::cos(radian),
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };

    return result;
};

// 2.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
    Matrix4x4 result = {};

    result = {
        std::cos(radian),
        0.0f,
        std::sin(-radian),
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        std::sin(radian),
        0.0f,
        std::cos(radian),
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };

    return result;
};

// 3.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
    Matrix4x4 result = {};

    result = {
        std::cos(radian),
        std::sin(radian),
        0.0f,
        0.0f,
        std::sin(-radian),
        std::cos(radian),
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };

    return result;
};

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{

    // 回転軸をひとつにまとめる（合成）
    Matrix4x4 rotateXYZMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

    // 回転と拡縮、移動をすべて合成
    Matrix4x4 result = Multiply(Multiply(MakeScaleMatrix(scale), rotateXYZMatrix), MakeTranslateMatrix(translate));

    return result;
};

// 透視投影行列（透視変換行列）
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
    Matrix4x4 result = {};

    result.m[0][0] = 1.0f / aspectRatio * (1.0f / tanf(fovY / 2.0f));
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f / tanf(fovY / 2.0f);
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = farClip / (farClip - nearClip);
    result.m[2][3] = 1.0f;
    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
    result.m[3][3] = 0.0f;

    return result;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
    Matrix4x4 result = {};

    result.m[0][0] = width / 2.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;
    result.m[1][0] = 0.0f;
    result.m[1][1] = -(height / 2.0f);
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = maxDepth - minDepth;
    result.m[2][3] = 0.0f;
    result.m[3][0] = left + (width / 2.0f);
    result.m[3][1] = top + (height / 2.0f);
    result.m[3][2] = minDepth;
    result.m[3][3] = 1.0f;

    return result;
}

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
    Vector3 result = {};

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
    // v1 と v2 の内積
    float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    // v2 の長さの二乗（ノルムの二乗）
    float lenSq = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;

    // v2 がゼロベクトルなら射影できないのでゼロベクトルを返す
    if (lenSq == 0.0f)
        return { 0, 0, 0 };

    // 射影スカラー係数を計算
    float scale = dot / lenSq;

    // v2 にスカラー倍して射影ベクトルを返す
    return {
        v2.x * scale,
        v2.y * scale,
        v2.z * scale
    };
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
    // 線分のベクトル（終点 - 始点）
    Vector3 ab = segment.diff;

    // 始点から点へのベクトル
    Vector3 ap = {
        point.x - segment.origin.x,
        point.y - segment.origin.y,
        point.z - segment.origin.z
    };

    // ab の長さの二乗を計算（ゼロ割防止のため）
    float abDotAb = ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;

    // ab がゼロベクトル（線分が点）なら始点を返す
    if (abDotAb == 0.0f) {
        return segment.origin;
    }

    // ap と ab の内積
    float abDotAp = ab.x * ap.x + ab.y * ap.y + ab.z * ap.z;

    // 最近接点の位置を示すスカラー t を求める（0 <= t <= 1 にクランプ）
    float t = abDotAp / abDotAb;
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

    // 最近接点の座標を計算（始点 + ab * t）
    return {
        segment.origin.x + ab.x * t,
        segment.origin.y + ab.y * t,
        segment.origin.z + ab.z * t
    };
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMattrix)
{

    const float kGridHalfWidth = 2.0f; // Gridの半分の幅
    const uint32_t kSubdivision = 10; // 分割数
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ

    // 奥から手前への線を順々に引いていく
    for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++) {

        // 上の情報を使ってワールド座標系上の始点と終点を求める
        Vector3 start = { float(-(kSubdivision / 2.0f) + xIndex) * kGridEvery, 0.0f, float(-kGridHalfWidth) };
        Vector3 end = { float(-(kSubdivision / 2.0f) + xIndex) * kGridEvery, 0.0f, float(kGridHalfWidth) };

        // スクリーン座標系まで変換をかける
        start = Transform(Transform(start, viewProjectionMatrix), viewportMattrix);
        end = Transform(Transform(end, viewProjectionMatrix), viewportMattrix);

        // 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF),原点は黒ぐらいがよいが、なんでも良い
        if (xIndex == kSubdivision / 2.0f) {
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
        } else {
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x080808FF);
        }
    }

    // 左から右も同じように順々に引いていく
    for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++) {

        // 奥から手前が左右に変わるだけ
        Vector3 start = { float(-kGridHalfWidth), 0.0f, float(-(kSubdivision / 2.0f) + zIndex) * kGridEvery };
        Vector3 end = { float(kGridHalfWidth), 0.0f, float(-(kSubdivision / 2.0f) + zIndex) * kGridEvery };

        // スクリーン座標系まで変換をかける
        start = Transform(Transform(start, viewProjectionMatrix), viewportMattrix);
        end = Transform(Transform(end, viewProjectionMatrix), viewportMattrix);

        // 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF),原点は黒ぐらいがよいが、なんでも良い
        if (zIndex == kSubdivision / 2.0f) {
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
        } else {
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x080808FF);
        }
    }
}

// 線分の描画
void DrawSegment(const Segment& seg, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color)
{
    // 始点と終点をワールド→スクリーン変換
    Vector3 svStart = Transform(Transform(seg.origin, viewProjectionMatrix), viewportMatrix);
    Vector3 svEnd = Transform(Transform({ seg.origin.x + seg.diff.x, seg.origin.y + seg.diff.y, seg.origin.z + seg.diff.z }, viewProjectionMatrix), viewportMatrix);

    // スクリーン座標で線を描く
    Novice::DrawLine(int(svStart.x), int(svStart.y), int(svEnd.x), int(svEnd.y), color);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    // 1. 8つの頂点を定義
    Vector3 vertices[8] = {
        { aabb.min.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z },
        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.max.y, aabb.max.z },
        { aabb.min.x, aabb.max.y, aabb.max.z },
    };

    // 2. 各頂点をワールド→スクリーン座標に変換
    Vector2 screenVertices[8];
    for (int i = 0; i < 8; ++i) {
        Vector4 temp = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
        temp = viewProjectionMatrix.Multiply(temp); // ビュー射影変換
        temp = viewportMatrix.Multiply(temp); // ビューポート変換

        // 同次除算（z値は必要に応じて）
        if (temp.w != 0.0f) {
            temp.x /= temp.w;
            temp.y /= temp.w;
        }

        screenVertices[i] = { temp.x, temp.y };
    }

    // 3. AABB の 12本の辺を結んで描画
    int indices[12][2] = {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, // 前面
        { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 }, // 背面
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 } // 側面
    };

    for (int i = 0; i < 12; ++i) {
        const Vector2& p1 = screenVertices[indices[i][0]];
        const Vector2& p2 = screenVertices[indices[i][1]];
        Novice::DrawLine(static_cast<int>(p1.x), static_cast<int>(p1.y),
            static_cast<int>(p2.x), static_cast<int>(p2.y),
            color);
    }
}

bool IsCollision(const Segment& segment, const AABB& aabb)
{
    float tmin = 0.0f;
    float tmax = 1.0f;

    // X軸
    if (std::abs(segment.diff.x) < 1e-6f) {
        if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x) {
            return false;
        }
    } else {
        float invDir = 1.0f / segment.diff.x;
        float t1 = (aabb.min.x - segment.origin.x) * invDir;
        float t2 = (aabb.max.x - segment.origin.x) * invDir;
        if (t1 > t2) {
            std::swap(t1, t2);
        }
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) {
            return false;
        }
    }

    // Y軸
    if (std::abs(segment.diff.y) < 1e-6f) {
        if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y) {
            return false;
        }
    } else {
        float invDir = 1.0f / segment.diff.y;
        float t1 = (aabb.min.y - segment.origin.y) * invDir;
        float t2 = (aabb.max.y - segment.origin.y) * invDir;
        if (t1 > t2) {
            std::swap(t1, t2);
        }
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) {
            return false;
        }
    }

    // Z軸
    if (std::abs(segment.diff.z) < 1e-6f) {
        if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z) {
            return false;
        }
    } else {
        float invDir = 1.0f / segment.diff.z;
        float t1 = (aabb.min.z - segment.origin.z) * invDir;
        float t2 = (aabb.max.z - segment.origin.z) * invDir;
        if (t1 > t2) {
            std::swap(t1, t2);
        }
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) {
            return false;
        }
    }

    return true;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // カメラの位置と角度
    Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
    Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
    Vector3 cameraPosition = { 0.0f, 0.0f, -10.0f };

    // マウス前フレーム座標
    int prevMouseX = 0;
    int prevMouseY = 0;

    // 感度設定
    const float moveSpeed = 0.01f;
    const float rotateSpeed = 0.005f;
    const float zoomSpeed = 0.01f;

    Segment segment {
        .origin{ -0.7f, 0.3f, 0.0f },
        .diff{ 2.0f, -0.5f, 2.0f },
    };

    AABB aabb {
        .min { -0.5f, -0.5f, -0.5f },
        .max { 0.5f, 0.5f, 0.5f },
    };

    uint32_t segmentColor = 0xFFFFFFFF;
    uint32_t aabbColor = 0xFFFFFFFF;

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        // ImGuiで操作中ならカメラ操作をスキップ
        if (!ImGui::IsAnyItemActive() && !ImGui::IsAnyItemHovered()) {
            int mouseX, mouseY;
            Novice::GetMousePosition(&mouseX, &mouseY);

            int deltaX = mouseX - prevMouseX;
            int deltaY = mouseY - prevMouseY;

            int wheel = Novice::GetWheel();

            // 左クリック：平行移動
            if (Novice::IsPressMouse(0)) {
                cameraTranslate.x -= deltaX * moveSpeed;
                cameraTranslate.y += deltaY * moveSpeed;
            }

            // 右クリック：回転
            if (Novice::IsPressMouse(1)) {
                cameraRotate.y += deltaX * rotateSpeed;
                cameraRotate.x += deltaY * rotateSpeed;

                const float pitchLimit = 1.57f;

                if (cameraRotate.x > pitchLimit) {
                    cameraRotate.x = pitchLimit;
                }

                if (cameraRotate.x < -pitchLimit) {
                    cameraRotate.x = -pitchLimit;
                }
            }

            // ホイール：ズーム
            cameraTranslate.z += -wheel * zoomSpeed;

            // 座標保存
            prevMouseX = mouseX;
            prevMouseY = mouseY;
        }

        // カメラの位置初期化
        if (keys[DIK_R] && !preKeys[DIK_R]) {
            cameraTranslate = { 0.0f, 1.9f, -6.49f };
            cameraRotate = { 0.26f, 0.0f, 0.0f };
            cameraPosition = { 0.0f, 0.0f, -10.0f };
        }

        Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
        Matrix4x4 viewMatrix = Inverse(cameraMatrix);
        Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
        Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

        if (IsCollision(segment, aabb)) {
            // 交差している
            aabbColor = 0xFF0000FF;
        } else {
            // 交差していない
            aabbColor = 0xFFFFFFFF;
        }

        ImGui::Text("Edit");
        ImGui::DragFloat3("aabb.min", &aabb.min.x, 0.1f);
        ImGui::DragFloat3("aabb.max", &aabb.max.x, 0.1f);
        ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.1f);
        ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.1f);

        // min <= max 制限
        for (int i = 0; i < 3; ++i) {
            if ((&aabb.min.x)[i] > (&aabb.max.x)[i]) {
                (&aabb.max.x)[i] = (&aabb.min.x)[i];
            }
        }

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawSegment(segment, viewProjectionMatrix, viewportMatrix, segmentColor);
        DrawAABB(aabb, viewProjectionMatrix, viewportMatrix, aabbColor);
        

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}