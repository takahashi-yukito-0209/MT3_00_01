#include <Novice.h>
#include <cmath>
#include <cstring>

const char kWindowTitle[] = "LE2B_15_タカハシ_ユキト_MT4_01_05";

// 3次元ベクトルを表す構造体
struct Vector3 {
    float x, y, z;
};

// クォータニオン構造体
struct Quaternion {
    float x, y, z, w; 

    // クォータニオンの加算
    Quaternion operator+(const Quaternion& r) const { return {x + r.x, y + r.y, z + r.z, w + r.w}; }
    // スカラー倍
    Quaternion operator*(float s) const { return {x * s, y * s, z * s, w * s}; }
};

// クォータニオンの内積を計算する
static float Dot(const Quaternion& a, const Quaternion& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// クォータニオンを正規化して長さ1の単位クォータニオンを返す
static Quaternion Normalize(const Quaternion& q) {
    float length = std::sqrt(Dot(q, q));
    if (length == 0.0f) return {0, 0, 0, 1};
    return {q.x / length, q.y / length, q.z / length, q.w / length};
}

// 軸と角度から回転クォータニオンを作成する
static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    float axisX = axis.x;
    float axisY = axis.y;
    float axisZ = axis.z;
    float axisLength = std::sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);
    if (axisLength == 0.0f) return {0.0f, 0.0f, 0.0f, 1.0f};
    // 軸を正規化
    float normX = axisX / axisLength;
    float normY = axisY / axisLength;
    float normZ = axisZ / axisLength;
    // 回転角の半角の正弦と余弦を利用してクォータニオンを構成
    float sinHalfAngle = std::sin(angle * 0.5f);
    float cosHalfAngle = std::cos(angle * 0.5f);
    return Normalize({normX * sinHalfAngle, normY * sinHalfAngle, normZ * sinHalfAngle, cosHalfAngle});
}

// 球面線形補間（Slerp）
static Quaternion Slerp(const Quaternion& qStartIn, const Quaternion& qEndIn, float t) {
    // 入力クォータニオンは単位化して扱う
    Quaternion qStart = Normalize(qStartIn);
    Quaternion qEnd = Normalize(qEndIn);

    // 内積（ドット積）を計算して角度関係を得る
    float dotProduct = Dot(qStart, qEnd);
    // 内積が負の場合は qEnd の符号を反転して最短経路で補間する
    if (dotProduct < 0.0f) {
        qEnd = qEnd * -1.0f;
        dotProduct = -dotProduct;
    }

    const float DOT_THRESHOLD = 0.9995f;
    if (dotProduct > DOT_THRESHOLD) {
        // クォータニオンがほぼ同一の場合は数値安定性のため線形補間を行う
        Quaternion linearResult = qStart * (1.0f - t) + qEnd * t;
        return Normalize(linearResult);
    }

    // 2つのクォータニオン間の角度（theta0）と補間角度（theta）を求める
    float theta0 = std::acos(dotProduct);
    float theta = theta0 * t;
    float sinTheta = std::sin(theta);
    float sinTheta0 = std::sin(theta0);

    // 補間に用いるスケール係数を計算
    float scaleStart = std::cos(theta) - dotProduct * sinTheta / sinTheta0;
    float scaleEnd = sinTheta / sinTheta0;

    Quaternion result = qStart * scaleStart + qEnd * scaleEnd;
    return Normalize(result);
}

// 小数第2位で四捨五入して返す
static float RoundTo2Decimal(float value) {
    return std::round(value * 100.0f) / 100.0f;
}

// 補間結果を指定フォーマットで画面に表示する関数
static void PrintInterpolatedLine(int x, int y, const char* label, const Quaternion& q) {
    float roundedX = RoundTo2Decimal(q.x);
    float roundedY = RoundTo2Decimal(q.y);
    float roundedZ = RoundTo2Decimal(q.z);
    float roundedW = RoundTo2Decimal(q.w);
    Novice::ScreenPrintf(x, y, "%6.2f %6.2f %6.2f %6.2f : %s", roundedX, roundedY, roundedZ, roundedW, label);
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

        // クォータニオンの補間処理
        Quaternion rotation0 = MakeRotateAxisAngleQuaternion({0.70710678f, 0.70710678f, 0.0f}, 0.3f);
        Quaternion rotation1 = MakeRotateAxisAngleQuaternion({0.70710678f, 0.0f, 0.70710678f}, 3.14159265f);

        // 補間結果
        Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
        Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
        Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
        Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
        Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///
        
        // 補間結果を画面に表示
        PrintInterpolatedLine(10, 10, "interpolate0, Slerp(q0, q1, 0.0f)", interpolate0);
        PrintInterpolatedLine(10, 40, "interpolate1, Slerp(q0, q1, 0.3f)", interpolate1);
        PrintInterpolatedLine(10, 70, "interpolate2, Slerp(q0, q1, 0.5f)", interpolate2);
        PrintInterpolatedLine(10, 100, "interpolate3, Slerp(q0, q1, 0.7f)", interpolate3);
        PrintInterpolatedLine(10, 130, "interpolate4, Slerp(q0, q1, 1.0f)", interpolate4);

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
