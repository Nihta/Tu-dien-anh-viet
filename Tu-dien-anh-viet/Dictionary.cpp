#include "Dictionary.h"

Dictionary::Dictionary() {
	// Cho phép đọc, ghi, hiện thị tiếng việt (unicode)
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	SetConsoleTitleW(L"Từ điển anh - việt");
	HANDLE hdlConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX consoleFont;
	consoleFont.cbSize = sizeof(consoleFont);
	GetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);
	memcpy(consoleFont.FaceName, L"Consolas", sizeof(consoleFont.FaceName));
	SetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);

	// Đọc dữ liệu từ điển từ file
	this->readDataFromFile();
}

// Đọc dữ liệu từ file
void Dictionary::readDataFromFile() {
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream file("data.txt");
	if (file.fail()) {
		wfstream file("./data.txt", ios::out);
		if (file.fail()) {
			wcout << L"Có lỗi xảy ra khi tạo file" << endl;
			system("exit");
		}
	}
	else {
		file.imbue(loc);
		while (!file.eof())
		{
			wstring eng;
			getline(file, eng);
			if (eng == L"") {
				break;
			}
			wstring vi;
			getline(file, vi);
			wstring blankLine;
			getline(file, blankLine);
			this->dataDictionary.insert(pair<wstring, wstring>(eng, vi));
			this->dataWordSorted.insert(eng);
		}
		file.close();
	}
}

// Ghi dữ liệu vào file
void Dictionary::writeDataToFile() {
	std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wofstream file(L"./data.txt");
	file.imbue(loc);
	for (pair<wstring, wstring> word : this->dataDictionary)
	{
		file << word.first << "\n"
			<< word.second << "\n\n";
	}
	file.close();
}

// In một string được căn giữa trong khoảng width
void Dictionary::alignCenter(short width, wstring content) {
	wcout << wstring((width - content.length() - 1) / 2, ' ') << content;
}

// Thêm mới một từ vào từ điển
void Dictionary::add() {
	const char numOfLine = 5;
	this->renderBorderMenu(numOfLine);

	gotoxy(marginLeft + 2, marginTop + 1);
	wcout << L"Nhập từ tiếng anh cần bổ sung: " << endl;
	gotoxy(marginLeft + 2, marginTop + 2);
	wcout << L">> ";
	this->setCursorVisible(true);
	wstring eng;
	getline(wcin, eng);
	this->setCursorVisible(false);
	if (eng == L"") return;

	transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
	if (this->dataDictionary.find(eng) != this->dataDictionary.end()) {
		gotoxy(marginLeft + 2, marginTop + numOfLine);
		this->alignCenter(this->width, L"Thêm thất bại! Từ này đã tồn tại");
		_getch();
		fflush(stdin);
		return;
	}

	gotoxy(marginLeft + 2, marginTop + 1);
	wcout << L"Nhập nghĩa tiếng việt:         " << endl;
	gotoxy(marginLeft + 2, marginTop + 2);
	wcout << wstring(3 + eng.length(), ' ');
	gotoxy(marginLeft + 2, marginTop + 2);
	wcout << L">> ";
	this->setCursorVisible(true);
	wstring vi;
	getline(wcin, vi);
	this->setCursorVisible(false);
	if (vi == L"") return;

	this->dataDictionary.insert(pair<wstring, wstring>(eng, vi));
	this->dataWordSorted.insert(eng);
	this->writeDataToFile();


	gotoxy(marginLeft + 2, marginTop + numOfLine);
	this->alignCenter(this->width, L"Thêm thành công!");
	_getch();
	fflush(stdin);
}

// Sửa từ
void Dictionary::edit() {
	const char numOfLine = 5;
	this->renderBorderMenu(numOfLine);

	// Nhập từ cần sửa
	gotoxy(marginLeft + 2, marginTop + 1);
	wcout << L"Nhập từ cần sửa:";
	gotoxy(marginLeft + 2, marginTop + 2);
	wcout << L">> ";
	this->setCursorVisible(true);
	wstring eng;
	getline(wcin, eng);
	this->setCursorVisible(false);
	if (eng == L"") return;

	// Kiểm tra xem trong từ điển có từ này hay không
	transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
	bool exists = this->dataDictionary.find(eng) != this->dataDictionary.end();

	if (exists)
	{
		gotoxy(marginLeft + 2, marginTop + 1);
		wcout << L"Nhập nghĩa tiếng việt mới:" << endl;
		gotoxy(marginLeft + 2, marginTop + 2);
		wcout << wstring(3 + eng.length(), ' ');
		gotoxy(marginLeft + 2, marginTop + 2);
		wcout << L">> ";
		this->setCursorVisible(true);
		wstring vi;
		getline(wcin, vi);
		this->setCursorVisible(false);
		if (vi == L"") return;

		this->dataDictionary[eng] = vi;
		this->writeDataToFile();

		gotoxy(marginLeft + 2, marginTop + numOfLine);
		this->alignCenter(this->width, L"Sửa thành công!");
	}
	else {
		gotoxy(marginLeft + 2, marginTop + numOfLine);
		this->alignCenter(this->width, L"Sửa thất bại! Không tìm thấy từ này");
	}
	_getch();
	fflush(stdin);
}

