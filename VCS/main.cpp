//
//  main.cpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#include <iostream>
#include "audio_stream.hpp"
#include "voice_interface.hpp"




int main(int argc, const char * argv[]) {

    std::cout << "Starting VCS\n";
	
    std::string model_dir = "/usr/local/share/pocketsphinx/model/en-us/";
    std::string model = model_dir + "en-us";
    std::string lm    = model_dir + "en-us.lm.bin";
    std::string dict  = model_dir + "cmudict-en-us.dict";
    std::string mllr  = "/Users/jonworms/Desktop/mllr_matrix";
    
    
    vcs::voice_interface vc(model.c_str(), lm.c_str(), dict.c_str(), mllr.c_str(), "computer");
    
    
    vc.on_keyword_recognized([](){
        std::cout << "keyword recognized" << std::endl;
    });
    
    vc.on_command_recognized([](const char *str){
        std::cout << str << std::endl;
    });
    
    vc.start();
    vcs::audio_stream as(16000);

    
    
    
    
    as.open([&](const void *buffer, unsigned long length){
        vc.search_for_keyword((int16*) buffer, length);
    });
    
    char q;
    std::cin >> q;
    
    
    
    
    
    return 0;
}
