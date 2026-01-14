#include <Novice.h>
#include <array>
#include <cmath>

// ウィンドウタイトル
const char kWindowTitle[] = "LE2B_15_タカハシ_ユキト_MT4_01_04";

// 3次元ベクトル構造体
struct Vector3 {
    float x, y, z;
};

// クォータニオン
struct Quaternion {
    float x, y, z, w;
};

// 4×4行列
struct Matrix4x4 {
    float m[4][4];
};

//----------------------------
// ベクトル演算関連
//----------------------------

// 内積（ドット積）
static inline float Dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 外積（クロス積）
static inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
    return { a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x };
}

// ベクトル加算
static inline Vector3 Add(const Vector3& a, const Vector3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }

// ベクトル減算
static inline Vector3 Sub(const Vector3& a, const Vector3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

// スカラー倍
static inline Vector3 Scale(const Vector3& v, float s) { return { v.x * s, v.y * s, v.z * s }; }

// ベクトルの長さ
static inline float Length(const Vector3& v) { return std::sqrt(Dot(v, v)); }

// 正規化（長さを1にする）
static inline Vector3 Normalize(const Vector3& v)
{
    float len = Length(v);
    if (len == 0.0f)
        return { 0.0f, 0.0f, 0.0f };
    float inv = 1.0f / len;
    return { v.x * inv, v.y * inv, v.z * inv };
}

//-------------------------------------
// 軸と角度からクォータニオンを生成
//-------------------------------------
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
    // 回転軸を正規化
    Vector3 n = Normalize(axis);

    // 角度の半分を使う（クォータニオンの定義）
    float half = angle * 0.5f;
    float s = std::sin(half);
    float c = std::cos(half);

    // クォータニオン (x, y, z, w) を作成
    return { n.x * s, n.y * s, n.z * s, c };
}

//-------------------------------------
// クォータニオンによるベクトル回転
//-------------------------------------
Vector3 RotateVector(const Vector3& v, const Quaternion& q)
{
    // クォータニオンのベクトル部
    Vector3 qv { q.x, q.y, q.z };

    // t = 2 * (qv × v)
    Vector3 t = Scale(Cross(qv, v), 2.0f);

    // v + w * t
    Vector3 v1 = Add(v, Scale(t, q.w));

    // 最終結果: v + w*t + (qv × t)
    Vector3 v2 = Add(v1, Cross(qv, t));
    return v2;
}

//-------------------------------------
// クォータニオン → 回転行列へ変換
//-------------------------------------
Matrix4x4 MakeRotateMatrix(const Quaternion& q)
{
    float x = q.x, y = q.y, z = q.z, w = q.w;

    // 各成分の積を事前計算
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    Matrix4x4 r {};

    // 回転行列の各要素を設定
    r.m[0][0] = 1.0f - 2.0f * (yy + zz);
    r.m[0][1] = 2.0f * (xy - wz);
    r.m[0][2] = 2.0f * (xz + wy);
    r.m[0][3] = 0.0f;

    r.m[1][0] = 2.0f * (xy + wz);
    r.m[1][1] = 1.0f - 2.0f * (xx + zz);
    r.m[1][2] = 2.0f * (yz - wx);
    r.m[1][3] = 0.0f;

    r.m[2][0] = 2.0f * (xz - wy);
    r.m[2][1] = 2.0f * (yz + wx);
    r.m[2][2] = 1.0f - 2.0f * (xx + yy);
    r.m[2][3] = 0.0f;

    // 同次座標用の行
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[3][3] = 1.0f;

    return r;
}

//-------------------------------------
// ベクトルを4×4行列で変換
//-------------------------------------
static Vector3 Transform(const Vector3& v, const Matrix4x4& m)
{
    return {
        // x成分
        v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + 1.0f * m.m[0][3],
        // y成分
        v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + 1.0f * m.m[1][3],
        // z成分
        v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + 1.0f * m.m[2][3],
    };
}

//-------------------------------------
// Windowsアプリのエントリーポイント
//-------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力状態を保持する配列
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // ウィンドウが閉じるまでループ
    while (Novice::ProcessMessage() == 0) {

        // フレーム開始
        Novice::BeginFrame();

        // キー入力の更新
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓ 更新処理
        ///

        // 回転用クォータニオンを作成（軸と角度を指定）
        Quaternion rotation = MakeRotateAxisAngleQuaternion({ 1.0f, 0.4f, -0.2f }, 0.45f);

        // 回転させる点
        Vector3 point { 2.1f, -0.9f, 1.3f };

        // クォータニオンから回転行列を生成
        Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);

        // クォータニオンで直接回転
        Vector3 rotateByQuaternion = RotateVector(point, rotation);

        // 行列で回転
        Vector3 rotateByMatrix = Transform(point, rotateMatrix);

        ///
        /// ↑ 更新処理ここまで
        ///

        ///
        /// ↓ 描画処理
        ///

        // クォータニオンの値を表示
        Novice::ScreenPrintf(0, 0, "% .2f % .2f % .2f % .2f    : rotation",
            rotation.x, rotation.y, rotation.z, rotation.w);

        // 回転行列の表示
        Novice::ScreenPrintf(0, 20, "rotateMatrix");
        Novice::ScreenPrintf(0, 40, "% .3f % .3f % .3f % .3f", rotateMatrix.m[0][0], rotateMatrix.m[1][0], rotateMatrix.m[2][0], rotateMatrix.m[3][0]);
        Novice::ScreenPrintf(0, 60, "% .3f % .3f % .3f % .3f", rotateMatrix.m[0][1], rotateMatrix.m[1][1], rotateMatrix.m[2][1], rotateMatrix.m[3][1]);
        Novice::ScreenPrintf(0, 80, "% .3f % .3f % .3f % .3f", rotateMatrix.m[0][2], rotateMatrix.m[1][2], rotateMatrix.m[2][2], rotateMatrix.m[3][2]);
        Novice::ScreenPrintf(0, 100, "% .3f % .3f % .3f % .3f", rotateMatrix.m[0][3], rotateMatrix.m[1][3], rotateMatrix.m[2][3], rotateMatrix.m[3][3]);

        // 回転結果の比較表示
        Novice::ScreenPrintf(0, 160, "% .2f % .2f % .2f    : rotateByQuaternion",
            rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
        Novice::ScreenPrintf(0, 180, "% .2f % .2f % .2f    : rotateByMatrix",
            rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);

        ///
        /// ↑ 描画処理ここまで
        ///

        // フレーム終了
        Novice::EndFrame();

        // ESCキーが押されたら終了
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリ終了
    Novice::Finalize();
    return 0;
}
