#include "utils.h"
#include <Arduino.h>
#include <stack>
using namespace std;
long timeDistance(long val,int dir) {
    stack<long> qs;
    long inc;
    if (val==0 ) {
        return dir==-1?0:1;
    }
    qs.emplace(1);
    qs.emplace(60); // 1 min
    qs.emplace(qs.top()*60); // 1 hour
    qs.emplace(qs.top()*24); // 1 day
    qs.emplace(qs.top()*30); // 1month
    inc=qs.top();
    // qs(60, 60 * 60, 60 * 60 * 24);
    while (!qs.empty())
    {
        long v=qs.top();
        qs.pop();
        if (abs(val)>=v) {
            inc=v;
            if (inc==3600) {
                if (val>=8*v) {
                    inc=v;
                } else {
                    inc=60*5;
                }
                
            }
            break;
        }
    }
    long rs=val+dir*inc;
    if (rs==0) {
        if (!qs.empty()) {
            rs=val+dir*qs.top();
        }
    }
    
    return rs;
}
long getTime(char *rs, long val, int dir)
{

    char units[] = {'s', 'm', 'h', 'd', 'm', 'y'};
    long unitsMap[] = {1, 60, 60 * 60, 60 * 60 * 24, 60 * 60 * 24 * 30, 60 * 60 * 24 * 30 * 12};

    if (val==0) {
        strcpy(rs,"0s");
        return 0;
    }
    int i = 0;
    int lastI = 0;

    while ((val / unitsMap[i] != 0) && (i < 6))
    {
        lastI = i;
        i++;
    }
    int nval;
    auto inc = dir * unitsMap[lastI];
    if (units[lastI] == 'h')
    {
        inc = dir * 5 * unitsMap[lastI - 1];
    }
    if (units[lastI] == 'd')
    {
        inc = dir * 1 * unitsMap[lastI - 1];
    }
    nval = val + inc;
    if ((nval == 0) && (lastI > 0))
    {
        lastI--;
        nval = val + dir * unitsMap[lastI];
    }
    val = nval;
    i = 0;
    while ((val / unitsMap[i] != 0) && (i < 6))
    {
        lastI = i;
        i++;
    }

    char buf1[15];
    int cu = val / unitsMap[lastI];
    itoa(cu, buf1, 10);
    char buf2[15] = {'\0'};
    char buf3[15] = {'\0'};

    if (lastI > 0)
    {
        int prev = (val - (cu * unitsMap[lastI])) / unitsMap[lastI - 1];
        if (prev != 0)
        {
            itoa(prev, buf2, 10);
            sprintf(buf3, "%s%c", buf2, units[lastI - 1]);
        }
    }

    // rs=(char*)malloc(strlen(buf+1));
    sprintf(rs, "%s%c%s", buf1, units[lastI], buf3);
    return val;
    // strcpy(rs,buf);
}