// jiedui.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <math.h>
#include <cstdlib>
#include <ctime>

#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#include<string.h>


#define random(a,b) (rand()%(b-a+1)+a)
#define NUMSIZE 50
#define STACKSIZE 50
#define GEN 100
#define Frac_Num 1
#define Num 0
#define ROWSIZE 100
#define MAX_LEN 20



using namespace std;

//definition of Node
typedef struct node {
	//the node of string
	char c = 0;
	int num = 0;
	int flag = -1;
	int frac[4] = {0,0,0,0};

}Node, NNode[NUMSIZE], *N_ptr;


//end definition

int _sup = 20, _inf = 1;
int _inf_len = 5, _sup_len = 9;
int _symbol = 1;
int _row = 50;
char *_oper = "+-*/";
string expression;
string str_key;
string path_exp = "D://result.txt";
string path_key = "D://key.txt";

bool N_gen_app = false;
int File_out = 1;
int File_app = 0;



vector<string> expressions;
vector<string> str_keys;
NNode suffix;
NNode* _sufarr[ROWSIZE];
Node _suffix_result;


bool set_range(int inf,int sup) {
	if (0 <= inf&&inf <= sup&&sup <= INT_MAX) {
		_inf = inf;
		_sup = sup;
		return true;
	}
	return false;

}//end set_range

bool set_len(int inf,int sup) {
	if (inf >= 2 && sup >= inf&&sup <= MAX_LEN) {
		_inf_len = inf;
		_sup_len = sup;
		return true;

	}//end if
	return false;
}//end set_len

bool set_mode(int symbol) {
	if (symbol == 0 || symbol==1 || symbol==2) {
		_symbol = symbol;
		return true;
	}//end if
	else return false;
}//end set_mode

bool set_row(int row) {
	if (row > 0 && row <= ROWSIZE) {
		_row = row;
	}//end if
}//end set_row

bool set_operator(char add,char min,char mul,char div,char power) {
	
}


void getnormalsuffix(int key, int NUM);

void init_suffix(Node& suffix);

void disp_key(int key);
void getnormalsuffix(int key, int NUM);
//考虑是否需要单独初始化其他变量*******

//definition of stack

typedef struct {
	NNode stack;
	int stacksize;
	int top;

}Nsqstack;
//the reload of operator == is ok;


void Ninitstack(Nsqstack &s) {
	int i = 0;
	for (i = 0; i<NUMSIZE; i++) init_suffix(s.stack[i]);
	s.stacksize = NUMSIZE;
	s.top = -1;
	return;
}//end Ninitstack

void Ndestroystack(Nsqstack &s) {
	s.stacksize = 0;
	s.top = -1;
	return;
}//end Ndestroystack

int  Ngettop(Nsqstack s, Node& suffix) {
	if (s.top == -1) return 0;
	suffix = s.stack[s.top];
	return 1;
}//Ngettop

void Npush(Nsqstack &s, Node suffix) {
	
	s.stack[++s.top] = suffix;
	
	return;
}//end Npush

int Npop(Nsqstack &s, Node &suffix) {
	if (s.top == -1) return 0;
	suffix = s.stack[s.top];
	init_suffix(s.stack[s.top]);
	s.top--;
	return 1;
}//end Npop

bool Nstackempty(Nsqstack s) {
	if (s.top == -1) return true;
	else return false;
}//end Nstackempty

 //end Initialization of stack

 //cal of two node/num

int gcd(int a, int b) {
	int i = 0, j = 0, k = 0;
	int result = 0;
	if (a < b) {
		i = a;
		a = b;
		b = i;
	}//end if
	if (a%b == 0)return b;
	result = a%b;
	while (result != 0) {
		a = b;
		b = result;
		result = a%b;
	}//end while
	return b;

}//end gcd


int cal(int a, int b, char ch) {
	//function of calculation
	int c = 0;
	switch (ch) {
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a*b;
	case '/':
		if (b == 0) return -10000;
		if (a%b == 0) {
			return a / b;
		}
		else return -1;// ********
	case '^':return pow(a, b);//********
	}
}//end cal

