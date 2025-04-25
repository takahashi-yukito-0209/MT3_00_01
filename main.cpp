#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_00_04";

struct Matrix4x4 {
    float m[4][4];
};

struct Vector3 {
    float x, y, z;
};

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

//1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
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

//2.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
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

//3.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
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

// 4x4行列の数値表示
static const int kRowHeight = 25;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
    Novice::ScreenPrintf(x, y, "%s", label);

    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            Novice::ScreenPrintf(x + column * kColumnWidth, y + 30 + row * kRowHeight, "%6.02f", matrix.m[row][column]);
        }
    }
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

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

        Vector3 rotate { 0.4f, 1.43f, -0.8f };
        Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
        Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
        Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
        Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

        MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");
        MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

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
