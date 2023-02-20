#pragma once
#ifdef ATXRAYDETECTOR_EXPORTS
#define ATXRAYDETECTOR_API __declspec(dllexport)
#else
#define ATXRAYDETECTOR_API __declspec(dllimport)
#endif

class XrayDoc;
class ImageMaker;
class DataContain;

class ATXRAYDETECTOR_API ATXrayDetector
{
private:
	typedef void *HW, *FT_HANDLE;
	
	typedef void(*CALLBACK_FUNCTION_Frame)();
	typedef void(*CALLBACK_FUNCTION_Event)(int,const char*);
	CALLBACK_FUNCTION_Frame callFramefunc;
	CALLBACK_FUNCTION_Event callEventfunc;

	bool mOpened;
	bool dataRead;

	int UsbIndex;

public:
	ATXrayDetector();
	~ATXrayDetector();

	int atd_GetUsbIndex();
	void atd_SetUsbIndex(int Value);


	bool atd_Connect();
	bool atd_Close();
	bool atd_Start();
	bool atd_Stop();
	bool atd_Calibration();
	bool atd_isOpen();
	bool atd_isRunning();

	void atd_SetCallback(CALLBACK_FUNCTION_Frame frame, CALLBACK_FUNCTION_Event event);

	void atd_Version();
	bool atd_HWND(HW INhMDC);
	void atd_Draw(unsigned short* buf);

	unsigned short* atd_Getadress();

	void  atd_SetScanSpeed(double Value);
	double  atd_GetScanSpeed();
	void  atd_SetIntegrationTimeRate(int Value);
	int  atd_GetIntegrationTimeRate();
	void atd_SetAutoCalibration(bool flag);
	bool atd_GetAutoCalibration();
    void atd_SetAverage(int Value);
    int  atd_GetAverage();
    void  atd_SetWidth(int Value);
    int  atd_GetWidth();
	void  atd_SetPixelSize(int Value);
	int  atd_GetPixelSize();
    void  atd_SetImageFormat(int Value);
    int  atd_GetImageFormat();
    void  atd_SetSensitivity(int Value);
    int  atd_GetSensitivity();
    void  atd_SetLinetoAvg(int Value);
    int  atd_GetLinetoAvg();
    void  atd_SetFrameHeight(int Value);
    int  atd_GetFrameHeight();
    void  atd_SetBytesPerPixel(int Value);
    int  atd_GetBytesPerPixel();
    void  atd_SetFliphorizontal(bool flag);
    bool  atd_GetFliphorizontal();
    void  atd_SetFlipvertical(bool flag);
    bool  atd_GetFlipvertical();
    void  atd_SetObjectDetection(bool flag);
    bool  atd_GetObjectDetection();
    void  atd_SetUesFilter(bool flag);
    bool  atd_GetUesFilter();
    void  atd_SetTempCorrection(bool flag);
    bool  atd_GetTempCorrection();
    void  atd_SetObjectDetectionThreshold(int Value);
    int  atd_GetObjectDetectionThreshold();
    void  atd_SetTotalLength(double Value);
    double  atd_GetTotalLength();
    void  atd_SetObjectLength(double Value);
    double  atd_GetObjectLength();
	void atd_SetBinning(int value);
	int atd_GetBinning();
	void atd_SetMode(int value);
	int atd_GetMode();
	int atd_GetImageWidth();
    bool  atd_LoadCalibrationData(const char str[]);
	bool  atd_SaveCalibrationData(const char str[]);



private:
	XrayDoc* mXrayDoc;
	ImageMaker* mImageMaker;
	DataContain* mDataContain;

	HDC mhdc;

	int ftStatus;
	FT_HANDLE ftHandle;

	unsigned short* g_pImgBuf;

	BYTE CstReadPipeNo;
	BYTE CstWritePipeNo;

	bool calibration;
	bool SendStartCommand(bool flag=false);
	bool SendStopCommand();

	void init();
	void checkOpen();
	void Write();
	void Read();
	bool Calibration();

};

