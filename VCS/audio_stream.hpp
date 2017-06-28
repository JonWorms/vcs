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
#include <sndfile.h>

namespace vcs {
    
    typedef std::function<void(const void*, unsigned long)> as_callback;
    
    class audio_stream {
        double sample_rate;
        
        // PortAudio / Input
        PaStream *pa_stream = nullptr;
        PaStreamParameters inputParameters, outputParameters;
        as_callback received_audio_callback;
        
        // Sndfile / Output
        
        
        
    public:
        
        audio_stream(double audio_sample_rate);
        void open(as_callback callback);
        
        // playback / sound files
        int load_sound_file(std::string path, std::string name);
        void play_sound_file(std::string name);
        
    };
    
    
}


#endif /* audio_input_stream_hpp */
