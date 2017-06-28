//
//  voice_interface.hpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#ifndef voice_interface_hpp
#define voice_interface_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <pocketsphinx.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

namespace vcs {

	

	
    class voice_interface {
        
        // callbacks
        std::function<void()> report_keyword_recognized = [](){};
        std::function<void(const char*)> report_command_recognized = [](const char* str){};

        // cmuspinx live decoding
        ps_decoder_t *decoder = nullptr;
        bool in_speech = false;
        bool utt_started = false;
        
		
		
    public:
        
        
        voice_interface(const char *hmm, const char *lm, const char *dict, const char *mllr, const char *keyword);
        
        
        // input data from audio
        int search_for_keyword(const int16 *data, size_t n_samples);
        
        void on_keyword_recognized(std::function<void()> func);
        void on_command_recognized(std::function<void(const char*)> func);
        
        void start();
        
    };
    
    
}



#endif /* voice_interface_hpp */