// Xóa một từ ra khỏi từ điển
void Dictionary::del() {
	const char numOfLine = 5;
	this->renderBorderMenu(numOfLine);

	// Nhập từ cần xóa
	gotoxy(marginLeft + 2, marginTop + 1);
	wcout << L"Nhập từ cần xóa:";
	gotoxy(marginLeft + 2, marginTop + 2);
	wcout << L">> ";
	this->setCursorVisible(true);
	wstring eng;
	getline(wcin, eng);
	this->setCursorVisible(false);
	if (eng == L"") return;

	// Kiểm tra xem trong từ điển có từ này hay không
	transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
	bool exists = this->dataDictionary.find(eng) != this->dataDictionary.end();

	if (exists)
	{
		this->dataDictionary.erase(eng);
		this->dataWordSorted.erase(eng);
		this->writeDataToFile();
		gotoxy(marginLeft + 2, marginTop + numOfLine);
		this->alignCenter(this->width, L"Xóa thành công!");
	}
	else
	{
		gotoxy(marginLeft + 2, marginTop + numOfLine);
		this->alignCenter(this->width, L"Xóa thất bại! Không tìm thấy từ này");
	}
	_getch();
	fflush(stdin);
}

// Tra cứu từ điển
void Dictionary::search() {
	system("cls");
	this->setCursorVisible(false);
	// Vẽ khung từ điển
	gotoxy(0, 0);  wcout << "+" << wstring(118, '-') << "+";
	for (unsigned int i = 1; i < 29; i++) {
		gotoxy(0, i); wcout << L"|";
		gotoxy(32, i); wcout << L"|";
		gotoxy(119, i); wcout << L"|";
	}
	gotoxy(0, 29); wcout << "+" << wstring(118, '-') << "+";
	gotoxy(1, 1); wcout << L">> ";

	// Lấy dữ liệu từ trong từ điển
	vector<wstring> vectorWordSorted(this->dataWordSorted.begin(), this->dataWordSorted.end());

	// Tìm kiếm, nhấn phím esc hoặc phím mũi tên trái để thoát
	wstring eng;
	while (true)
	{
		gotoxy(4, 1);
		wcout << setw(28) << eng;

		char ch = _getch();
		fflush(stdin);

		// Nếu bấm phím mũi tên bất kì
		if (ch == -32)
		{
			// Phím mũi tên trái
			if (_getch() == 75)
			{
				break;
			}
		}

		// Phím esc
		if (ch == 27)
		{
			break;
		}

		// Phím backspace
		if (ch == 8 && eng.length() > 0) {
			eng.pop_back();
		}

		// Nếu bấm chữ cái hoặc phím cách
		if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == 32)
		{
			eng.push_back(ch);
		}

		transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
		size_t count = 0;
		for (unsigned int i = 0; eng.length() != 0 && i < vectorWordSorted.size() && count < 27; i++)
		{
			if (vectorWordSorted[i].length() >= eng.length() && vectorWordSorted[i].substr(0, eng.length()) == eng) {
				// In từ tiếng anh
				gotoxy(4, 2 + count);
				wcout << setw(28) << vectorWordSorted[i];
				// In nghĩa
				gotoxy(34, 2 + count);
				wcout << std::left << setw(85);
				// Nếu dài quá thì cắt chuỗi
				if (this->dataDictionary[vectorWordSorted[i]].size() > 85) {
					wcout << this->dataDictionary[vectorWordSorted[i]].substr(0, 80) + L" ... ";
				}
				else {
					wcout << this->dataDictionary[vectorWordSorted[i]];
				}
				count++;
			}
			else if (count > 0) {
				break;
			}
		}
		// Xóa những hàng không có dữ liệu
		for (size_t i = count; i < 27; i++) {
			gotoxy(4, 2 + i); wcout << setw(28) << "";
			gotoxy(34, 2 + i); wcout << setw(85) << "";
		}
	}
}


