#pragma once
#include <iostream>
#include <vector>
#include <set>
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
#include <algorithm>
#include <cwctype>
using namespace std;

class Dictionary
{
private:
	unordered_map<wstring, wstring> dataDictionary;
	set<wstring> dataWordSorted;
	const size_t marginLeft = 40;
	const size_t width = 40;
	const size_t marginTop = 10;
public:
	Dictionary();
	void alignCenter(short width, wstring content);
	void readDataFromFile();
	void writeDataToFile();
	void add();
	void edit();
	void del();
	void search();
	void showFull();
	void setCursorVisible(bool isVisible);
	void gotoxy(size_t x, size_t y);
	void renderBorderMenu(size_t height);
	void renderMenu(size_t line, vector<wstring> dataMenu);
	void handleChooseMenu(size_t option);
	void menu();
	~Dictionary();
};

