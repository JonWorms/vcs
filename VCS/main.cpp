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
//-I/usr/local/include -I/usr/local/include/sphinxbase  -I/usr/local/include/pocketsphinx




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    /*
    vcs::voice_interface voice_listener("Computer", [](){
    
    
    }, [](std::string words){
    
        
        return false;
    
    });
    
    */
    
    
    std::string model_dir = "/Users/jonworms/Downloads/sphinx-lm/en-us/";
    vcs::voice_interface vc(model_dir + "en-us", model_dir + "en-us.lm.bin", model_dir + "cmudict-en-us.dict", "computer");
    vcs::audio_stream as(16000);
    
    
    as.open([&](const void *buffer, unsigned long length){
    
    
        vc.search_for_keyword((int16*) buffer, length);
        
    
    });
    
    char q;
    std::cin >> q;
    
    
    
    
    
    return 0;
}
