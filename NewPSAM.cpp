#include <iostream>
#include <vector>
#include <cmath>

class GranularPitch {
public:
    GranularPitch(int sampleRate, int grainSize, int overlap)
        : sampleRate(sampleRate), grainSize(grainSize), overlap(overlap) {}

    void process(const std::vector<float>& input, std::vector<float>& output, float pitchShift) {
        int numGrains = input.size() / (grainSize - overlap);
        output.resize(input.size());

        for (int i = 0; i < numGrains; ++i) {
            int start = i * (grainSize - overlap);
            if (start + grainSize > input.size()) break;

            std::vector<float> grain(input.begin() + start, input.begin() + start + grainSize);
            applyPitchShift(grain, pitchShift);
            for (int j = 0; j < grainSize; ++j) {
                output[start + j] += grain[j];
            }
        }
    }

private:
    int sampleRate;
    int grainSize;
    int overlap;

    void applyPitchShift(std::vector<float>& grain, float pitchShift) {
        int newSize = static_cast<int>(grain.size() * std::pow(2.0f, pitchShift));
        std::vector<float> shiftedGrain(newSize);

        for (int i = 0; i < newSize; ++i) {
            float srcIndex = i / std::pow(2.0f, pitchShift);
            if (srcIndex < grain.size()) {
                shiftedGrain[i] = grain[static_cast<int>(srcIndex)];
            }
        }
        grain = shiftedGrain;
    }
};

int main() {
    int sampleRate = 44100;
    int grainSize = 1024;
    int overlap = 512;
    GranularPitch granularPitch(sampleRate, grainSize, overlap);

    std::vector<float> input = {/* Your audio samples here */};
    std::vector<float> output;
    float pitchShift = 1.0f; // Example pitch shift

    granularPitch.process(input, output, pitchShift);

    // Output the processed audio
    for (const auto& sample : output) {
        std::cout << sample << std::endl;
    }

    return 0;
}
