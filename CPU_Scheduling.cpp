/* OS Project02 */

#include <iostream>
#include <iomanip>
#include <thread>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <queue>

using namespace std ;

struct TokenType {
	int ID;
	double CPU_burst;
	int arrival_time;
	int priority;
	double left;
	bool done;
	double hrrn;
	int loc;
	bool check;
	int time_slice;
	int localloc;
	int waiting;
	int turnaround;
} ;

double Start, End;
time_t now = time(0); // t passed as argument in function time()

string token( int &first, int &i, string line ) {
	if ( i == 0 ) {
		first = 0;
	} // if
	else {
		first = i;
	} // else 
	
	int num = 0;
	while ( line[i] == '\t' || line[i] == ' ' ) {
		num++;
		i++;
	} // while
	
	while ( line[i] != '\t' && line[i] != '\0' && line[i] != ' ' ) {
		num++;
		i++;
	} // while
	
	if ( line[i] != '\0' ) {
		while ( line[i] == '\t' || line[i] == ' ' ) {
			num++;
			i++;
		} // while
	} // if

	return line.substr( first, num ) ; // 切token 
} // token()

void SaveFile( TokenType &job, string line ) {
	string temp ;
	int num = 0;
	int first = 0;
	
	temp = token( first, num, line );
	job.ID = atoi( temp.c_str() );
	
	temp = token( first, num, line );
	job.CPU_burst = atoi( temp.c_str() );
	
	temp = token( first, num, line );
	job.arrival_time = atoi( temp.c_str() );
	
	temp = token( first, num, line );
	job.priority = atoi( temp.c_str() );
	
	job.done = false;
	job.check = false;
	job.hrrn = 0;
	job.waiting = 0;
	job.turnaround = 0;
} // SaveFile()

void Swap( TokenType &token1, TokenType &token2 ) {
	TokenType temp;
	temp = token1;
	token1 = token2;
	token2 = temp;
} // Swap()

void LineUpAT( vector<TokenType> &list ) {
	for ( int i = 0 ; i < list.size() ; i++ ) {
		for ( int j = 0 ; j < list.size() - i - 1 ; j++ ) {
			if ( list[j].arrival_time > list[j+1].arrival_time ) {
				Swap( list[j], list[j+1] );
			} // if
			else if ( list[j].arrival_time == list[j+1].arrival_time ) {
				if ( list[j].ID > list[j+1].ID ) {
					Swap( list[j], list[j+1] );
				} // if
			} // else if
		} // for
	} // for
} // LineUpAT()

void FCFS( vector<TokenType> &list, vector<TokenType> &ans ) {
	/* 1. Arrival Time 
		2. PID (same arrival time)
	*/
	int totalTime;
	TokenType job;
	
	LineUpAT( list );
	for ( int i = 0 ; i < list.size() ; i++ ) {
		job = list[i];
		if ( job.arrival_time > totalTime ) {
			job.ID = 45;
			ans.push_back( job );
			totalTime++;
			i--;
		} // if
		else {
			totalTime = totalTime + list[i].CPU_burst;
			for ( int j = 0 ; j < list[i].CPU_burst ; j++ ) {
				job = list[i];
				ans.push_back( job );
			} // for
			
			list[i].turnaround = totalTime - list[i].arrival_time;
			list[i].waiting = list[i].turnaround - list[i].CPU_burst;
		} // else
	} // for
} // FCFS()

