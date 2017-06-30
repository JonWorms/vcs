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



int port_audio::callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    audio_stream *as = (audio_stream*) userData;
    
    
    // TODO: Find a better solution than ignoring the mic during playback
    if(as->playback_data == nullptr) {
        as->received_audio_callback(inputBuffer, framesPerBuffer);
        if(as->playback_queue.size() > 0) {
            as->playback.lock();
            as->playback_data = &as->sounds[as->playback_queue.front()];
            as->playback_total_frames = as->playback_data->size() * 0.5;
            as->playback_frame_index = 0;
            as->playback_queue.pop();
            as->playback.unlock();
        }
    }
    
    
    
    
    if(as->playback_data != nullptr) {
        
        float* output = (float*) outputBuffer;
        float* data = &as->playback_data->at(as->playback_frame_index*2);
        
        if(as->playback_total_frames - as->playback_frame_index > framesPerBuffer) {
            for(int i = 0 ; i < framesPerBuffer ; i++) {
                *output++ = *data++;
                *output++ = *data++;
            }
            as->playback_frame_index += framesPerBuffer;
        } else {
            int frames_remaining = as->playback_total_frames - as->playback_frame_index;
            for(int i = 0 ; i < frames_remaining ; i++ ) {
                *output++ = *data++;
                *output++ = *data++;
            }
            as->playback_data = nullptr;
        }
    }
    
    
    
    return 0;
};





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
    inputParameters.suggestedLatency = info->defaultLowOutputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    info = Pa_GetDeviceInfo(outputParameters.device);
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = info->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    
    if(Pa_IsFormatSupported(&inputParameters, &outputParameters, sample_rate) != 0) {
        throw std::runtime_error("unsupported input format");
    }
    
	cout << "done\n";
    
}


void audio_stream::open() {
    PaError err;
    err = Pa_OpenStream(&input_stream, &inputParameters, &outputParameters, sample_rate, buffer_size, paNoFlag, &port_audio::callback, this);
    if(err == paNoError) { err = Pa_StartStream(input_stream); }
    if(err != paNoError) { throw runtime_error(Pa_GetErrorText(err)); }
}



int audio_stream::load_sound_file(std::string path, std::string name) {
    
    // open and read the file
    SF_INFO file_info;
    file_info.format = 0; // set to zero for read
    
    // open file
    SNDFILE *file =  sf_open(path.c_str(), SFM_READ, &file_info);
    
    // IF NO SAMPLE RATE CONVERSION NEEDED
    if(((double)file_info.samplerate) == sample_rate && file_info.channels == 2) {
        // create a buffer
        float buffer[buffer_size];
        unsigned long r_len = 1; // number of bytes read
        vector<float> *data = &sounds[name]; // audio data
        while(r_len > 0) { // while file has data
            r_len = sf_read_float(file, buffer, buffer_size);
            // copy from buffer to audio data vector
            for(int i = 0 ; i < r_len ; i++) {
                data->push_back(buffer[i]);
            }
        }
        sf_close(file);
        return 0;
    }
    
    
    // we need to resample the file
    
    // libsamplerate does its work with floats
    
    // read file into vector of floats
    float buffer[buffer_size];
    std::vector<float> data;
    unsigned long r_len = 1;
    while(r_len > 0) {
        r_len = sf_read_float(file, buffer, buffer_size);
        //r_len = sf_readf_float(file, buffer, buffer_size/file_info.channels);
        for(int i = 0 ; i < r_len ; i++) {
            data.push_back(buffer[i]);
        }
    }
    sf_close(file);

    
    // vector for converted data
    vector<float> conv(data.size());
    
    
    // conversion info:
    SRC_DATA c_data;
    c_data.data_in = &data[0];
    c_data.data_out = &conv[0];
    c_data.input_frames = file_info.frames;
    c_data.output_frames = file_info.frames;
    c_data.src_ratio = sample_rate / (double)file_info.samplerate;
    //c_data.end_of_input = 1;
    // convert sample rate
    int err = src_simple(&c_data, SRC_SINC_BEST_QUALITY, 2);
    if(err != 0) {
        cout << src_strerror(err) << endl;
        return err;
    }
    //TODO: Upsampling
    
    // create a vector inthe output map for the audio data
    conv.erase(conv.begin() + c_data.output_frames_gen * 2, conv.end());
    sounds[name] = conv;
    
    
    return 0;
}

void audio_stream::play_sound_file(std::string name) {
    
    playback.lock();
    playback_queue.push(name);
    playback.unlock();
    
}
