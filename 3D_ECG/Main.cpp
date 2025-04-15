#include "DxLib.h"
#include <vector>
#include <cmath>

// �S�d�}���̔g�`�֐�
float ECGWave(float t) {
    t = fmod(t * 5.0f, 1.0f);

    if (t < 0.1f) return sinf(t * DX_PI * 10.0f) * 10.0f; // P�g
    if (t < 0.2f) return 0.0f;
    if (t < 0.25f) return -50.0f; // Q�g
    if (t < 0.3f) return 100.0f;  // R�g
    if (t < 0.35f) return -25.0f; // S�g
    if (t < 0.6f) return sinf((t - 0.35f) * DX_PI * 4.0f) * 20.0f; // T�g
    return 0.0f;
}

// ���_���X�g����
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

        // �J�����ݒ�
        SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -200), VGet(0, 0, 0));

        // ���Ԃ̃I�t�Z�b�g�Ŕg�������悤��
        timeOffset += 0.005f;

        // �g�`�f�[�^����
        auto ecgLine = GenerateECGLine(1000, 200.0f, timeOffset);

        // ����`��
        for (size_t i = 1; i < ecgLine.size(); ++i) {
            DrawLine3D(ecgLine[i - 1], ecgLine[i], GetColor(0, 255, 0));
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}