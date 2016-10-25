//显示的结构
#pragma once
#include "PiTypeDef.h"
#include <vector>
#include <deque>
#include <list>
#include "define_gnl.h"
/************************************************************************
    程序有3种项目配置:
        debug:          使用真实数据, 数据不发送给平台
        Test_Debug:     使用模拟数据, 数据不发送给平台, 模拟出一堆实时数据
        Release_Debug:  实际发布版本
/************************************************************************/
using namespace std;
struct TAG_DATA_DESC;
//加密函数
typedef  bool (_stdcall *FunRFIDEncrypt)(PCSTR, char*);

#define NUM_IMITATE_READER          "30"      //模拟读卡器个数
#define DEBUG_DATA  //调试数据， 针对数据问题的调试, e.g: 前段发来时间


#ifdef Release_Debug    //发布， 实际工作版本
        
#elif defined TEST_DEBUG    //自己调试， 没设备数据版本

    #define DEBUG_ALL   //调试版本, 不报警
    #define RTDATA_IMITATE
    #define RT_DATA_SELF    //RT数据只发给自己
    #define TEST_UDP    //调试UDP, 关闭 发送前检测连接

#else   //有设备数据， 要调试
    #define RT_DATA_SELF    //RT数据只发给自己
    #define DEBUG_ALL   //调试版本, 不报警
#endif

/************************************************************************/
/*  日志输出控制开关
/************************************************************************/
#define BOOL_OUTPUT_NATIVE_TIMESTAMP            //原生数据自带的时间戳转成1970-ms
//#define BOOL_OUTPUT_TWO_TIMES_SPEND           //收到这个包距离上个包的时间


#define MSG_TYPE_RTDATA                 1       //消息类型- 实时标签数据
//#define LOG_THREAD                            //线程中写日志






#define FILL_TIME 2 //补发UDP数据的周期数

#define FLAG_BK     3   //布控标记
#define FLAG_CK     2   //撤控标记

#define NUM_READER_MAX_ANTENNA              4       //一个读卡器最大支持的天线数
#define NUM_CIRCLE_LANE_EPC_NUM_MAX         4       //一个读卡器一个车道一个周期最大能读到的EPC数
#define NUM_CIRCLE_EPC_NUM_MAX              20       //一个读卡器一个周期最大能读到的EPC数
#define DIRECTION_EAST_TO_WEST              4       //方向,东向西
#define DIRECTION_LOW                       2       // 天线ID小的那个方向
#define NUM_READER_NODATA_TIMEOUT           3       //读卡器n秒没来数据了,方便做一些操作

#define NUM_READER_LIMIT                    80	    //读写器数量


#define IS_HIGH_LANE(laneId)    (laneId >= 3 && laneId <= 4)

#define IP_SPECIAL                          "192.192.12.22"//特殊点，处理两个方向
#define NUMBER_DEVICE_HIGH_LANE             2//特殊点, 天线ID高的那个方向的天线数
#define TABLE_LOCAL_DISPOSE                 "T_ITGS_LOCAL_DISPOSE"


#define NUM_READER_RECV_CACHE_MAX           4000        //一个读卡器接收缓冲区最大能存放原始数据的条数
#define NUM_READER_DB_CHCHE_MAX             20000       //一个读卡器数据库队列最大能存放的条数
#define NUM_READER_DB_CACHE_RATIO_LOG       1000        //在原来DB队列个数的基础上, 每增加该系数写一次log
#define NUM_UPDATE_UI_CIRCLE                5000        //多久的周期更新界面的剩余条数,单位ms
#define NUM_LOG_THREAD_SLEEP                40          //写日志线程延时
//序列
#define SEQ_RFID_PASSCAR                "SEQ_RFID_PASSCAR"
#define SEQ_RFID_ALARM                  "SEQ_RFID_ALARM"

