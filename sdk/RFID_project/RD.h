
// #define CONFIG_FILE L"D:/svn_work/c++/sdk/config.ini"
// #define CONFIG_FILEA "D:/svn_work/c++/sdk/config.ini"
#define CONFIG_FILE L"config.ini"
#define CONFIG_FILEA "config.ini"

//自定义消息
#define UM_SHOW_DATA	WM_USER + 100
#define UM_DELAY_MSG	WM_USER + 101
struct ComplexSt
{
	void* p1;
	void* p2;
};

struct TrackInfo
{
	int loopId;//车道id
	int avgSpeed;//平均速
	int userRatio;//占有率
	int avgDistance;;//车头间距
	int carNumB;//车流量   大
	int carNumM;//车流量	中
	int carNumS;//车流量    小
	int wait;//等待时间	
	int isHaveData;//车道标识
};

struct TrafficInfo
{
	char tag[4];
	char devsn[12];//设备编号
	char tim[24];//上传时间
	int len;//保留不使用
	TrackInfo info[32];
	int jpeg_file_len;//jpeg文件长度，网络字节序,用ntohl转换
	void* jpeg_file_buf;//抓拍图像数据
};

//显示的结构
struct TrackShow
{
	char tag[5];
	char devsn[11];//设备编号
	char tim[25];//上传时间

	char loopId[4];//车道id
	char avgSpeed[8];//平均速
	char userRatio[6];//占有率 0 - 1000 .3
	char avgDistance[7];;//车头时距
	char carNumB[4];//车流量   大  4byte, 最多百辆车
	char carNumM[4];//车流量	中
	char carNumS[4];//车流量    小
	char carTotal[5];//总车流量
	int wait;//等待时间	
	int isHaveData;//车道标识
};