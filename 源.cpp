

#include<vector>
#include<map>
#include<set>
#include <algorithm>
#include<string>
#include<fstream> 
#include<stack>
#include<cstring>
#include<iostream>
using namespace std;

#pragma warning(disable:4996)
#define MAX 50

/*
文法 非终结符类
成员： 非终结符，可推导式/文法产生式右侧
*/
class VN {
public:
	string str;
	vector<string> right;//文法产生式右侧
	VN(const string&s)
	{
		str = s;
	}
	void insert(char s[])
	{
		right.push_back(s);
	}
	void print()
	{
		printf("%s->%s", str.c_str(), right[0].c_str());
		for (int i = 1; i < right.size(); i++)
			printf("|%s", right[i].c_str());
		puts("");
	}

};

vector<VN> VN_set;//非终结符表
set<char> VT_set;//终结符集合
map<string, int >VN_dic;//非终结符字典 

map<char, int> VT_dic;

bool check(char buffer[])
{
	int len = strlen(buffer);
	for (int i = 0; i < len - 1; i++)
	{
		if (isupper(buffer[i]) && isupper(buffer[i + 1]))
			return false;
	}
	return true;
}


bool visited[MAX];
set<char> first[MAX];//FIRSTVT集
set<char> last[MAX];//LASTVT集
stack<string >tree;
void FVT(int i)
{
	if (visited[i])
		return;
	visited[i] = true;
	for (int k = 0; k < VN_set[i].right.size(); k++)
	{
		string str = VN_set[i].right[k];
		if (isupper(str[0]))
		{
			string s = str.substr(0, 1);
			int x = VN_dic[s] - 1;
			if (str.length() > 1 && !isupper(str[1]))
				first[i].insert(str[1]);
			FVT(x);
			set<char> ::iterator it = first[x].begin();
			for (; it != first[x].end(); it++)
				first[i].insert(*it);
		}
		else
		{
			first[i].insert(str[0]);
		}
	}
}
void FirstVT()
{
	memset(visited, false, MAX);
	for (int i = 0; i < VN_set.size(); i++)
	{
		if (visited[i])
			continue;
		FVT(i);
	}
	puts("************FIRSTVT*******************");
	for (int i = 0; i < VN_set.size(); i++)
	{
		cout << VN_set[i].str << "   ";

		set<char> ::iterator it = first[i].begin();
		for (; it != first[i].end(); it++)
			cout << *it << " ";
		puts("");
	}
}
void LVT(int i)
{
	if (visited[i])
		return;
	visited[i] = true;
	for (int k = 0; k < VN_set[i].right.size(); k++)
	{
		string str = VN_set[i].right[k];
		int len = str.length() - 1;
		if (isupper(str[len]))
		{
			string s = str.substr(len, 1);
			int x = VN_dic[s] - 1;
			if (str.length() > 1 && !isupper(str[len - 1]))
				last[i].insert(str[len - 1]);
			LVT(x);
			set<char> ::iterator it = last[x].begin();
			for (; it != last[x].end(); it++)
				last[i].insert(*it);
		}
		else
		{
			last[i].insert(str[len]);
		}

	}
}
void LastVT()
{
	memset(visited, false, MAX);
	for (int i = 0; i < VN_set.size(); i++)
	{
		if (visited[i])
			continue;
		LVT(i);
	}
	puts("************LASTVT*******************");
	for (int i = 0; i < VN_set.size(); i++)
	{
		cout << VN_set[i].str << "  ";
		set<char> ::iterator it = last[i].begin();
		for (; it != last[i].end(); it++)
			cout << *it << " ";
		puts("");
	}
}

char R[MAX][MAX];