//3个ip, 会重复读到一辆车的点
#define STR_FILTER_READER_IP_LEFT              "192.192.12.23"
#define STR_FILTER_READER_IP_MIDDLE            "192.192.12.22"
#define STR_FILTER_READER_IP_RIGHT             "192.192.12.21"

#define STR_DB_TEMP_SEQUENCE                    L"SequenceId"
// 重复读EPC的分别是哪几个天线
#define NUM_FILTER_READER_LANE_LEFT             1
#define NUM_FILTER_READER_LANE_MIDDLE_LEFT      1
#define NUM_FILTER_READER_LANE_MIDDLE_RIGHT     4
#define NUM_FILTER_READER_LANE_RIGHT            1

//过滤epc被两个读卡器读到
#define NUM_FILTER_READER_EPC_CACHE             40
#define NUM_FILTER_EPC_LIST                     10             //过滤的epc列表个数

const UINT TAG_DATA_REMAIN_SIZE = 21;
const byte FRONT_FIX_DATA_SIZE  = 9;

//入库相关
const UINT NUM_BATCH_WRITE_DB               =     4;         //车辆入库至少10辆车再入库

//UDP 发送周期
#define UDP_CIRCLE_250

#ifdef UDP_CIRCLE_250
    const UINT NUM_SEND_UDP_CYCLE       = 250;//发送UDP周期
    const UINT NUM_SEND_CIRCLE_TIMES    = 4;//一秒发几个周期
#else
    const UINT NUM_SEND_UDP_CYCLE       = 500;//发送UDP周期
    const UINT NUM_SEND_CIRCLE_TIMES    = 2;//一秒发几个周
#endif


#ifdef TEST_DEBUG
    //#define INFO(msg) OutputDebugStringA(msg)
#elif defined(LOG_THREAD)
    #define INFO(msg) CSingleLog::PutThread(msg);
#else
    #define INFO(msg) CSingleLog::WriteLog(msg);
#endif

//内存报警类型结构
class ALARM_RECORD_DATA
{
public:
    void Reset()
    {
        strID       = "";
        strHPHM     = "";
        strHPYS     = "";
        strBCKBJ    = "";
        strBJLX     = "";
    }
    string  strID;
    string  strBJLX;//报警类型
    string  strHPHM;
    string  strHPYS;
    string  strBCKBJ;
};

struct TrackShow
{
	wstring id;//id
	wstring deviceId; //设备编号
	//WCHAR deviceName;//设备名称
	//WCHAR direction;;//方向编号
	wstring laneId;//车道id

	wstring tagCode;//标签代码
	wstring CarCardID;//号牌号码
    wstring strHPYS;//号牌颜色
	wstring CarKind;//车牌种类
	wstring speed;//车辆速度
	wstring EnterTime;//进入时间
	wstring LevelTime;//离开时间
	wstring responseCount;//感应次数
    wstring strCount;   //该车道DB队列剩余条数
    wstring carCount;    //缓冲里有多少辆车
};


struct RECV_DATA_TCP
{
	u_short err;//错误码
	byte	optType;
	byte	epcLen;//epc长度
	byte	epc[64];//
	byte	ret;//结果
	byte	dataLen;//数据长度
	byte	data[64];//数据
	byte	antennaId;//天线ID
	byte	byProtocol;//协议
	byte	epcRCnt;//EPC读次数
	byte	user;//成功次数
	byte	timestamp[14];//时间戳
};

//***************** 新版本协议 *****************
//新的长连接, 一次性250ms全部发上来

#pragma pack(push)
#pragma pack(1)
struct TAG_DATA
{
    byte				byTagCount;//标签个数
    TAG_DATA_DESC*		pTddArr;//标签数据描述符数组
    byte				ReaderNameLen;//读写器名称长度
    char*				ReaderName;//读写器名称
};

