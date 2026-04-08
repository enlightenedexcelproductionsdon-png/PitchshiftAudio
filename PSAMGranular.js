class GranularPitchShifter {
    constructor(semitones, grainSize, overlap) {
        this.semitones = semitones;
        this.grainSize = grainSize;
        this.overlap = overlap;
    }

    shiftPitch(audioBuffer) {
        const sampleRate = audioBuffer.sampleRate;
        const newBuffer = new AudioBuffer({
            length: audioBuffer.length,
            sampleRate: sampleRate
        });

        const pitchFactor = Math.pow(2, this.semitones / 12);
        const grainCount = Math.floor(audioBuffer.length / this.grainSize);
        
        for (let i = 0; i < grainCount; i++) {
            const start = i * this.grainSize;
            const end = start + this.grainSize;
            const grain = audioBuffer.getChannelData(0).slice(start, end);
            const shiftedGrain = this.applyPitchShift(grain, pitchFactor);
            const outputStart = start - (this.overlap * this.grainSize);
            newBuffer.copyToChannel(shiftedGrain, 0, outputStart);
        }

        return newBuffer;
    }

    applyPitchShift(grain, pitchFactor) {
        const shiftedGrain = new Float32Array(grain.length);
        for (let i = 0; i < grain.length; i++) {
            const index = Math.floor(i * pitchFactor);
            if (index < grain.length) {
                shiftedGrain[i] = grain[index];
            }
        }
        return shiftedGrain;
    }
}
