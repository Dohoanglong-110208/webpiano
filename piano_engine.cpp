#include <cmath>
#include <vector>
#include <emscripten/emscripten.h>

// Danh sách bài hát hiện tại (mặc định là Orange)
std::vector<int> currentSong = {60, 62, 64, 65, 67, 69, 71, 72}; // Ví dụ: C4, D4, E4, F4, G4, A4, B4, C5
int currentStep = 0;
int score = 0;

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    double getFrequencyFromMidi(int midiNote) {
        return 440.0 * std::pow(2.0, (midiNote - 69.0) / 12.0);
    }

    EMSCRIPTEN_KEEPALIVE
    int getTargetMidiNote() {
        return (currentStep < currentSong.size()) ? currentSong[currentStep] : -1;
    }

    // Kiểm tra nốt bấm: Trả về 1 (Đúng), 2 (Hoàn thành bài & Reset điểm), 0 (Sai)
    EMSCRIPTEN_KEEPALIVE
    int checkUserInput(int pressedMidi) {
        if (currentSong.empty() || currentStep >= currentSong.size()) return 0;

        if (pressedMidi == currentSong[currentStep]) {
            score += 10;
            currentStep++;
            if (currentStep >= currentSong.size()) {
                currentStep = 0;
                score = 0; // Reset điểm số về 0 khi hoàn thành trọn vẹn bài nhạc
                return 2;
            }
            return 1;
        } else {
            if (score >= 2) score -= 2;
            return 0;
        }
    }

    EMSCRIPTEN_KEEPALIVE
    int getScore() {
        return score;
    }

    // Nạp bài hát mới từ danh sách mã MIDI truyền từ JavaScript sang
    EMSCRIPTEN_KEEPALIVE
    void loadCustomSong(int* midiArray, int length) {
        if (length <= 0) return;
        currentSong.clear();
        for (int i = 0; i < length; i++) {
            currentSong.push_back(midiArray[i]);
        }
        currentStep = 0;
        score = 0;
    }
}

