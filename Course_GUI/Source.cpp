#include <Windows.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <cmath>
#include "SystemVersionInfo.cpp"
#include "ProcessorInfo.cpp"
#include "RunningProcesses.cpp"
#include "processor_names.cpp"
#include "HDD.cpp"
#include "display_info.cpp"
//#include "temperature_info.cpp"





#define ID_BUTTON_PROCESS_LIST 3001
#define ID_BUTTON_PROCESS_MEMORY_INFO 3002
#define ID_BUTTON_PROCESS_PROCESSOR_INFO 3003
#define ID_BUTTON_PROCESS_SYSTEM_INFO 3004
#define ID_BUTTON_PROCESS_HDD_INFO 3005

#define ID_BUTTON_UPDATING_ALLOW 3006

#define OPERATION_PROCESSOR_INFO 0
#define OPERATION_RUNNING_PROCESSES 1
#define OPERATION_MEMORY_INFO 2
#define OPERATION_SYSTEM_INFO 3
#define OPERATION_HDD_INFO 4

LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName);
void showButtonMenu();
int close(HWND hWnd);
int showSystemInfo();
int showProcessorInfo();
int showMemoryInfo();
int showRunningProcessesList(std::vector<std::string> runningProcList_names,
	std::vector<DWORD> runningProcList_id);
int refreshScreen();
int resize(HWND hWnd);
int timerTick(HWND hWnd);
int freeMemory();
int refreshMemoryInfo(); 
int refreshProcessesList();
int showWorkSpace(); 
void showMenu();
void resolveLocation();
int initInfo();
int showDisksInfo();
void refreshHDDInfo();


struct ButtonLocStruct
{
	int height = 30;
	int width = 90;
	int margin = 10;
} *LPButtonLocStruct;

struct WorkSpaceLoc
{
	int x;
	int y;
	int width;
	int height;
	int margin = 10;
	RECT Rect;
	HBRUSH Brush = CreateSolidBrush(RGB(236, 252, 159));
} *LPWorkSpaceLoc;

struct MenuLoc
{
	int x;
	int y;
	int width;
	int height;
	int margin = 0;
	RECT Rect;
	HBRUSH Brush = CreateSolidBrush(RGB(181, 0, 30));
} *LPMenuLoc;




HINSTANCE HInstance;
int width = 700, height = 600;
int window_width, window_height;
HDC hdc;
byte curColorWindow = COLOR_WINDOW;
RECT win_rect{ 0, 0, window_width, window_height };
HWND hWnd;
HANDLE HButton_List_processes;
HANDLE HButton_Memory_info;
HANDLE HButton_System_info;
HANDLE HButton_Processor_info;
HANDLE HButton_HDD_info;
HANDLE HButton_is_updating_allow;

int curHDD_line = 0;
int curProcesses_line = 0;
int processes_scrolling_step = 10;
int HDD_scrolling_step = 1;
int is_updating = 0;

std::string processorInfoString = "";
std::string SystemInfoString = "";
std::string MemoryInfoString = "";

ButtonLocStruct ButtonLocation;
WorkSpaceLoc WorkSpaceLocation;
MenuLoc MenuLocation;
HDD_info hdd_info;
DisplayInfo displayInfo;




int curOperation = OPERATION_PROCESSOR_INFO;



int work_x; int work_y;
int work_widtht; int work_heigh;



struct MemoryTotal
{
	int B;
	int KB;
	int MB;
	int GB;

} *LPMemoryTotal;


class MemoryInfo
{
public:
	MemoryInfo()
	{
		this->update();
	}

	void update()
	{
		GlobalMemoryStatus(&(this->memoryStat));
	}

	std::string getMemoryPercentageUsed()
	{
		std::string PercentageUsed = std::to_string(this->memoryStat.dwMemoryLoad);
		return PercentageUsed;
	}

	MemoryTotal getMemoryTotal()
	{
		/*
		long long sizeMem = 0;
		GetPhysicallyInstalledSystemMemory((PULONGLONG)&sizeMem);
		return this->getMemoryStructFromBytes(sizeMem);
		*/
		return this->getMemoryStructFromBytes(
			this->memoryStat.dwTotalPhys);
	}

	std::string getMemoryTotal_string()
	{
		long long sizeMem = 0;
		GetPhysicallyInstalledSystemMemory((PULONGLONG)&sizeMem);
		return std::to_string(sizeMem / 1024);
	}




