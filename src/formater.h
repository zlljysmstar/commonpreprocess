// author: Zhu, Liang. email: zhuliang2011@foxmail.com build-date: 2014-04-16

#pragma once

#define YEAR_MONTH_DAY 0
#define YEAR_MONTH 1
#define MONTH_DAY 2

#define INVALID_INDEX 10

class formater // 1992-5-1
{
private:
    unsigned int idx_year;
    unsigned int idx_month;
    unsigned int idx_day;
    bool flag;
    unsigned int select;
public:
    typedef std::string result_type;
    formater(unsigned int y,unsigned int m,unsigned int d,bool f=false,unsigned int s=YEAR_MONTH_DAY):idx_year(y),idx_month(m),idx_day(d),flag(f),select(s){}
    std::string operator()(boost::xpressive::smatch const &m) const
    {
        char s[15];
        int year = 0;
        if(select == YEAR_MONTH_DAY || select == YEAR_MONTH)
        {
            year = atoi(m[idx_year].str().c_str());
            if(flag)
            {
                if(year > 30)
                    year += 1900;
                else
                    year += 2000;
            }
        }

        if(select == YEAR_MONTH_DAY)
            sprintf(s,"%04d-%02d-%02d",year,atoi(m[idx_month].str().c_str()),atoi(m[idx_day].str().c_str()));
        else if(select == YEAR_MONTH)
            sprintf(s,"%04d-%02d",year,atoi(m[idx_month].str().c_str()));
        else if(select == MONTH_DAY)
            sprintf(s,"%02d-%02d",atoi(m[idx_month].str().c_str()),atoi(m[idx_day].str().c_str()));

        return s;
    }
};

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 number: */
