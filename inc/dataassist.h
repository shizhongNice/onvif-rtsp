#ifndef _DATA_ASSIST_H_
#define _DATA_ASSIST_H_

#include <time.h>
#include <sys/time.h>

#define IW_ENCODING_TOKEN_MAX 		128 //64

enum ENCRYPTYPE
{
	ENCRY_NONE,
	ENCRY_WEP_OPEN,
	ENCRY_AES,
	ENCRY_TKIP,
	ENCRY_NR
};

enum ETHATTR
{
	TYPE_IP =	0,
	TYPE_NETMASK = 1,
	TYPE_GATEWAY = 2,
	TYPE_PRIO	= 3,
	TYPE_MAX = 4
};

typedef enum _fs_type
{
	FS_TYPE_FAT32,
	FS_TYPE_UNKNOWN,
	FS_TYPE_EXFAT,
	FS_TYPE_MAX
}fs_type_e;

typedef enum device_mmc_err_type
{
	DEV_MMC_ERR_TYPE_NONE 			=  0,
	DEV_MMC_ERR_TYPE_NO_DEVICE 		= -1,
	DEV_MMC_ERR_TYPE_CHECK_FAIL 	= -2,
	DEV_MMC_ERR_TYPE_MOUNT_FAIL 	= -3,
	DEV_MMC_ERR_TYPE_FORMAT_FAIL	= -4,
	DEV_MMC_ERR_TYPE_NOT_OPEN 		= -5,
	DEV_MMC_ERR_TYPE_NOT_CLOSE 		= -6,
}mmc_err_type_e;

typedef enum sdev_err_state_sub
{
	SDEV_ERR_STATE_NONE          	= 0,
	SDEV_ERR_STATE_RW_FAIL         	= 1,
	SDEV_ERR_STATE_SPEED_FAIL      	= 2,
	SDEV_ERR_STATE_FILESYS_FAIL 	= 3,
}sdev_err_state_e;

typedef enum device_type_
{
	DEV_TYPE_MMC = 0,				//tf卡
	DEV_TYPE_UDISK,					//U盘
	DEV_TYPE_TTYUSB,				//2303串口
	DEV_TYPE_HID,					//HID设备
	DEV_TYPE_MAX_NUM,
}dev_type_e;

//设备插入状态
typedef enum device_insert_state
{
	DEV_STATE_OUT = 0,				//设备拔出
	DEV_STATE_INSERT,				//设备插入
}dev_insert_state_e;

//自动夜视模式下到夜视状态状态
typedef enum night_mode_state
{
	NIGHT_MODE_NIGHT_STATE = 0,		//夜视状态
	NIGHT_MODE_DAY_STATE = 1,		//日间状态
	PHOTO_ALARM_DISABLE = 2,		//由于软光敏模块变化引起的暂时禁止报警
	PHOTO_ALARM_ENABLE = 3,			//软光敏模块变化引完成，不再禁止报警

}night_mode_state_e;

//设备信息
typedef struct sdk_dev_params
{
	char path[64];					//文件路径
	fs_type_e fsType;				//文件系统
	int fd;							//文件描述符
}sdk_dev_params_s;

//pir有无状态
typedef enum pir_state_check
{
	PIR_STATE_DISAPPERA = 0,		//人消失
	PIR_STATE_APPERA = 1,			//人出现
}pir_state_check_e;

//报警有无状态
typedef enum alarm_type_tag
{
	HL_ALARM_TYPE_NULL = 0,
	HL_ALARM_TYPE_MOTION = 1,		///< 视频移动报警
	HL_ALARM_TYPE_AUDIO,			///< 声音骤起报警
	HL_ALARM_TYPE_OUT,				///< 门磁??
	HL_ALARM_TYPE_SMOKE,			///< 声音烟雾报警
	HL_ALARM_TYPE_CO,				///< 声音一氧化碳报警
	HL_ALARM_TYPE_IOT= 11,          //iot下发指令，生成的报警视频
	HL_ALARM_TYPE_MAX,
}sdk_alarm_type_e;

//报警有无状态
typedef enum alarm_state_check
{
	ALARM_STATE_DISAPPERA = 0,		//报警消失
	ALARM_STATE_APPERA    = 1,		//报警发生
}sdk_alarm_state_e;

