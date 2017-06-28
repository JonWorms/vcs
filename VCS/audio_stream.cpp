//
//  audio_input_stream.cpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#include "audio_stream.hpp"


using namespace vcs;
using namespace std;


int pa_cb(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
    
    as_callback cb = *((as_callback*) userData);
    cb(inputBuffer, framesPerBuffer);
    return 0;
}


audio_stream::audio_stream(double sr) : sample_rate(sr) {
    
    const PaDeviceInfo *info;
	
	cout << "Initalizing port audio...";
    // initialize port audio
    PaError err = Pa_Initialize();
    if(err != paNoError) {
        const char *err_text = Pa_GetErrorText(err);
        throw std::runtime_error(err_text);
    }
    
    // setup i/o parameters:
    inputParameters.device = Pa_GetDefaultInputDevice();
    info = Pa_GetDeviceInfo(inputParameters.device);
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = info->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
	
	
    outputParameters.device = Pa_GetDefaultOutputDevice();
    info = Pa_GetDeviceInfo(outputParameters.device);
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = info->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    
    // are they supported?
    if(Pa_IsFormatSupported(&inputParameters, &outputParameters, sample_rate) != 0) {
        throw std::runtime_error("unsupported format");
    }
    
	
	cout << "done\n";
    
}


void audio_stream::open(function<void(const void *buffer, unsigned long length)> callback) {
    
    
    received_audio_callback = callback;
    PaError err = Pa_OpenStream(&pa_stream, &inputParameters, &outputParameters, sample_rate, 2048, paNoFlag, &pa_cb, &received_audio_callback);
    
    
    if(err == paNoError) {
        err = Pa_StartStream(pa_stream);
    }
    
    if(err != paNoError) {
        const char *err_text = Pa_GetErrorText(err);
        throw std::runtime_error(err_text);
    }

}



int audio_stream::load_sound_file(std::string path, std::string name) {
    
    return 0;
}

void audio_stream::play_sound_file(std::string name) {
    
}