	MemoryTotal getTotalPageFile()
	{
		return this->getMemoryStructFromBytes(
			this->memoryStat.dwTotalPageFile);
	}


private:

	MEMORYSTATUS memoryStat;

	


	MemoryTotal getMemoryStructFromBytes(int countBytes)
	{
		MemoryTotal memoryTotal;
		
		memoryTotal.B = countBytes;
		memoryTotal.KB = countBytes / 1024;
		memoryTotal.MB = countBytes / (1024 * 1024);
		memoryTotal.GB = countBytes / (1024 * 1024 * 1024);
		
		/*
		memoryTotal.B = 0;
		memoryTotal.KB = (countBytes) % 1024;
		memoryTotal.MB = (countBytes / 1024)
			% 1024;
		memoryTotal.GB = countBytes / (1024 * 1024);
		*/
		return memoryTotal;
	}
};


void OnEraseBkGnd(HWND hwnd)
{
	/* Vars */
	HDC dc; /* Standard Device Context; used to do the painting */

			/* rect = Client Rect of the window;
			Temp = Temparory rect tangle for the color bands */
	RECT rect, temp;
	HBRUSH color; /* A brush to do the painting with */

				  /* Get the dc for the wnd */
	dc = GetDC(hwnd);

	/* Get the client rect */
	

	/* Start color; Change the R,G,B values
	to the color of your choice */
	int r1 = 255, g1 = 0, b1 = 0;

	/* End Color; Change the R,G,B values
	to the color of your choice */
	int r2 = 255, g2 = 255, b2 = 0;

	/* loop to create the gradient */
	for (int i = 0; i<MenuLocation.width; i++)
	{
		/* Color ref. for the gradient */
		int r, g, b;
		/* Determine the colors */
		r = r1 + (i * (r2 - r1) / MenuLocation.width);
		g = g1 + (i * (g2 - g1) / MenuLocation.width);
		b = b1 + (i * (b2 - b1) / MenuLocation.width);

		/* Fill in the rectangle information */

		/* The uper left point of the rectangle
		being painted; uses i as the starting point*/
		temp.left = i;
		/* Upeer Y cord. Always start at the top */
		temp.top = 0;
		/* Okay heres the key part,
		create a rectangle thats 1 pixel wide */
		temp.right = i + 1;
		/* Height of the rectangle */
		temp.bottom = MenuLocation.height;

		/* Create a brush to draw with;
		these colors are randomized */
		color = CreateSolidBrush(RGB(r, g, b));

		/* Finally fill in the rectangle */
		FillRect(dc, &temp, color);
	}
}

std::string MemoryTotalToString(MemoryTotal memory_total);

RunningProcesses* runningProcesses;
MemoryInfo* memoryInfo;
ProcessorsName* processorsName;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)

{
	HInstance = hInstance;
	LPCTSTR lpzClass = TEXT("course_work");
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	RECT screen_rect, window_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // ?????????? ??????
													 //AdjustWindowRect(
	int x = screen_rect.right / 2 - width / 2;
	int y = screen_rect.bottom / 2 - height / 2;

    hWnd = CreateWindow(lpzClass, TEXT("course_work"),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
		x, y, width, height, NULL, NULL,
		hInstance, NULL);

	GetClientRect(hWnd, &window_rect); 
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;



	






	if (!hWnd) return 2;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	resolveLocation();
	initInfo();
	SetBkMode(hdc, TRANSPARENT);
	refreshScreen();
	showMenu();
	showButtonMenu();
	showWorkSpace();
	curOperation = OPERATION_PROCESSOR_INFO;
	showProcessorInfo();
	

	SetTimer(hWnd, 1, 1000, NULL);
	MSG msg = { 0 };   
	int iGetOk = 0;   
	
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) 
	{
		if (iGetOk == -1) return 3;  
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	wcWindowClass.hInstance = hInst;
	wcWindowClass.lpszClassName = lpzClassName;
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // ??????????? ??????
}

LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case  WM_COMMAND:
		switch(wParam)
		{

		case ID_BUTTON_PROCESS_LIST:
			curProcesses_line = 0;
			showWorkSpace();
			curOperation = OPERATION_RUNNING_PROCESSES;
			refreshProcessesList();
			break;
		case ID_BUTTON_PROCESS_MEMORY_INFO:
			showWorkSpace();
			curOperation = OPERATION_MEMORY_INFO;
			refreshMemoryInfo();
			break;
		case ID_BUTTON_PROCESS_SYSTEM_INFO:
			showWorkSpace();
			curOperation = OPERATION_SYSTEM_INFO;
			showSystemInfo();
			break;
		case ID_BUTTON_PROCESS_PROCESSOR_INFO:
			showWorkSpace();
			curOperation = OPERATION_PROCESSOR_INFO; 
			showProcessorInfo();
			break;
			//return showProcessorInfo();
		case ID_BUTTON_PROCESS_HDD_INFO:
			curHDD_line = 0;
			showWorkSpace();
			curOperation = OPERATION_HDD_INFO;
			refreshHDDInfo();
			break;
		case ID_BUTTON_UPDATING_ALLOW:
			is_updating = (++is_updating) % 2;
		}
		break;

	case WM_SIZE:
		resize(hWnd);
		switch (curOperation)
		{
		case OPERATION_RUNNING_PROCESSES:
			curProcesses_line = 0;
			showWorkSpace();
			return refreshProcessesList();
		case OPERATION_MEMORY_INFO:
			showWorkSpace();
			return refreshMemoryInfo();
		case OPERATION_SYSTEM_INFO:
			showWorkSpace();
			return showSystemInfo();
		case OPERATION_PROCESSOR_INFO:
			showWorkSpace();
			return showProcessorInfo();
		case OPERATION_HDD_INFO:
			curHDD_line = 0;
			refreshHDDInfo();
		}
		break;

	case WM_TIMER:
		if (is_updating)
			timerTick(hWnd);
		break;

	case WM_KEYDOWN: 
		switch (wParam)
		{
		case VK_DOWN:
			if ((curOperation == OPERATION_HDD_INFO) &&
				(curHDD_line < 30))
			{
				curHDD_line++;
				refreshHDDInfo();
			}
			if ((curOperation == OPERATION_RUNNING_PROCESSES) &&
				(curProcesses_line < 30))
			{
				curProcesses_line++;
				refreshProcessesList();
			}
			break;

		case VK_UP:
			if ((curOperation == OPERATION_HDD_INFO) &&
				(curHDD_line > 0))
			{
				curHDD_line--;
				refreshHDDInfo();
			}
			if ((curOperation == OPERATION_RUNNING_PROCESSES) &&
				(curProcesses_line > 0))
			{
				curProcesses_line--;
				refreshProcessesList();
			}
			break;
		}
		break;
	case WM_DESTROY:

		close(hWnd);
		PostQuitMessage(0);  
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void showMenu()
{
	//FillRect(hdc, &MenuLocation.Rect, MenuLocation.Brush);
	if (hWnd != NULL) 
		OnEraseBkGnd(hWnd);
	RECT rect{ ButtonLocation.width * 5 + ButtonLocation.margin * 6 + 75,
		ButtonLocation.margin,
		ButtonLocation.width * 5 + ButtonLocation.margin * 6 + 75 + 60, 
		ButtonLocation.margin + 20 };
	DrawText(hdc, "update",
		SystemInfoString.size(),
		&rect,
		0);
}


void resolveLocation()
{
	MenuLocation.y = 0;
	MenuLocation.x = 0;
	MenuLocation.width = window_width;
	MenuLocation.height = ButtonLocation.height + ButtonLocation.margin * 2;
	MenuLocation.Rect = { MenuLocation.x ,
		MenuLocation.y,
		MenuLocation.x + MenuLocation.width,
		MenuLocation.y + MenuLocation.height };



	WorkSpaceLocation.y = MenuLocation.y + MenuLocation.height + 5;
	WorkSpaceLocation.x = 5;
	WorkSpaceLocation.width = window_width - 10;
	WorkSpaceLocation.height = window_height - WorkSpaceLocation.y - 5;
	WorkSpaceLocation.Rect = { WorkSpaceLocation.x ,
		WorkSpaceLocation.y,
		WorkSpaceLocation.x + WorkSpaceLocation.width,
		WorkSpaceLocation.y + WorkSpaceLocation.height };
}

