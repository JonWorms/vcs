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




voice_interface::voice_interface(std::string hmm, std::string lm, std::string dict, std::string mllr, std::string keyword) {
 

	cout << "Initalizing Pocketsphinx...";
	cmd_ln_t *config = nullptr;
	config = cmd_ln_init(NULL, ps_args(), TRUE,
						 "-hmm", hmm.c_str(),
						 //"-lm", lm.c_str(),
						 "-dict", dict.c_str(),
						 "-mllr", mllr.c_str(),
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
	ps_set_search(decoder, "keyword");
	
    //ps_set_kws(decoder, "command", {"lights on", "lights off", "alarm", "set"});
	
    ps_set_lm_file(decoder, "command", lm.c_str());
	

	/*
	for(auto sub: subjects) {
		vector<float> p_thresholds;
		for(unsigned long i = 0 ; i < predicates[sub].size() ; i++) {
			p_thresholds.push_back(1.0);
		}
		ps_set_kws(decoder, sub.c_str(), predicates[sub], p_thresholds);
	}*/
	
	
	
	
	/*
	vocabulary.push_back("turn");
	vocabulary.push_back("on");
	vocabulary.push_back("off");
	vocabulary.push_back("the");
	vocabulary.push_back("lights");
	vocabulary.push_back("music");
	vocabulary.push_back("reset");
	vocabulary.push_back("set");
	vocabulary.push_back("alarm");
	vocabulary.push_back("seven");
	
	
	
	
	for(int i = 0 ; i < vocabulary.size() ; i++) {
		vocab_thresholds.push_back(2.0);
	}
	
	ps_set_kws(decoder, "commands", vocabulary, vocab_thresholds);
	*/
	//ps_start_utt(decoder);
	
	cout << "done\n";
	
	
	
}



void voice_interface::start() {
    ps_start_utt(decoder);
}





bool voice_interface::traverse_utterance(const int16_t *data, size_t n_samples) {
	cout << "meow" << endl;
	ps_process_raw(decoder, data, n_samples, false, false);
	const char *hyp = ps_get_hyp(decoder, NULL);
	if(hyp != NULL) {
		string hyp_str(hyp);
		cout << "command: ";
		command += ("." + hyp_str);
		cout << command;
		cout << endl;
		ps_set_search(decoder, command.c_str());
		return traverse_utterance(data, n_samples);
	}
	cout << "break" << endl;
	return false;
}


bool voice_interface::traverse_utterance(const char *hyp, const int16_t *data, size_t n_samples) {
	string hyp_str;

	unsigned long i = 0;
	while(hyp[i] != 0 && hyp[i] != ' ') {
		hyp_str.push_back(hyp[i]);
		i++;
	}
	
	command = (command.empty()) ? hyp_str : (command + "." + hyp_str);
	cout << "command: ";
	cout << command;
	cout << endl;
	ps_set_search(decoder, command.c_str());

	return traverse_utterance(data, n_samples);
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
                ps_set_search(decoder, "command");
				cout << "Listening for command...\n";
			} else {
                ps_set_search(decoder, "keyword");
				cout << "Listening for keyword...\n";
            }
        }
        
        ps_start_utt(decoder);
        utt_started = false;

    }
    
    return 0;
}
