//
//  audio_input_stream.hpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#ifndef audio_input_stream_hpp
#define audio_input_stream_hpp

#include <stdio.h>
#include <functional>
#include <portaudio.h>
#include <stdexcept>

namespace vcs {
    
    class audio_stream {
        
        uint64_t sample_rate;
        uint16_t bandwidth;
        
        
    public:
        audio_stream(uint16_t audio_sample_rate, uint16_t frequency_bandwidth);
        
    };
    
    
}


#endif /* audio_input_stream_hpp */