void RR( vector<TokenType> &list, int time_slice, vector<TokenType> &ans ) {
	/* 1. Arrival Time 先後次序 
		2. PID (min)  由小到大 
		 *timeout, 讓新來的先做
		 *process若提早結束，new process要有新的time slice 
	*/
	int finish = 0;
	int id = 0;
	int loc = 0;
	int order = 0;
	int totalTime = 0;
	TokenType job;
	queue<TokenType> queue;
	
	LineUpAT( list );
	
	for ( int i = 0 ; i < list.size() ; i++ ) {
		list[i].left = list[i].CPU_burst;
		list[i].loc = i;
	} // for
	
	list[0].check = true;
	job = list[0];
	queue.push( job );
	while ( finish != list.size() ) {
		job = queue.front();
		if ( job.arrival_time > totalTime ) {
			job.ID = 45;
			ans.push_back( job );
			totalTime++;
		} // if
		else {
			queue.pop();
			id = job.loc;
			if ( list[id].left > time_slice ) {
				totalTime = totalTime + time_slice;
				list[id].left = list[id].left - time_slice;
				job = list[id];
				for ( int i = 0 ; i < time_slice ; i++ ) {
					ans.push_back( job );
				} // for
			} // if
			else if ( list[id].left <= time_slice && !list[id].done ) {
				int left = list[id].left;
				totalTime = totalTime + list[id].left;
				list[id].left = 0;
				list[id].done = true;
				list[id].turnaround = totalTime - list[id].arrival_time;
				list[id].waiting = list[id].turnaround - list[id].CPU_burst;
				finish++;
				job = list[id];
				for ( int i = 0 ; i < left ; i++ ) {
					ans.push_back( job );
				} // for
			} // else

			for ( int i = 1 ; i < list.size() ; i++ ) {
				if ( list[i].arrival_time <= totalTime && !list[i].done && !list[i].check ) {
					list[i].check = true;
					job = list[i];
					queue.push( job );
				} // if
			} // for

			if ( list[id].left > 0 ) {
				job = list[id];
				queue.push( job );
			} // if
		
			if ( queue.size() == 0 ) {
				bool found = false;
				for ( int i = 0 ; i < list.size() && !found ; i++ ) {
					if ( list[i].left > 0 ) {
						list[i].check = true;
						job = list[i];
						queue.push( job );
						found = true;
					} // if
				} // for
			} // if
		} // else
	} // while
} // RR()

void SRTF( vector<TokenType> &list, vector<TokenType> &ans ) {
	// 1. 剩餘CPU burst min
	//  2. arrival time min
	//   3. PID (小->大)
	int finish = 0;
	int shortestloc = 0;
	double next = 0;
	int totalTime = 0;
	TokenType job;
	
	LineUpAT( list );
	for ( int i = 0 ; i < list.size() ; i++ ) {
		list[i].left = list[i].CPU_burst;
	} // for
	
	while ( finish != list.size() ) {
		shortestloc = -99999;
		next = 99999;
		for ( int i = 0 ; i < list.size() ; i++ ) {
			if ( list[i].arrival_time <= totalTime && !list[i].done ) {
				if ( list[i].left < next ) {
					next = list[i].left;
					shortestloc = i;
				} // if
				else if ( list[i].left == next ) { 
					if ( list[i].arrival_time < list[shortestloc].arrival_time ) {
						next = list[i].left;
						shortestloc = i;
					} // if
					else if ( list[i].arrival_time == list[shortestloc].arrival_time ) {
						if ( list[i].ID < list[shortestloc].ID ) {
							next = list[i].left;
							shortestloc = i;
						} // if
					} // else if
				} // else if
			} // if
		} // for
		
		if ( shortestloc != -99999 && !list[shortestloc].done ) {
			list[shortestloc].left--;
			totalTime++;
			job = list[shortestloc];
			ans.push_back( job );
			if ( list[shortestloc].left == 0 ) {
				list[shortestloc].done = true;
				finish++;
				list[shortestloc].turnaround = totalTime - list[shortestloc].arrival_time;
				list[shortestloc].waiting = list[shortestloc].turnaround - list[shortestloc].CPU_burst;
			} // if
		} // if
		else if ( shortestloc == -99999 ) {
			job.ID = 45; // -
			ans.push_back( job );
			totalTime++;
		} // else
	} // while
} // SRTF()