int frac_divide(Node &suffix_result) {
	//
	int temp = 0;
	int para = 0;

	if (suffix_result.flag==2&&suffix_result.frac[3] == 0) {
		return -10000;//divide 0 error
	}
	if (suffix_result.frac[2] < suffix_result.frac[3]) {
		temp = gcd(suffix_result.frac[2], suffix_result.frac[3]);
		if (temp != 1) {
			suffix_result.frac[2] /= temp;
			suffix_result.frac[3] /= temp;
		}//if has a gcd larger than 1
		suffix_result.flag = 2;
		suffix_result.num = 0;


		return 2;
	}//end if '/' get frac<1
	else if (suffix_result.frac[2] == suffix_result.frac[3]) {
		suffix_result.num = 1+suffix_result.frac[1];
		suffix_result.flag = 0;
		for (int i = 0; i <= 3; i++) {
			suffix_result.frac[i] = 0;
		}//end for
		return 0;
	}//end else if '/' get 1
	else {
		para = suffix_result.frac[2] % suffix_result.frac[3];
		if (para == 0) {
			//can get a intergar
			suffix_result.num = suffix_result.frac[1] + suffix_result.frac[2] / suffix_result.frac[3];
			suffix_result.flag = 0;
			for (int i = 0; i <= 3; i++) {
				suffix_result.frac[i] = 0;
			}//end for
			return 0;
		}//end if is a >1 integar
		else {
			temp = suffix_result.frac[2] - para;
			temp = temp / suffix_result.frac[3];
			suffix_result.frac[1] += temp;
			suffix_result.frac[2] = suffix_result.frac[2] - temp*suffix_result.frac[3];

			temp = gcd(suffix_result.frac[2], suffix_result.frac[3]);
			if (temp != 1) {
				suffix_result.frac[2] /= temp;
				suffix_result.frac[3] /= temp;
			}//if has a gcd larger than 1
		
			suffix_result.flag = 2;
			suffix_result.num = 0;
			return 2;

		}//end else is a >1 fraction

	}//end else '/' get >1 num&frac
}

