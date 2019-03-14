#include <Windows.h>
#include <vector>
#include <string>
#include <tlhelp32.h>

class RunningProcessContainer
{
public:

	RunningProcessContainer(std::vector<std::string> RunningProcesses_names,
		std::vector<DWORD> RunningProcesses_id,
		std::vector<DWORD> ListRunningProcesses_threadsCount,
		std::vector<DWORD> ListRunningProcesses_priority)
	{
		this->RunningProcesses_id = RunningProcesses_id;
		this->RunningProcesses_names = RunningProcesses_names;
		this->ListRunningProcesses_threadsCount = ListRunningProcesses_threadsCount;
		this->ListRunningProcesses_priority = ListRunningProcesses_priority;
		this->count = this->RunningProcesses_id.size();
	}

	int getCount()
	{
		return this->count;
	}

	std::string getRunningProcess_name(int index)
	{
		return this->RunningProcesses_names[index];
	}

	DWORD getRunningProcess_id(int index)
	{
		return this->RunningProcesses_id[index];
	}

	DWORD getProcess_threadsCount(int index)
	{
		return this->ListRunningProcesses_threadsCount[index];
	}

	DWORD getProcess_priority(int index)
	{
		return this->ListRunningProcesses_priority[index];
	}


private:
	std::vector<std::string> RunningProcesses_names;
	std::vector<DWORD> RunningProcesses_id;
	std::vector<DWORD> ListRunningProcesses_threadsCount;
	std::vector<DWORD> ListRunningProcesses_priority;
	int count;
};



class RunningProcesses
{
public:
	RunningProcesses()
	{
		this->ToolHelpSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	}

	RunningProcessContainer	getListRunningProcesses()
	{
		std::vector<std::string> ListRunningProcesses_names;
		std::vector<DWORD>       ListRunningProcesses_id;
		std::vector<DWORD>       ListRunningProcesses_threadsCount;
		std::vector<DWORD>       ListRunningProcesses_priority;

		this->ProcEntry.dwSize = sizeof(PROCESSENTRY32);
		Process32First(this->ToolHelpSnapShot, &ProcEntry);
		do
		{
			ListRunningProcesses_names.push_back(this->ProcEntry.szExeFile);
			ListRunningProcesses_id.push_back(this->ProcEntry.th32ProcessID);
			ListRunningProcesses_priority.push_back(this->ProcEntry.pcPriClassBase);
			ListRunningProcesses_threadsCount.push_back(this->ProcEntry.cntThreads);

		} while (Process32Next(this->ToolHelpSnapShot, &ProcEntry));
		return RunningProcessContainer(ListRunningProcesses_names,
			ListRunningProcesses_id,
			ListRunningProcesses_threadsCount,
			ListRunningProcesses_priority);
	}

private:
	HANDLE ToolHelpSnapShot;
	PROCESSENTRY32 ProcEntry;
};
