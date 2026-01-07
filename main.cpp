#include <Novice.h>
#include <cmath>
#include <cstdio>

const char kWindowTitle[] = "LE2C_17_タカハシ_ユキト_MT3_00_";

// Quaternion 型と関連関数の実装
struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
    Quaternion r;
    float vx1 = lhs.x, vy1 = lhs.y, vz1 = lhs.z, w1 = lhs.w;
    float vx2 = rhs.x, vy2 = rhs.y, vz2 = rhs.z, w2 = rhs.w;
    r.x = w1 * vx2 + w2 * vx1 + (vy1 * vz2 - vz1 * vy2);
    r.y = w1 * vy2 + w2 * vy1 + (vz1 * vx2 - vx1 * vz2);
    r.z = w1 * vz2 + w2 * vz1 + (vx1 * vy2 - vy1 * vx2);
    r.w = w1 * w2 - (vx1 * vx2 + vy1 * vy2 + vz1 * vz2);
    return r;
}

Quaternion IdentityQuaternion() {
    return {0.0f, 0.0f, 0.0f, 1.0f};
}

Quaternion Conjugate(const Quaternion& q) {
    return {-q.x, -q.y, -q.z, q.w};
}

float Norm(const Quaternion& q) {
    return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion Normalize(const Quaternion& q) {
    float n = Norm(q);
    if (n == 0.0f) return q;
    return {q.x / n, q.y / n, q.z / n, q.w / n};
}

Quaternion Inverse(const Quaternion& q) {
    float len2 = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (len2 == 0.0f) return q;
    Quaternion c = Conjugate(q);
    return {c.x / len2, c.y / len2, c.z / len2, c.w / len2};
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

        // クォータニオンのサンプル計算
        Quaternion q1 = {2.0f, 3.0f, 4.0f, 1.0f};
        Quaternion q2 = {1.0f, 3.0f, 5.0f, 2.0f};
        Quaternion qIdentity = IdentityQuaternion();
        Quaternion qConj = Conjugate(q1);
        Quaternion qInv = Inverse(q1);
        Quaternion qNorm = Normalize(q1);
        Quaternion qMul1 = Multiply(q1, q2);
        Quaternion qMul2 = Multiply(q2, q1);
        float norm = Norm(q1);
        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // 右側: 結果表示
        Novice::ScreenPrintf(0, 0, "%.2f  %.2f  %.2f  %.2f : identity", qIdentity.x, qIdentity.y, qIdentity.z, qIdentity.w);
        Novice::ScreenPrintf(0, 0 + 20, "%.2f  %.2f  %.2f  %.2f : Conjugate", qConj.x, qConj.y, qConj.z, qConj.w);
        Novice::ScreenPrintf(0, 0 + 40, "%.2f  %.2f  %.2f  %.2f : Inverse", qInv.x, qInv.y, qInv.z, qInv.w);
        Novice::ScreenPrintf(0, 0 + 60, "%.2f  %.2f  %.2f  %.2f : Normalize", qNorm.x, qNorm.y, qNorm.z, qNorm.w);
        Novice::ScreenPrintf(0, 0 + 80, "%.2f  %.2f  %.2f  %.2f : Multiply(q1, q2)", qMul1.x, qMul1.y, qMul1.z, qMul1.w);
        Novice::ScreenPrintf(0, 0 + 100, "%.2f  %.2f  %.2f  %.2f : Multiply(q2, q1)", qMul2.x, qMul2.y, qMul2.z, qMul2.w);
        Novice::ScreenPrintf(0, 0 + 120, "%.2f : Norm", norm);
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
