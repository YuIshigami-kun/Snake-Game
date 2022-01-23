#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <conio.h>
#include <thread>

#define width int(210)
#define height int(30)
#define swidth int(50)
#define sheigth int(20)

#pragma comment(lib, "user32")

using namespace std;

bool dead;
string limits, stats;
int fx, fy, length, speed, points, seconds, obsCount;
char input;
vector<int> px(width*height - 1);
vector<int> py(width*height - 1);
vector<int> obs(100);

string Square(int twidth, int theight){
	string x = "";
	
	for(int i = 0; i <= theight; i++){
		for(int s = 0; s <= twidth; s++){
			if(i == 0 || i == theight){
				if(s != twidth){
					x += "#";
				}else{
					x += "\n";
				}
			}else{
				if(s == 0 || s == twidth - 1){
					x += "#";
				}else if(s == twidth){
					x += "\n";
				}else{
					x += " ";
				}
			}
		}
	}
	
	return x;
}

void Time(){
	while(dead == false){
		seconds++;
		Sleep(1000);
	}
}

string Format(){
	int h, m, s = seconds;
	
	m = trunc(s/60);
	s = s%60;
	
	h = trunc(m/60);
	m = m%60;
	
	return to_string(h) + "h " + to_string(m) + "' " + to_string(s) + "''";
}

void Banner(){
	cout << "\t\t     _______..__   __.      ___       __  ___  _______ " << endl;
    cout << "\t\t    /       ||  \\ |  |     /   \\     |  |/  / |   ____|" << endl;
	cout << "\t\t   |   (----`|   \\|  |    /  ^  \\    |  '  /  |  |__" << endl;
    cout << "\t\t    \\   \\    |  . `  |   /  /_\\  \\   |    <   |   __|" << endl;
	cout << "\t\t.----)   |   |  |\\   |  /  _____  \\  |  .  \\  |  |____" << endl;
	cout << "\t\t|_______/    |__| \\__| /__/     \\__\\ |__|\\__\\ |_______|" << endl;
	
	cout << endl << "Press any key to start...";
	_getch();
}

bool Dead(){
	cout << " ____                 _ _" << endl;
	cout << "|  _ \\  ___  __ _  __| | |" << endl;
	cout << "| | | |/ _ \\/ _` |/ _` | |" << endl;
	cout << "| |_| |  __/ (_| | (_| |_|" << endl;
	cout << "|____/ \\___|\\__,_|\\__,_(_)" << endl;
	
	cout << endl << "Would you like to continue?(y/n): ";
	
	string x; cin >> x;
	
	if(x == "y"){
		return true;
	}else{
		return false;
	}
}

void Setup(){
	dead = false;
	length = 1;
	speed = 300;
	input = 'x';
	points = 0;
	seconds = 0;
	obsCount = 0;
	fx = -1; fy = -1;
	px[0] = rand() % (width - 1) + 1; py[0] = rand() % (height - 1) + 1;
	
	time_t nTime;
	srand((unsigned) time(&nTime));
	
	limits = Square(width, height);
	stats = Square(swidth, sheigth);
	
	thread(Time).detach();
}

void Stats(){
	stats.replace(124, 5, "STATS");
	stats.replace(205, 49, "#################################################");
	
	string x = Format();
	
	stats.replace(359, ("Length: " + to_string(length)).length(), "Length: " + to_string(length));
	stats.replace(461, ("Points: "  + to_string(points)).length(), "Points: " + to_string(points));
	stats.replace(563, ("Time played: " + x).length(), "Time played: " + x);
	stats.replace(665, ("Obstacles count: " + to_string(obsCount)).length(), "Obstacles count: " + to_string(obsCount));
}

void Keys(){
	if(_kbhit()){
		input = _getch();
	}
	
	for(int i = length; i > 0; i--){
		px[i] = px[i - 1];
		py[i] = py[i - 1];
	}
	
	switch(input){
		case 's':
			py[0]++;
			break;
		case 'w':
			py[0]--;
			break;
		case 'd':
			px[0]++;
			break;
		case 'a':
			px[0]--;
			break;
	}
}

void GenObs(){
	int x, y;
	bool keep = true;
	
	while(keep == true){
		x = rand() % width;
		y = rand() % height;
		
		keep = false;
		
		for(int i = 0; i < length; i++){
			if(px[i] == x || py[i] == y) keep = true;
	 	}	
		
		for(int i = 0; i < obsCount; i += 2){
			if(obs[i] == x || obs[i + 1] == y) keep = true;
		}
		
		if((x > fx-5 && x < fx+5) || (y > fy-5 && y < fy+5)) keep = true;
	}
	
	obs[obsCount*2] = x; obs[obsCount*2 + 1] = y;
	obsCount++;
}

void GenFruit(){
	bool keep = true;
	
	while(keep == true){
		fx = rand() % (width - 1) + 1;
		fy = rand() % (height - 1) + 1;
		
		keep = false;
		
		for(int i = 0; i < length; i++){
			if(px[i] == fx || py[i] == fy) keep = true;
		}
		
		
	}
}

void Draw(){
	string result = limits;
	
	if(fx != -1 && fy != -1) result.replace((width*fy) + fy + fx, 1, "0");

	if(result[(width*py[0]) + py[0] + px[0]] == '#'){
		dead = true;
		return;
	}
	
	if(px[0] == fx && py[0] == fy){
		length++;
		points++;
		if(speed > 1) speed -= 10;
		fx = -1; fy = -1;
	}
	
	for(int i = 0; i < length; i++){
		if(px[i] == px[0] && py[i] == py[0] && i != 0){
			dead = true;
			return;
		}
		
		result.replace((width*py[i]) + py[i] + px[i], 1, "O");
	}	
	
	for(int i = 0; i < obsCount; i += 2){
		result.replace((width * obs[i + 1]) + obs[i + 1] + obs[i], 2, "##");
		result.replace((width * (obs[i + 1] + 1)) + obs[i + 1] + 1 + obs[i], 2, "##");
	}
	
	
	if(result[(width*py[0]) + py[0] + px[0]] == '#'){
		dead = true;
		return;
	}
	
	
	cout << result << endl << stats;
}

void Update(){
	if(fx == -1 || fy == -1) GenFruit();
	
	if(points > 3 && obsCount < 100 && rand()%100 > 95) GenObs();
	
	
	Keys();
	Stats();
	system("cls");	
	Draw();
	
	Sleep(speed);
}


int main(){
	bool cont = true;
	::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	
	while(cont == true){
		Setup();
		system("cls");
		Banner();
		
		while(dead == false){
			Update();
		}	
		
		cont = Dead();
	}
	return 0;
}