//报警是否要转动的状态
typedef enum alarm_rotation_state
{
	ALARM_NO_ROTATION = 0,		//无需转动
	ALARM_NEED_ROTATION,		//应当转动
	ALARM_LONGTIME_NO_CHANGED,	//长时间无变化
}sdk_rotation_state_e;

//框的坐标信息【单位像素, 画布默认以低清通道码流的像素作为坐标系，左上角为坐标0点】
typedef struct alarm_rect_info
{
	int xMin;						//左上像素位置
	int xMax;						//右上像素位置
	int yMin;						//左下像素位置
	int yMax;						//右下像素位置
	int dx;							//中心像素位置
	int dy;							//中心像素位置
}alarm_rect_info_s;

//云台要转到的绝对角度信息【ak平台使用】
typedef struct alarm_angle_info
{
	sdk_rotation_state_e state;		//是否要转动的状态
	int xAngle;						//水平绝对角度
	int yAngle;						//垂直绝对角度
}alarm_angle_info_s;

//报警对象信息[此结构体为ai人形/人脸/车辆检测预留][可后续拓展]
typedef struct alarm_obj_info
{
	float objCredibility;			//对象可信度
	int   objName;					//对象名
	char  reserve[16];				//拓展预留
}alarm_obj_info_s;

//报警信息
typedef struct sdk_alarm_info
{
	sdk_alarm_type_e  type; 		//报警类型
	sdk_alarm_state_e state;		//报警状态
	alarm_rect_info_s rectInfo;		//框的坐标信息【单位像素, 画布默认为360p】
	alarm_angle_info_s angleInfo;	//云台要转到的绝对角度信息【ak平台使用】
	alarm_obj_info_s  objInfo;		//报警对象信息
}alarm_info_s;

/** OSD图层类型 **/
typedef enum osd_layer_type
{
	HL_OSD_TYPE_TIMESTAMP = 0,		//时间戳水印
	HL_OSD_TYPE_LOGO,				//logo水印
	HL_OSD_TYPE_BRIGHTNESS,
	HL_OSD_TYPE_RECT,				//矩形框水印0
	HL_OSD_TYPE_RECT1,				//矩形框水印1
	HL_OSD_TYPE_RECT2,				//矩形框水印2
	HL_OSD_TYPE_NUM,
}osd_layer_type_e;

/** OSD图层使能MASK **/
#define HL_OSD_LAYER_TIMESTAMP	(1<<HL_OSD_TYPE_TIMESTAMP)	//时间戳水印
#define HL_OSD_LAYER_LOGO		(1<<HL_OSD_TYPE_LOGO)		//logo水印
#define HL_OSD_LAYER_BRIGHTNESS	(1<<HL_OSD_TYPE_BRIGHTNESS)	//brightness水印
#define HL_OSD_LAYER_RECT		(1<<HL_OSD_TYPE_RECT)		//矩形框水印0
#define HL_OSD_LAYER_RECT1		(1<<HL_OSD_TYPE_RECT1)		//矩形框水印1
#define HL_OSD_LAYER_RECT2		(1<<HL_OSD_TYPE_RECT2)		//矩形框水印2

//得到对应图层类型是否使能
#define HL_OSD_LAYER_SWITCH_STATE(layerType, layerMask)   (((layerMask)>>(layerType))&1)

/** 视频通道枚举 **/
typedef enum VIDEO_CHANNEL
{
	HL_VIDEO_CHN_MAIN = 0,			///< main stream  1
	HL_VIDEO_CHN_2END,				///< extra stream 2
	HL_VIDEO_CHN_3IRD,				///< extra stream 3
	HL_VIDEO_CHN_JPEG,				///< JPEG stream
	HL_VIDEO_CHN_NUM
}v_channel_e;

/** 音频通道枚举 **/
typedef enum AUDIO_CHANNEL
{
	HL_AUDIO_CHN_MAIN = 0,			///< not encode channel
	HL_AUDIO_CHN_NUM	///< audio only one channel,but can get pcm and encode_frame in this channel
}a_channel_e;

/** 视频帧头信息类型枚举 **/
typedef enum VIDEO_NALU_TYPE
{
	HL_NALU_TYPE_PSLICE = 1, 		///< PSLICE types
	HL_NALU_TYPE_ISLICE = 5, 		///< ISLICE types
	HL_NALU_TYPE_SEI    = 6, 		///< SEI types
	HL_NALU_TYPE_SPS    = 7, 		///< SPS types
	HL_NALU_TYPE_PPS    = 8, 		///< PPS types
}v_nalu_type_e;

