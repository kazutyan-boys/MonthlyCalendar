#include <iostream>
#include <iomanip>
using namespace std;

//--------------------------------------------
// うるう年判定
//--------------------------------------------
bool is_leap_year(int year)
{
    return (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);
}

//--------------------------------------------
// 月末日を返す
//--------------------------------------------
unsigned int last_day(int year, unsigned int month)
{
    static const unsigned int days[] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31 };

    if (month == 2 && is_leap_year(year))
        return 29;

    return days[month - 1];
}

//--------------------------------------------
// 曜日を返す
// 戻り値
// 1=Mon
// 2=Tue
// ...
// 7=Sun
//--------------------------------------------
unsigned int week_day(int y, unsigned int m, unsigned int d)
{
    if (m < 3)
    {
        m += 12;
        --y;
    }

    int k = y % 100;
    int j = y / 100;

    int h = (d + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

    switch (h)
    {
    case 2:
        return 1; // Mon
    case 3:
        return 2; // Tue
    case 4:
        return 3; // Wed
    case 5:
        return 4; // Thu
    case 6:
        return 5; // Fri
    case 0:
        return 6; // Sat
    case 1:
        return 7; // Sun
    }

    return 0;
}

//--------------------------------------------
// 月間カレンダー表示
//--------------------------------------------
void print_month_calendar(int year, unsigned int month)
{
    cout << year << "年 " << month << "月\n\n";
    cout << "Mon Tue Wed Thu Fri Sat Sun\n";

    unsigned int first = week_day(year, month, 1);
    unsigned int last = last_day(year, month);

    unsigned int index = 1;

    // 月初めまで空白を表示
    for (unsigned int i = 1; i < first; ++i, ++index)
    {
        cout << "    ";
    }

    // 日付表示
    for (unsigned int day = 1; day <= last; ++day)
    {
        cout << setw(3) << day << ' ';

        if (index % 7 == 0)
            cout << '\n';

        ++index;
    }

    cout << '\n';
}

//--------------------------------------------
// main
//--------------------------------------------

int main()
{
    //--------------------------------------------
    // 変数宣言
    //--------------------------------------------
    int year, month;

    //--------------------------------------------
    // 年・月の入力
    //--------------------------------------------
    cout << "年を入力してください：";
    if (!(cin >> year))
    {
        cout << "数字を入力してください。" << endl;
        return 1;
    }
    //--------------------------------------------
    // 年の範囲チェック
    //--------------------------------------------
    if (year < 1)
    {
        cout << "年は1以上を入力してください。" << endl;
        return 1;
    }

    cout << "月を入力してください：";
    if (!(cin >> month))
    {
        cout << "数字を入力してください。" << endl;
        return 1;
    }

    //--------------------------------------------
    // 月の範囲チェック
    //--------------------------------------------
    if (month < 1 || month > 12)
    {
        cout << "月は1～12を入力してください。" << endl;
        return 1;
    }

    //--------------------------------------------
    // カレンダー表示
    //--------------------------------------------
    print_month_calendar(year, month);
    return 0;
}