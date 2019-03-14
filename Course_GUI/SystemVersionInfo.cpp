#include <Windows.h>
#include <vector>
#include <string>

class SystemVersionInfo
{
public:

	SystemVersionInfo()
	{
	}


	std::string getSystemVersion()
	{
		if (!this->IsWindows10SP_OrGreater(0))
			return "windows 10";
		if (!this->IsWindows8SP_OrGreater(0))
			return "windows 8";
		if (!this->IsWindowsXPSP_OrGreater(0))
			return "windows XP";
		if (!this->IsWindowsVistaSP_OrGreater(0))
			return "windows vista";
		
		return "underfined";
	}

	std::string getUserName()
	{
		char user_name[30];
		int len = 30;
		GetUserName(user_name, (LPDWORD) &len);
		return user_name;
	}

	std::string getComputerName()
	{
		char computer_name[30];
		int len = 30;
		GetComputerName(computer_name, (LPDWORD)&len);
		return computer_name;
	}

private:

	int IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
	{
		OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0 };
		DWORDLONG        const dwlConditionMask = VerSetConditionMask(
			VerSetConditionMask(
				VerSetConditionMask(
					0, VER_MAJORVERSION, VER_GREATER_EQUAL),
				VER_MINORVERSION, VER_GREATER_EQUAL),
			VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

		osvi.dwMajorVersion = wMajorVersion;
		osvi.dwMinorVersion = wMinorVersion;
		osvi.wServicePackMajor = wServicePackMajor;

		return VerifyVersionInfoW(&osvi, 
			VER_BUILDNUMBER | VER_MAJORVERSION | 
			VER_MINORVERSION | VER_PLATFORMID |
			VER_SERVICEPACKMINOR | VER_SUITENAME |
			VER_PRODUCT_TYPE | VER_SERVICEPACKMAJOR,
			dwlConditionMask) != FALSE;
	}

	int IsWindowsVistaSP_OrGreater(int sp)
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), sp);
	}

	int IsWindows10SP_OrGreater(int sp)
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), sp);
	}


	int IsWindows8SP_OrGreater(int sp)
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), sp);
	}

	int IsWindowsXPSP_OrGreater(int sp)
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), sp);
	}
};