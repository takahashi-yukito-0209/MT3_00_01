#include <Novice.h>
#include <assert.h>
#include <cmath>
#include <cstdint>
#include <imgui.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_02_04";

struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
};

struct Matrix4x4 {
    float m[4][4];
};

struct Sphere {
    Vector3 center; //!< 中心点
    float radius; //!< 半径
};

struct Line {
    Vector3 origin, // 始点
        diff; // 終点への差分ベクトル
};

struct Ray {
    Vector3 origin, diff;
};

struct Segment {
    Vector3 origin, diff;
};

struct Plane {
    Vector3 normal; //!< 法線
    float distance; //!< 距離
};

struct Triangle {
    Vector3 vertices[3]; // 頂点a,b,c
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
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFFFFFFFF);
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
            Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFFFFFFFF);
        }
    }
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    const uint32_t kSubdivision = 16; // 分割数
    const float kLonEvery = 2.0f * float(M_PI) / kSubdivision; // 経度分割1つ分の角度
    const float kLatEvery = float(M_PI) / kSubdivision; // 緯度分割1つ分の角度

    // 緯度の方向に分割　-π/2 ~ π/2
    for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
        float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度
        // 経度の方向に分割　0 ~ 2π
        for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
            float lon = lonIndex * kLonEvery; // 現在の経度

            // world座標系でのa,b,cを求める
            Vector3 a, b, c;
            a = { cosf(lat) * cosf(lon), sinf(lat), cosf(lat) * sinf(lon) };
            b = { cosf(lat + kLatEvery) * cosf(lon), sinf(lat + kLatEvery), cosf(lat + kLatEvery) * sinf(lon) };
            c = { cosf(lat) * cosf(lon + kLonEvery), sinf(lat), cosf(lat) * sinf(lon + kLonEvery) };

            a.x = a.x * sphere.radius + sphere.center.x;
            a.y = a.y * sphere.radius + sphere.center.y;
            a.z = a.z * sphere.radius + sphere.center.z;

            b.x = b.x * sphere.radius + sphere.center.x;
            b.y = b.y * sphere.radius + sphere.center.y;
            b.z = b.z * sphere.radius + sphere.center.z;

            c.x = c.x * sphere.radius + sphere.center.x;
            c.y = c.y * sphere.radius + sphere.center.y;
            c.z = c.z * sphere.radius + sphere.center.z;

            // a,b,c,をScreen座標系まで変換
            a = Transform(a, viewProjectionMatrix);
            a = Transform(a, viewportMatrix);
            b = Transform(b, viewProjectionMatrix);
            b = Transform(b, viewportMatrix);
            c = Transform(c, viewProjectionMatrix);
            c = Transform(c, viewportMatrix);

            // ab,bcで線を引く
            Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
            Novice::DrawLine(int(b.x), int(b.y), int(c.x), int(c.y), color);
        }
    }
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

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    const float planeSize = 3.0f; // 平面の大きさ
    const float halfSize = planeSize / 2.0f;

    // 平面の中心点（法線ベクトル * -d）
    Vector3 center = Multiply(-plane.distance, plane.normal);

    // 平面上に張る2軸を計算（法線と直交するベクトル）
    Vector3 up = { 0.0f, 1.0f, 0.0f };
    if (fabsf(Dot(plane.normal, up)) > 0.99f) {
        up = { 1.0f, 0.0f, 0.0f }; // 法線がY軸に近い場合はX軸を使う
    }

    Vector3 axisX = Normalize(Cross(up, plane.normal));
    Vector3 axisZ = Normalize(Cross(plane.normal, axisX));

    // 4頂点を生成（ワールド座標系）
    Vector3 v0 = Add(Add(center, Multiply(-halfSize, axisX)), Multiply(-halfSize, axisZ));
    Vector3 v1 = Add(Add(center, Multiply(+halfSize, axisX)), Multiply(-halfSize, axisZ));
    Vector3 v2 = Add(Add(center, Multiply(+halfSize, axisX)), Multiply(+halfSize, axisZ));
    Vector3 v3 = Add(Add(center, Multiply(-halfSize, axisX)), Multiply(+halfSize, axisZ));

    // ワールド→スクリーン座標系に変換
    Vector3 sv0 = Transform(Transform(v0, viewProjectionMatrix), viewportMatrix);
    Vector3 sv1 = Transform(Transform(v1, viewProjectionMatrix), viewportMatrix);
    Vector3 sv2 = Transform(Transform(v2, viewProjectionMatrix), viewportMatrix);
    Vector3 sv3 = Transform(Transform(v3, viewProjectionMatrix), viewportMatrix);

    // 平面の枠を線で描画
    Novice::DrawLine((int)sv0.x, (int)sv0.y, (int)sv1.x, (int)sv1.y, color);
    Novice::DrawLine((int)sv1.x, (int)sv1.y, (int)sv2.x, (int)sv2.y, color);
    Novice::DrawLine((int)sv2.x, (int)sv2.y, (int)sv3.x, (int)sv3.y, color);
    Novice::DrawLine((int)sv3.x, (int)sv3.y, (int)sv0.x, (int)sv0.y, color);
}

