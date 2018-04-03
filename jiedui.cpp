// jiedui.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <math.h>
#include <cstdlib>
#include <ctime>


#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

//#include<string.h>


#define random(a,b) (rand()%(b-a+1)+a)
#define NUMSIZE 50
#define STACKSIZE 50
#define GEN 100

using namespace std;



//definition of Node
typedef struct node {
	//the node of string
	char c;
	int num;
	int flag;
	int frac[4];

}Node, NNode[NUMSIZE];

//end definition

void init_suffix(Node& suffix);

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



int isoperator(char ch) {
	//judge if a char is operator or not
	int m = 0;
	switch (ch) {
	case '#':
	case 'q':
	case 'l'://ln
	case ')':
	case 't'://tan
	case 'a':
	case '^':
	case 'c'://cos
	case 's'://sin
	case '(':
	case '+':
	case '-':
	case '*':
	case '/': m = 1; break;
	default:m = 0;

	}
	return m;
}

int cal(int a, int b, char ch) {
	//function of calculation
	int c = 0;
	switch (ch) {
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a*b;
	case '/':return a / b;// ********
	case '^':return pow(a, b);//********
	}
}//end cal
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
			return 1;
		}//end if '+'
		else if (op == '*') {
			suffix_result.num = suffix_left.num * suffix_right.num;
			suffix_result.flag = 0;
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
			if (suffix_left.num < suffix_right.num) {
				temp = gcd(suffix_left.num, suffix_right.num);
				if (temp != 1) {
					suffix_left.num  /= temp;
					suffix_right.num /= temp;
				}//if has a gcd larger than 1
				suffix_result.frac[1] = 0;
				suffix_result.frac[2] = suffix_left.num;
				suffix_result.frac[3] = suffix_right.num;
				suffix_result.flag = 2;

				return 2;
			}//end if '/' get frac<1
			else if (suffix_left.num == suffix_right.num) {
				suffix_result.num = 1;
				suffix_result.flag = 0;
				return 1;
			}//end else if '/' get 1
			else {
				para = suffix_left.num % suffix_right.num;
				if (para == 0) {
					//can get a intergar
					suffix_result.num = suffix_left.num / suffix_right.num;
					suffix_result.flag = 0;
					return 1;
				}//end if is a >1 integar
				else {
					temp = suffix_left.num - para;
					temp = temp / suffix_right.num;
					suffix_result.frac[1] = temp;
					suffix_left.num = suffix_left.num - temp*suffix_right.num;
					
					temp = gcd(suffix_left.num, suffix_right.num);
					if (temp != 1) {
						suffix_left.num /= temp;
						suffix_right.num /= temp;
					}//if has a gcd larger than 1
					suffix_result.frac[2] = suffix_left.num;
					suffix_result.frac[3] = suffix_right.num;
					suffix_result.flag = 2;
					suffix_result.flag = 0;
					return 1;

				}//end else is a >1 fraction
				
			}//end else '/' get >1 num&frac
		}//end else if '/'

	}//end if two integar
	else if (flag_l == 2 && flag_r == 0|| flag_l == 0 && flag_r == 2|| flag_l == 2 && flag_r == 2) {


	}


}//end cal with Node



void getf(char *&p, char *q) {
	//capture the number(able to get double)
	//46 is '.'
	char *temp = q;
	int k = 1;
	if (*p<48 && *p>57) return;
	while (*p == 46 || *p >= 48 && *p <= 57) {
		*temp = *p;
		p++, temp++;
	}

	*temp = '\0';
}//end getf



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



int preop(char a, char b) {
	//-1,0,1 means the priority of a is smaller/equal_to/larger than b
	//-2 means error
	if (a == '+' || a == '-') {
		if (b == '*' || b == '/' || b == '(' || b == '^') return -1;
		else return 1;
	}
	if (a == '*' || a == '/') {
		if (b == '(' || b == '^') return -1;
		else return 1;
	}
	if (a == '(') {
		if (b == ')') return 0;
		else if (b == '#') return -2;
		else return -1;
	}
	if (a == ')') {
		if (b == '(')return -2;
		else return 1;
	}
	if (a == '^') {
		if (b == '(') return -1;
		else return 1;
	}
	if (a == '#') {
		if (b == ')') return -2;
		else if (b == '#') return 0;
		else return -1;
	}
}//end preop

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




