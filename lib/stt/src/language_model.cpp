#include "language_model.hpp"


using namespace stt;
using namespace std;
using namespace filesystem;


vector<language_model> language_model::languages() {
	vector<language_model> models;
	string model_dir_path(CMUSPHINX_MODEL_DIR);
	for(auto& p: filesystem::directory_iterator(model_dir_path)) {
		models.emplace_back(language_model(p.path()));
	}
	return models;
}

filesystem::path find_file_named(string name, string in) {
	for(auto& p: directory_iterator(in)) {
		path pth = p.path();
		if(pth.filename() == name) {
			return pth;
		}
	}
	throw stt::exception("could not find model file: " + name);
}

filesystem::path find_file_with_type(string type, string in) {
	for(auto& p: directory_iterator(in)) {
		path pth = p.path();
		if(pth.extension() == type) {
			return pth;
		}
	}
	throw stt::exception("could not find model file: " + type);
}




language_model::language_model(string model_path) { 
	using namespace filesystem;
	path mp = model_path;
	if(!exists(mp)) {
		throw stt::exception("model path does not exist");
	}
	name = mp.filename();
	hmm_path  = find_file_named(name, model_path);
	lm_path   = find_file_with_type(".bin", model_path);
	dict_path = find_file_with_type(".dict", model_path);

	try {
		mllr_path = find_file_with_type(".mllr", model_path);
	} catch (stt::exception& e) {} // just ignore for optional mllr matrix
}

const char* language_model::hmm() {
	return hmm_path.c_str();
}

const char* language_model::lm() {
	return lm_path.c_str();
}

const char* language_model::dict() {
	return dict_path.c_str();
}

const char* language_model::mllr() {
	if(mllr_path.length() == 0) {
		return NULL;
	}
	return mllr_path.c_str();
}