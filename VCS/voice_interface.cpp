//
//  voice_interface.cpp
//  VCS
//
//  Created by Jon Worms on 6/23/17.
//  Copyright © 2017 Off The Farm. All rights reserved.
//

#include "voice_interface.hpp"

using namespace std;
using namespace vcs;

const char *keyword_search = "keyword";


voice_interface::voice_interface(string hmm, string lm, string dict, string keyword) {
    
    cmd_ln_t *config = nullptr;
    config = cmd_ln_init(NULL, ps_args(), TRUE,
                                      "-hmm", hmm.c_str(),
                                      //"-lm", lm.c_str(),
                                      "-dict", dict.c_str(),
                                      "-logfn", "/dev/null",
                                      NULL);
    
    
    
    
    if(config == nullptr) {
        throw runtime_error("failed to create pockesphinx configuration");
    }
    
    
    
    decoder = ps_init(config);
    if(decoder == nullptr) {
        throw runtime_error("failed to initialize decoder");
    }
    
    ps_set_keyphrase(decoder, "keyword", "computer");
    ps_set_lm_file(decoder, "lm", lm.c_str());
    ps_set_search(decoder, "keyword");
    ps_start_utt(decoder);
    
}


int voice_interface::search_for_keyword(const int16 *data, size_t n_samples) {

    ps_process_raw(decoder, data, n_samples, false, false);
    in_speech = ps_get_in_speech(decoder);
    
    if(in_speech && !utt_started) {
        utt_started = true;
    }
    
    if(!in_speech && utt_started) {
        ps_end_utt(decoder);
        const char *hyp = ps_get_hyp(decoder, NULL);
        if (hyp != NULL) {
            cout << hyp << endl;
            
            if(strcmp("keyword", ps_get_search(decoder)) == 0) {
                ps_set_search(decoder, "lm");
            } else {
                ps_set_search(decoder, "keyword");
            }
        }
        
        ps_start_utt(decoder);
        utt_started = false;

    }
    
    return 0;
}
