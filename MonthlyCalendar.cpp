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
// 固定日付の祝日名を返す
// 祝日ではない場合は nullptr を返す
//--------------------------------------------
const char* holiday_name(int year, unsigned int month, unsigned int day)
{
    if (month == 1 && day == 1)
        return "元日";

    if (month == 2 && day == 11)
        return "建国記念の日";

    // 現在の天皇誕生日（2020年以降）
    if (year >= 2020 && month == 2 && day == 23)
        return "天皇誕生日";

    if (month == 4 && day == 29)
        return "昭和の日";

    if (month == 5 && day == 3)
        return "憲法記念日";

    if (month == 5 && day == 4)
        return "みどりの日";

    if (month == 5 && day == 5)
        return "こどもの日";

    if (month == 8 && day == 11)
        return "山の日";

    if (month == 11 && day == 3)
        return "文化の日";

    if (month == 11 && day == 23)
        return "勤労感謝の日";

    return nullptr;
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
        const char* holiday = holiday_name(year, month, day);

        cout << setw(2) << day << (holiday ? "* " : "  ");

        if (index % 7 == 0)
            cout << '\n';

        ++index;
    }

    cout << '\n';
    cout << "\n祝日（*）:\n";

    bool found_holiday = false;

    for (unsigned int day = 1; day <= last; ++day)
    {
        const char* holiday = holiday_name(year, month, day);

        if (holiday != nullptr)
        {
            cout << month << "月" << day << "日 " << holiday << '\n';
            found_holiday = true;
        }
    }

    if (!found_holiday)
        cout << "この月に登録されている固定祝日はありません。\n";
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