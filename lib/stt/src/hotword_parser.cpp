#include "hotword_parser.hpp"


using namespace stt;
using namespace std;


/*
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
*/

/*
cmd_ln_t* init(const char *hmm, const char *dict, const char *mllr) {
    cmd_ln_t *config = nullptr;
    if(mllr == NULL) {
        config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-dict", dict, "-logfn", "/dev/null", NULL);
    } else {
        config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", hmm, "-dict", dict, "-mllr", mllr, "-logfn", "/dev/null", NULL);
    }
    return config;
}*/





hotword_parser::hotword_parser(vector<string> hotwords, engine *eng) {
	
	if((decoder = eng->create_decoder()) == NULL) {
		throw stt::exception("could not initialize cmusphinx decoder");
	}

	
}

hotword_parser::~hotword_parser() {
	if(decoder != NULL) {
		ps_free(decoder);
	}
}

void hotword_parser::add_hotword(string hotword) {
	
}