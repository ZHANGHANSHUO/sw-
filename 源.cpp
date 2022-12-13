#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm> 
#include<stdlib.h>
#include<memory.h>
using namespace std;
#pragma warning(default:4716)


string animal[] = { "Penguin","Swallow","Ostrich","Zebra","Giraffe","Tiger","Panther" };

string feature[] = { "hairy","milk","feathery","fly","lay eggs","eat meat","canine teeth","claws","Eyes in front","hoofed","ruminate","yellowish"," dark spot",

//                              0         1         2        3            4             5                 6                7               8              9            10             11            12

	 "black-striped","long neck","long legs","Can't fly","swimming","black and white","fly well","mammals","bird","meat","hooves",

	//       13                  14              15           16            17               18                    19                20      21      22      23    

 "Penguin","Swallow","Ostrich","Zebra","Giraffe","Tiger","Panther" };


//     24     25     26    27       28      29    30
typedef struct {  
	int relation[6];   
	int name;		    
}Rule;

typedef struct { 
	int animal;        
	float confidence; 
	int site;         
	int num;           
	int size;          
}Result;

vector<Result> result;
 
Rule rule[15] = { {{0,-1},20},{{1,-1},20},{{2,-1},21},{{3,4,-1},21},{{5,-1},22},
		{{6,7,8,-1},22},{{20,9,-1},23},{{20,10,-1},23},{{20,22,11,12,-1},30},
		{{20,22,11,13,-1},29},{{23,14,15,12,-1},28},{{23,13,-1},27},
		{{21,14,15,16,18,-1},26},{{21,19,-1},25},{{21,17,18,16,-1},24} };

int flag[23] = { 0 };

int IsAnimal(int a);
int change_speices();                                                             
int fnum();                                                                   
int z_inference();                                                             
int category();                                                         
int cal_confi();                                                         
int r_inference();                                                           
void input();                                                            
void menu();                                                                
bool Compare(const Result& a, const Result& b) {
	return a.confidence > b.confidence;
}
void Rsort(vector<Result>& r) {
	sort(r.begin(), r.end(), Compare);
	return;
}

void menu() {
	for (int i = 0; i < 24; i++) {
		if (i % 4 == 0 && i != 0)
			cout << endl;
		cout << setiosflags(ios::left) << setw(3) << i << ".";
		cout << setiosflags(ios::left) << setw(15) << feature[i];
	}
	memset(flag, 0, sizeof(flag));
}

void input() {
	for (int i = 0; i < 24; i++)
		flag[i] = 0;
	int  ti = 0;
	cout << "\ninput selection(end -1):";
	while (ti != -1) {
		cin >> ti;
		if (ti >= 0 && ti <= 23)
			flag[ti] = 1;
		else if (ti != -1) {
			cout << "Input error! Please enter a number between 0~23!" << endl; 
			cin.clear();		
			cin.sync(); 		
			cout << "Please continue to enter: ";
		}
	}
}
 
int IsAnimal(int a) {
	if (a >= 24 && a <= 30)
		return 1;
	return 0;
}

int IsAnimal_speices(int a) {
	if (a >= 20 && a <= 23)
		return 1;
	return 0;
}
 
int fnum() {
	int fum = 0;
	for (int i = 0; i < 24; i++)
		if (flag[i] == 1)
			fum++;
	return fum;
}

int category() {
	bool k;
	int count = 0;
	for (int i = 20; i < 24; i++) {
		k = false;
		if (flag[i] == 1) {
			k = true;
			count++;
			if (count == 1)
				cout << "Can't reason about specific animals! Category is ";
			cout << setiosflags(ios::left) << setw(10) << feature[i];
		}
	}
	cout << endl;
	if (!k)
		cout << "Sorry! No such animal in the system" << endl;
	return 1;
}

// change_speices --》 flag 发生变化  推理是否有 物种种类 并将用到的事实 清空
//如 有毛 --》哺乳动物  就将flag中哺乳动物的项置一 并将有毛这一特征flag清0
int change_speices() {
	int i, j, k, ti;
	bool t;
	int temp[23] = { 0 }; 
	int f[23] = { 0 };  
	for (i = 0; i < 8; i++) {
		t = true;
		j = 0;
		ti = rule[i].relation[j];
		while (ti != -1) {
			if (flag[ti] == 1) temp[ti] = 1;
			else {
				memset(temp, 0, sizeof(temp));
				t = false;
				break;
			}
			j++;
			ti = rule[i].relation[j];
		}
		if (t) {
			for (int k = 0; k <= 20; k++)
				if (temp[k] == 1)
					f[k] = 1;
			flag[rule[i].name] = 1;
		}
		memset(temp, 0, sizeof(temp));
	}
	
	for (i = 0; i <= 20; i++)
		if (f[i] == 1)
			flag[i] = 0;
	return 1;
}
 