int cal(Node suffix_left, Node suffix_right, Node &suffix_result,char op) {
	//cal:with para list having two node,and one result node
	//the value return as -1,0,1 meanging result is -,0,+ 
	//return -10000 means divide 0 error,2 means get a fraction
	int flag_l = suffix_left.flag, flag_r = suffix_right.flag;
	int temp = 0;
	int para = 0;
	if (flag_l == 0 && flag_r == 0) {
		if (op == '+' ) {
			suffix_result.num = suffix_left.num + suffix_right.num;
			suffix_result.flag = 0; 
			if (suffix_result.num == 0)
				return 0;
			else
				return 1;
		}//end if '+'
		else if (op == '*') {
			suffix_result.num = suffix_left.num * suffix_right.num;
			suffix_result.flag = 0;
			if (suffix_result.num == 0)
				return 0;
			else
				return 1;
		}//end else if '*'
		else if (op == '-') {
			if (suffix_left.num < suffix_right.num) {
				suffix_result.flag = 0;
				return -1;
			}//end if '-' get -
			else if (suffix_left.num == suffix_right.num) {
				suffix_result.num = 0;
				suffix_result.flag = 0;
				return 0;
			}//end else if '-' get 0
			else {
				suffix_result.num = suffix_left.num - suffix_right.num;
				suffix_result.flag = 0;
				return 1;
			}//end else '-' get +
		}//end else if '-'
		else if (op == '/') {
			if (suffix_right.num == 0) {
				return -10000;//divide 0 error
			}
			if (suffix_left.num == 0) {
				suffix_result.flag = 0;
				suffix_result.num = 0;
				return 0;
			}
			suffix_result.frac[1] = 0;
			suffix_result.frac[2] = suffix_left.num;
			suffix_result.frac[3] = suffix_right.num;
			
			temp=frac_divide(suffix_result);
			if (temp == -10000) {
				return -10000;
			}//end divide 0 error
			else if (temp == 2) {
				if (suffix_result.frac[1] == 0) {
					if (suffix_result.frac[2] == 0) {
						return 0;
					}//end if equal to 0
					else return 1;
				}//end if frac[1]==0
				else return 1;

			}
			else if (temp == 0) {
				if (suffix_result.num == 0)
					return 0;
				else
					return 1;
			}
			
			
		}//end else if '/'

	}//end if two integar
	else if (flag_l == 2 && flag_r == 0|| flag_l == 0 && flag_r == 2|| flag_l == 2 && flag_r == 2) {
		//if there is a frac&num or frac&frac
		
		if (op == '+') {

			if (flag_l == 2 && flag_r == 0 ) {
				//if there is a frac | num
				suffix_result.frac[1] = suffix_left.frac[1]+suffix_right.num;
				suffix_result.frac[2] = suffix_left.frac[2];
				suffix_result.frac[3] = suffix_left.frac[3];
				suffix_result.flag = 2;
			}//end f|n +
			else if (flag_l == 0 && flag_r == 2) {
				//if there is a num | frac
				suffix_result.frac[1] = suffix_right.frac[1] + suffix_left.num;
				suffix_result.frac[2] = suffix_right.frac[2];
				suffix_result.frac[3] = suffix_right.frac[3];
				suffix_result.flag = 2;
			}//end n|f +
			else if (flag_l == 2 && flag_r == 2) {
				//if there is a frac | frac
				suffix_result.frac[1] = suffix_right.frac[1] + suffix_left.frac[1];
				suffix_result.frac[2] = suffix_right.frac[2] * suffix_left.frac[3] + 
					suffix_left.frac[2] * suffix_right.frac[3];

				suffix_result.frac[3] = suffix_right.frac[3] * suffix_left.frac[3];
				suffix_result.flag = 2;
				
			}//end if f|f +
			frac_divide(suffix_result);
			return 1;
		}//end if +

		else if (op == '*') {
			
			if (flag_l == 2 && flag_r == 0) {
				//if there is a frac | num
				suffix_result.frac[1] = suffix_left.frac[1] * suffix_right.num;
				suffix_result.frac[2] = suffix_left.frac[2] * suffix_right.num;
				suffix_result.frac[3] = suffix_left.frac[3];
				suffix_result.flag = 2;
			}//end f|n *
			else if (flag_l == 0 && flag_r == 2) {
				//if there is a num | frac
				suffix_result.frac[1] = suffix_right.frac[1] * suffix_left.num;
				suffix_result.frac[2] = suffix_right.frac[2] * suffix_left.num;
				suffix_result.frac[3] = suffix_right.frac[3];
				suffix_result.flag = 2;
			}//end n|f *
			else if (flag_l == 2 && flag_r == 2) {
				//if there is a frac | frac
				suffix_result.frac[1] = suffix_right.frac[1] * suffix_left.frac[1];

				suffix_result.frac[2] = suffix_right.frac[2] * suffix_left.frac[2] +
					suffix_left.frac[2] * suffix_right.frac[1] * suffix_right.frac[3] +
					suffix_right.frac[2] * suffix_left.frac[1] * suffix_left.frac[3];

				suffix_result.frac[3] = suffix_right.frac[3] * suffix_left.frac[3];
				suffix_result.flag = 2;
				
			}//end if f|f *
			if (suffix_result.frac[1] == 0 && suffix_result.frac[2] == 0) {
				return 0;
			}
			frac_divide(suffix_result);
			return 1;
		}//end if f|n、n|f、f|f *
		else if (op == '-') {
			if (flag_l == 2 && flag_r == 0) {
				//if there is a frac | num
				suffix_result.frac[1] = suffix_left.frac[1] - suffix_right.num;
				
				suffix_result.frac[2] = suffix_left.frac[2];
				suffix_result.frac[3] = suffix_left.frac[3];
				suffix_result.flag = 2;
			}//end f|n -
			else if (flag_l == 0 && flag_r == 2) {
				//if there is a num | frac  1
				suffix_result.frac[1] = suffix_left.num - suffix_right.frac[1] - 1;
				suffix_result.frac[2] = suffix_right.frac[3] - suffix_right.frac[2];
				suffix_result.frac[3] = suffix_right.frac[3];
				suffix_result.flag = 2;
			}//end n|f -
			else if (flag_l == 2 && flag_r == 2) {
				//if there is a frac | frac  
				suffix_result.frac[1] = suffix_left.frac[1] - suffix_right.frac[1];
				suffix_result.frac[2] =
					suffix_left.frac[2] * suffix_right.frac[3] - suffix_right.frac[2] * suffix_left.frac[3];

				suffix_result.frac[3] = suffix_right.frac[3] * suffix_left.frac[3];
				suffix_result.flag = 2;

			}//end if f|f -

			if (suffix_result.frac[1] < 0) {
				return -1;
			}//if frac is <0
			else if (suffix_result.frac[1] == 0) {
				if (suffix_result.frac[2] < 0) {
					return -1;
				}//if frac[1]==0, and frac[2]<0
				else if (suffix_result.frac[2] == 0) {
					suffix_result.num = 0;
					suffix_result.flag = 0;
					for (int i = 0; i <= 3; i++) {
						suffix_result.frac[i] = 0;
					}//end for
					return 0;
				}// if frac[1]==0, and frac[2]==0
				
			}//if frac[1]==0 
			else if (suffix_result.frac[1] > 0 && suffix_result.frac[2] < 0) {
				suffix_result.frac[1] -= 1;
				suffix_result.frac[2] += suffix_result.frac[3];
			}
			else if (suffix_result.frac[1] > 0 && suffix_result.frac[2] == 0) {
				suffix_result.num = suffix_result.frac[1];
				suffix_result.flag = 0;
				for (int i = 0; i <= 3; i++) {
					suffix_result.frac[i] = 0;
				}
			}
			frac_divide(suffix_result);
			return 1;
		}//if '-'
		else if (op == '/') {
			if (suffix_right.flag==0&&suffix_right.num == 0) return -10000;//divide 0 error

			if (flag_l == 2 && flag_r == 0) {
				//if there is a frac | num
				suffix_left.frac[3] *= suffix_right.num;
				suffix_right.frac[1] = 0;
				suffix_right.frac[2] = suffix_left.frac[1];
				suffix_right.frac[3] = suffix_right.num;
				suffix_right.num = 0;
				suffix_right.flag = 2;
				suffix_left.frac[1] = 0;
				cal(suffix_left, suffix_right, suffix_result, '+');
				return 1;

			}//end f|n /
			else if (flag_l == 0 && flag_r == 2) {
				//if there is a num | frac
				if (suffix_left.num == 0) {
					suffix_result.num = 0;
					suffix_result.flag = 0;
					return 0;//get 0
				}//end get 0

				suffix_result.frac[1] = 0;
				suffix_result.frac[2] = suffix_right.frac[3] * suffix_left.num;
				suffix_result.frac[3] = suffix_right.frac[1] * suffix_right.frac[3] + suffix_right.frac[2];
				suffix_result.flag = 2;
				
				frac_divide(suffix_result);
				return 1;
			}//end n|f /
			else if (flag_l == 2 && flag_r == 2) {
				//if there is a frac | frac
				suffix_result.frac[1] = 0;

				suffix_result.frac[2] = suffix_right.frac[3] * (suffix_left.frac[3] *
					suffix_left.frac[1] + suffix_left.frac[2]);

				suffix_result.frac[3] = suffix_left.frac[3] * (suffix_right.frac[3] *
					suffix_right.frac[1] + suffix_right.frac[2]);
				suffix_result.flag = 2;
				frac_divide(suffix_result);
				return 1;
			}//end if f|f *

		}//end f|n、n|f、f|f '/'
	}//end f|n、n|f、f|f


}//end cal with Node

 //end cal of two node/m

 //srand_generator

