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

unordered_map<wstring, wstring> dictionary;

// Đọc dữ liệu từ điển từ file
void readData()
{
    locale loc(locale(), new codecvt_utf8<wchar_t>);
    wifstream file(L"data.txt");
    if (file.fail()) {
        // Nếu file không tồn tại thì tạo file mới
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
            dictionary.insert(pair<wstring, wstring> (eng, vi));
        }
        file.close();
    }
} 

// Cập nhật dữ liệu từ điển vào file
void updateData()
{
    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    wofstream file(L"./data.txt");
    file.imbue(loc);
    for (pair<wstring, wstring> word : dictionary)
    {
        file << word.first << "\n";
        file << word.second << "\n\n";
    }
    file.close();
}

void showFull()
{
    system("cls");
    wcout << std::left
        << "| "
        << setw(30) << L"Từ"
        << "| "
        << setw(85) << L"Nghĩa"
        << "|" << endl;
    wcout << "|" << wstring(118, '-') << "|" << endl;
    for (auto word : dictionary)
    {
        wcout << std::left
            << "| "
            << setw(30) << word.first
            << "| "
            << setw(85) << word.second
            << "|" << endl;
    }

}

// Thêm từ vào từ điển
void add()
{
    wstring eng;
    wcout << L"Nhập từ tiếng anh: ";
    getline(wcin, eng);

    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    wstring vi;
    wcout << L"Nhập nghĩa tiếng việt: ";
    getline(wcin, vi);
    dictionary.insert(pair<wstring, wstring>(eng, vi));
    updateData();
}

// Xóa từ trong từ điển
void del()
{
    wstring eng;
    wcout << L"Nhập từ cần xóa: ";

    getline(wcin, eng);
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    const bool exists = dictionary.find(eng) != dictionary.end();
    if (exists)
    {
        dictionary.erase(eng);
        wcout << L"Xóa thành công!" << endl;
    }
    else
    {
        wcout << L"Không có từ nào như vậy!" << endl;
    }
    updateData();
}

// Tìm kiếm từ
void search()
{
    wstring eng;
    wcout << L"Nhập từ cần tìm: ";
    wcin >> eng;
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);

    bool exists = dictionary.find(eng) != dictionary.end();
    if (exists)
    {
        wcout << eng << " : " << dictionary.at(eng) << endl;
    }
    else {
        wcout << L"Không tìm thấy" << endl;
    }
}

// Sửa từ
void edit()
{

    wcout << L"Nhập từ cần sửa: ";
    wstring eng;
    getline(wcin, eng);
    transform(eng.begin(), eng.end(), eng.begin(), std::towlower);
    bool exists = dictionary.find(eng) != dictionary.end();

    if (exists)
    {
        wcout << L"Nhập nghĩa mới: ";
        wstring vi;
        getline(wcin, vi);
        dictionary[eng] = vi;
    }
}

// bl = false để ẩn con trỏ nhập
void setCursorVisible(bool bl) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = bl;
    SetConsoleCursorInfo(hConsole, &info);
}

void gotoxy(short x, short y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { x, y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void renderMenu(short line, vector<wstring> dataMenu) {
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


void handleChooseMenu(short option) {
    setCursorVisible(true);
    system("cls");
    switch (option)
    {
    case 0:
        add();
        break;
    case 1:
        del();
        break;
    case 2:
        search();
        break;
    case 3:
        edit();
        break;
    case 4:
        showFull();
        break;
    }
}



void menu() {



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
            line = (line == 0) ?  lineMax : line - 1;
            system("cls");
            renderMenu(line, vectorMenu);
            break;
        case 80: // Phím mũi tên xuống
            line = (line == lineMax) ?  0 : line + 1;
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

// Cho phép đọc, hiện thị tiếng việt (unicode)
void enableViMode() {
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stdout), _O_U8TEXT);
    SetConsoleTitleW(L"Từ điển anh - việt");
    HANDLE hdlConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX consoleFont;
    consoleFont.cbSize = sizeof(consoleFont);
    GetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);
    memcpy(consoleFont.FaceName, L"Consolas", sizeof(consoleFont.FaceName));
    SetCurrentConsoleFontEx(hdlConsole, FALSE, &consoleFont);
}

int main()
{
    enableViMode();

    readData();

    menu();

    system("cls");
    wcout << L"Good bye";

    return 0;
}