void showButtonMenu()
{
	
	HButton_System_info = CreateWindow("BUTTON",
		"о системе",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED,
		ButtonLocation.margin, ButtonLocation.margin, 
		ButtonLocation.width, ButtonLocation.height, hWnd,
		(HMENU)ID_BUTTON_PROCESS_SYSTEM_INFO, HInstance, NULL);
	
	
	HButton_Processor_info = CreateWindow("BUTTON",
		"процессор",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED,
		ButtonLocation.width + ButtonLocation.margin * 2, ButtonLocation.margin,
		ButtonLocation.width, ButtonLocation.height, hWnd,
		(HMENU)ID_BUTTON_PROCESS_PROCESSOR_INFO, HInstance, NULL);

	HButton_List_processes = CreateWindow("BUTTON",
		"процессы",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED,
		ButtonLocation.width*2 + ButtonLocation.margin * 3, ButtonLocation.margin,
		ButtonLocation.width, ButtonLocation.height, hWnd,
		(HMENU)ID_BUTTON_PROCESS_LIST, HInstance, NULL);

	HButton_System_info = CreateWindow("BUTTON",
		"ОЗУ",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED,
		ButtonLocation.width*3 + ButtonLocation.margin * 4, ButtonLocation.margin,
		ButtonLocation.width, ButtonLocation.height, hWnd,
		(HMENU)ID_BUTTON_PROCESS_MEMORY_INFO, HInstance, NULL);

	HButton_HDD_info = CreateWindow("BUTTON",
		"HDD",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED,
		ButtonLocation.width * 4 + ButtonLocation.margin * 5, ButtonLocation.margin,
		ButtonLocation.width, ButtonLocation.height, hWnd,
		(HMENU)ID_BUTTON_PROCESS_HDD_INFO, HInstance, NULL);

	
	HButton_is_updating_allow = CreateWindow("BUTTON",
		"",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_OVERLAPPED | BS_AUTOCHECKBOX,
		ButtonLocation.width * 5 + ButtonLocation.margin * 6 + 50, ButtonLocation.margin,
		15, 15, hWnd,
		(HMENU)ID_BUTTON_UPDATING_ALLOW, HInstance, NULL);
	
}

int close(HWND hWnd)
{
	KillTimer(hWnd, 1);
	return freeMemory();
}


int GetCpuUsage()
{
	static ULARGE_INTEGER TimeIdle, TimeKernel, TimeUser;
	FILETIME Idle, Kernel, User;
	ULARGE_INTEGER uIdle, uKernel, uUser;
	GetSystemTimes(&Idle, &Kernel, &User);
	memcpy(&uIdle, &Idle, sizeof(FILETIME));
	memcpy(&uKernel, &Kernel, sizeof(FILETIME));
	memcpy(&uUser, &User, sizeof(FILETIME));
	long long t;
	t = (((((uKernel.QuadPart - TimeKernel.QuadPart) + (uUser.QuadPart - TimeUser.QuadPart)) -
		(uIdle.QuadPart - TimeIdle.QuadPart))*(100)) / ((uKernel.QuadPart -
			TimeKernel.QuadPart) + (uUser.QuadPart - TimeUser.QuadPart)));
	TimeIdle.QuadPart = uIdle.QuadPart;
	TimeUser.QuadPart = uUser.QuadPart;
	TimeKernel.QuadPart = uKernel.QuadPart;
	return(static_cast<int>(t));



}

