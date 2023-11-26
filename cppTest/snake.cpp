#include <windows.h>
#include <iostream>
#include <algorithm>
using namespace std;
#include <conio.h>
#include <time.h>

class Snake {
public:
	enum Direction { STOP, UP, DOWN, LEFT, RIGHT, };
	Snake(int m, int n) :
		m_x((rand() % (n-2)) + 1),
		m_y((rand() % (m-2)) + 1),
		m_len(1), m_score(0.0), m_dir(STOP), m_speed(200.0) {
			;
	}

	int getX() { return m_x; }
	int getY() { return m_y; }
	int getLen() { return m_len; }
	float getScore() { return m_score; }
	float getSpeed() { return m_speed; }
	Direction getDir() { return m_dir; }
	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }
	void setLen(int len) { m_len = len; }
	void setScore(float score) { m_score = score; }
	void setSpeed(float speed) { m_speed = speed; }
	void setDir(Direction dir) { m_dir = dir; }

private:
	float m_score, m_speed;
	int m_x, m_y, m_len;
	Direction m_dir;
};

class Fruit {
public:
	Fruit(int m, int n) :
		m_x((rand() % (n-2)) + 1), 
		m_y((rand() % (m-2)) + 1) {
			;
	}

	int getX() { return m_x; }
	int getY() { return m_y; }
	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }
	void update(int m, int n) {
		m_x = (rand() % (n-2)) + 1;
		m_y = (rand() % (m-2)) + 1;
	}

private:
	int m_x, m_y;
};

void drawWindow(int m, int n, Snake& mysnake, Fruit& myfruit) {
	system("cls");
	string side(n, '#');
	string body = '#' + string(n-2, ' ') + '#';
	for(int i = 0; i < m; i++) {
		string s2print;
		if(i == 0 || i == m - 1) {
			s2print = side;
		} else {
			s2print = body;
			if(i == myfruit.getY()) {	//draw snake food
				s2print[myfruit.getX()] = '$';
			}
			if (i == mysnake.getY()) {	//draw snake head
				s2print[mysnake.getX()] = 'O';
			}
		}
		cout << s2print << endl;
	} cout << "Score: " << mysnake.getScore() << endl;
	cout << "Speed: " << 200.0 - mysnake.getSpeed() << endl;
}

void changePos(int m, int n, Snake& mysnake, Fruit& myfruit) {
	int x = 0, y = 0;
	if(_kbhit()) {
		switch (_getch()) {
			case 'w':
				mysnake.setDir(Snake::Direction::UP);
				break;
			case 's':
				mysnake.setDir(Snake::Direction::DOWN);
				break;
			case 'a':
				mysnake.setDir(Snake::Direction::LEFT);
				break;
			case 'd':
				mysnake.setDir(Snake::Direction::RIGHT);
				break;
			case 'x':
				mysnake.setDir(Snake::Direction::STOP);
				break;
		}
	}

	switch (mysnake.getDir()) {
		case Snake::Direction::UP:
			y = mysnake.getY() - 1;
			y = y <= 0 ? m - 2 : y;
			mysnake.setY(y);
			break;
		case Snake::Direction::DOWN:
			y = mysnake.getY() + 1;
			y = y >= m - 1 ? 1 : y;
			mysnake.setY(y);
			break;
		case Snake::Direction::LEFT:
			x = mysnake.getX() - 1;
			x = x <= 0 ? n - 2 : x;
			mysnake.setX(x);
			break;
		case Snake::Direction::RIGHT:
			x = mysnake.getX() + 1;
			x = x >= n - 1 ? 1 : x;
			mysnake.setX(x);
			break;
		case Snake::Direction::STOP:
			break;
	}

	if(mysnake.getX() == myfruit.getX()
	&& mysnake.getY() == myfruit.getY()) {
		myfruit.update(m, n);
		mysnake.setScore(mysnake.getScore()+5-(rand()%100)/100.0);
		mysnake.setSpeed(mysnake.getSpeed()-2.5+(rand()%100)/100.0);
		if(mysnake.getSpeed() <= 0) mysnake.setSpeed(0); 
	}
}

int main() {
	srand(time(nullptr));
	int m = 15, n = 25;
	Snake mysnake(m, n);
	Fruit myfruit(m, n);
	while(1) {
		drawWindow(m, n, mysnake, myfruit);
		changePos(m, n, mysnake, myfruit);
		Sleep(mysnake.getSpeed());
	} return 0;
}
