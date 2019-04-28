// 参考: https://www.cnblogs.com/sddai/p/8475424.html
// 参考: https://blog.csdn.net/jiacli/article/details/44535351
#include<iostream>
#include<string>
#include<vector>
#include"basic.h"
using namespace std;

//HMM前向算法2019_4_23
/*
 已知初始pi，状态转移矩阵参数A，发射矩阵参数B，求某个观测序列发生的概率。即求发生这个观测序列的 所有隐含状态序列 的概率之和，不同的隐含状态序列，
 会由于发射矩阵的不同，而产生相同的观测序列。
*/

int main(){

	// 1维度vector赋值 https://blog.csdn.net/zhuwj06/article/details/6209918
	//初始概率
	float pi_arr[3] = { 0.2,0.4,0.4 };
	vector<float> pi(pi_arr, pi_arr + 3);

	for (int i = 0; i < 3; i++){
		cout << pi[i] << endl;
	}

	//https://blog.csdn.net/sinat_34524133/article/details/79015031
	//概率转移矩阵
	int hidden_state_seq_num=3;
	
	vector<vector<float>> transition_matrix(hidden_state_seq_num, vector<float>(hidden_state_seq_num));
	float transition_matrix_array[3][3] = {
		{0.5,0.2,0.3},
		{0.3,0.5,0.2},
		{0.2,0.3,0.5}
	};
	for (int i = 0; i < transition_matrix.size(); i++){
		for (int j = 0; j < transition_matrix[i].size(); j++){
			transition_matrix[i][j] = transition_matrix_array[i][j];
		}
	}
	//发射概率矩阵
	int output_cases_num = 2;
	vector<vector<float>> emmision_matrix(hidden_state_seq_num, vector<float>(output_cases_num));
	float emmision_matrix_array[3][2] = {
		{0.5,0.5},
		{0.4,0.6},
		{0.7,0.3}
	};
	for (int i = 0; i < emmision_matrix.size(); i++){
		for (int j = 0; j < emmision_matrix[i].size(); j++){
			emmision_matrix[i][j] = emmision_matrix_array[i][j];
		}
	}
	/*
	----------------暴力算法[错误]--------------------
	
	*/
	//红白红观测序列，红为0，白为1，观测序列为010
	//P(010)=P(010|所有序列组合）
	//此问题中，隐状态序列有3个值组成（观测长度T），每个状态下（共有N个状态）都可以转向其他状态包括当前状态。
	int watch_value[3] = { 0, 1, 0 };
	int seq_len = 3;
	vector<vector<float>> compose_state_emmision(hidden_state_seq_num, vector<float>(hidden_state_seq_num));
	for (int i = 0; i < seq_len; i++){
		//需要对初始概率做广播，与转移矩阵的每一个值做计算，含义是在当前初始状态下，下一个隐状态为(i,j)的概率，i=0
		transition_matrix;
		for (int j = 0; j < transition_matrix.size(); j++){
			
			float current_emmision = emmision_matrix[j][watch_value[i]];//
			
			float current_j = pi[j];
			for (int k = 0; k < transition_matrix[j].size(); k++){
				compose_state_emmision[j][k] = transition_matrix[j][k] * pi[j] * current_emmision;
			}
			
		}
	}
	//求矩阵各元素的和即为，各种情况下，最终观测序列为010的概率
	float P_010=0;
	for (int i = 0; i < compose_state_emmision.size(); i++){
		for (int j = 0; j < compose_state_emmision[i].size(); j++){
			P_010 += compose_state_emmision[i][j];
		}
	}
	cout << "观测值为010时的概率为: " << P_010 << endl;
	

	
	//----------------暴力算法---------------------------
	//P(I|lambda),序列I可能出现的种类有A(3,3)=6种XXXX,此处为可以重复的全排列
	//状态序列为1，2，3
	//需要参考 https://www.cnblogs.com/ybf-yyj/p/8490851.html
	string watch_pos_arr[3] = { "1", "2", "3"};
	vector<string> watch_pos(watch_pos_arr, watch_pos_arr + 3);
	vector<string> route;
	
	vector<string> all_route = get_group(watch_pos, route, 0, 2);

	for (int i = 0; i < all_route.size(); i++){
		cout << all_route[i] << endl;
	}

	//all_route为不同的隐状态序列
	//对于一条隐状态序列，求出现观测序列的概率。
	vector<int> route_int;
	string route_str = "1 2 3";
	route_int = split_1(route_str, ' ');
	float transition_value = 1.0;
	
	for (int i = 1; i < transition_matrix.size(); i++){//第一个路劲的取值，由初始pi决定。
		transition_value *= transition_matrix[i][route_int[i]-1];
	}
	float pi_transition_value=pi[route_int[0]-1] * transition_value;//转移矩阵的概率.
	//观测序列的概率
	string watch_value_str = "0 1 0";
	vector<int> watch_value_vi = split_1(watch_value_str, ' ');
	float emmision_value = 1.0;
	for (int i = 0; i < watch_value_vi.size(); i++){
		emmision_value *= emmision_matrix[i][watch_value_vi[i]];
	}
	float one_route_proba=pi_transition_value*emmision_value;
	cout << one_route_proba << endl;

	float O_proba = 0.0;
	for (int route_index = 0; route_index < all_route.size(); route_index++){
		//all_route为不同的隐状态序列
		//对于一条隐状态序列，求出现观测序列的概率。
		
		route_int = split_1(all_route[route_index], ' ');
		float transition_value = 1.0;

		for (int i = 1; i < transition_matrix.size(); i++){//第一个路劲的取值，由初始pi决定。
			transition_value *= transition_matrix[i][route_int[i] - 1];
		}
		float pi_transition_value = pi[route_int[0] - 1] * transition_value;//转移矩阵的概率.
		//观测序列的概率
		string watch_value_str = "0 1 0";
		vector<int> watch_value_vi = split_1(watch_value_str, ' ');
		float emmision_value = 1.0;
		for (int i = 0; i < watch_value_vi.size(); i++){
			//emmision_value *= emmision_matrix[i][watch_value_vi[i]];//emmison_matrix中的i应该为第几个盒子，即当前的隐状态
			emmision_value *= emmision_matrix[route_int[i]-1][watch_value_vi[i]];//emmison_matrix中的i应该为第几个盒子，即当前的隐状态
		}
		float one_route_proba = pi_transition_value*emmision_value;
		cout << one_route_proba << endl;
		O_proba += one_route_proba;

	}
	cout << "all_proba is: " << O_proba << endl;


	//------------------------前向算法

	string watch_value_strs = "0 1 0";
	vector<int> watch_value_vis = split_1(watch_value_strs, ' ');

	
	vector<float> hidden_probs;
	vector<float> hidden_probs_c;
	int current_epoch = 0;
	hidden_probs_c = forward_proba(pi, transition_matrix, emmision_matrix, watch_value_vis, hidden_probs, current_epoch);
	float all_prob = 0.0;
	for (int i = 0; i < hidden_probs_c.size(); i++){
		all_prob += hidden_probs_c[i];
	}
	cout <<"all_prob: "<< all_prob << endl;





	system("pause");
	return 0;
}