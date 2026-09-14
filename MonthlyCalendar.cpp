#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//--------------------------------------------
// 祝日データ
//--------------------------------------------
struct Holiday
{
    int month;
    int day;
    string name;
};

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
// 3=Wed
// 4=Thu
// 5=Fri
// 6=Sat
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
        return 1;
    case 3:
        return 2;
    case 4:
        return 3;
    case 5:
        return 4;
    case 6:
        return 5;
    case 0:
        return 6;
    case 1:
        return 7;
    }

    return 0;
}

//--------------------------------------------
// 第n月曜日の日付を返す
//--------------------------------------------
int nth_monday(int year, int month, int n)
{
    unsigned int first = week_day(year, month, 1);

    int first_monday = 1;

    if (first != 1)
        first_monday = 9 - first;

    return first_monday + (n - 1) * 7;
}

//--------------------------------------------
// 春分の日を返す（近似式）
// 対象：1900～2099年程度
//--------------------------------------------
int vernal_equinox_day(int year)
{
    return static_cast<int>(
        20.8431 + 0.242194 * (year - 1980)
        - (year - 1980) / 4);
}

//--------------------------------------------
// 秋分の日を返す（近似式）
// 対象：1900～2099年程度
//--------------------------------------------
int autumnal_equinox_day(int year)
{
    return static_cast<int>(
        23.2488 + 0.242194 * (year - 1980)
        - (year - 1980) / 4);
}

//--------------------------------------------
// 指定日が祝日データに存在するか
//--------------------------------------------
bool has_holiday(const vector<Holiday>& holidays,
    int month, int day)
{
    for (const auto& holiday : holidays)
    {
        if (holiday.month == month &&
            holiday.day == day)
        {
            return true;
        }
    }

    return false;
}

//--------------------------------------------
// 指定日の祝日名を返す
//--------------------------------------------
string holiday_name(const vector<Holiday>& holidays,
    int month, int day)
{
    for (const auto& holiday : holidays)
    {
        if (holiday.month == month &&
            holiday.day == day)
        {
            return holiday.name;
        }
    }

    return "";
}

//--------------------------------------------
// 年間の祝日を作成する
//--------------------------------------------
vector<Holiday> create_holidays(int year)
{
    vector<Holiday> holidays;

    //----------------------------------------
    // 固定日祝日
    //----------------------------------------
    holidays.push_back({ 1, 1, "元日" });
    holidays.push_back({ 2, 11, "建国記念の日" });
    holidays.push_back({ 2, 23, "天皇誕生日" });
    holidays.push_back({ 4, 29, "昭和の日" });
    holidays.push_back({ 5, 3, "憲法記念日" });
    holidays.push_back({ 5, 4, "みどりの日" });
    holidays.push_back({ 5, 5, "こどもの日" });
    holidays.push_back({ 8, 11, "山の日" });
    holidays.push_back({ 11, 3, "文化の日" });
    holidays.push_back({ 11, 23, "勤労感謝の日" });

    //----------------------------------------
    // 第○月曜日の祝日
    //----------------------------------------
    holidays.push_back(
        { 1, nth_monday(year, 1, 2), "成人の日" });

    holidays.push_back(
        { 7, nth_monday(year, 7, 3), "海の日" });

    holidays.push_back(
        { 9, nth_monday(year, 9, 3), "敬老の日" });

    holidays.push_back(
        { 10, nth_monday(year, 10, 2), "スポーツの日" });

    //----------------------------------------
    // 春分の日・秋分の日
    //----------------------------------------
    holidays.push_back(
        { 3, vernal_equinox_day(year), "春分の日" });

    holidays.push_back(
        { 9, autumnal_equinox_day(year), "秋分の日" });

    return holidays;
}

