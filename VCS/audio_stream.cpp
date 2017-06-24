//
//  audio_input_stream.cpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#include "audio_input_stream.hpp"

using namespace vcs;
using namespace std;


static int portaudio_callback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
    
    
    
    return 0;
}


audio_stream::audio_stream(uint16_t sr, uint16_t b) : sample_rate(sr), bandwidth(b) {
    
    // initialize port audio
    PaError err = Pa_Initialize();
    if(err != paNoError) {
        const char *err_text = Pa_GetErrorText(err);
        throw std::runtime_error(err_text);
    }
    
    
    
}





