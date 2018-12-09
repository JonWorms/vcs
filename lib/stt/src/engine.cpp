#include "engine.hpp"


using namespace stt;
using namespace std;

engine::engine(){}

void engine::set_model(language_model model) {
	std::cout << "setting language model" << std::endl;
	cmd_ln_free(); // free global configuration (if exists)
	if(model.mllr() == NULL) {
		configuration = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", model.hmm(), "-lm", model.lm(), "-dict", model.dict(), "-logfn", "/dev/null", NULL);				
	} else {
		configuration = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", model.hmm(), "-lm", model.lm(), "-dict", model.dict(), "-mllr", model.mllr(), "-logfn", "/dev/null", NULL);
	}
	
	if(configuration == NULL) {
		throw stt::exception("could not configure audio engine");
	}
	
}

ps_decoder_t* engine::create_decoder() {
	if(configuration == NULL) {
		throw stt::exception("engine not initialized");
	}
	return ps_init(configuration);
}