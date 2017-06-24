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
    
    
    vcs::audio_stream *as = (vcs::audio_stream*) userData;
    as->portaudio_callback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
    
    
    /*
    
    int16_t *in = (int16_t*) inputBuffer;
    int16_t *out = (int16_t*) outputBuffer;
    
    
    vector<int16_t> input;
    const unsigned long numBytes = (framesPerBuffer * 1);
    int i = 0;
    while(i < numBytes) {
        input.push_back(in[i]);
        i++;
    }
    
    as->delay_playback.push(input);
    
    if(as->delay_playback.size() > 100) {
        vector<int16_t> output = as->delay_playback.front();
        as->delay_playback.pop();
        
        for(int i = 0 ; i < output.size() ; i++) {
            *out++ = output[i];
        }
        
    }
    */
    
    return 0;
}


audio_stream::audio_stream(double sr) : sample_rate(sr) {
    
    const PaDeviceInfo *info;
    
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
    
}


void audio_stream::open(function<void(const void *buffer, unsigned long length)> callback) {
    
    
    received_audio_callback = callback;
    PaError err = Pa_OpenStream(&pa_stream, &inputParameters, &outputParameters, sample_rate, 128, paNoFlag, &pa_cb, this);
    
    
    if(err == paNoError) {
        err = Pa_StartStream(pa_stream);
    }
    
    if(err != paNoError) {
        const char *err_text = Pa_GetErrorText(err);
        throw std::runtime_error(err_text);
    }

}


void audio_stream::portaudio_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) {
    
    received_audio_callback(inputBuffer, framesPerBuffer);
    
}


