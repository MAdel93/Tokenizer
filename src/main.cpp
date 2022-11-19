#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <regex>
#include <omp.h>
using namespace std;
queue <string> q;

void open_file (int nthreads);
void Tokenize (int nthreads);

int main()
{
	
	int nthreads=4;
	auto s_time = omp_get_wtime();
	open_file(nthreads);
	cout << "The size of queue is: " << q.size() << "\n";
	Tokenize (nthreads);
	auto e_time = omp_get_wtime();
	cout << "The execution time is: " << e_time-s_time << "\n";

	
}

void open_file(int nthreads){
	ifstream fin;
	string line, A = "text", C = ".txt";
	omp_set_num_threads(nthreads);
    #pragma omp parallel private (fin)
	{
		int id = omp_get_thread_num();
		//nthrds = omp_get_num_threads();
		for (int i = id; i <= 8; i=i+nthreads)
		{
			string address = A + to_string(i) + C;
			// cout<< address<<"\n";
			fin.open(address);
			if (fin.is_open())
			{
				while (getline(fin, line))
				{
					stringstream ss(line);
					#pragma omp critical
						q.push(ss.str());
				// cout<<ss.str()<<"\n";
				}
			}
			fin.close();
		}
	}
}

void Tokenize(int nthreads){
	string s, str;
	int CL=0;
	vector<string> tokens;
	omp_set_num_threads(nthreads);
    #pragma omp parallel firstprivate (str,s)
	{
		int id = omp_get_thread_num();
		//#pragma omp parallel for
		for (int i=id;!q.empty();i=i+nthreads)
		{
			#pragma omp critical
			{
				CL=CL+1;
				//cout << "itteration is:" <<CL << endl;
				str = q.front();
				q.pop();
			}
			regex reg("\\s+");
 
    		sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    		sregex_token_iterator end;
 
    		vector<string> tokens(iter, end);
			#pragma omp critical
			{
				cout << str << "\n";
				for (const string &s: tokens) {
        			cout << s << endl;
    			}
			}
		}
	}
	cout << "number of itteration is:" <<CL << endl;
}




// For serial program
	/*while (!q.empty())
	{
		CL=CL+1;
		str = q.front();
		q.pop();
		
		regex reg("\\s+");
 
    	sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    	sregex_token_iterator end;
 
    	vector<string> tokens(iter, end);
		cout << str << "\n";
		for (const string &s: tokens) {
        	cout << s << endl;			
		}
	}
	cout << "number of itteration is:" <<CL << endl;*/