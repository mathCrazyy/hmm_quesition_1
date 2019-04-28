#include"basic.h"

using namespace std;

float force_proba(vector<float> init_prob, vector<vector<float>> transition_matrix, vector<vector<float>> emmision_matrix,
	vector<string> watch_value,
	vector<float> hidden_probs, int current_epoch){
	
	vector<string> route;
	vector<string> all_route = get_group(watch_value, route, 0, 2);
	float O_proba = 0.0;
	for (int route_index = 0; route_index < all_route.size(); route_index++){
		//all_route为不同的隐状态序列
		//对于一条隐状态序列，求出现观测序列的概率。
		vector<int> route_int;
		route_int = split_1(all_route[route_index], ' ');
		float transition_value = 1.0;
		int pre_current = -1;
		for (int i = 0; i < route_int.size() - 1; i++){//最后一个状态不再进行状态上的转移。
			cout << "从" << route_int[i] - 1 << "往" << route_int[i + 1] - 1 << endl;
			transition_value *= transition_matrix[route_int[i] - 1][route_int[i + 1] - 1];
		}

		float pi_transition_value = init_prob[route_int[0] - 1] * transition_value;//转移矩阵的概率.
		//观测序列的概率
		string watch_value_str = "0 1 0";
		vector<int> watch_value_vi = split_1(watch_value_str, ' ');
		float emmision_value = 1.0;
		for (int i = 0; i < watch_value_vi.size(); i++){
			//emmision_value *= emmision_matrix[i][watch_value_vi[i]];//emmison_matrix中的i应该为第几个盒子，即当前的隐状态
			emmision_value *= emmision_matrix[route_int[i] - 1][watch_value_vi[i]];//emmison_matrix中的i应该为第几个盒子，即当前的隐状态
		}
		float one_route_proba = pi_transition_value*emmision_value;
		cout << one_route_proba << endl;
		O_proba += one_route_proba;

	}
	return O_proba;

}


vector<float> forward_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<int> watch_value,
	vector<float> hidden_probs, int current_epoch){

	//当前状态下通过一定概率出来的 观测值
	int in_emmision_watch_value = watch_value[current_epoch];

	if (current_epoch == 0){
		for (int i = 0; i < init_prob.size(); i++){
			cout << init_prob[i] << " * " << emmision[i][in_emmision_watch_value] << endl;
			//初始概率（各个隐状态）*被选中的状态下的发射概率（发射为当前观测值的概率）
			hidden_probs.push_back(init_prob[i] * emmision[i][in_emmision_watch_value]);
		}
		init_prob = hidden_probs;
	}
	else{
		vector<float> new_hidden_probs;

		for (int tran = 0; tran < transition.size(); tran++){//每个状态都可能在目前的基础上，达到某个观测状态的目标
			float pre_current_prob = 0.0;
			for (int i = 0; i < hidden_probs.size(); i++){
				cout << hidden_probs[i] << " * " << transition[i][tran] << endl;//在i状态上，转向第tran个状态的概率
				pre_current_prob += hidden_probs[i] * transition[i][tran];

			}
			cout << emmision[tran][in_emmision_watch_value] << endl;//第tran个状态的发射概率
			new_hidden_probs.push_back(pre_current_prob*emmision[tran][in_emmision_watch_value]);
		}

		hidden_probs = new_hidden_probs;
	}
	current_epoch += 1;
	if (current_epoch == watch_value.size()){
		return hidden_probs;
	}
	return forward_proba(init_prob, transition, emmision, watch_value, hidden_probs, current_epoch);
}



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
			new_route.push_back(route[i] + " " + basic_ele[j]);
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
