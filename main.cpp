#define NOMINMAX
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdint>
#include <imgui.h>
#include <math.h>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_03_01";

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

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
    return Add(Multiply(1.0f - t, v1), Multiply(t, v2));
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

// 球の描画
void DrawSphere(int x, int y, float radius, uint32_t color)
{
    Novice::DrawEllipse(x - int(radius), y - int(radius), int(radius) * 2, int(radius) * 2, 0.0f, color, kFillModeSolid);
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

    Vector3 translates[3] = {
        { 0.2f, 1.0f, 0.0f },
        { 0.4f, 0.0f, 0.0f },
        { 0.3f, 0.0f, 0.0f },
    };

    Vector3 rotates[3] = {
        { 0.0f, 0.0f, -6.8f },
        { 0.0f, 0.0f, -1.4f },
        { 0.0f, 0.0f, 0.0f },
    };

    Vector3 scales[3] = {
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
    };

    uint32_t kColorRed = 0xFF0000FF;
    uint32_t kColorGreen = 0x00FF00FF;
    uint32_t kColorBlue = 0x0000FFFF;

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

        // ImGuiで各ボーンのパラメータ編集
        ImGui::Begin("Joint Controls");

        for (int i = 0; i < 3; i++) {
            ImGui::PushID(i);
            if (i == 0) {
                ImGui::Text("Shoulder");
            } else if (i == 1) {
                ImGui::Text("Elbow");
            } else if (i == 2) {
                ImGui::Text("Hand");
            }
            ImGui::DragFloat3("Translate", &translates[i].x, 0.01f);
            ImGui::DragFloat3("Rotate", &rotates[i].x, 0.01f);
            ImGui::DragFloat3("Scale", &scales[i].x, 0.01f, 0.01f, 10.0f);
            ImGui::Separator();
            ImGui::PopID();
        }

        ImGui::End();

        // 肩のワールド行列（スケール・回転・移動）
        Matrix4x4 shoulderMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]);

        // 肘のワールド行列 = 肩の行列 × 肘のローカル行列
        Matrix4x4 elbowLocalMatrix = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
        Matrix4x4 elbowMatrix = Multiply(elbowLocalMatrix, shoulderMatrix);

        // 手のワールド行列 = 肘の行列 × 手のローカル行列
        Matrix4x4 handLocalMatrix = MakeAffineMatrix(scales[2], rotates[2], translates[2]);
        Matrix4x4 handMatrix = Multiply(handLocalMatrix, elbowMatrix);

        // ボーンの位置（ワールド座標）＝ 各行列の移動成分
        Vector3 shoulderPos = { shoulderMatrix.m[3][0], shoulderMatrix.m[3][1], shoulderMatrix.m[3][2] };
        Vector3 elbowPos = { elbowMatrix.m[3][0], elbowMatrix.m[3][1], elbowMatrix.m[3][2] };
        Vector3 handPos = { handMatrix.m[3][0], handMatrix.m[3][1], handMatrix.m[3][2] };

        // 画面座標へ変換
        shoulderPos = Transform(Transform(shoulderPos, viewProjectionMatrix), viewportMatrix);
        elbowPos = Transform(Transform(elbowPos, viewProjectionMatrix), viewportMatrix);
        handPos = Transform(Transform(handPos, viewProjectionMatrix), viewportMatrix);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // グリッド描画
        DrawGrid(viewProjectionMatrix, viewportMatrix);

        // 球描画
        DrawSphere(int(shoulderPos.x), int(shoulderPos.y), scales[0].x * 5.0f, kColorRed);
        DrawSphere(int(elbowPos.x), int(elbowPos.y), scales[1].x * 5.0f, kColorGreen);
        DrawSphere(int(handPos.x), int(handPos.y), scales[2].x * 5.0f, kColorBlue);

        // ボーンの線描画
        Novice::DrawLine(int(shoulderPos.x), int(shoulderPos.y), int(elbowPos.x), int(elbowPos.y), 0xFFFFFFFF); // 白線
        Novice::DrawLine(int(elbowPos.x), int(elbowPos.y), int(handPos.x), int(handPos.y), 0xFFFFFFFF); // 白線

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
