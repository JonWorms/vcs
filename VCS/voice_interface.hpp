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
#include <iostream>

namespace vcs {
    
    
    class voice_interface {
        
        //std::string keyword;
        //std::function<void()> report_keyword_recognized;
        //std::function<bool(std::string)> report_words_recognized;
        

        ps_decoder_t *decoder = nullptr;

        bool in_speech = false;
        bool utt_started = false;
        
    public:
        
        //voice_interface(std::string keyword, std::function<void()> keyword_recognized, std::function<bool(std::string)> words_recognized);
        
        
        voice_interface(std::string hmm, std::string lm, std::string dict, std::string keyword);
        
        
        int search_for_keyword(const int16 *data, size_t n_samples);
        
        
    };
    
    
}



#endif /* voice_interface_hpp */