bool srand_generator(int inf,int sup,int div) {
	//set the srand generator
	int j = 0;
	j = random(inf, sup);
	if (j < div) return false;
	else return true;
}//end srand_generator

int srand_generator(int div) {
	//set the srand generator
	int j = 0;
	j = random(1, GEN);
	int beg = 0;
	int temp = 100 / div;
	int result = 0;
	for (; result<=100; ) {
		result += temp;
		if (j <= result&&j > result - temp)
			break;
		else beg++;	
	}//end for
	return beg;

}//end srand_generator

 //end srand_generator

void init_suffix(Node& suffix) {
	suffix.c = '\0';
	suffix.num = 0;
	suffix.flag = -1;
	for (int i = 0; i <= 3; i++) {
		suffix.frac[i] = 0;
	}
	return;
}//end init_suffix


void set_suffix(Node& suffix,int inf,int sup,int symbol,char* oper) {
	//set_suffix:setting char/num/fraction with differernt value of para symbol
	//case 0:num case 1: char case 2:fraction
	//inf: the inferior bound,sup: the superior bound
	//oper: the set of operator
	
	int len_oper = strlen(oper);
	int temp = 0;				//used to get the gcd of fraction
	switch (symbol) {
	case 0:
		//set num
		suffix.num = random(inf, sup);
		suffix.flag = 0;
		break;
	case 1:
		//set char
		
		suffix.flag = 1;
		suffix.c = oper[random(0, len_oper - 1)];
		break;
	case 2:
		//set fraction
		//(optional)using suffix.frac[0] to get the sup bound of the first num
		suffix.flag = 2;
		for (int i = 1; i <= 3; i++) {
			suffix.frac[i] = random(inf, sup);
		}
		temp = gcd(suffix.frac[2], suffix.frac[3]);
		if (temp != 1) {
			suffix.frac[2] /= temp;
			suffix.frac[3] /= temp;
		}//end if gcd!=1 get the prime version

		if (suffix.frac[2] == suffix.frac[3]) {
			suffix.flag = 0;
			suffix.num = suffix.frac[1] + 1;
			for (int i = 0; i <= 3; i++)
				suffix.frac[i] = 0;
		}//end if equal,change to num
		
		else if (suffix.frac[2] > suffix.frac[3]) {
			temp = suffix.frac[2];
			suffix.frac[2] = suffix.frac[3];
			suffix.frac[3] = temp;
		}//get the true fraction
		
		break;
	case 3:
		//optional: set the double
		break;

	}//end switch

}//end set_suffix

void set_num_suffix(Node& suffix, int inf, int sup, char* oper) {
	//set the num&fraction of suffix
	int flag = 0;
	flag = srand_generator(inf, sup, (inf + sup) / 2);

	if (flag == 0) {
		//set a num
		set_suffix(suffix, inf, sup, 0, oper);
	}
	else {
		//set a fraction
		set_suffix(suffix, inf, sup, 2, oper);
	}//end else

}//end set_num_suffix

