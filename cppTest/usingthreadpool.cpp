#include <iostream>
using namespace std;

#include "threadpool.h"
#include <unistd.h>
#include <random>
#include <time.h>

class Req {
public:
	Req(int val = 0) : m_val(val) {}
	~Req(){}
	void process() {
		cout << "my val: " << m_val << endl;
		cout << this << endl;
	}
private:
	int m_val;
};

int main() {
	srand(time(nullptr));
	vector<Req> myreq{0,1,2,3,4,5,6,7,8,9,};		//how can I initialize them individually
	Threadpool<Req> mypool(8);	//how can I catch execption

	while(1) {
		int times = rand() % 5;
		for(int i = 0; i < times; i++) {
			int round = rand() % 10;
			mypool.append(&myreq[round]);
		} sleep(1);
	} return 0;
}