//--------------------------------------------
// 振替休日を追加する
//--------------------------------------------
void add_substitute_holidays(int year,
    vector<Holiday>& holidays)
{
    vector<Holiday> substitutes;

    for (const auto& holiday : holidays)
    {
        // 日曜日の祝日だけを対象にする
        if (week_day(year, holiday.month, holiday.day) != 7)
            continue;

        int month = holiday.month;
        int day = holiday.day + 1;

        // 翌日以降の、祝日でない日を探す
        while (true)
        {
            if (day > static_cast<int>(last_day(year, month)))
            {
                ++month;
                day = 1;
            }

            if (!has_holiday(holidays, month, day) &&
                !has_holiday(substitutes, month, day))
            {
                substitutes.push_back(
                    { month, day, "振替休日" });
                break;
            }

            ++day;
        }
    }

    for (const auto& holiday : substitutes)
    {
        holidays.push_back(holiday);
    }
}

//--------------------------------------------
// 国民の休日を追加する
//--------------------------------------------
void add_citizen_holidays(int year,
    vector<Holiday>& holidays)
{
    for (int month = 1; month <= 12; ++month)
    {
        int last = last_day(year, month);

        for (int day = 2; day < last; ++day)
        {
            // すでに祝日なら対象外
            if (has_holiday(holidays, month, day))
                continue;

            // 前日・翌日がともに祝日なら国民の休日
            if (has_holiday(holidays, month, day - 1) &&
                has_holiday(holidays, month, day + 1))
            {
                holidays.push_back(
                    { month, day, "休日" });
            }
        }
    }
}

//--------------------------------------------
// 月間カレンダー表示
//--------------------------------------------
void print_month_calendar(int year, unsigned int month)
{
    vector<Holiday> holidays = create_holidays(year);

    add_substitute_holidays(year, holidays);
    add_citizen_holidays(year, holidays);

    cout << year << "年 " << month << "月\n\n";
    cout << "Mon Tue Wed Thu Fri Sat Sun\n";

    unsigned int first = week_day(year, month, 1);
    unsigned int last = last_day(year, month);

    unsigned int index = 1;

    //----------------------------------------
    // 月初めまで空白を表示
    //----------------------------------------
    for (unsigned int i = 1; i < first; ++i, ++index)
    {
        cout << "    ";
    }

    //----------------------------------------
    // 日付表示
    //----------------------------------------
    for (unsigned int day = 1; day <= last; ++day)
    {
        bool is_holiday = has_holiday(
            holidays, month, day);

        if (is_holiday)
        {
            cout << setw(2) << day << "* ";
        }
        else
        {
            cout << setw(3) << day << "  ";
        }

        if (index % 7 == 0)
            cout << '\n';

        ++index;
    }

    cout << '\n';

    //----------------------------------------
    // 祝日一覧
    //----------------------------------------
    cout << "祝日（*）:\n";

    bool found = false;

    for (const auto& holiday : holidays)
    {
        if (holiday.month == static_cast<int>(month))
        {
            cout << month << "月"
                << holiday.day << "日 "
                << holiday.name << '\n';

            found = true;
        }
    }

    if (!found)
    {
        cout << "この月に祝日はありません。\n";
    }

    cout << '\n';
}

//--------------------------------------------
// main
//--------------------------------------------
int main()
{
    //----------------------------------------
    // 変数宣言
    //----------------------------------------
    int year, month;

    //----------------------------------------
    // 年の入力
    //----------------------------------------
    cout << "年を入力してください：";

    if (!(cin >> year))
    {
        cout << "数字を入力してください。" << endl;
        return 1;
    }

    //----------------------------------------
    // 年の範囲チェック
    //----------------------------------------
    if (year < 1)
    {
        cout << "年は1以上を入力してください。" << endl;
        return 1;
    }

    //----------------------------------------
    // 月の入力
    //----------------------------------------
    cout << "月を入力してください：";

    if (!(cin >> month))
    {
        cout << "数字を入力してください。" << endl;
        return 1;
    }

    //----------------------------------------
    // 月の範囲チェック
    //----------------------------------------
    if (month < 1 || month > 12)
    {
        cout << "月は1～12を入力してください。" << endl;
        return 1;
    }

    //----------------------------------------
    // カレンダー表示
    //----------------------------------------
    print_month_calendar(year, month);

    return 0;
}