int cal_confi() {
	for (int i = 0; i < result.size(); i++) {
		for (int j = 8; j < 15; j++) {
			if (result[i].animal == rule[j].name) {
				result[i].confidence = 1.0 * result[i].num / result[i].size;
				break;
			}
		}
	}
}


int z_inference() {
	int ti, num;
	int i, j;
	int fum = fnum();
	cout << endl;
	for (i = 8; i < 15; i++) {   
		Result temp;
		j = 0; num = 0;
		ti = rule[i].relation[j];
		while (ti != -1) {
			if (flag[ti] == 1) num++;
			j++;
			ti = rule[i].relation[j];
		}
		
		if (num != 0 && fum <= j) {  
			if (IsAnimal(rule[i].name)) { 
				temp.animal = rule[i].name;
				int size = j; 
				temp.size = size;
				temp.confidence = 1.0 * num / size;
				temp.site = i;
				temp.num = num;
				result.push_back(temp);
			}
		}
	}

	if (!result.empty())
		Rsort(result);  

	/*
	//打印排序后的vector
	for(vector<Result>::iterator it = result.begin();it != result.end();++it){
		cout<<setiosflags(ios::left)<<setw(10)<<feature[(*it).animal]<<" ";
		cout<<(*it).confidence<<" ";
	}
	cout<<endl;
	*/
	
	if (result.empty()) {   
		category();
	}
	else if (result.front().confidence == 1.0) { 
		cout << "This animal is " << feature[result.front().animal] << endl;
		result.clear(); 
		return 1;
	}
	else 
		r_inference();
}

int r_inference() {
	vector<Result>::iterator it = result.begin();
	int enquire[23];  
	memset(enquire, -1, sizeof(enquire));

	for (int i = 0; i < result.size();) {                                                                                            
		bool in_i = true;                                                                                                                 
		int  nu = result[i].size;
		for (int j = 0; j < nu; j++) {                                                                                                        
			if (flag[rule[result[i].site].relation[j]] == 0) {
				int en = rule[result[i].site].relation[j];
				char c;
				if (enquire[en] == -1) {                                                                                                   
					cout << "Does this animal have this characteristic?" << feature[rule[result[i].site].relation[j]] << endl;
					cout << "Y(y) or N(n) or D(don't know) : ";
					cin >> c;
					while (c != 'Y' && c != 'y' && c != 'N' && c != 'n' && c != 'D' && c != 'd') {
						cout << "Please enter Y(y) or N(n) or D(d)!" << endl;
						cin >> c;
					};
				}

				if (enquire[en] == 1 || c == 'Y' || c == 'y') {                                                                                        
					result.erase(it + i);
				}                                                                                      
				else if (enquire[en] == 0 || c == 'N' || c == 'n') {                                                                                                        
					enquire[en] = 0;
					result.erase(it + i);                                                                                                        
					in_i = false;                                                                                                                          
					if (result.empty())                                                                                                     
						category();
					break;
				}
				else if (enquire[en] == 2 || c == 'D' || c == 'd') { enquire[en] = 2; }                                              
			}
		}
		if (in_i)
			++i;
	}

	if (!result.empty()) {
		 
		cal_confi();
		if (result.size() > 1) 
			Rsort(result);

		
		if (result.front().confidence == 1.0) {
			cout << "This animal is " << feature[result.front().animal] << endl;
		}
		else {
			cout << "Possible animals (confidence from big to small) :";
			for (vector<Result>::iterator it = result.begin(); it != result.end(); ++it)
				cout << setiosflags(ios::left) << setw(10) << feature[(*it).animal] << " ";
			cout << endl;
		}
		result.clear(); 
	}

	return 1;
}
int main() {
	char q = 'a';
	while (q != 'N' && q != 'n') 
	{
		menu();
		input();
		change_speices();
		z_inference();
		cout << "\n continue？(Y/N)" << endl;
		cin >> q;
		system("cls");
	}
	return 0;
}
