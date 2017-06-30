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


voice_interface::voice_interface(const char *hmm, const char *lm, const char *dict, const char *mllr, const char *keyword, size_t bs) : buffer(bs, 0) {

    
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
    
    std::thread([&](){
        while(true) {
            search_for_keyword();
        }
    }).detach();
    
}


void voice_interface::on_keyword_recognized(std::function<void ()> func) {
    report_keyword_recognized = func;
}

void voice_interface::on_command_recognized(std::function<void(const char*)> func) {
    report_command_recognized = func;
}


// const int16 *data, size_t n_samples
void voice_interface::search_for_keyword() {

    size_t n_samples = (buffer_size < 2048) ? buffer_size : 2048;
    
    if(n_samples == 0) {
        return;
    }
    
    if(read_index + n_samples < buffer.size()) {
        ps_process_raw(decoder, &buffer[read_index], n_samples, false, false);
        read_index += n_samples;
        
        if(read_index == buffer.size()) {
            read_index = 0;
        }
        
    } else {
        size_t a_samples = buffer.size() - read_index;
        ps_process_raw(decoder, &buffer[read_index], a_samples, false, false);
        a_samples = n_samples - a_samples;
        ps_process_raw(decoder, &buffer[0], a_samples, false, false);
        read_index = a_samples;
    }
    
    buffer_size -= n_samples;
    
    
    
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
    
}



void voice_interface::enqueue_audio_data(const int16_t *data, size_t n_samples) {
    unsigned long w1 = write_index + n_samples;
    unsigned long w2 = (w1 > buffer.size()) ? w1 - buffer.size() : 0;
    
    if(w2 > 0) {
        w1 = buffer.size();
    }
    
    int i = 0;
    while(write_index < w1) {
        buffer[write_index] = data[i];
        i++;
        write_index++;
    }
    if(write_index == buffer.size()) {
        write_index = 0;
    }
    
    while(write_index < w2) {
        buffer[write_index] = data[i];
        i++;
        write_index++;
    }
    
    buffer_size += n_samples;
    if(buffer_size > buffer.size()) {
        read_index = write_index;
        buffer_size = buffer.size();
        cout << "ring buffer overflow" << endl;
    }
    
}