int initInfo()
{
	/*
	GetDiskFreeSpaceEx(
		_In_opt_   "C:\\",
		_Out_opt_  PULARGE_INTEGER lpFreeBytesAvailable,
		_Out_opt_  PULARGE_INTEGER lpTotalNumberOfBytes,
		_Out_opt_  PULARGE_INTEGER lpTotalNumberOfFreeBytes
	);
	*/

	runningProcesses = new RunningProcesses();

	memoryInfo = new MemoryInfo();

	

	ProcessorInfo* processorInfo = new ProcessorInfo();
	processorInfo->getPageSize();

	processorsName = new ProcessorsName("D:\\proc_names.txt");

	processorInfoString.append("processor name: ");
	processorInfoString.append(
		processorsName->getProcessorNameFromRevision(processorInfo->getFamily(),
													processorInfo->getModel(),
													processorInfo->getStepping()).append("\n"));
	processorInfoString.append("processor architecture: ");
	processorInfoString.append(processorInfo->getProcessorArchitecture().append("\n"));
	processorInfoString.append("processors count: ");
	processorInfoString.append(std::to_string(processorInfo->getProcessorsCoresNum()).append("\n"));
	processorInfoString.append("processor type: ");
	processorInfoString.append(processorInfo->getProcessorType().append("\n"));
	processorInfoString.append("processor Revision: ");
	processorInfoString.append(processorInfo->getProcessorRevision().append("\n"));
	processorInfoString.append("page size: ");
	processorInfoString.append(std::to_string(processorInfo->getPageSize()).append("\n"));



	SystemVersionInfo* systemVersionInfo = new SystemVersionInfo();
	systemVersionInfo->getSystemVersion();

	SystemInfoString.append("system: ");
	SystemInfoString.append(systemVersionInfo->getSystemVersion().append("\n"));
	SystemInfoString.append("username: ");
	SystemInfoString.append(systemVersionInfo->getUserName() + "\n");
	SystemInfoString.append("computer name: ");
	SystemInfoString.append(systemVersionInfo->getComputerName() + "\n");

	
	SystemInfoString.append("video device name: ");
	SystemInfoString.append(displayInfo.getDeviceName() + "\n");
	SystemInfoString.append("video card name: ");
	SystemInfoString.append(displayInfo.getVideoCardName() + "\n");
	SystemInfoString.append("screen resolution: ");
	SystemInfoString.append(displayInfo.getScreenResolution() + "\n");
	SystemInfoString.append("screen realSize: ");
	SystemInfoString.append(displayInfo.getScreenRealSize() + "\n");
	SystemInfoString.append("screen technology: ");
	SystemInfoString.append(displayInfo.getScreenTechnology() + "\n");
	SystemInfoString.append("screen driver version: ");
	SystemInfoString.append(displayInfo.getScreenDriverVersion() + "\n");
	SystemInfoString.append("Number of adjacent color bits for each pixel: ");
	SystemInfoString.append(displayInfo.getBitPixel() + "\n");


	return 0;
}

int freeMemory()
{
	return 0;
}

int timerTick(HWND hWnd)
{

	if ((curOperation != OPERATION_MEMORY_INFO) && 
		(curOperation != OPERATION_RUNNING_PROCESSES) &&
		(curOperation != OPERATION_PROCESSOR_INFO) &&
		(curOperation != OPERATION_HDD_INFO))
		return 0;
	if (curOperation == OPERATION_MEMORY_INFO)
		refreshMemoryInfo();
	if (curOperation == OPERATION_RUNNING_PROCESSES)
		refreshProcessesList();
	if (curOperation == OPERATION_HDD_INFO)
		refreshHDDInfo();
	if (curOperation == OPERATION_PROCESSOR_INFO)
	{
		showWorkSpace();
		showProcessorInfo();
	}
	
	return 0;
}

int refreshMemoryInfo()
{
	showWorkSpace();
	showMemoryInfo();
	return 0;
}

int refreshProcessesList()
{
	showWorkSpace();
	RunningProcessContainer runningProcContainer(runningProcesses->getListRunningProcesses());
	std::string processes_info_str;

	if (processes_scrolling_step * curProcesses_line >= runningProcContainer.getCount())
	{
		curProcesses_line--;
	}

	
	
	for (int i = curProcesses_line * processes_scrolling_step; i < runningProcContainer.getCount(); i++)
	{
		processes_info_str += std::to_string(i) + ") name: " +  runningProcContainer.getRunningProcess_name(i) +
			"     id: " + std::to_string(runningProcContainer.getRunningProcess_id(i)) + "   " + 
			"threads count: " + std::to_string(runningProcContainer.getProcess_threadsCount(i)) + "   " + 
			"priority: " + std::to_string(runningProcContainer.getProcess_priority(i)) + "\n";
	}
	RECT rect{ WorkSpaceLocation.x, WorkSpaceLocation.y,
		WorkSpaceLocation.width, WorkSpaceLocation.height };
	DrawText(hdc, processes_info_str.c_str(),
		processes_info_str.size(),
		&rect,
		0);
	return 0;
}

int resize(HWND hWnd)
{
	RECT window_rect;
	GetClientRect(hWnd, &window_rect); 
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;
	win_rect = { 0, 0, window_width, window_height };
	resolveLocation();
	refreshScreen();
	showMenu();
	showWorkSpace();
	return 0;
}