void LineUpP( vector<TokenType> &list ) {
	for ( int i = 0 ; i < list.size() ; i++ ) {
		for ( int j = 0 ; j < list.size() - i - 1 ; j++ ) {
			if ( list[j].priority > list[j+1].priority ) {
				Swap( list[j], list[j+1] );
			} // if
			else if ( list[j].priority == list[j+1].priority ) {
				if ( list[j].arrival_time > list[j+1].arrival_time ) {
					Swap( list[j], list[j+1] );
				} // if
			} // else if
		} // for
	} // for
} // LineUpP()

void Find( vector<TokenType> list, TokenType job, int &loc ) {
	for ( int i = 0 ; i < list.size() ; i++ ) {
		if ( job.priority >= list[i].priority ) {
			loc = i+1;
		} // if
		else if ( job.priority < list[i].priority ) {
			loc = i;
			return;
		} // else if
	} // for
} // Find()

bool Same( vector<TokenType> list, TokenType job ) {
	for ( int i = 0 ; i < list.size() ; i++ ) {
		if ( job.priority == list[i].priority && job.ID != list[i].ID ) {
			return true;
		} // if
	} // for

	return false;
} // Same

void PPRR( vector<TokenType> &list, int time_slice, vector<TokenType> &ans ) {
	// preemptive priority + round robin
	// 1. priority (min)
	//  2. rr (priority同)
 
	int totalTime = 0;
	int loc = 0;
	int next = 0;    // queue的min priority 
	int finish = 0;  // 判斷是不是第一個 
	bool same = false;
	TokenType job;
	TokenType before;
	before.time_slice = 0;
	queue<TokenType> list2;
	vector<TokenType> line;

	LineUpP( list );
	
	for ( int i = 0 ; i < list.size() ; i++ ) {
		list[i].left = list[i].CPU_burst;
		list[i].loc = i;
		list[i].time_slice = 0;
	} // for

	while ( finish != list.size() ) {
		for ( int i = 0 ; i < list.size() ; i++ ) {
			if ( list[i].arrival_time <= totalTime && !list[i].check && !list[i].done ) {
				if ( line.size() != 0 ) {
					list[i].check = true;
					Find( line, list[i], loc );  // 照順序 
					job = list[i];
					line.insert( line.begin() + loc, job );
				} // if
				else if ( line.size() == 0 ) {
					list[i].check = true;
					job = list[i];
					line.push_back( job );
				} // else if
			} // if
		} // for
		
		if ( line.size() == 0 ) {
			job.ID = 45;
			job.priority = -1;
			before = job;
			before.time_slice = 0;
			before.localloc = -1;
			ans.push_back( job );
			totalTime++;
		} // if
		else {
			job = line[0];
			loc = 0;  // line位置 
			int idx = line[0].loc;  // list 位置 
			if ( Same( line, job ) ) {  // RR time_slice
				if ( before.priority == job.priority && list[before.loc].time_slice < time_slice ) {
					job = list[before.loc];
					Swap( line[0], line[job.localloc] );
				} // if
				else if ( before.priority == job.priority && list[before.loc].time_slice >= time_slice ) {
					list[before.loc].time_slice = 0;
				} // else if
			} // if
			
			job = line[0];
			idx = line[0].loc;  // list
			loc = 0;
			if ( list[idx].left > 0 ) {
				totalTime++;
				list[idx].left--;
				list[idx].time_slice++;
				job = list[idx];
				before = job;
				ans.push_back( job );
			} // if
				
			if ( list[idx].left > 0 ) {
				job = line[0];
				line.erase( line.begin() );
				Find( line, job, loc );
				list[idx].localloc = loc;
				line.insert( line.begin() + loc, job );
			} // if
			else if ( list[idx].left == 0 ) {
				line.erase( line.begin() + loc );
				list[idx].done = true;
				finish++;
				list[idx].turnaround = totalTime - list[idx].arrival_time;
				list[idx].waiting = list[idx].turnaround - list[idx].CPU_burst;
			} // else if
		} // else
	} // while
} // PPRR()