struct RECV_DATA_TCP_SET
{
	short				msgVer;//消息版本
	byte				msgType;//消息类型, 1表示实时标签数据，2表示离线标签数据
	UNINT				msgSN;//消息序列号
	short				uTagDataLen;//消息数据部分长度
	TAG_DATA            tagData;//消息数据部分
    void*               pTAG_DATA;//保存释放的标签相关数据指针
};


struct TAG_DATA_DESC
{
	byte	byEpcLen;//Epc长度
	char*	pEpc;//Epc
	byte	byTagMemDataLen;//标签内存数据长度
	void*	pTagMemData;//内存数据
	byte	byAntennaID;//天线
    byte	byReadCnt;//读次数
	byte	byProtocal;//协议
	byte	byRssi;//信号强度
	char	szTimeStamp[17];//时间戳
};
#pragma pack(pop)
//***************** 新版本协议 *****************



//UDP发送数据
typedef struct tagSendData
{
    string  strDZBQH;	//电子标签号RFID
    byte	nCXFX;		//车行方向
    byte	nCDH;		//车道号/天线号
    int		nGYCS;		//感应次数

    time_t tSCSJ;	    //采集时间
    DWORD dwIndex;      //包流水号, 同一个250ms周期的流水号一样
    void Reset()
    {
        nCXFX       = 0;
        nCDH        = 0;
        nGYCS       = 0;
        dwIndex     = 0;
        strDZBQH    = "";
    }
    bool operator< (const tagSendData& sd) const
    {
        return strDZBQH < sd.strDZBQH;
    }
}SENDDATA;

//一辆车的数据, 包括总读到次数, 进入时间和离开时间
class SDPassCar
{
public:
    /*SDPassCar()
    {
        
    };*/
    SENDDATA sd;
    time_t  tNewTime;  //该车最新的时间
    bool bPass;        //如果通过, 则不更新该车
};

//判决车辆是否通过
struct timeDecidePassCar
{
    time_t  tNow;
    time_t  tCarTimeout;
    //UINT    nPassCarCount;      //通过的车辆数
};

//类型定义
typedef vector<SENDDATA> SendDataList;  //一个周期的所有epc数据
typedef deque<SENDDATA> DqSendData;
typedef list<SENDDATA>  LISTSendData;
typedef deque<SDPassCar> DqPassCar;

//UDP缓冲结构， 用来补发漏掉的数据
class CACHE_SD
{   
public:
    byte laneId;//车道号
    byte byDirection;//方向,一个读卡器多个方向中需要用到
    string EPC;
    byte remainTime;//剩余次数, delete this record when this field == 0
    void Reset()
    {
        laneId = 0;
        EPC  = "";
        byDirection = 0;
        remainTime = FILL_TIME;
    }
};

//EPC结构体信息, 用于车被两个读卡器读到时的过滤
class EpcInfo
{
public:
    byte    byReaderID;
    byte    byAntennaId;
    string  strEpc;

    void Reset()
    {
        byReaderID  = 0;
        byAntennaId = 0;
        strEpc      = "";
    }
};

class PassCarData
{
public:
    string strDeviceID; //设备ID
    string strDDBH;     //地点编号
    string strDZBQH;    //EPC
    string strSBMC;     //设备名称
    string strHPHM;     //号牌号码
    string strHPYS;     //号牌颜色
    string strHPZL;     //号牌种类
    byte    byFX;       //方向
    byte    byCDH;      //车道号
    string  strBeginTime;
    string  strEndTime;
    ULONG   lGYCS;      //感应次数
    void Reset()
    {
        strDeviceID     = "";
        strDDBH         = "";    
        strDZBQH        = "";   
        strSBMC         = "";    
        strHPHM         = "";    
        strHPYS         = ""; 
        strHPZL         = "";
        strBeginTime    = "";
        strEndTime      = "";
        byFX            = 0;
        byCDH           = 0;
        lGYCS           = 0;

    }
};


typedef vector<CACHE_SD>        vectorCache;
typedef deque<SendDataList*>    LaneQueue;      //一个车道所有周期的数据