// 三角形のワイヤーフレーム描画
void DrawTriangle(const Triangle& tri, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color)
{
    // 頂点をワールド→スクリーン変換
    Vector3 sv0 = Transform(Transform(tri.vertices[0], viewProjectionMatrix), viewportMatrix);
    Vector3 sv1 = Transform(Transform(tri.vertices[1], viewProjectionMatrix), viewportMatrix);
    Vector3 sv2 = Transform(Transform(tri.vertices[2], viewProjectionMatrix), viewportMatrix);

    // スクリーン座標で線を描く
    Novice::DrawLine(int(sv0.x), int(sv0.y), int(sv1.x), int(sv1.y), color);
    Novice::DrawLine(int(sv1.x), int(sv1.y), int(sv2.x), int(sv2.y), color);
    Novice::DrawLine(int(sv2.x), int(sv2.y), int(sv0.x), int(sv0.y), color);
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


bool IsCollision(const Triangle& triangle, const Segment& segment)
{
    // 三角形の頂点を取得
    Vector3 a = triangle.vertices[0];
    Vector3 b = triangle.vertices[1];
    Vector3 c = triangle.vertices[2];

    // 三角形の法線を求める
    Vector3 ab = Subtract(b, a);
    Vector3 ac = Subtract(c, a);
    Vector3 n = Normalize(Cross(ab, ac));

    // 平面の方程式 ax + by + cz + d = 0 の d を計算
    float d = -Dot(n, a);

    // 線分の方向ベクトル
    Vector3 dir = segment.diff;

    // 線分の方向と平面の法線の内積（垂直なら交差しない）
    float denom = Dot(n, dir);
    if (fabsf(denom) < 1e-6f)
        return false; // 平行または面上を通過

    // 線分が平面と交差する割合（t: 線分の0～1の範囲での交点位置）
    float t = -(Dot(n, segment.origin) + d) / denom;
    if (t < 0.0f || t > 1.0f)
        return false; // 線分の範囲外なら交差なし

    // 平面上の交点を求める
    Vector3 p = Add(segment.origin, Multiply(t, dir));

    // 重心座標による三角形内判定の準備
    Vector3 v0 = ab;
    Vector3 v1 = ac;
    Vector3 v2 = Subtract(p, a);

    // 重心座標（バリセントリック座標）を計算
    float d00 = Dot(v0, v0);
    float d01 = Dot(v0, v1);
    float d11 = Dot(v1, v1);
    float d20 = Dot(v2, v0);
    float d21 = Dot(v2, v1);
    float denom2 = d00 * d11 - d01 * d01;
    if (fabsf(denom2) < 1e-6f)
        return false; // 三角形が退化している

    // 各頂点に対応する重心座標 u, v, w を求める
    float v = (d11 * d20 - d01 * d21) / denom2;
    float w = (d00 * d21 - d01 * d20) / denom2;
    float u = 1.0f - v - w;

    // u, v, w がすべて 0以上なら三角形の内側にある
    return (u >= 0.0f && v >= 0.0f && w >= 0.0f);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // マウス前フレーム座標
    int prevMouseX = 0;
    int prevMouseY = 0;

    // 感度設定
    const float moveSpeed = 0.01f;
    const float rotateSpeed = 0.005f;
    const float zoomSpeed = 0.01f;

    // カメラの位置と角度
    Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
    Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
    Vector3 cameraPosition = { 0.0f, 0.0f, -10.0f };

    // 三角形の頂点
    Triangle triangle = {
        { { -1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f } }
    };

    // 線分（始点と終点）
    Vector3 segmentStart = { 0.0f, 0.5f, -1.0f };
    Vector3 segmentEnd = { 0.0f, 0.5f, 2.0f };
    Segment segment = { segmentStart, { segmentEnd.x - segmentStart.x, segmentEnd.y - segmentStart.y, segmentEnd.z - segmentStart.z } };

    unsigned int triangleColor = 0xFFFFFFFF;
    unsigned int segmentColor = 0xFFFFFFFF;

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

        if (IsCollision(triangle, segment)) {
            segmentColor = 0xFF0000FF;
        } else {
            segmentColor = 0xFFFFFFFF;
        }

        ImGui::Begin("Triangle & Segment Control");

        // カメラのUI
        ImGui::DragFloat3("Camera Position", &cameraTranslate.x, 0.1f);
        ImGui::DragFloat3("Camera Rotation", &cameraRotate.x, 0.01f);

        ImGui::Separator();

        // 三角形頂点ごとにx,y,z個別にDragFloatを展開
        ImGui::Text("Triangle Vertex 0");
        ImGui::DragFloat("V0.x", &triangle.vertices[0].x, 0.1f);
        ImGui::DragFloat("V0.y", &triangle.vertices[0].y, 0.1f);
        ImGui::DragFloat("V0.z", &triangle.vertices[0].z, 0.1f);

        ImGui::Separator();

        ImGui::Text("Triangle Vertex 1");
        ImGui::DragFloat("V1.x", &triangle.vertices[1].x, 0.1f);
        ImGui::DragFloat("V1.y", &triangle.vertices[1].y, 0.1f);
        ImGui::DragFloat("V1.z", &triangle.vertices[1].z, 0.1f);

        ImGui::Separator();

        ImGui::Text("Triangle Vertex 2");
        ImGui::DragFloat("V2.x", &triangle.vertices[2].x, 0.1f);
        ImGui::DragFloat("V2.y", &triangle.vertices[2].y, 0.1f);
        ImGui::DragFloat("V2.z", &triangle.vertices[2].z, 0.1f);

        ImGui::Separator();

        // 線分の始点
        ImGui::Text("Segment Start");
        ImGui::DragFloat("Start.x", &segment.origin.x, 0.1f);
        ImGui::DragFloat("Start.y", &segment.origin.y, 0.1f);
        ImGui::DragFloat("Start.z", &segment.origin.z, 0.1f);

        ImGui::Separator();

        // 線分の終点
        ImGui::Text("Segment End");
        ImGui::DragFloat("End.x", &segmentEnd.x, 0.1f);
        ImGui::DragFloat("End.y", &segmentEnd.y, 0.1f);
        ImGui::DragFloat("End.z", &segmentEnd.z, 0.1f);

        segment.diff = {
            segmentEnd.x - segment.origin.x,
            segmentEnd.y - segment.origin.y,
            segmentEnd.z - segment.origin.z
        };

        ImGui::End();


        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        DrawGrid(viewProjectionMatrix, viewportMatrix);
        DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, triangleColor);
        DrawSegment(segment, viewProjectionMatrix, viewportMatrix, segmentColor);

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