void N_init_suffix(NNode &suffix) {
	int i = 0;
	for (i = 0; i < NUMSIZE; i++) {
		init_suffix(suffix[i]);
	}


}//end N_init_suffix


void suffix_generator() {
	//generate the suffix erpression

	//initialization
	N_init_suffix(suffix);

	int symbol = _symbol;

	int temp = 0,temp1;//temp variable(use for case 3)
	int i = 0, j = 0, k = 0;// invariable of count


	char oper2[10];
	for (i = 0; i < int(strlen(_oper)); i++) {
		oper2[i] = _oper[i];
	}
	oper2[i] = 0, i = 0;
	char* oper = oper2;


	bool flag = 0;			// flag of generating operator or num

	int sup = _sup;// the superior bound of the num
	int inf = _inf; // the inferior bound of the num

	int count_num  = 0; //the number of num in the experssion
	int count_oper = 0; //the number of operator in the experssion

	int inf_len = _inf_len;//the inferior bound of the length of expression
	int sup_len = _sup_len;//the superior bound of the length of expression
	
	int len_oper = strlen(oper);		 //the length of the operator set

	int length;
	if (inf_len == sup_len) {
		length = inf_len;
	}
	else {
		length = random(inf_len, sup_len);//the length of the exp,randomized
	}

	string string1 = oper;

	//end definition

	if (length % 2 == 0) length++;
	
	
	suffix[0].flag = 3;    //3 ************* 3 means begin
	suffix[0].num = length;//store the length
	//end initialization
	temp = string1.find('/');  
	if (symbol==0 && temp>0) {//bool 型的狗逼错！！！！！
		symbol = 3;
		char oper1[10];
		for (i = 0, j = 0; i < len_oper; i++) {
			if (oper[i] != '/') {
				oper1[j] = oper[i];
				j++;
			}//end if
				
		}
		oper1[j] = 0;
		oper = oper1;
		len_oper = strlen(oper);
	}
	
	


	switch (symbol) {
	case 0:
		//case of only intergar
		suffix[1].num = random(inf, sup);
		suffix[1].flag = 0;
		suffix[2].num = random(inf, sup);
		suffix[2].flag = 0;
		count_num = 2;
		i = 3;

		//end initialization
		
		while (count_num < (1 + (length - 1) / 2)) {
			if (count_num <= count_oper + 1) {
				//the prefix is full(n numbers and n-1 operators)
				suffix[i].flag = 0;
				suffix[i++].num = random(inf, sup);
				count_num++;
			}//end if
			else if (count_oper == ((length - 1) / 2 - 1)) {
				//if there is almost done，then num should not be the last
				suffix[i].flag = 0;
				suffix[i++].num = random(inf, sup);
				count_num++;
			}
			else {
				flag = srand_generator(inf, sup, (inf + sup) / 2);
				//generating num & operator with equal possibility
				if (flag == 0) {
					//generating the number
					suffix[i].flag = 0;
					suffix[i++].num = random(inf, sup);
					count_num++;
				}//end if
				else {
					//generating the operator
					suffix[i].flag = 1;
					suffix[i++].c = oper[random(0, len_oper - 1)];
					count_oper++;
				}//end else
			}//end else
				
			
			

		}//end while

		break;//end case 0

	case 1:
		//initialization
		set_num_suffix(suffix[1], inf, sup, oper);
		set_num_suffix(suffix[2], inf, sup, oper);

		
		count_num = 2;
		i = 3;
		//end initialization
		while (count_num < (1 + (length - 1) / 2)) {
			if (count_num <= count_oper + 1) {
				//the prefix is full(n numbers and n-1 operators)
				set_num_suffix(suffix[i++], inf, sup, oper);
				count_num++;
			}//end if
			else if (count_oper == ((length - 1) / 2 - 1)) {
				//if there is almost done，then num should not be the last
				set_num_suffix(suffix[i++], inf, sup, oper);
				count_num++;
			}
			else {
				flag = srand_generator(inf, sup, (inf + sup) / 2);
				//generating num & operator with equal possibility
				if (flag == 0) {
					//generating the number
					set_num_suffix(suffix[i++], inf, sup, oper);
					count_num++;
				}//end if
				else {
					//generating the operator
					set_suffix(suffix[i++], inf, sup, 1, oper);				
					count_oper++;
				}//end else
			}//end else

		}//end while

		break;


	case 2:
		//生成小数****************8

		break;
	case 3:
		//case of only intergar,with /
		suffix[1].num = random(inf, sup);
		suffix[1].flag = 0;
		suffix[2].num = random(inf, sup);
		suffix[2].flag = 0;
		count_num = 2;
		i = 3;
		
		//end initialization

		while (count_num < (1 + (length - 1) / 2)) {
			if (srand_generator(10, 20, 15)) {
				if (count_num <= ((length - 1) / 2 - 1)) {
					temp = random(inf, int(floor(sqrt(sup))));
					temp1 = random(inf, int(floor(sqrt(sup))));
					suffix[i].num = temp1*temp;
					suffix[i++].flag = 0;
					suffix[i].num = temp;
					suffix[i++].flag = 0;
					suffix[i].c = '/';
					suffix[i++].flag = 1;
					count_num += 2;
					count_oper++;

				}//if no out of range
				else continue;
				
			}//end if
			else {
				if (count_num <= count_oper + 1) {
					//the prefix is full(n numbers and n-1 operators)
					suffix[i].flag = 0;
					suffix[i++].num = random(inf, sup);
					count_num++;
				}//end if
				else if (count_oper == ((length - 1) / 2 - 1)) {
					//if there is almost done，then num should not be the last
					suffix[i].flag = 0;
					suffix[i++].num = random(inf, sup);
					count_num++;
				}
				else {
					flag = srand_generator(inf, sup, (inf + sup) / 2);
					//generating num & operator with equal possibility
					if (flag == 0) {
						//generating the number
						suffix[i].flag = 0;
						suffix[i++].num = random(inf, sup);
						count_num++;
					}//end if
					else {
						//generating the operator
						suffix[i].flag = 1;
						suffix[i++].c = oper[random(0, len_oper - 1)];
						count_oper++;
					}//end else
				}//end else
			}//end else


		}//end while

		break;//end case 3


	}


	while (count_oper < (length - 1) / 2) {
		//fill the last operator
		suffix[i].flag = 1;
		suffix[i++].c = oper[random(0, len_oper - 1)];
		count_oper++;

	}//end while
	suffix[i].flag = -1;// not need

}//end suffix_generator

