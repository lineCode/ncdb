#pragma once
class CPiRandom
{
public:
    CPiRandom(void);
    ~CPiRandom(void);
public:
    bool Init();
    static int GetRandomNum(int nMax);
};