void Dictionary::showFull()
{
	system("cls");
	wcout << std::left
		<< "| "
		<< setw(30) << L"Từ"
		<< "| "
		<< setw(85) << L"Nghĩa"
		<< "|" << endl;
	wcout << "|" << wstring(118, '-') << "|" << endl;
	for (auto word : this->dataDictionary)
	{
		wcout << std::left
			<< "| "
			<< setw(30) << word.first
			<< "| "
			<< setw(85) << word.second
			<< "|" << endl;
	}
	_getch();
}

// Cho phép hiện thị con trỏ nhập hay không (true là hiện)
void Dictionary::setCursorVisible(bool isVisible) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 20;
	info.bVisible = isVisible;
	SetConsoleCursorInfo(hConsole, &info);
}

// Di chuyển vị trí con trỏ nhập đến vị trí có tọa độ (x, y)
void Dictionary::gotoxy(size_t x, size_t y) {
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x, y };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}


void Dictionary::renderMenu(size_t line, vector<wstring> dataMenu) {
	// Màu sắc
	HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);

	// Ẩn con trỏ nhập 
	setCursorVisible(false);

	// In các lựa chọn trong menu
	for (size_t i = 0; i < dataMenu.size(); i++)
	{
		gotoxy(marginLeft + 2, marginTop + 1 + i);
		if (i == line) {
			SetConsoleTextAttribute(hConsoleColor, 11);
			wcout << L">>  " << dataMenu[i] << endl;
		}
		else {
			SetConsoleTextAttribute(hConsoleColor, 15);
			wcout << L"    " << dataMenu[i] << endl;
		}
	}
}

// Vẽ khung menu
void Dictionary::renderBorderMenu(size_t height) {
	HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleColor, 15);
	this->setCursorVisible(false);
	this->gotoxy(marginLeft, marginTop);
	wcout << "+" << wstring(width, '-') << "+\n";
	for (size_t i = 0; i < height; i++) {
		this->gotoxy(marginLeft, marginTop + 1 + i);
		wcout << "|";
		this->gotoxy(marginLeft + 1 + width, marginTop + 1 + i);
		wcout << "|";
	}
	this->gotoxy(marginLeft, marginTop + 1 + height);
	wcout << "+" << wstring(width, '-') << "+\n";
}

void Dictionary::handleChooseMenu(size_t option) {
	system("cls");
	switch (option)
	{
	case 0:
		this->add();
		break;
	case 1:
		this->del();
		break;
	case 2:
		this->search();
		break;
	case 3:
		this->edit();
		break;
	case 4:
		this->showFull();
		break;
	}
}

void Dictionary::menu() {
	const vector<wstring> vectorMenu = {
		L"1. Thêm từ",
		L"2. Xóa từ",
		L"3. Tra cứu",
		L"4. Sửa từ",
		L"5. Hiển thị từ điển",
		L"0. Thoát"
	};
	size_t vectorSize = vectorMenu.size();
	const size_t lineMax = vectorSize - 1;
	size_t line = 0;

	system("cls");
	this->setCursorVisible(false);
	this->renderBorderMenu(vectorSize);
	this->renderMenu(line, vectorMenu);

	bool isExit = false;
	while (!isExit)
	{
		wchar_t key = _getwch();
		switch (key)
		{
		case 72: // Phím mũi tên lên
			line = (line == 0) ? lineMax : line - 1;
			renderMenu(line, vectorMenu);
			break;
		case 80: // Phím mũi tên xuống
			line = (line == lineMax) ? 0 : line + 1;
			renderMenu(line, vectorMenu);
			break;
		case 75: // Phím mũi trên trái
			renderMenu(line, vectorMenu);
			break;
		case 77: // Phím mũi tên phải
			handleChooseMenu(line);
			setCursorVisible(false);
			renderMenu(line, vectorMenu);
			break;
		case 27: // ESC
			isExit = true;
			break;
		case 13: // Enter
			handleChooseMenu(line);
			setCursorVisible(false);
			system("cls");
			this->renderBorderMenu(vectorSize);
			this->renderMenu(line, vectorMenu);
			break;
		}

	}
}

Dictionary::~Dictionary() {
}