void Table()
{
	//map<char, int> VT_dic;
	set<char>::iterator it = VT_set.begin();
	int k = 0;
	for (; it != VT_set.end(); it++)
	{
		VT_dic[*it] = k;
		k++;
	}
	for (int i = 0; i < MAX; i++)
		memset(R[i], 32, MAX);
	for (int i = 0; i < VN_set.size(); i++)
	{
		for (int j = 0; j < VN_set[i].right.size(); j++)
		{
			//int x=VN_dic[VN_]
			string test = VN_set[i].right[j];
			//cout << test << "----->" << endl;
			int len = test.length();
			for (k = 0; k < len - 1; k++)
			{
				//aa x[k]=x[k+1]
			
				if (!isupper(test[k])&&!isupper(test[k + 1]))
				{

					if (R[VT_dic[test[k]]][test[k + 1]] == '>' || R[VT_dic[test[k]]][test[k + 1]] == '<')
					{
						cout << "文法非算符优先文法" << endl;
						exit(-1);
					}
					R[VT_dic[test[k]]][test[k + 1]] = '=';

				}
				//aA  x[k]<FIRST[A]
				else if (!isupper(test[k])&&isupper(test[k + 1]))
				{
					string s = test.substr(k + 1, 1);
					int x = VN_dic[s]-1;
					//cout << s << ":"<<x << endl;
					set<char>::iterator iter = first[x].begin();
					for (; iter != first[x].end(); iter++)
					{
						if (R[VT_dic[test[k]]][VT_dic[*iter]] == '>' || R[VT_dic[test[k]]][VT_dic[*iter]] == '=')
						{
							cout << "文法非算符优先文法" << endl;
							exit(-1);
						}
						R[VT_dic[test[k]]][VT_dic[*iter]] = '<';

					}

				}
				//Aa  LASTVT[A]>a
				else if (isupper(test[k])&&!isupper(test[k + 1]))
				{
					//cout << "C!!!!!!!!!" << endl;
					string s = test.substr(k, 1);
					
					int x = VN_dic[s]-1;
					//cout << s << "mark"<<x<<last[x].size();
					set<char>::iterator iter = last[x].begin();

					for (; iter != last[x].end(); iter++)
					{
						if (R[VT_dic[*iter]][test[k + 1]] == '<' || R[VT_dic[*iter]][test[k + 1]] == '=')
						{
							cout << "文法非算符优先文法" << endl;
							exit(-1);
						}
						//cout << *iter<<"-!-";
						R[VT_dic[*iter]][VT_dic[test[k + 1]]] = '>';
					}

					if (k > 0 && !isupper(test[k - 1]))
					{
						if (R[VT_dic[test[k - 1]]][test[k + 1]] == '>' || R[VT_dic[test[k - 1]]][test[k + 1]] == '<')
						{
							cout << "文法非算符优先文法" << endl;
							exit(-1);
						}
						R[VT_dic[test[k - 1]]][VT_dic[test[k + 1]]] = '=';
					}


				}
				//cout << k << "---next" << test[k] << endl;
			}

		}


	}
	puts("*************算符优先表*****************");
	it = VT_set.begin();
	cout << " " << "   ";
	for (; it != VT_set.end(); it++)
	{
		cout << *it << "   ";
	}
	cout << endl;
	it = VT_set.begin();

	int len0 = VT_set.size();
	for (; it != VT_set.end(); it++)
	{
		cout << *it << "   ";
		for (int i = 0; i < len0; i++)
		{
		
			cout << R[VT_dic[*it]][i] << "   ";
		}
		cout << endl;
	}
}

string get_stack(vector<char>& stk)
{
	vector<char>::iterator it = stk.begin();
	string s = "";
	for (; it != stk.end(); it++)
	{
		s += *it;
	}
	return s;
}
/*
匹配两产生式是否相等
*/
bool isequal(string &s1, string &s2)
{
	if (s1.length() != s2.length())
		return false;
	for (int i = 0; i < s1.length(); i++)
	{
		if (isupper(s1[i]))
		{
			if (isupper(s2[i]))
				continue;
			return false;
		}
		else
		{
			if (s1[i] != s2[i])
				return false;
		}
	}
	return true;
}
/*
匹配待规约子串的对应非终结符
*/
string Find(string & str)
{
	for (int i = 0; i < VN_set.size(); i++)
	{
		string test;
		for (int j = 0; j < VN_set[i].right.size(); j++)
		{
			test = VN_set[i].right[j];
			if (isequal(test, str))
				return "N";
		}
	}
	return "";
}