int calculate() {
	//calculate the suffix
	//symbol decides the type of calculation
	//未考虑分数，小数，真分数，返回整数
	//未考虑中间出现负数
	Nsqstack s;
	Ninitstack(s);
	
	int symbol = _symbol;

	int length = suffix[0].num;//the length of exp
	int len_num = (length + 1) / 2;
	int len_oper = (length - 1) / 2;

	if (length >= 3) {
		Npush(s, suffix[1]);
		Npush(s, suffix[2]);
	}
	else return -10000;

	Node suffix_left, suffix_right,suffix_result;
	init_suffix(suffix_left);
	init_suffix(suffix_right);
	init_suffix(suffix_result);

	int i = 3;
	int left = 0, right = 0;//left number and right number 
	char op = 0;			//operator
	int result = 0;			//result



	switch (symbol) {
	case 0:
		//all the num is integar
		while (i <= length) {
			init_suffix(suffix_result);
			if (suffix[i].flag == 0) {
				//get a num
				Npush(s, suffix[i]);
				i++;
			}//end if

			else if (suffix[i].flag == 1) {
				//get a operator
				Npop(s, suffix_right);
				right = suffix_right.num;
				Npop(s, suffix_left);
				left = suffix_left.num;

				op = suffix[i].c;
				result = cal(left, right, op);
				if (result == -1) return -1;
				else if (result == -10000) return -10000;
				suffix_result.num = result;
				suffix_result.flag = 0;
				Npush(s, _suffix_result);
				i++;
			}//end else if

		}//end while

		Npop(s, suffix_result);
		result = suffix_result.num;
		break;
		//end case 0
	case 1:
		//the num includes fracs&integars
		while (i <= length) {
			init_suffix(suffix_result);
			if (suffix[i].flag == 0||suffix[i].flag == 2) {
				//get a num
				Npush(s, suffix[i]);
				i++;
			}//end if

			else if (suffix[i].flag == 1) {
				//get a operator
				Npop(s, suffix_right);
				
				Npop(s, suffix_left);
				

				op = suffix[i].c;
				result = cal(suffix_left, suffix_right, suffix_result, op);
				if (result == -10000) return -10000;//error
				else if (result == -1) return -1;//get <0
				Npush(s, suffix_result);
				i++;
			}//end else if

		}//end while

		Npop(s, _suffix_result);
		return 1;
		break;
		//end case 1

	case 2:

		break;
	}
	

	return result;
}//end calculate ,return int*********