/** 音频帧类型枚举 **/
typedef enum VIDEO_FRAME_TYPE
{
	HL_I_FRAME 	= 0,				//I帧
	HL_P_FRAME 	= 1,				//P帧
	HL_B_FRAME 	= 2,
	HL_S_FRAME 	= 3,
	HL_DATA_TEXT = 5,
}v_frame_type_e;

/** 视频编码类型枚举 **/
typedef enum VIDEO_ENCODE_TYPE	//当该通道不进行编码时，编码类型可随意设置
{
	HL_V_ENCODE_TYPE_YUV = 0,		///< not encode
	HL_V_ENCODE_TYPE_H264,
	HL_V_ENCODE_TYPE_H265,
	HL_V_ENCODE_TYPE_JPEG,
	HL_V_ENCODE_TYPE_NUM
}v_enc_type_e;

/** 音频编码类型枚举 **/
typedef enum AUDIO_ENCODE_TYPE	//当该通道不进行编码时，编码类型可随意设置
{
	HL_A_ENCODE_TYPE_PCM = 0,		///< not encode
	HL_A_ENCODE_TYPE_AAC,
	HL_A_ENCODE_TYPE_G711A,
	HL_A_ENCODE_TYPE_AMR,
	HL_A_ENCODE_TYPE_NUM
}a_enc_type_e;

/** 视频编码比特率控制类型枚举 **/
typedef enum VIDEO_ENCODE_BITRATE_CTRL_TYPE
{
	HL_V_BITRATE_TYPE_CBR = 0,		///< fixed bitrate
	HL_V_BITRATE_TYPE_VBR,			///< Variable bitrate
	HL_V_BITRATE_TYPE_MBR,			///< mix bitrate
	HL_V_BITRATE_TYPE_NUM
}v_enc_bit_type_e;

/** 视频图像质量枚举 **/
typedef enum VIDEO_ENCODE_IQ_TYPE
{
	HL_IMG_QUALITY_WOREST  = 1,		///< 最坏
	HL_IMG_QUALITY_GENERAL = 3,		///< 一般
	HL_IMG_QUALITY_GOOD    = 4,		///< 良好
	HL_IMG_QUALITY_BEST    = 6,		///< 最好
}v_enc_iq_type_e;

/** 音频噪声抑制等级枚举 **/
typedef enum AUDIO_NOISE_SUPPRESSION_LEVEL
{
	HL_A_NS_DISABLE = 0,			///< 禁能此功能
	HL_A_NS_LEAST   = 1,			///< 最小
	HL_A_NS_MEDIUM  = 2,			///< 中等
	HL_A_NS_BEST    = 3,			///< 最大
}a_ns_level_e;

/** 视频分辨率枚举 **/
typedef enum VIDIO_RESOLUTION
{
	HL_RESOLUTION_180P = 0,			///< 320 * 180
	HL_RESOLUTION_CIF,				///< 352 * 288
	HL_RESOLUTION_270P,				///< 480 * 270
	HL_RESOLUTION_360P,				///< 640 * 360
	HL_RESOLUTION_VGA,				///< 640 * 480
	HL_RESOLUTION_720P,				///< 1280 * 720
	HL_RESOLUTION_1080P,			///< 1920 * 1080
	HL_RESOLUTION_2048x1024,		///< 2048 * 1024 (2:1 1080p)
	HL_RESOLUTION_2304x1296,		///< 2304 * 1296 (2k)
	HL_RESOLUTION_2560x1440,		///< 2560 * 1440 (2.5k)
	HL_RESOLUTION_2592x1520,		///< 2592 * 1520 
    HL_RESOLUTION_2592x1536,		///< 2592 * 1536
    HL_RESOLUTION_2592x1944,		///< 2592 * 1944
    HL_RESOLUTION_2688x1520,		///< 2688 * 1520
    HL_RESOLUTION_2716x1524,		///< 2716 * 1524
	HL_RESOLUTION_3840x1920,		///< 3840 * 1920 (2:1 4k)
	HL_RESOLUTION_3840x2160,		///< 3840 * 2160 (4k)

	HL_RESOLUTION_NUM
}v_resolution_e;

