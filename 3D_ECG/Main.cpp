#include "DxLib.h"
#include <vector>
#include <cmath>

// 心電図風の波形関数
float ECGWave(float t) {
    t = fmod(t * 5.0f, 1.0f);

    if (t < 0.1f) return sinf(t * DX_PI * 10.0f) * 10.0f; // P波
    if (t < 0.2f) return 0.0f;
    if (t < 0.25f) return -50.0f; // Q波
    if (t < 0.3f) return 100.0f;  // R波
    if (t < 0.35f) return -25.0f; // S波
    if (t < 0.6f) return sinf((t - 0.35f) * DX_PI * 4.0f) * 20.0f; // T波
    return 0.0f;
}

// 頂点リスト生成
std::vector<VECTOR> GenerateECGLine(int resolution, float length, float offsetTime) {
    std::vector<VECTOR> line;

    for (int i = 0; i < resolution; ++i) {
        float t = static_cast<float>(i) / resolution;
        float x = t * length - length / 2.0f;
        float y = ECGWave(t + offsetTime);
        float z = 0.0f;
        line.push_back(VGet(x, y, z));
    }
    return line;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    float timeOffset = 0.0f;

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        ClearDrawScreen();

        // カメラ設定
        SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -200), VGet(0, 0, 0));

        // 時間のオフセットで波が流れるように
        timeOffset += 0.005f;

        // 波形データ生成
        auto ecgLine = GenerateECGLine(1000, 200.0f, timeOffset);

        // 線を描画
        for (size_t i = 1; i < ecgLine.size(); ++i) {
            DrawLine3D(ecgLine[i - 1], ecgLine[i], GetColor(0, 255, 0));
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}