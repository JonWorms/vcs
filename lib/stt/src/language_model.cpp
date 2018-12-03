#include "language_model.hpp"


using namespace stt;
using namespace std;


void read_language_models(vector<string> *models) {
	
}

vector<string> language_model::languages() {
	vector<string> models;
	
	DIR *d = opendir(CMUSPHINX_MODEL_DIR);
	struct dirent *dp;
	
	while((dp = readdir(d)) != NULL) {
		if(*(dp->d_name) == '.') {
			continue;
		}
		models.emplace_back(dp->d_name);
		std::cout << models.back() << std::endl;
	}
	closedir(d);	
	
	return models;
}