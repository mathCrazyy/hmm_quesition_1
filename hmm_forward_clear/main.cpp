/*
	功能:
	HMM求一个观测序列出现的概率，即求所有导致这个观测序列产生的所有隐状态序列的概率之和。
	
	--------------暴力求解--------------
	1. 确定观测序列长度为watchvalue_seq_len,观测序列中的值域为1-n，即一个求watchvalue_seq_len个数字组成序列种类的全排列过程。
		函数: get_group
	2. 初始状态pi的取值下，即相当于已知第一个状态下的状态转移，直到最后一个状态前的状态转移乘机。【最后一个状态不再做状态跳转】
	3. 在状态序列跳转中，穿插着发射概率，从初始状态下就开始计算发射概率，到最后一个状态，比转移概率的计算要多一个。
	4. 对pi*状态转移序列乘积*发射概率序列乘积求和，即为一个观测序列出现的概率。


	--------------前向算法--------------
	1. 在初始状态（时刻）下，分别计算每个隐状态的情况下，产生第一个观测值的概率，记录这个概率组。
	2. 在第二个状态下，走向每个隐状态的概率。即以走向A隐状态为例，在1的基础上，概率组中的每个值（在某个隐状态上）
		乘以该隐状态-->A的转移概率，求和即为到隐状态A的所有概率，再乘以隐状态A到观测值的发射概率。更新这个概率组。
	3. 递归方式不断重复2，直到达到观测长度。
	宏观上考虑，即为不断从内部状态转移的情况组合内部状态呈现的观测值，像推浪一样，出来的层层浪，每层中的现象都是结合了之前现象的可能性，
	最后的现象（概率组合）内部呈现的各个概率之和，即为这个现象出现的概率。
	
	*/

#include"basic.h"

using namespace std;

int main(){
	// 1维度vector赋值 https://blog.csdn.net/zhuwj06/article/details/6209918
	//初始概率
	float pi_arr[3] = { 0.2, 0.4, 0.4 };
	vector<float> pi(pi_arr, pi_arr + 3);

	for (int i = 0; i < 3; i++){
		cout << pi[i] << endl;
	}

	//https://blog.csdn.net/sinat_34524133/article/details/79015031
	//概率转移矩阵
	int hidden_state_seq_num = 3;

	vector<vector<float>> transition_matrix(hidden_state_seq_num, vector<float>(hidden_state_seq_num));
	float transition_matrix_array[3][3] = {
		{ 0.5, 0.2, 0.3 },
		{ 0.3, 0.5, 0.2 },
		{ 0.2, 0.3, 0.5 }
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
		{ 0.5, 0.5 },
		{ 0.4, 0.6 },
		{ 0.7, 0.3 }
	};
	for (int i = 0; i < emmision_matrix.size(); i++){
		for (int j = 0; j < emmision_matrix[i].size(); j++){
			emmision_matrix[i][j] = emmision_matrix_array[i][j];
		}
	}

	//暴力求解
	string watch_pos_arr[3] = { "1", "2", "3" };
	vector<string> watch_pos(watch_pos_arr, watch_pos_arr + 3);
	vector<float> hidden_probs;
	int current_epoch = 0;
	float watch_value_proba=force_proba(pi, transition_matrix, emmision_matrix, watch_pos, hidden_probs,current_epoch);
	cout << watch_value_proba << endl;


	//前向算法求解

	string watch_value_strs = "0 1 0";
	vector<int> watch_value_vis = split_1(watch_value_strs, ' ');

	vector<float> hidden_probs_force;
	vector<float> hidden_probs_c;
	int current_epoch_force = 0;
	hidden_probs_c = forward_proba(pi, transition_matrix, emmision_matrix, watch_value_vis, hidden_probs_force, current_epoch_force);
	float all_prob = 0.0;
	for (int i = 0; i < hidden_probs_c.size(); i++){
		all_prob += hidden_probs_c[i];
	}
	cout << "all_prob: " << all_prob << endl;

	//前向算法求解...上一个..竟然把for循环写成了递归...尴尬..
	float res=forward_proba_for(pi, transition_matrix, emmision_matrix, watch_value_vis, hidden_probs_force);
	cout << "res: " << res << endl;

	system("pause");
	return 0;
}