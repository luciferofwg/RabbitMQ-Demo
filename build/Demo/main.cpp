#include <string>
#include <iostream>

using namespace std;

#define TIME_LEN 125

void DateTimeFormat(std::string& strDateTime)
{
    std::string strYear;
    std::string strMonth;
    std::string strDay;
    std::string strHour;
    std::string strMinu;
    std::string strSec;
    if (19 == strDateTime.length())
    {
        strYear = strDateTime.substr(0, 4);
        strMonth = strDateTime.substr(5, 2);
        strDay = strDateTime.substr(8, 2);
        strHour = strDateTime.substr(11, 2);
        strMinu = strDateTime.substr(14, 2);
        strSec = strDateTime.substr(17, 2);
    }
    else
    {
        strYear = strDateTime.substr(0, 4);
        strMonth = strDateTime.substr(5, 2);
        strDay = strDateTime.substr(8, 2);
        strHour = strDateTime.substr(11, 2);
        strMinu = strDateTime.substr(14, 2);
        strSec = strDateTime.substr(17, 2);
    }

    cout << strYear << " | "
        << strMonth << " | "
        << strDay << " | "
        << strHour << " | "
        << strMinu << " | "
        << strSec << endl;
}

std::string DateTimeFormat2(const std::string& strDateTime)
{
    std::string strRet;
    int pos = 0;
    strRet += strDateTime.substr(pos, 4); 
    pos += 4;
    strRet += "-";

    strRet += strDateTime.substr(pos, 2);
    pos += 2;
    strRet += "-";

    strRet += strDateTime.substr(pos, 2);
    pos += 2;
    strRet += " ";

    strRet += strDateTime.substr(pos, 2);
    pos += 2;
    strRet += ":";

    strRet += strDateTime.substr(pos, 2);
    pos += 2;
    strRet += ":";

    strRet += strDateTime.substr(pos, 2);
        
    return strRet;
}
int main()
{
    //std::string str1 = "2018-07-22 23:24:25";
    //DateTimeFormat(str1);

    std::string str2 = "20180722120425";
    cout << DateTimeFormat2(str2) << endl;
}