int refreshScreen()
{
	FillRect(hdc, &win_rect, MenuLocation.Brush);
	return 0;
}

int showWorkSpace()
{
	FillRect(hdc, &WorkSpaceLocation.Rect, WorkSpaceLocation.Brush);
	return 0;
}

void refreshHDDInfo()
{
	showWorkSpace();
	showDisksInfo();
}




int showRunningProcessesList(std::vector<std::string> runningProcList_names, 
	std::vector<DWORD> runningProcList_id)
{

	return 0;
}

int showMemoryInfo()
{
	memoryInfo->update();
	std::string memoryPercentageUsed = memoryInfo->getMemoryPercentageUsed();
	MemoryTotal memory_total = memoryInfo->getMemoryTotal();
	//MemoryTotal memory_TotalPage = memoryInfo->getTotalPageFile;
	MemoryInfoString = "";
	//MemoryInfoString += "всего ОЗУ " + MemoryTotalToString(memory_total);
	
	MemoryInfoString += "всего ОЗУ " + memoryInfo->getMemoryTotal_string() + "\n";
	MemoryInfoString += "использовано процентов: " + memoryPercentageUsed + "\n";
	//MemoryInfoString += "размер зам " + MemoryTotalToString(memory_total);

	RECT rect{ WorkSpaceLocation.x, WorkSpaceLocation.y,
		WorkSpaceLocation.width, WorkSpaceLocation.height };
	DrawText(hdc, MemoryInfoString.c_str(),
		MemoryInfoString.size(),
		&rect,
		0);

	return 0;

	/*
	
	*/
}

std::string MemoryTotalToString(MemoryTotal memory_total)
{
	std::string memTotalString = "GB:";
	memTotalString += std::to_string(memory_total.GB);
	memTotalString += " MB:";
	memTotalString += std::to_string(memory_total.MB);
	memTotalString += " KB:";
	memTotalString += std::to_string(memory_total.KB);
	memTotalString += " B:";
	memTotalString += std::to_string(memory_total.B);
	memTotalString += "\n";
	return memTotalString;
}



int showProcessorInfo()
{
	RECT rect{ WorkSpaceLocation.x, WorkSpaceLocation.y,
		WorkSpaceLocation.width, WorkSpaceLocation.height };
	
	//system("mode 40, 4");
	
	
	DrawText(hdc, processorInfoString.c_str(),
		processorInfoString.size(),
		&rect,
		0);
	int cp;
	cp = GetCpuUsage();

	rect = { WorkSpaceLocation.x, WorkSpaceLocation.y + WorkSpaceLocation.height / 2,
		WorkSpaceLocation.width, WorkSpaceLocation.height  };

	std::string processorUsage = "processor usage: " + std::to_string(cp);
	DrawText(hdc, processorUsage.c_str(),
		processorUsage.size(),
		&rect,
		0);

	return 0;
}

int showSystemInfo()
{
	RECT rect{ WorkSpaceLocation.x, WorkSpaceLocation.y,
		WorkSpaceLocation.width, WorkSpaceLocation.height };
	DrawText(hdc, SystemInfoString.c_str(),
		SystemInfoString.size(),
		&rect,
		0);
	return 0;
}

int showDisksInfo()
{
	std::string info = "";
	std::vector<std::string> disks = hdd_info.getDisks();
	std::vector<unsigned int> disks_free_space;

	if (HDD_scrolling_step * curHDD_line >= disks.size())
	{
		curHDD_line--;
	}

	for (int i = 0; i < disks.size(); i++)
	{
		disks_free_space.push_back(hdd_info.getFreeDiskSpace(disks[i]));
	}

	info += "свободное место на дисках: \n";
	for (int i = HDD_scrolling_step * curHDD_line; i < disks.size(); i++)
	{
		info += disks[i] + " " + std::to_string(disks_free_space[i]) + "\n";
	}
	info += "\n\n";

	std::vector<std::string> volume = hdd_info.getVolume();
	for (int i = HDD_scrolling_step * curHDD_line; i < volume.size(); i++)
	{
		info += volume[i] + "\n";
	}

	RECT rect{ WorkSpaceLocation.x, WorkSpaceLocation.y,
		WorkSpaceLocation.width, WorkSpaceLocation.height };
	DrawText(hdc, info.c_str(),
		info.size(),
		&rect,
		0);
	return 0;
}