#include"basic.h"
#include<string>
#include<iostream>

using namespace std;

vector<int> split_1(const std::string &text, char sep) {
	std::vector<int> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(atoi(text.substr(start, end - start).c_str()));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(atoi(text.substr(start).c_str()));
	}
	return tokens;
}

vector<string> get_group(vector<string>& basic_ele, vector<string> &route, int current_epoch, int seq_len){
	if (current_epoch == 0){
		route = basic_ele;
	}
	vector<string> new_route;
	for (int i = 0; i < route.size(); i++){
		for (int j = 0; j < basic_ele.size(); j++){
			new_route.push_back(route[i] +" "+ basic_ele[j]);
		}
	}
	current_epoch += 1;
	if (current_epoch == seq_len){
		route = new_route;
		for (int i = 0; i < new_route.size(); i++){
			cout << new_route[i] << endl;
		}
		return new_route;
	}
	return get_group(basic_ele, new_route, current_epoch, seq_len);

}

int str_route(string route_str, vector<int> &route){
	if (route_str.length() == 0){
		return 0;
	}
	

	for (int i = 0; i < route_str.length(); i++){
		route.push_back(route_str[i]);
	}
	return 1;

}

vector<float> forward_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<int> watch_value,
	vector<float> hidden_probs, int current_epoch){
	
	//当前状态下通过一定概率出来的 观测值
	int in_emmision_watch_value = watch_value[current_epoch];
	
	if (current_epoch == 0){
		for (int i = 0; i < init_prob.size(); i++){
			cout << init_prob[i] << " * " << emmision[i][in_emmision_watch_value] << endl;
			hidden_probs.push_back(init_prob[i] * emmision[i][in_emmision_watch_value]);//初始概率（选状态）*被选中的状态下的发射概率（发射为当前观测值的概率）
		}
		init_prob = hidden_probs;
	}
	else{
		vector<float> new_hidden_probs;
		float pre_current_prob = 0.0;
		for (int i = 0; i < hidden_probs.size(); i++){
			cout << hidden_probs[i] << " * " << transition[current_epoch - 1][i] << endl;
			pre_current_prob += hidden_probs[i] * transition[current_epoch - 1][i];
			
		}
		cout << emmision[current_epoch - 1][in_emmision_watch_value] << endl;
		new_hidden_probs.push_back(pre_current_prob*emmision[current_epoch-1][in_emmision_watch_value]);
		hidden_probs = new_hidden_probs;
	}
	current_epoch += 1;
	if (current_epoch == watch_value.size()){
		return hidden_probs;
	}
	return forward_proba(init_prob, transition, emmision, watch_value, hidden_probs, current_epoch);
	
	
	
}