void fileout() {
	if (path_exp.empty() || path_key.empty()) {
		return;
	}//end if
	int i;
	int exp_size = expressions.size();//length of file
	int keys_size = str_keys.size();
	ofstream ofn_exp, ofn_key;
	if (File_app == 0) {
		ofn_exp.open(path_exp, ios::out);
		ofn_key.open(path_key, ios::out);
		for (i = 0; i < exp_size; i++) {
			ofn_exp << expressions[i] << endl;
		}//end for ofn_exp
		for (i = 0; i < keys_size; i++) {
			ofn_key << str_keys[i] << endl;
		}//end for ofn_key
	}//end if not app
	else if (File_app == 1) {
		ofn_exp.open(path_exp, ios::app);
		ofn_key.open(path_key, ios::app);
		for (i = 0; i < exp_size; i++) {
			ofn_exp << expressions[i] << endl;
		}//end for ofn_exp
		for (i = 0; i < keys_size; i++) {
			ofn_key << str_key[i] << endl;
		}//end for ofn_key
	}//end else if need app

}//end fileout


void N_generate() {
	//N_generate:generate _row exp and key
	//set into expressions and 

	int key;
	int count = 0;
	if (N_gen_app == false) {
		expressions.clear();
		str_keys.clear();
	}//end if
	
	while (count<_row) {
		suffix_generator();
		key=calculate();
		if (key != -1 && key != -10000) {
			getnormalsuffix(key, count);
			expressions.push_back(expression);
			disp_key(key);
			cout << str_key << endl;
			str_keys.push_back(str_key);
			count++;
		}//end if
		
	}//end while
	if (File_out == 1) {
		fileout();
	}


}//end N_generate

void getexp(string &exps, string &keys) {
	int key;
	int temp = 0;
	do {
		suffix_generator();
		key = calculate();
	} while (key == -1 || key == -10000);
	
	getnormalsuffix(key, temp);
	exps = expression;
	disp_key(key);
	keys = str_key;

}//end getexp


void getexp_i(int index, string &exps, string &str_key) {
	int size_exp = expressions.size();
	int size_keys = str_keys.size();
	if (index < 0|| index >= size_exp || index >= size_keys) {
		return;
	}//end if err
	exps = expressions[index];
	str_key = str_keys[index];
}//end getexp_i

//Initialize _sufarr

void Initialize() {
	//Initialize the ptr array
	int i;
	for (i = 0; i < ROWSIZE; i++) {
		_sufarr[i] = (NNode *)malloc(sizeof(NNode));
		N_init_suffix(*(_sufarr[i]));
	}
}//end Initialize

//void Reset_Initial() {
//
//}

void Clear() {
	//Clear: delete the space of _sufarr
	int i;
	for (i = 0; i < ROWSIZE; i++) {
		if (_sufarr[i] != NULL) {
			free(_sufarr[i]);
		}//end if
	}//end for

}//end clear

//Clear _sufarr




void output(string Expression, int key, int NUM) {
	string dist = "D://result.txt";
	string dist1 = "D://key.txt";
	fstream ofn(dist, ios::app);
	fstream ofn_key(dist1, ios::app);
	ofn << NUM + 1 << ". " << Expression << " = " << endl;
	ofn_key << NUM + 1 << ". " << key << endl;
}//end output

string needbracket(string a, char c) {
	//suffix get char by char 
	if (c == '+' || c == '-') return a;
	int len = a.length();
	if (len < 3) return a;
	int flag = 0;
	for (int i = 0; i < len; i++) {
		if (a[i] == '+' || a[i] == '-') flag = 1;
		if (a[i] == ')') flag = 0;
	}
	if (flag == 1) {
		string c = "(";
		c += a;
		c += ")";
		return c;
	}
	else return a;
}//end needbracket


void getnormalsuffix( int key, int NUM) {

	stack<string> Expression;
	string a;
	string b;
	char lastop = 0;
	int flag = 0;
	for (int i = 1; i <= suffix[0].num; i++) {
		// if is a num/frac then change to string else change to mid
		if (suffix[i].flag == -1) break;
		else if (suffix[i].flag == 0) {
			Expression.push(to_string(suffix[i].num));
			flag++;
			//cout << suffix[i].num << ' ';
		}//end elseif
		else if (suffix[i].flag == 2) {
			//cout << suffix[i].frac[1] << '`' << suffix[i].frac[2] << '/' << suffix[i].frac[3] << ' ';
			string c;
			if (suffix[i].frac[1] != 0) {
				c += to_string(suffix[i].frac[1]);
				c += '`';
			}//end if
			c += to_string(suffix[i].frac[2]);
			c += '/';
			c += to_string(suffix[i].frac[3]);
			Expression.push(c);
			flag++;
		}//end elseif
		else {
			a = Expression.top();
			Expression.pop();
			b = Expression.top();
			Expression.pop();

			if (flag == 0) {
				a = needbracket(a, suffix[i].c);
				b = needbracket(b, suffix[i].c);
			}//end if
			else {
				if (flag == 1) {
					b = needbracket(b, suffix[i].c);
				}//end else
			}//end else	
			b += ' ';
			b += suffix[i].c;
			b += ' ';
			b += a;
			Expression.push(b);
			lastop = suffix[i].c;
			//cout << suffix[i].c << ' ';
			flag = 0;
		}//end else

	}//end for
	
	expression=Expression.top();
	//output(a, key, NUM);
	cout << endl << expression << "    " << key << endl;

}//end getnormalsuffix

