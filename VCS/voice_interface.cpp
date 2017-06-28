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



// this is here because there does not appear to be any way to set a kws without using a file
// so this creates a file in /tmp from the argument keyphrases and thresholds, then loads that
// file with the original ps_set_kws
int ps_set_kws(ps_decoder_t *ps, const char *name, const vector<string> &keyphrases, const vector<float> &thresholds) {
    
    if(keyphrases.size() != thresholds.size()) { return 1; }
    ofstream kws_file("/tmp/vcs_kws");
    if(!kws_file.is_open()) { return 2; }
    
    for(int i = 0 ; i < keyphrases.size() ; i++) {
        kws_file << keyphrases[i] << " /" << thresholds[i] << "/\n";
    }
    
    kws_file.close();
    return ps_set_kws(ps, name, "/tmp/vcs_kws");
}

int ps_set_kws(ps_decoder_t *ps, const char *name, const vector<string> &keyphrases) {
	
	ofstream kws_file("/tmp/vcs_kws");
	if(!kws_file.is_open()) { return 2; }
	
	for(int i = 0 ; i < keyphrases.size() ; i++) {
		kws_file << keyphrases[i] << " /1.0/\n";
	}
	
	kws_file.close();
	return ps_set_kws(ps, name, "/tmp/vcs_kws");
}



cmd_ln_t* init(const char *hmm, const char *dict, const char *mllr) {
    cmd_ln_t *config = nullptr;
    if(mllr == NULL) {
        config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-dict", dict, "-logfn", "/dev/null", NULL);
    } else {
        config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-dict", dict, "-mllr", mllr, "-logfn", "/dev/null", NULL);
    }
    return config;
}


voice_interface::voice_interface(const char *hmm, const char *lm, const char *dict, const char *mllr, const char *keyword) {

    
	cout << "Initalizing Pocketsphinx...";
    
	cmd_ln_t *config = nullptr;
    config = init(hmm, dict, mllr);
	if(config == nullptr) {
		throw runtime_error("failed to create pockesphinx configuration");
	}
	
	
	decoder = ps_init(config);
	if(decoder == nullptr) {
		throw runtime_error("failed to initialize decoder");
	}

    if(ps_set_lm_file(decoder, "command", lm) == -1) {
        throw runtime_error("failed to load lm file");
    }

	ps_set_keyphrase(decoder, "keyword", "computer");
	ps_set_search(decoder, "keyword");
	
	cout << "done\n";
	
    
    
}



void voice_interface::start() {
    ps_start_utt(decoder);
}


void voice_interface::on_keyword_recognized(std::function<void ()> func) {
    report_keyword_recognized = func;
}

void voice_interface::on_command_recognized(std::function<void(const char*)> func) {
    report_command_recognized = func;
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
            if(strcmp("keyword", ps_get_search(decoder)) == 0) {
                report_keyword_recognized();
                ps_set_search(decoder, "command");
			} else {
                report_command_recognized(hyp);
                ps_set_search(decoder, "keyword");
            }
        }
        ps_start_utt(decoder);
        utt_started = false;

    }
    
    return 0;
}