typedef enum VIDIO_OSD_COLOR
{
	HL_OSD_BLACK = 0,	//= 0xff000000, /**< 黑色 */
	HL_OSD_WHITE,		//= 0xffffffff, /**< 白色 */
	HL_OSD_RED	,		//= 0xffff0000, /**< 红色 */
	HL_OSD_GREEN,		//= 0xff00ff00, /**< 绿色 */
	HL_OSD_BLUE	,		//= 0xff0000ff, /**< 蓝色 */
	HL_OSD_YELLOW,		//= 0xffffff00, /**< 黄色 */
}v_osd_color_e;

/** 连网状态枚举 **/
typedef enum NETWORK_STATE
{
	SDKNET_STATE_OFFLINE = 0,
	SDKNET_STATE_ONLINE,
}network_state_e;

/** 视频帧信息结构体 **/
typedef struct VIDEO_FRAME_INFO
{
	unsigned char *pBuf;			///< frame address
	unsigned int   len;             ///< frame len
	unsigned int   frameSeq;		///< frame sequence number
	struct timeval timestamp;		///< timestamp
	v_frame_type_e frameType;		///< frame Type, like: I P B
	unsigned char  reserve[16];		///< 不同平台特殊信息空间预留
}v_frame_info_s;

/** 音频帧信息结构体 **/
typedef struct AUDIO_FRAME_INFO
{
	unsigned char *pBuf;			///< frame address
	unsigned int len;             	///< frame len
	unsigned int frameSeq;			///< frame sequence number
	struct timeval timestamp;		///< timestamp
	unsigned char  reserve[16];		///< 不同平台特殊信息空间预留
}a_frame_info_s;

/** SDK时间结构体 **/
typedef struct SDK_TIME
{
	int  year;		///< year
	int  month;		///< month January = 1, February = 2, and so on.
	int  day;		///< day
	int  wday;		///< weekday, Sunday = 0, Monday = 1, and so on
	int  hour;		///< hour
	int  minute;	///< min
	int  second;	///< sec
	int  isdst;		///< 1: using daylight saving time
}sdk_time_s;

/** 视频参数结构体 **/
typedef struct SDK_VIDEO_PARAM
{
	int kbps;						///< 期望最大比特率
	int fps;						///< 帧率
	v_resolution_e resulotion;		///< 分辨率
	int flipVertical;				///< 上下翻转标志(0-not flip, 1-do flip)
	int flipHorizontal;				///< 左右翻转标志(0-not flip, 1-do flip)
	int yuvEnable;					///< 通道是否取未编码视频
	int encEnable;					///< 通道是否取编码后视频
	int osdEnable;					///< OSD使能
	v_enc_type_e encodeType;  		///< 编码类型
	v_enc_bit_type_e bitType;		///< 编码比特率控制类型
	int gop;						///< 编码帧组长度（帧率的倍数）
	int yuvBufSize;					///< 帧buffer大小
	int encodeBufSize;				///< 帧buffer大小
	int jpegEnable;					///< JPG使能
	int algoEnable;					///< 视频检测算法使能
}sdk_video_param_s;

/** 音频参数结构体 **/
typedef struct SDK_AUDIO_PARAM
{
	int sampleRate;					///< 采样率
	int bitWidth;					///< 采样宽度
	int fps;						///< 帧率
	int trackType;					///< 声道数量 1 is mono, 2 is stereo
	int aecEnable;					///< 消回声使能标志
	a_ns_level_e nsLevel;			///< 噪声抑制等级
	int pcmEnable;					///< 通道是否取未编码音频
	int encEnable;					///< 通道是否取编码后音频
	a_enc_type_e encodeType;		///< 编码类型
	int gain;						///< 增益值
	int volume;						///< 音量
	int pcmBufSize;					///< 帧buffer大小
	int encodeBufSize;				///< 帧buffer大小
}sdk_audio_param_s;

/** spk参数结构体 **/
typedef struct SDK_SPEAKER_PARAM
{
	int sampleRate;					///< 采样率
	int bitWidth;					///< 采样宽度
	int fps;						///< 帧率
	int trackType;					///< 声道数量
	int gain;						///< 增益值
	int volume;						///< 音量
	int pcmBufSize;					///< 帧buffer大小
	int cacheSec;					///< 允许缓存的秒数(如10秒，则播放十秒之内的文件，会一次性缓存，不会阻塞播放)
}sdk_speaker_param_s;

