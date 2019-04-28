// �ο�: https://www.cnblogs.com/sddai/p/8475424.html
// �ο�: https://blog.csdn.net/jiacli/article/details/44535351
#include<iostream>
#include<string>
#include<vector>
#include"basic.h"
using namespace std;

//HMMǰ���㷨2019_4_23
/*
 ��֪��ʼpi��״̬ת�ƾ������A������������B����ĳ���۲����з����ĸ��ʡ�����������۲����е� ��������״̬���� �ĸ���֮�ͣ���ͬ������״̬���У�
 �����ڷ������Ĳ�ͬ����������ͬ�Ĺ۲����С�
*/

int main(){

	// 1ά��vector��ֵ https://blog.csdn.net/zhuwj06/article/details/6209918
	//��ʼ����
	float pi_arr[3] = { 0.2,0.4,0.4 };
	vector<float> pi(pi_arr, pi_arr + 3);

	for (int i = 0; i < 3; i++){
		cout << pi[i] << endl;
	}

	//https://blog.csdn.net/sinat_34524133/article/details/79015031
	//����ת�ƾ���
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
	//������ʾ���
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
	----------------�����㷨[����]--------------------
	
	*/
	//��׺�۲����У���Ϊ0����Ϊ1���۲�����Ϊ010
	//P(010)=P(010|����������ϣ�
	//�������У���״̬������3��ֵ��ɣ��۲ⳤ��T����ÿ��״̬�£�����N��״̬��������ת������״̬������ǰ״̬��
	int watch_value[3] = { 0, 1, 0 };
	int seq_len = 3;
	vector<vector<float>> compose_state_emmision(hidden_state_seq_num, vector<float>(hidden_state_seq_num));
	for (int i = 0; i < seq_len; i++){
		//��Ҫ�Գ�ʼ�������㲥����ת�ƾ����ÿһ��ֵ�����㣬�������ڵ�ǰ��ʼ״̬�£���һ����״̬Ϊ(i,j)�ĸ��ʣ�i=0
		transition_matrix;
		for (int j = 0; j < transition_matrix.size(); j++){
			
			float current_emmision = emmision_matrix[j][watch_value[i]];//
			
			float current_j = pi[j];
			for (int k = 0; k < transition_matrix[j].size(); k++){
				compose_state_emmision[j][k] = transition_matrix[j][k] * pi[j] * current_emmision;
			}
			
		}
	}
	//������Ԫ�صĺͼ�Ϊ����������£����չ۲�����Ϊ010�ĸ���
	float P_010=0;
	for (int i = 0; i < compose_state_emmision.size(); i++){
		for (int j = 0; j < compose_state_emmision[i].size(); j++){
			P_010 += compose_state_emmision[i][j];
		}
	}
	cout << "�۲�ֵΪ010ʱ�ĸ���Ϊ: " << P_010 << endl;
	

	
	//----------------�����㷨---------------------------
	//P(I|lambda),����I���ܳ��ֵ�������A(3,3)=6��XXXX,�˴�Ϊ�����ظ���ȫ����
	//״̬����Ϊ1��2��3
	//��Ҫ�ο� https://www.cnblogs.com/ybf-yyj/p/8490851.html
	string watch_pos_arr[3] = { "1", "2", "3"};
	vector<string> watch_pos(watch_pos_arr, watch_pos_arr + 3);
	vector<string> route;
	
	vector<string> all_route = get_group(watch_pos, route, 0, 2);

	for (int i = 0; i < all_route.size(); i++){
		cout << all_route[i] << endl;
	}

	//all_routeΪ��ͬ����״̬����
	//����һ����״̬���У�����ֹ۲����еĸ��ʡ�
	vector<int> route_int;
	string route_str = "1 2 3";
	route_int = split_1(route_str, ' ');
	float transition_value = 1.0;
	
	for (int i = 1; i < transition_matrix.size(); i++){//��һ��·����ȡֵ���ɳ�ʼpi������
		transition_value *= transition_matrix[i][route_int[i]-1];
	}
	float pi_transition_value=pi[route_int[0]-1] * transition_value;//ת�ƾ���ĸ���.
	//�۲����еĸ���
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
		//all_routeΪ��ͬ����״̬����
		//����һ����״̬���У�����ֹ۲����еĸ��ʡ�
		
		route_int = split_1(all_route[route_index], ' ');
		float transition_value = 1.0;

		for (int i = 1; i < transition_matrix.size(); i++){//��һ��·����ȡֵ���ɳ�ʼpi������
			transition_value *= transition_matrix[i][route_int[i] - 1];
		}
		float pi_transition_value = pi[route_int[0] - 1] * transition_value;//ת�ƾ���ĸ���.
		//�۲����еĸ���
		string watch_value_str = "0 1 0";
		vector<int> watch_value_vi = split_1(watch_value_str, ' ');
		float emmision_value = 1.0;
		for (int i = 0; i < watch_value_vi.size(); i++){
			//emmision_value *= emmision_matrix[i][watch_value_vi[i]];//emmison_matrix�е�iӦ��Ϊ�ڼ������ӣ�����ǰ����״̬
			emmision_value *= emmision_matrix[route_int[i]-1][watch_value_vi[i]];//emmison_matrix�е�iӦ��Ϊ�ڼ������ӣ�����ǰ����״̬
		}
		float one_route_proba = pi_transition_value*emmision_value;
		cout << one_route_proba << endl;
		O_proba += one_route_proba;

	}
	cout << "all_proba is: " << O_proba << endl;


	//------------------------ǰ���㷨

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