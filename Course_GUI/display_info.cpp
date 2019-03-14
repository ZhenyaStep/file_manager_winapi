#include <Windows.h>
#include <string>

class DisplayInfo
{
public:
	DisplayInfo()
	{
	}

	std::string getDeviceName()
	{
		DISPLAY_DEVICE Display;
		Display.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(NULL, 0, &Display, EDD_GET_DEVICE_INTERFACE_NAME);
		return Display.DeviceName;
	}

	std::string getVideoCardName()
	{
		DISPLAY_DEVICE Display;
		Display.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(NULL, 0, &Display, EDD_GET_DEVICE_INTERFACE_NAME);
		return Display.DeviceString;
	}


	std::string getScreenResolution()
	{
		HDC hdcScreen = GetWindowDC(GetDesktopWindow());
		int  nXRes = GetDeviceCaps(hdcScreen, HORZRES); //����� ���������� �� �����������
		int  nYRes = GetDeviceCaps(hdcScreen, VERTRES); //����� ���������� �� ��������� 
		std::string resolution = std::to_string(nXRes) + "x" + std::to_string(nYRes);
		return resolution;
	}

	std::string getScreenRealSize()
	{
		HDC hdcScreen = GetWindowDC(GetDesktopWindow());
		int  nXRes = GetDeviceCaps(hdcScreen, HORZSIZE); //����� ���������� �� �����������
		int  nYRes = GetDeviceCaps(hdcScreen, VERTSIZE); //����� ���������� �� ��������� 
		std::string realSize = std::to_string(nXRes) + "x" + std::to_string(nYRes);
		return realSize;
	}


	std::string getScreenDriverVersion()
	{
		HDC hdcScreen = GetWindowDC(GetDesktopWindow());
		int  version = GetDeviceCaps(hdcScreen, DRIVERVERSION); //����� ���������� �� ���������� 
		std::string driverVersion = std::to_string(version);
		return driverVersion;
	}

	std::string getScreenTechnology()
	{
		HDC hdcScreen = GetWindowDC(GetDesktopWindow());
		int  screenTech = GetDeviceCaps(hdcScreen, TECHNOLOGY); //����� ���������� �� ���������� 
		std::string screenTecnhology = "underfined";
		switch (screenTech)
		{
		case DT_PLOTTER:
			screenTecnhology = "Vector plotter";
		case DT_RASDISPLAY:
			screenTecnhology = "Raster display";
		case DT_RASPRINTER:
			screenTecnhology = "Raster printer";
		case DT_RASCAMERA:
			screenTecnhology = "Raster camera";
		case DT_CHARSTREAM:
			screenTecnhology = "Character stream";
		case DT_METAFILE:
			screenTecnhology = "Metafile";
		case DT_DISPFILE:
			screenTecnhology = "Display file";
		}
		return screenTecnhology;
	}


	

	std::string getBitPixel()
	{
		HDC hdcScreen = GetWindowDC(GetDesktopWindow());
		int bits = GetDeviceCaps(hdcScreen, BITSPIXEL); //����� ���������� �� ���������� 
		std::string bitsPerPixel = std::to_string(bits);
		return bitsPerPixel;
		
	}
private:
};