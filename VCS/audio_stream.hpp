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
#include <iostream>

namespace vcs {
    
    class audio_stream {
        double sample_rate;
        
        PaStream *pa_stream = nullptr;
        PaStreamParameters inputParameters, outputParameters;
        std::function<void(const void*, unsigned long)> received_audio_callback;
        
        
        
        
        
    public:
        
        /* audio playback loop
        std::queue<std::vector<int16_t>> delay_playback;
        int sample_size;
        */
        audio_stream(double audio_sample_rate);
        void open(std::function<void(const void *buffer, unsigned long length)> callback);
        
        
        void portaudio_callback(const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags);
        
    };
    
    
}


#endif /* audio_input_stream_hpp */