void HRRN( vector<TokenType> &list, vector<TokenType> &ans ) {
	// 1. ratio高優先 
	//  2. arrival time min
	//   3. PID min
	// HRRN ratio = ( waiting + CPU burst ) / CPU burst
	int finish = 0;
	int HRRNloc = 0;
	double next = 0;
	int totalTime = 0;
	TokenType job;
	
	LineUpAT( list );
	for ( int i = 0 ; i < list.size() ; i++ ) {
		list[i].left = list[i].CPU_burst;
	} // for
	
	while ( finish != list.size() ) {
		HRRNloc = -99999;
		next = -99999;
		for ( int i = 0 ; i < list.size() ; i++ ) {
			if ( list[i].arrival_time <= totalTime && !list[i].done ) {
				// 算ratio(max)
				double turnaround = ( totalTime - list[i].arrival_time ) + list[i].CPU_burst;
				list[i].hrrn = turnaround / list[i].CPU_burst;

				if ( list[i].hrrn > next ) {
					next = list[i].hrrn;
					HRRNloc = i;
				} // if
				else if ( list[i].hrrn == next ) { 
					if ( list[i].arrival_time < list[HRRNloc].arrival_time ) {
						next = list[i].hrrn;
						HRRNloc = i;
					} // if
					else if ( list[i].arrival_time == list[HRRNloc].arrival_time ) {
						if ( list[i].ID < list[HRRNloc].ID ) {
							next = list[i].hrrn;
							HRRNloc = i;
						} // if
					} // else if
				} // else if
			} // if
		} // for

		if ( HRRNloc != -99999 && !list[HRRNloc].done ) {
			totalTime = totalTime + list[HRRNloc].left;
			list[HRRNloc].left = 0;
			list[HRRNloc].done = true;
			list[HRRNloc].turnaround = totalTime - list[HRRNloc].arrival_time;
			list[HRRNloc].waiting = list[HRRNloc].turnaround - list[HRRNloc].CPU_burst;
			job = list[HRRNloc];
			for ( int i = 0 ; i < list[HRRNloc].CPU_burst ; i++ ) {
				ans.push_back( job );
			} // for

			finish++;
		} // if
		else if ( HRRNloc == -99999 ) {
			job.ID = 45;
			ans.push_back( job );
			totalTime++;
		} // else if
	} // while
} // HRRN()

void ALL( vector< vector<TokenType> > &tlist, int time_slice, vector< vector<TokenType> > &ans ) {
	FCFS( tlist[0], ans[0] );
	RR( tlist[1], time_slice, ans[1] );
	SRTF( tlist[2], ans[2] );
	PPRR( tlist[3], time_slice, ans[3] );
	HRRN( tlist[4], ans[4] );
} // ALL()

void ReadFile( vector<TokenType> &list, string file, int &method, int &time_slice ) {
	string line, temp;
	TokenType job;
	
	int num = 0;
	int first = 0;
	bool correct = true;
	char* filename = (char*)file.c_str();
	ifstream infile ; 
	infile.open( filename ) ;

    getline( infile, line ) ;
    temp = token( first, num, line ) ;
    method = atoi( temp.c_str() );
    temp = token( first, num, line ) ;
    time_slice = atoi( temp.c_str() );

    getline( infile, line ) ;  // columns
	while ( getline( infile, line ) ) {
		if ( line.size() != 0 ) {
			SaveFile( job, line );
			list.push_back( job );
		} // if
	} // while

	infile.close();
} // ReadFile()

string Way( int method ) {
	if ( method == 1 ) {
		return "FCFS";
	} // if
	else if ( method == 2 ) {
		return "RR";
	} // else if
	else if ( method == 3 ) {
		return "SRTF";
	} // else if
	else if ( method == 4 ) {
		return "PPRR";
	} // else if 
	else if ( method == 5 ) {
		return "HRRN";
	} // else if
	else if ( method == 6 ) {
		return "ALL";
	} // else if
} // Way()

void Sort( vector<TokenType> &list ) {
	for ( int i = 0 ; i < list.size() ; i++ ) {
		for ( int j = 0 ; j < list.size() - i - 1 ; j++ ) {
			if ( list[j].ID > list[j+1].ID ) {
				Swap( list[j], list[j+1] );
			} // if
		} // for
	} // for
} // Sort()

