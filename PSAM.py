import numpy as np
import soundfile as sf
from scipy.signal import resample

def granular_pitch_shift(audio_file, shift_factor, grain_size=2048, overlap=512):
    # Load audio file
    audio, sample_rate = sf.read(audio_file)
    
    # Calculate the number of grains
    num_grains = int(len(audio) / (grain_size - overlap))
    output = np.zeros(int(len(audio) * shift_factor))
    
    for i in range(num_grains):
        start = i * (grain_size - overlap)
        end = start + grain_size
        
        if end > len(audio):
            break
        
        grain = audio[start:end]
        grain_resampled = resample(grain, int(grain_size * shift_factor))
        
        output_start = int(start * shift_factor)
        output[output_start:output_start + len(grain_resampled)] += grain_resampled
    
    return output, sample_rate

# Example usage
output_audio, sr = granular_pitch_shift('input_audio.wav', 1.5)
sf.write('output_audio.wav', output_audio, sr)
