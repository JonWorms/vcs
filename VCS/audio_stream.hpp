//
//  audio_input_stream.hpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#ifndef audio_stream_hpp
#define audio_stream_hpp

#include <stdio.h>
#include <functional>
#include <portaudio.h>
#include <stdexcept>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <sndfile.h>
#include <samplerate.h>
#include <thread>
#include <mutex>

namespace vcs {
    
    class port_audio {
    public:
        static int callback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
    };
    
    
    
    typedef std::function<void(const void*, unsigned long)> as_callback;
    
    class audio_stream {
        friend class port_audio;
        
        int buffer_size = 2048;
        double sample_rate;
        
        // Input
        PaStream *input_stream = nullptr;
        PaStreamParameters inputParameters;
        
        
        // Output
        PaStreamParameters outputParameters;
        
        
        std::map<std::string, std::vector<float>> sounds;
        
        std::mutex playback;
        std::queue<std::string> playback_queue;
        
        int playback_frame_index = 0;
        int playback_total_frames = 0;
        std::vector<float> *playback_data = nullptr;
        
        
    public:

        as_callback received_audio_callback;

        audio_stream(double audio_sample_rate);
        void open();

        // playback / sound files
        int load_sound_file(std::string path, std::string name);
        void play_sound_file(std::string name);
        
    };
    
    
}


#endif /* audio_input_stream_hpp */
