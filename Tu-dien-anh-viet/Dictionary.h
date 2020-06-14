#pragma once
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <algorithm>;
#include <cwctype>;
using namespace std;
class Dictionary
{
private:
	unordered_map<wstring, wstring> dataDictionary;
public:
	Dictionary();
	

	void readDataFromFile();
	void writeDataToFile();
	void add();
	void edit();
	void del();
	void search();
	void showFull();
	void setCursorVisible(bool isVisible);
	void gotoxy(short x, short y);
	void renderMenu(short line, vector<wstring> dataMenu);
	void handleChooseMenu(short option);
	void menu();
	~Dictionary();
};

