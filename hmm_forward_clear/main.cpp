/*
	����:
	HMM��һ���۲����г��ֵĸ��ʣ��������е�������۲����в�����������״̬���еĸ���֮�͡�
	
	--------------�������--------------
	1. ȷ���۲����г���Ϊwatchvalue_seq_len,�۲������е�ֵ��Ϊ1-n����һ����watchvalue_seq_len������������������ȫ���й��̡�
		����: get_group
	2. ��ʼ״̬pi��ȡֵ�£����൱����֪��һ��״̬�µ�״̬ת�ƣ�ֱ�����һ��״̬ǰ��״̬ת�Ƴ˻��������һ��״̬������״̬��ת��
	3. ��״̬������ת�У������ŷ�����ʣ��ӳ�ʼ״̬�¾Ϳ�ʼ���㷢����ʣ������һ��״̬����ת�Ƹ��ʵļ���Ҫ��һ����
	4. ��pi*״̬ת�����г˻�*����������г˻���ͣ���Ϊһ���۲����г��ֵĸ��ʡ�


	--------------ǰ���㷨--------------
	1. �ڳ�ʼ״̬��ʱ�̣��£��ֱ����ÿ����״̬������£�������һ���۲�ֵ�ĸ��ʣ���¼��������顣
	2. �ڵڶ���״̬�£�����ÿ����״̬�ĸ��ʡ���������A��״̬Ϊ������1�Ļ����ϣ��������е�ÿ��ֵ����ĳ����״̬�ϣ�
		���Ը���״̬-->A��ת�Ƹ��ʣ���ͼ�Ϊ����״̬A�����и��ʣ��ٳ�����״̬A���۲�ֵ�ķ�����ʡ�������������顣
	3. �ݹ鷽ʽ�����ظ�2��ֱ���ﵽ�۲ⳤ�ȡ�
	����Ͽ��ǣ���Ϊ���ϴ��ڲ�״̬ת�Ƶ��������ڲ�״̬���ֵĹ۲�ֵ��������һ���������Ĳ���ˣ�ÿ���е������ǽ����֮ǰ����Ŀ����ԣ�
	�������󣨸�����ϣ��ڲ����ֵĸ�������֮�ͣ���Ϊ���������ֵĸ��ʡ�
	
	*/

#include"basic.h"

using namespace std;

int main(){
	// 1ά��vector��ֵ https://blog.csdn.net/zhuwj06/article/details/6209918
	//��ʼ����
	float pi_arr[3] = { 0.2, 0.4, 0.4 };
	vector<float> pi(pi_arr, pi_arr + 3);

	for (int i = 0; i < 3; i++){
		cout << pi[i] << endl;
	}

	//https://blog.csdn.net/sinat_34524133/article/details/79015031
	//����ת�ƾ���
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
	//������ʾ���
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

	//�������
	string watch_pos_arr[3] = { "1", "2", "3" };
	vector<string> watch_pos(watch_pos_arr, watch_pos_arr + 3);
	vector<float> hidden_probs;
	int current_epoch = 0;
	float watch_value_proba=force_proba(pi, transition_matrix, emmision_matrix, watch_pos, hidden_probs,current_epoch);
	cout << watch_value_proba << endl;


	//ǰ���㷨���

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

	//ǰ���㷨���...��һ��..��Ȼ��forѭ��д���˵ݹ�...����..
	float res=forward_proba_for(pi, transition_matrix, emmision_matrix, watch_value_vis, hidden_probs_force);
	cout << "res: " << res << endl;

	system("pause");
	return 0;
}