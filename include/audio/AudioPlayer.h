#pragma once

class AudioPlayer {
    public: 
        void Play();
        void Pause();
        void Stop();
        void Rewind();
        void Skip();
        bool isPlaying() const;

    private: 
        bool isPlaying = false;
};