#ifndef _SDK_STRUCT_OSD_PARAM_
#define _SDK_STRUCT_OSD_PARAM_

/** 水印层参数结构体 **/
typedef struct _osd_param
{
	int layerSwitch;				//图层选择，启用的层置1，不启用的置0
	//timestamp
	int time_x_StartingPoint;		//x起点
	int time_y_StartingPoint;		//y起点
	int time_scalNum;				//缩放比例分子，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（置3）
	int time_scalDen;				//缩放比例分母，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（也置1）
	//logo
	int logo_x_StartingPoint;		//x起点
	int logo_y_StartingPoint;		//y起点
	int logo_scalNum;				//缩放比例分子，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（置3）
	int logo_scalDen;				//缩放比例分母，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（也置1）
	//brightness
	int brightness_x_StartingPoint;		//x起点
	int brightness_y_StartingPoint;		//y起点
	int brightness_scalNum;				//缩放比例分子，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（置3）
	int brightness_scalDen;				//缩放比例分母，一般主码流不需缩放（置1），次码流若为360p则缩放3倍（也置1）
}osd_param_s;

/** 水印层数据更新结构体 -- 时间戳  **/
typedef struct _osd_update_timestamp
{
	struct tm curLocaltime;

}osd_update_timestamp_s;

/** 画框的数据结构【单位像素, 画布默认以低清通道码流的像素作为坐标系，左上角为坐标0点】  **/
typedef struct _osd_update_rect
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	int linewidth;
	v_osd_color_e color;
}osd_update_rect_s;
#endif

/** 云台转动方向 **/
#define MOTOR_X_DIR_LEFT	0x1
#define MOTOR_X_DIR_RIGHT	0x2
#define MOTOR_Y_DIR_UP		0x1
#define MOTOR_Y_DIR_DOWN	0x2
#define MOTOR_X_DIR_STOP	0x0
#define MOTOR_Y_DIR_STOP	0x0

#define MOTOR_X_ANGLE_MAX	350		// X轴转动最大角度
#define MOTOR_Y_ANGLE_MAX	40	 	// Y轴转动最大角度

/** 云台任务失败原因 **/
typedef enum MOTOR_TASK_ERR
{
	MOTOR_TASK_ERR_NONE 	=  0,	// 
	MOTOR_TASK_INIT_ERR 	= -1,	// 初始化失败
	MOTOR_TASK_OPEN_ERR 	= -2,	// 打开设备失败
	MOTOR_TASK_PARA_ERR 	= -3,	// 参数错误
	MOTOR_TASK_INT_ERR 		= -4,	// 任务被打断
	MOTOR_TASK_CANCEL_ERR 	= -5,	// 任务被取消（当前执行着更高优先级任务）
	MOTOR_TASK_TIMEOUT_ERR 	= -6,	// 执行任务超时
}motor_task_err_e;

/** 云台任务优先级 **/
typedef enum MOTOR_TASK_PRIORITY
{
	MOTOR_TASK_PRIORITY_RESET = 0,
	MOTOR_TASK_PRIORITY_HIGH,
	MOTOR_TASK_PRIORITY_NORMAL,
	MOTOR_TASK_PRIORITY_LOW,

	MOTOR_TASK_PRIORITY_NUMBER,	//优先级数量
}motor_task_priority_e;

typedef int(*videoAlgoCallBack)(void);
typedef int(*videoFrameCallBack)(v_frame_info_s *pFrameInfo);
typedef int(*audioFrameCallBack)(a_frame_info_s *pFrameInfo);
typedef int(*fTalkCallBack) (long lRealHandle, unsigned char* pBuffer, int nBufLen);
typedef int(*FactoryResetCallBack)(void);
typedef int(*SetupKeyDownCallback)(void);
typedef int(*DeviceMonitorCallback)(dev_insert_state_e state);
typedef int(*PirStateCheckCallback)(pir_state_check_e state);
typedef int(*NightStateCheckCallback)(night_mode_state_e state);
typedef int(*AlarmStateCheckCallback)(alarm_info_s *pInfo);
typedef int(*MotorTaskSuccessCallback)(float Xangle, float Yangle);
typedef int(*MotorTaskFailCallback)(int errCode);
#endif