void suffix_generator(NNode &suffix,char* oper,int symbol) {
	//generate the suffix erpression

	int i = 0, j = 0, k = 0;// invariable of count
	bool flag = 0;			// flag of generating operator or num

	int sup = 20;// the superior bound of the num
	int inf = 1; // the inferior bound of the num

	int count_num  = 0; //the number of num in the experssion
	int count_oper = 0; //the number of operator in the experssion

	int inf_len = 5;//the inferior bound of the length of expression
	int sup_len = 9;//the superior bound of the length of expression
	
	int len_oper = strlen(oper);		 //the length of the operator set
	int length = random(inf_len,sup_len);//the length of the exp,randomized

	//end definition

	if (length % 2 == 0) length++;
	
	//initialization
	suffix[0].flag = 3;    //3 ************* 3 means begin
	suffix[0].num = length;//store the length
	//end initialization


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
				//if there is almost done��then num should not be the last
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
				//if there is almost done��then num should not be the last
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
		//����С��****************8

		break;

	}


	while (count_oper < (length - 1) / 2) {
		//fill the last operator
		suffix[i].flag = 1;
		suffix[i++].c = oper[random(0, len_oper - 1)];
		count_oper++;

	}//end while
	suffix[i].flag = -1;// not need

}//end suffix_generator

void node_out_file(fstream filename, Node suffix) {
	//f_out a node to the file
	if (suffix.flag == -1) return;
	else if (suffix.flag = 0) {
		filename << suffix.num << ' ';
	}//end elseif
	else {
		filename << suffix.c << ' ';
	}//end else
}//end node_out_file




int calculate(NNode suffix,int symbol) {
	//calculate the suffix
	//symbol decides the type of calculation
	//δ���Ƿ�����С�������������������
	//δ�����м���ָ���
	Nsqstack s;
	Ninitstack(s);
	int length = suffix[0].num;//the length of exp
	int len_num = (length + 1) / 2;
	int len_oper = (length - 1) / 2;

	if (length >= 3) {
		Npush(s, suffix[1]);
		Npush(s, suffix[2]);
	}
	else return -10000;

	Node suffix_left, suffix_right, suffix_result;
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
				suffix_result.num = result;
				suffix_result.flag = 0;
				Npush(s, suffix_result);
				i++;
			}//end else if

		}//end while

		Npop(s, suffix_result);
		result = suffix_result.num;
		break;
		//end case 0
	case 1:
		//the num includes fracs&integars




		break;
	case 2:

		break;
	}

	return result;
}//end calculate ,return int*********



int main()
{
	Nsqstack s;
	Ninitstack(s);
	Node suffixq,suffix1;
	init_suffix(suffixq);
	init_suffix(suffix1);
	set_suffix(suffixq, 1, 10, 2, "+-*");
	Npush(s, suffixq);
	Ngettop(s, suffix1);
	cout << suffix1.frac[1] << "`" << suffix1.frac[2] <<"/" << suffix1.frac[3] << endl;
	cout << suffixq.frac[1] << "`" << suffixq.frac[2] <<"/" << suffixq.frac[3] << endl;
	
	
	
	
	NNode suffix;
	int i = 0;
	int key = 0;//key is the result of suffix
	string dist = "C:/Users/38084/Documents/Visual Studio 2015/Projects/jiedui/jiedui/result.txt";
	string dist1 = "C:/Users/38084/Documents/Visual Studio 2015/Projects/jiedui/jiedui/key.txt";
	
	srand((unsigned)time(NULL));
	char* oper = "+-*";
	for (i = 0; i < NUMSIZE; i++) {
		init_suffix(suffix[i]);
	}//end for i
	fstream ofn(dist,ios::out);
	fstream ofn_key(dist1, ios::out);

	for (int j = 0; j < 100; j++) {
		suffix_generator(suffix, oper,0);
		ofn << j + 1 << "	";
		//key=calculate(suffix);
		//ofn_key << j + 1 << "	" << key << endl;
		for (i = 1; i <= suffix[0].num; i++) {
			if (suffix[i].flag == -1) break;
			else if (suffix[i].flag == 0) {

				ofn << suffix[i].num << ' ';
				cout << suffix[i].num << ' ';
			}//end elseif
			else if (suffix[i].flag == 2) {
				ofn << suffix[i].frac[1] <<'`'<<suffix[i].frac[2]<<'/'<<suffix[i].frac[3]<< ' ';
				cout << suffix[i].frac[1] << '`' << suffix[i].frac[2] << '/' << suffix[i].frac[3] << ' ';

			}
			else {
				ofn << suffix[i].c << ' ';
				cout << suffix[i].c << ' ';
			}//end else
			
		}//end for j
		cout << endl;
		ofn << endl;

	}//end for i
	system("pause");
	return 0;

}//end main

