#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT4_01_01";

// 3次元ベクトル構造体
typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

// 4x4行列構造体
typedef struct Matrix4x4 {
    float m[4][4];
} Matrix4x4;

float Length(const Vector3 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3 v)
{
    float length = Length(v);
    if (length == 0.0f) {
        // 長さがゼロの場合はゼロベクトルを返す
        return { 0.0f, 0.0f, 0.0f };
    }

    float invLength = 1.0f / length;
    return {
        v.x * invLength,
        v.y * invLength,
        v.z * invLength
    };
}

// 回転軸と回転角から4x4回転行列を作成する関数
Matrix4x4 MakeRotateAxisAngle(const Vector3 axis, float angle)
{
    // 回転軸を正規化
    Vector3 k = Normalize(axis);
    float kx = k.x;
    float ky = k.y;
    float kz = k.z;

    // 三角関数を計算
    float c = cosf(angle);
    float s = sinf(angle);
    float one_minus_c = 1.0f - c;

    Matrix4x4 result;

    // 4x4回転行列の要素計算 (ロドリゲスの回転公式)

    // R00, R11, R22 (対角成分)
    result.m[0][0] = kx * kx * one_minus_c + c;
    result.m[1][1] = ky * ky * one_minus_c + c;
    result.m[2][2] = kz * kz * one_minus_c + c;

    // R01, R10
    result.m[0][1] = kx * ky * one_minus_c - kz * s; // +k_z s が -k_z s になる
    result.m[1][0] = kx * ky * one_minus_c + kz * s; // -k_z s が +k_z s になる

    // R02, R20
    result.m[0][2] = kx * kz * one_minus_c + ky * s; // -k_y s が +k_y s になる
    result.m[2][0] = kx * kz * one_minus_c - ky * s; // +k_y s が -k_y s になる

    // R12, R21
    result.m[1][2] = ky * kz * one_minus_c - kx * s; // +k_x s が -k_x s になる
    result.m[2][1] = ky * kz * one_minus_c + kx * s; // -k_x s が +k_x s になる

    // 4列目（平行移動成分）
    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;

    // 4行目
    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;

    // 変換の同次座標成分
    result.m[3][3] = 1.0f;

    return result;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // 軸ベクトルを定義
    Vector3 axis = { 1.0f, 1.0f, 1.0f };

    // 角度を定義 (ラジアン)
    float angle = 0.44f;

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

        // 関数を呼び出し、回転行列を作成
        Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        Novice::ScreenPrintf(0, 0, "Rotate Matrix:");

        // 回転行列の表示
        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                Novice::ScreenPrintf(0 + y * 60, 20 + x * 20, "% .3f", rotateMatrix.m[y][x]);
            }
        }

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