void disp_key(int key) {

	if (key == -1 || key == -10000) {
		return;
	}
	str_key.clear();
	if (_suffix_result.flag == 0) {
		str_key.append(to_string(_suffix_result.num));
	}//end if
	else if (_suffix_result.flag == 2) {
		str_key.append(to_string(_suffix_result.frac[1]));
		str_key.append("~");
		str_key.append(to_string(_suffix_result.frac[2]));
		str_key.append("/");
		str_key.append(to_string(_suffix_result.frac[3]));

	}//end eles if
	else if (_suffix_result.flag==3) {
		str_key.append(to_string(_suffix_result.num));
	}//end else if double
} //end disp_key


int main()
{
	Nsqstack s;
	Ninitstack(s);
	/*Node suffixq,suffix1;
	init_suffix(suffixq);
	init_suffix(suffix1);
	set_suffix(suffixq, 1, 10, 2, "+-*");
	Npush(s, suffixq);
	Ngettop(s, suffix1);
	cout << suffix1.frac[1] << "`" << suffix1.frac[2] <<"/" << suffix1.frac[3] << endl;
	cout << suffixq.frac[1] << "`" << suffixq.frac[2] <<"/" << suffixq.frac[3] << endl;*/
	//testing stack
	
	
	
	
	Node suffix_result;
	int i = 0;
	int key = 0;//key is the result of suffix ******* change to node
	string dist = "C:/Users/38084/Documents/Visual Studio 2015/Projects/jiedui/jiedui/result.txt";
	string dist1 = "C:/Users/38084/Documents/Visual Studio 2015/Projects/jiedui/jiedui/key.txt";
	


	srand((unsigned)time(NULL));
	
	init_suffix(suffix_result);
	for (i = 0; i < NUMSIZE; i++) {
		init_suffix(suffix[i]);
	}//end for i

	N_generate();
	string a, b;
	getexp_i(49, a, b);
	cout << a << '	' << b << endl;
	getexp(a, b);
	cout << a << '	' << b << endl;
	

	fstream ofn(dist,ios::out);
	fstream ofn_key(dist1, ios::out);// change to output and change api with path 

	for (int j = 0; j < 50; j++) {
		suffix_generator();
		key = calculate();

		getnormalsuffix( key, j);

		disp_key(key);
		cout << str_key << endl;
		str_key.clear();
		//for (i = 1; i <= suffix[0].num; i++) {
		//	{
		//		if (suffix[i].flag == -1) break;//****************securrity checkpoint 
		//		else if (suffix[i].flag == 0) {

		//			ofn << suffix[i].num << ' ';
		//			cout << suffix[i].num << ' ';
		//		}//end elseif
		//		else if (suffix[i].flag == 2) {
		//			ofn << suffix[i].frac[1] << '`' << suffix[i].frac[2] << '/' << suffix[i].frac[3] << ' ';
		//			cout << suffix[i].frac[1] << '`' << suffix[i].frac[2] << '/' << suffix[i].frac[3] << ' ';

		//		}
		//		else {
		//			ofn << suffix[i].c << ' ';
		//			cout << suffix[i].c << ' ';
		//		}//end else
		//	}
		//	

		//}//end for j
		
		/*ofn << j + 1 << "	"<<"answer:	";
		cout << j + 1 << "	" << "answer:	";
		
		if (key == -10000) {
			ofn_key << key << endl;
			cout << key << endl;
		}
		else if (key == -1) {
			ofn_key << key << endl;
			cout <<  key << endl;
		}
		else {
			if (_suffix_result.flag == 2) {
				ofn << _suffix_result.frac[1] << '`' << _suffix_result.frac[2] << '/' << _suffix_result.frac[3] << ' ';
				cout << _suffix_result.frac[1] << '`' << _suffix_result.frac[2] << '/' << _suffix_result.frac[3] << ' ';
			}
			else if (_suffix_result.flag == 0) {
				ofn << _suffix_result.num << ' ';
				cout << _suffix_result.num << ' ';
			}
		}
		cout << endl;
		ofn << endl;*/
		

	}//end for i
	system("pause");
	return 0;

}//end main