/*
算符优先分析

*/
void analysis(string& s)
{

	int step = 1;
	vector<char> stk;
	
	//tree.push(s);
	stk.push_back('#');
	for (int i = 0; i<s.length(); i++)
	{
		int len = stk.size();
		//符号栈内首个终结符
		char top = stk[len - 1];
		for (int j = len - 1; j >= 0; j--)
			if (isupper(stk[j]))
				continue;
			else
			{
				top = stk[j];
				break;
			}
		//cout << top << "!!!!!!!!!!" << endl;
		//R[VT_dic[stk[]]
		if (top == '#')
		{
			char r = s[i] == '#' ? '=' : '<';
			if (i == s.length() - 1)
				printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], " ", "移进");
			else
				printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], s.substr(i + 1).c_str(), "移进");

			stk.push_back(s[i]);

		}
		else
		{
		
			if (s[i] != '#'&&!VT_set.count(s[i]))
			{
				cout << s[i]<<"---->文法无法识别的字符" << endl;
				exit(-1);
			}
			char r = s[i] == '#' ? '>' : R[VT_dic[top]][VT_dic[s[i]]];
			
			//char r = R[VT_dic[top]][VT_dic[s[i]]];
			//cout << top << "   " <<s[i]<< endl;
			//移进动作
			if (r == '<' || r == '=')
			{
				if (i == s.length() - 1)
					printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], " ", "移进");
				else
					printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], s.substr(i + 1).c_str(), "移进");

				stk.push_back(s[i]);
			}
			//规约
			else
			{
				if (r == 32)
				{
					cout << top<<"---"<<s[i]<<"--->文法无法处理" << endl;
					exit(-1);
				}
				if (i == s.length() - 1)
					printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], " ", "规约");
				else
					printf("%-14d|%-15s%-15c%-15c%-15s%-15s\n", step, get_stack(stk).c_str(), r, s[i], s.substr(i + 1).c_str(), "规约");
				int stk_x = stk.size() - 2;
				while (true)
				{
					if (stk.empty())
						break;
					if (!isupper(stk[stk_x]) )
					{
						if (R[VT_dic[stk[stk_x]]][VT_dic[top]] == '=')
							top = stk[stk_x];
						else if (R[VT_dic[stk[stk_x]]][VT_dic[top]] == '<')
							break;
					}
						
					stk_x--;
				}
				stack<char> temp;
				for (int j = stk.size() - 1; j > stk_x; j--)
				{
					temp.push(stk[j]);
					stk.pop_back();
				}
				string str = "";//待规约字符
				
				while (!temp.empty())
				{
					str += temp.top();
					temp.pop();
				}
				string  s0 = get_stack(stk);
				string t0;
				if (!tree.empty())
				{
					t0 = tree.top();
					if(t0!=(s0+str))
						tree.push(s0 + str);
				}
				else
				   tree.push(s0+str);
				//cout << str << endl;

				str= Find(str);
			//	cout << str << endl;
				for (int m = 0; m < str.length(); m++)
					stk.push_back(str[m]);
				//t0 = tree.top();
				
				tree.push(s0 + str);
				i--;
			}

		}
		//stk.push_back(s[i]);
		//cout << i << endl;
		step++;
	}

}

