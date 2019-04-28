#include<iostream>
#include<string>
#include<vector>

using namespace std;


//暴力算法
float force_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<string> watch_value,
	vector<float> hidden_probs, int current_epoch);

//前向算法
vector<float> forward_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<int> watch_value,
	vector<float> hidden_probs, int current_epoch);

//可重复的全排列
vector<string> get_group(vector<string> &basic_ele, vector<string>& route, int current_epoch, int seq_len);


/*
	副功能函数
*/
vector<int> split_1(const std::string &text, char sep);

int str_route(string route_str, vector<int> &route);