void OutputFile( vector<TokenType> list, vector<TokenType> ans, string file, int method ) {
	string output = "out_" + file + ".txt";
	char* filename = (char*)output.c_str();
	fstream outfile;
	string name = Way( method );
	outfile.open( filename, ios::out );
	Sort( list );
	
	if ( method == 4 ) {
		outfile << "Priority RR" << endl;
	} // if
	else {
		outfile << name << endl;
	} // else

	if ( name == "RR" ) {
		outfile << "==          " << name << "==" << endl;
	} // if
	else {
		outfile << "==        " << name << "==" << endl;
	} // else 

	for ( int i = 0 ; i < ans.size() ; i++ ) {
		if ( ans[i].ID >= 0 && ans[i].ID <= 9 ) {
			outfile << ans[i].ID;
		} // if
		else if ( ans[i].ID == 45 ) {
			outfile << "-";
		} // else if
		else {
			outfile << (char)(ans[i].ID+55);
		} // else 
	} // for
	
	outfile << endl;
	outfile << "===========================================================" << endl;
	outfile << endl;
	outfile << "Waiting Time" << endl;
	outfile << "ID" << "\t" << name << endl;
	outfile << "===========================================================" << endl;
	for ( int i = 0 ; i < list.size() ; i++ ) {
		outfile << list[i].ID << "\t" << list[i].waiting << endl;
	} // for
	outfile << "===========================================================" << endl;
	
	outfile << endl;
	outfile << "Turnaround Time" << endl;
	outfile << "ID" << "\t" << name << endl;
	outfile << "===========================================================" << endl;
	for ( int i = 0 ; i < list.size() ; i++ ) {
		outfile << list[i].ID << "\t" << list[i].turnaround << endl;
	} // for
	outfile << "===========================================================" << endl;
	outfile << endl;
	outfile.close();
} // OutputFile()

void OutputALL( vector< vector<TokenType> > tlist, vector< vector<TokenType> > ans, string file ) {
	vector<string> ways;
	ways.push_back( "FCFS" );
	ways.push_back( "RR" );
	ways.push_back( "SRTF" );
	ways.push_back( "PPRR" );
	ways.push_back( "HRRN" );
	string output = "out_" + file + ".txt";
	char* filename = (char*)output.c_str();
	fstream outfile;
	outfile.open( filename, ios::out );
	
	for ( int i = 0 ; i < tlist.size() ; i++ ) {
		Sort( tlist[i] );
	} // for
	
	outfile << "All" << endl;

	for ( int i = 0 ; i < tlist.size() ; i++ ) {
		if ( ways[i] == "RR" ) {
			outfile << "==          " << ways[i] << "==" << endl;
		} // if
		else {
			outfile << "==        " << ways[i] << "==" << endl;
		} // else

		for ( int j = 0 ; j < ans[i].size() ; j++ ) {
			if ( ans[i][j].ID >= 0 && ans[i][j].ID <= 9 ) {
				outfile << ans[i][j].ID;
			} // if
			else if ( ans[i][j].ID == 45 ) {
				outfile << "-";
			} // else if
			else {
				outfile << (char)(ans[i][j].ID+55);
			} // else
		} // for
		outfile << endl;
	} // for
	
	outfile << "===========================================================" << endl;
	outfile << endl;
	outfile << "Waiting Time" << endl;
	outfile << "ID" << "\t" << "FCFS" << "\t" << "RR" << "\t" << "SRTF" << "\t" << "PPRR" << "\t" << "HRRN" << endl;
	outfile << "===========================================================" << endl;
	for ( int i = 0 ; i < tlist[0].size() ; i++ ) {
		outfile << tlist[0][i].ID;
		for ( int j = 0 ; j < tlist.size() ; j++ ) {
			outfile << "\t" << tlist[j][i].waiting;
		} // for
		outfile << endl;
	} // for
	outfile << "===========================================================" << endl;
	
	outfile << endl;
	outfile << "Turnaround Time" << endl;
	outfile << "ID" << "\t" << "FCFS" << "\t" << "RR" << "\t" << "SRTF" << "\t" << "PPRR" << "\t" << "HRRN" << endl;
	outfile << "===========================================================" << endl;
	for ( int i = 0 ; i < tlist[0].size() ; i++ ) {
		outfile << tlist[0][i].ID;
		for ( int j = 0 ; j < tlist.size() ; j++ ) {
			outfile << "\t" << tlist[j][i].turnaround;
		} // for
		outfile << endl;
	} // for
	outfile << "===========================================================" << endl;
	outfile << endl;
	outfile.close();
	ways.clear();
} // OutputALL()

