#include<iostream>
#include<string>
#include<vector>

using namespace std;


//�����㷨
float force_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<string> watch_value,
	vector<float> hidden_probs, int current_epoch);

//ǰ���㷨
vector<float> forward_proba(vector<float> init_prob, vector<vector<float>> transition, vector<vector<float>> emmision,
	vector<int> watch_value,
	vector<float> hidden_probs, int current_epoch);

//���ظ���ȫ����
vector<string> get_group(vector<string> &basic_ele, vector<string>& route, int current_epoch, int seq_len);


/*
	�����ܺ���
*/
vector<int> split_1(const std::string &text, char sep);

int str_route(string route_str, vector<int> &route);
