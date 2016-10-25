#pragma once
#include "parestful.h"


//新闻


class CMarkup;
class CNewsMng;

class CPARestNews :
    public CPARestful
{
public:
    CPARestNews(void);
    ~CPARestNews(void);

//self
public:
    /************************************************************************
        fun:    获取新闻数据, 如果是定时消息, 则新闻列表为空
        remark:
        ret:   
    ************************************************************************/
    virtual bool GetNewsData( CNewsMng& lstNews );
    bool GetNewsItem(tagDATA_NEWS &strData);


//inherit
public:
    virtual tstring GetUrl();
protected:
    virtual MAP_STRING GetParam();
};