void Interface( vector<TokenType> list, string file, int method, int time_slice ) {
	vector< vector<TokenType> > ans;
	vector< vector<TokenType> > tlist;
	for ( int i = 0 ; i < 5 ; i++ ) {
		vector<TokenType> v;
		ans.push_back( v );
	} // for
	
	if ( method == 1 ) {
		FCFS( list, ans[0] );
		OutputFile( list, ans[0], file, 1 );
	} // if
	else if ( method == 2 ) {
		RR( list, time_slice, ans[1] );
		OutputFile( list, ans[1], file, 2 );
	} // else if
	else if ( method == 3 ) {
		SRTF( list, ans[2] );
		OutputFile( list, ans[2], file, 3 );
	} // else if
	else if ( method == 4 ) {
		PPRR( list, time_slice, ans[3] );
		OutputFile( list, ans[3], file, 4 );
	} // else if
	else if ( method == 5 ) {
		HRRN( list,ans[4] );
		OutputFile( list, ans[4], file, 5 );
	} // else if
	else if ( method == 6 ) {
		for ( int i = 0 ; i < 5 ; i++ ) {
			tlist.push_back( list );
		} // for
		
		ALL( tlist, time_slice, ans );
		OutputALL( tlist, ans, file );
	} // else if
	else {
		cout << "Wrong Method !!!!!" << endl;
	} // else
	
	ans.clear();
	tlist.clear();
} // Interface()

int main() {
	vector<TokenType> list ;
	int num = 0;
	int copies = 0;
	int method = 0;
	int time_slice = 0;
	string middlename;
	string input;
	
	cout << "Continue(1) Finish(0) --> ";
    cin >> num;
    
	while ( cin.fail() ) {
		cout << "Illegal Input!!! Continue(1) Finish(0) --> ";
		cin.clear();
		while ( cin.get() != '\n' ) {
			continue;
		} // while
		cin >> num ;
	} // while

	while ( num != 0 ) {
    	if ( num == 1 ) {
    		cout << "Input filename : " ;
    		string name;
    		cin >> middlename;
			input = middlename + ".txt";
			char* filename = (char*)input.c_str();

       		ifstream infile ; // 讀檔案
			infile.open( filename ) ;
			while ( !infile ) { // 無此檔案
				if ( strcmp ( name.c_str(), "0") == 0  )
					break  ;
				else {
					cout << "Illegal filename!!! Input filename : " ;
					cin >> middlename;
					input = middlename + ".txt";
					filename = (char*)input.c_str();
					infile.open( filename );
				} // else
   			} // while
       		
       		Start = clock();
			ReadFile( list, input, method, time_slice );
			Interface( list, middlename, method, time_slice );
            End = clock();
            float time = float(End - Start)/CLOCKS_PER_SEC ;
			cout << "Time --> " << time << "s" << endl;
		} // if
		
		list.clear();
		cout << "Continue(1) Finish(0) --> ";
		cin >> num;
		while ( cin.fail() ) {
			cout << "Illegal Input!!! Continue(1) Finish(0) --> ";
			cin.clear();
			while ( cin.get() != '\n' ) {
				continue;
			} // while
			cin >> num ;
		} // while
	} // while
	
	cout << "Finish !!! " << endl;
} // main()
