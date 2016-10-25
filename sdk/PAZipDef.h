#pragma once

struct  PATH_INFO
{
public:
    PATH_INFO(){}   /*stl 容器需要*/
    PATH_INFO(const tstring& szPath, const tstring& szFolder)
    {
        strPathSoft = szPath;
        strFolder = szFolder;
    }
    tstring     strPathSoft;        //配置文件 最终的路径信息
    tstring     strFolder;          //配置文件 文件夹名
};

enum ENUM_PACK_TYPE
{
    type_im = 0,        //优绘
    type_mm,            //优漫
};

enum ENUM_PATH_TYPE
{
    pt_mm_panel = 0,        //优漫
    pt_mm_color,      
    pt_mm_brush,      
    pt_im_panel,        //优绘
    pt_im_color,      
    pt_im_brush,      
};

enum ENUM_PACK_ERROR
{
    err_ok = 0,
    err_pack_no_exist,
    err_unKnown,
};