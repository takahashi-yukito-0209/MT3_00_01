#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_00_";

struct Matrix4x4 {
    float m[4][4];
};

const int kRowHeight = 20;

// デバッグ用: 画面に行列を表示する
static void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    // 小さな値が負のゼロとして表示されるのを防ぐため、閾値以下は 0 として表示する
    auto Sanitize = [](float v) -> float { return (std::fabs(v) < 0.0005f) ? 0.0f : v; };
    for (int i = 0; i < 4; ++i) {
        Novice::ScreenPrintf(x, y + kRowHeight * (i + 1), "%6.03f %6.03f %6.03f %6.03f",
                              Sanitize(mat.m[0][i]), Sanitize(mat.m[1][i]), Sanitize(mat.m[2][i]),
                              Sanitize(mat.m[3][i]));
    }
}

struct Vector3 {
    float x, y, z;
};

// ベクトル演算ヘルパー
static float Dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vector3 Cross(const Vector3& a, const Vector3& b) {
    return Vector3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

static Vector3 Normalize(const Vector3& v) {
    float len = std::sqrt(Dot(v, v));
    if (len == 0.0f) {
        return Vector3{0.0f, 0.0f, 0.0f};
    }
    float inv = 1.0f / len;
    return Vector3{v.x * inv, v.y * inv, v.z * inv};
}

static Vector3 operator-(const Vector3& v) {
    return Vector3{-v.x, -v.y, -v.z};
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
    // 正規化
    Vector3 f = Normalize(from); // 回転元の単位ベクトル
    Vector3 t = Normalize(to); // 回転先の単位ベクトル

    Matrix4x4 m;

    // 行列を単位行列で初期化
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            if (row == column) {
                m.m[row][column] = 1.0f;
            } else {
                m.m[row][column] = 0.0f;
            }
        }
    }

    // f と t の内積を計算
    float dotValue = Dot(f, t);

    // ほぼ同じ向きの場合（角度が 0 度に近い）
    // 単位行列を返す
    if (dotValue > 0.999999f) {
        return m;
    }

    // 正反対の向きの場合（角度が 180 度に近い）
    if (dotValue < -0.999999f) {
        // f と直交する任意のベクトルを選ぶ
        // 成分の絶対値が最も小さい軸を基準にし、Cross 積が 0 にならないようにする
        float absX = std::fabs(f.x);
        float absY = std::fabs(f.y);
        float absZ = std::fabs(f.z);

        Vector3 arbitraryVector;

        // z 成分が最小なら Z 軸を選択
        if (absZ <= absX && absZ <= absY) {
            arbitraryVector = Vector3 { 0.0f, 0.0f, 1.0f };
        }
        // y 成分が最小なら Y 軸を選択
        else if (absY <= absX && absY <= absZ) {
            arbitraryVector = Vector3 { 0.0f, 1.0f, 0.0f };
        }
        // それ以外の場合は X 軸を選択
        else {
            arbitraryVector = Vector3 { 1.0f, 0.0f, 0.0f };
        }

        // 回転軸 = f と arbitraryVector の外積を正規化したもの
        Vector3 axis = Normalize(Cross(f, arbitraryVector));

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        // 180 度回転用の回転行列を直接構築
        // R = 2 * (軸ベクトルの外積行列) - I
        m.m[0][0] = 2.0f * x * x - 1.0f;
        m.m[0][1] = 2.0f * x * y;
        m.m[0][2] = 2.0f * x * z;

        m.m[1][0] = 2.0f * x * y;
        m.m[1][1] = 2.0f * y * y - 1.0f;
        m.m[1][2] = 2.0f * y * z;

        m.m[2][0] = 2.0f * x * z;
        m.m[2][1] = 2.0f * y * z;
        m.m[2][2] = 2.0f * z * z - 1.0f;

        return m;
    }

    // 0 度でも 180 度でもない
    
    // 回転軸に相当するベクトル v = f × t（外積）
    Vector3 crossVector = Cross(f, t);

    float vX = crossVector.x;
    float vY = crossVector.y;
    float vZ = crossVector.z;

    // スキュー対称行列 K を作成
    float K[3][3] = {
        { 0.0f, -vZ, vY },
        { vZ, 0.0f, -vX },
        { -vY, vX, 0.0f }
    };

    // K の 2 乗（K*K）を計算
    float KK[3][3];
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            KK[row][column] = 0.0f;
            for (int k = 0; k < 3; ++k) {
                KK[row][column] += K[row][k] * K[k][column];
            }
        }
    }

    // Rodrigues の回転公式用の係数
    float coefficient = 1.0f / (1.0f + dotValue);

    // 回転行列の 3x3 部分を計算
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            float identityValue;
            if (row == column) {
                identityValue = 1.0f;
            } else {
                identityValue = 0.0f;
            }

            float rotationValue = identityValue
                + K[row][column]
                + KK[row][column] * coefficient;

            m.m[row][column] = rotationValue;
        }
    }

    // 完成した回転行列を返す
    return m;
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

        Vector3 from0 = Normalize(Vector3 { 1.0f, 0.7f, 0.5f });
        Vector3 to0 = -from0;
        Vector3 from1 = Normalize(Vector3 { -0.6f, 0.9f, 0.2f });
        Vector3 to1 = Normalize(Vector3 { 0.4f, 0.7f, -0.5f });
        Matrix4x4 rotateMatrix0 = DirectionToDirection(Normalize(Vector3 { 1.0f, 0.0f, 0.0f }), Normalize(Vector3 { -1.0f, 0.0f, 0.0f }));
        Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
        Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
        MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
        MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

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
