#include <Windows.h>
#include <vector>
#include <string>

const int PROCESSOR_LEVEL_80386 = 3;
const int PROCESSOR_LEVEL_80486 = 4;
const int PROCESSOR_LEVEL_PENTIUM = 5;
const int PROCESSOR_LEVEL_PENTIUMII = 6;








class ProcessorInfo
{
public:
	ProcessorInfo()
	{
		GetSystemInfo(&(this->system_info));
	}

	std::string getProcessorInfo()
	{
		return "underfined";
	}

	std::string getProcessorArchitecture()
	{
		std::string processorArchitecture = "underfined";
		switch (this->system_info.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_AMD64:
			processorArchitecture = "x64 (AMD or Intel)";
			break;
		case PROCESSOR_ARCHITECTURE_ARM:
			processorArchitecture = "ARM";
			break;
		case PROCESSOR_ARCHITECTURE_ARM64:
			processorArchitecture = "ARM64";
			break;
		case PROCESSOR_ARCHITECTURE_IA64:
			processorArchitecture = "Intel Itanium-based";
			break;
		case PROCESSOR_ARCHITECTURE_INTEL:
			processorArchitecture = "x86";
			break;
		case PROCESSOR_ARCHITECTURE_UNKNOWN:
			processorArchitecture = "Unknown architecture";
			break;
		default:
			processorArchitecture = "undefined";
		}
		return processorArchitecture;
	}


	//precessor level = Family, 
	//wpRocessorRevision(High) = model
	//wpRocessorRevision(Low) = stepping
	//milkyway.cs.rpi.edu/milkyway/cpu_list.php
	std::string getProcessorRevision()
	{
		int family = this->system_info.wProcessorLevel;
		int model = HIBYTE(this->system_info.wProcessorRevision);
		int stepping = LOBYTE(this->system_info.wProcessorRevision);
		std::string processorRevision = "family = ";
		processorRevision.append(std::to_string(family));
		processorRevision.append(", model = ");
		processorRevision.append(std::to_string(model));
		processorRevision.append(", stepping = ");
		processorRevision.append(std::to_string(stepping));
		return processorRevision;

	}

	int getFamily()
	{
		return this->system_info.wProcessorLevel;
	}

	int getModel()
	{
		return HIBYTE(this->system_info.wProcessorRevision);
	}

	int getStepping()
	{
		return LOBYTE(this->system_info.wProcessorRevision);
	}

	std::string getProcessorLevel()
	{
		switch (this->system_info.wProcessorLevel)
		{
		case PROCESSOR_LEVEL_80386:
			return "intel 80386";
		case PROCESSOR_LEVEL_80486:
			return "intel 80486";
		case PROCESSOR_LEVEL_PENTIUM:
			return "intel pentium";
		case PROCESSOR_LEVEL_PENTIUMII:
			return "intel pentium Pro, II, III, IV";
		default:
			return "underfined";
		}
	}


	std::string getProcessorType()
	{
		switch (this->system_info.dwProcessorType)
		{
		case PROCESSOR_INTEL_386:
			return "intel 386";
		case PROCESSOR_INTEL_486:
			return "intel 486";
		case PROCESSOR_INTEL_IA64:
			return "intel IA64";
		case PROCESSOR_INTEL_PENTIUM:
			return "intel pentium";
		case PROCESSOR_ALPHA_21064:
			return "alpha 21064";
		case PROCESSOR_AMD_X8664:
			return "amd x8664";
		case PROCESSOR_MIPS_R4000:
			return "mips R4000";
		default:
			return "underfined";
		}
	}

	DWORD getProcessorsCoresNum()
	{
		return this->system_info.dwNumberOfProcessors;
	}

	DWORD getPageSize()
	{
		return this->system_info.dwPageSize;
	}


private:
	SYSTEM_INFO system_info;
};
