#include "Dictionary.h"

Dictionary::Dictionary() {
    // Cho phép đọc, hiện thị tiếng việt (unicode)
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stdout), _O_U8TEXT);
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

void Dictionary::readDataFromFile(){
    locale loc(locale(), new codecvt_utf8<wchar_t>);
    wifstream file("data.txt");
    if (file.fail()) {
        wfstream file("./data.txt", ios::out);
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
        }
        file.close();
    }
}

void Dictionary::writeDataToFile() {
    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    wofstream file(L"./data.txt");
    file.imbue(loc);
    for (pair<wstring, wstring> word : this->dataDictionary)
    {
        file << word.first << "\n";
        file << word.second << "\n\n";
    }
    file.close();
}


void Dictionary::add() {
    wcout << L"Nhập từ tiếng anh: ";
    wstring eng;
    getline(wcin, eng);
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    wcout << L"Nhập nghĩa tiếng việt: ";
    wstring vi;
    getline(wcin, vi);

    this->dataDictionary.insert(pair<wstring, wstring>(eng, vi));
    this->writeDataToFile();
}

void Dictionary::edit() {
    wcout << L"Nhập từ cần sửa: ";
    wstring eng;
    getline(wcin, eng);
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
    bool exists = this->dataDictionary.find(eng) != this->dataDictionary.end();

    if (exists)
    {
        wcout << L"Nhập nghĩa mới: ";
        wstring vi;
        getline(wcin, vi);
        this->dataDictionary[eng] = vi;
    }
}

void Dictionary::del() {
    wstring eng;
    wcout << L"Nhập từ cần xóa: ";

    getline(wcin, eng);
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    const bool exists = this->dataDictionary.find(eng) != this->dataDictionary.end();
    if (exists)
    {
        this->dataDictionary.erase(eng);
        wcout << L"Xóa thành công!" << endl;
    }
    else
    {
        wcout << L"Không có từ nào như vậy!" << endl;
    }
    this->writeDataToFile();
}

void Dictionary::search() {
    wstring eng;
    wcout << L"Nhập từ cần tìm: ";
    wcin >> eng;
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    bool exists = this->dataDictionary.find(eng) != this->dataDictionary.end();
    if (exists)
    {
        wcout << eng << " : " << this->dataDictionary.at(eng) << endl;
    }
    else {
        wcout << L"Không tìm thấy" << endl;
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

}


void Dictionary::setCursorVisible(bool isVisible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = isVisible;
    SetConsoleCursorInfo(hConsole, &info);
}
void Dictionary::gotoxy(short x, short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { x, y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
	}



void Dictionary::renderMenu(short line, vector<wstring> dataMenu) {
    system("cls");
    setCursorVisible(false);
    // Màu sắc
    HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    for (size_t i = 0; i < dataMenu.size(); i++)
    {
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


void Dictionary::handleChooseMenu(short option) {
    setCursorVisible(true);
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
    L"5. Hiện thị toàn bộ từ (không theo thứ tự)"
    };
    const short lineMax = vectorMenu.size() - 1;
    short line = 0;

    renderMenu(line, vectorMenu);

    bool isExit = false;
    while (!isExit)
    {
        wchar_t key = _getwch();
        switch (key)
        {
        case 72: // Phím mũi tên lên
            line = (line == 0) ? lineMax : line - 1;
            system("cls");
            renderMenu(line, vectorMenu);
            break;
        case 80: // Phím mũi tên xuống
            line = (line == lineMax) ? 0 : line + 1;
            system("cls");
            renderMenu(line, vectorMenu);
            break;
        case 75: // Phím mũi trên trái
            system("cls");
            renderMenu(line, vectorMenu);
            break;
        case 77:
            handleChooseMenu(line);

            wcout << L"\nNhấn phím bất kì để tiếp tục" << endl;
            _getch();
            fflush(stdin);
            setCursorVisible(false);
            renderMenu(line, vectorMenu);
            break;
        case 27: // ESC
            isExit = true;
            break;
        case 13: // Enter
            handleChooseMenu(line);

            wcout << L"\nNhấn phím bất kì để tiếp tục" << endl;
            _getch();
            setCursorVisible(false);
            renderMenu(line, vectorMenu);
            break;
        }

    }
}

Dictionary::~Dictionary() {
	}