/*
生成语法分析树
*/
void showtree()
{
	string t1, t2;
	if (!tree.empty())
	{
		t1 = tree.top();
		tree.pop();
		printf("%-20c", 32);
		for (int i = 0; i < t1.length(); i++)
		{
			if (!isupper(t1[i]))
				printf("%-5c", 32);
			else
				printf("%-5c", t1[i]);
		}
		cout << endl;
	}
	while (!tree.empty())
	{
		t2 = tree.top();
		
		if (t1.length() <= t2.length())
		{
			int i;
			printf("%-20c", 32);
			for (i = 0; i < t1.length(); i++)
			{
				if (!isupper(t1[i]) && t1[i] == t2[i])
					printf("%-5c", 32);
				else
					printf("%-5c", '+');
			}
			for (; i < t2.length(); i++)
				printf("%-5c",'+');
			cout << endl;
			printf("%-20c", 32);
			int j;
			for (j = 0; j < t1.length(); j++)
			{
				if (!isupper(t1[j]) && t1[j] == t2[j])
					printf("%-5c", 32);
				else
					printf("%-5c", t2[j]);
			}
			for (; j < t2.length(); j++)
				printf("%-5c", t2[j]);
			cout << endl;
		}
		else
		{
			int i;
			printf("%-20c", 32);
			for (i = 0; i < t2.length(); i++)
			{
				if (!isupper(t1[i]) && t1[i] == t2[i])
					printf("%-5c", 32);
				else
					printf("%-5c", '+');
			}
		
			cout << endl;
			printf("%-20c", 32);
			int j;
			for (j = 0; j < t2.length(); j++)
			{
				if (!isupper(t1[j]) && t1[j] == t2[j])
					printf("%-5c", 32);
				else
					printf("%-5c" ,t2[j]);
			}
			
			cout << endl;


		}
		t1 = t2;
		tree.pop();
	}
}
int main()
{
	char infile[40];
	cout << "请输入文法文件的文件名" << endl;
	cin >> infile;
	ifstream in;
	in.open(infile, ios::in);
	if (!in.is_open())
	{
		cout << "can't open the file" << endl;
		in.close();
		exit(-1);
	}
	char buffer[255];
	while (in.getline(buffer, 100))
	{
		if (!check(buffer))
		{
			cout << "文法非算符文法！" << endl;
			exit(-1);
		}
		int len = strlen(buffer);
		int i;
		for (i = 0; i < len; i++)
			if (buffer[i] == '-')
				break;
		//cout << buffer[i] << endl;
		buffer[i] = 0;
		if (!VN_dic[buffer])
		{
			VN_set.push_back(VN(buffer));
			VN_dic[buffer] = VN_set.size();
			//cout << VN_dic[buffer] << endl;
		}
		int x = VN_dic[buffer] - 1;
	
		for (int k = 0; k < i; k++)
		{
			if (!isupper(buffer[k]) && buffer[k] != '|')
			{
				
				VT_set.insert(buffer[k]);
			}
		}
		for (int k = i + 2; k < len; k++)
		{
			if (!isupper(buffer[k]) && buffer[k] != '|')
			{
				
				VT_set.insert(buffer[k]);
				
			}
		}
		char temp[255] = { '\0' };
		//cout << buffer + i + 2 << "******" << sizeof(buffer + i + 2)<<endl;

		memcpy(temp, buffer + i + 2, strlen(buffer + i + 2));
		char demin[] = "|";
		//cout << temp << endl;
		char *pch = NULL;//存放分割字符串
		pch = strtok(temp, demin);
		//cout << pch << endl;
		while (pch != NULL)
		{
			VN_set[x].insert(pch);
			//cout << pch << endl;
			pch = strtok(NULL, demin);
		}


	}
	in.close();


	cout << "************VT集*******************" << endl;
	set<char >::iterator it = VT_set.begin();
	for (; it != VT_set.end(); it++)
		printf("%c ", *it);
	puts("");
	cout<<"*************产生式*****************"<<endl;
	for (int i = 0; i < VN_set.size(); i++)
		VN_set[i].print();
	cout<<"************************************"<<endl;

	FirstVT();
	LastVT();

	Table();
	string str_test;
	cout << "请输入待分析的字符串" << endl;
	cin >> str_test;
	str_test.append("#");
	analysis(str_test);
	showtree();

	return 0;
}



