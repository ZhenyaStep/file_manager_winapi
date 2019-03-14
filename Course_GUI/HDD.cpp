#include <windows.h>
#include <string>
#include <vector>

class HDD_info
{
public:
	HDD_info()
	{
		getDisks();
		getVolume();
	}

	unsigned __int64 getFreeDiskSpace(std::string dir_path)
	{
		unsigned __int64 bytes;

		//char* str;
		//strcpy(dir_path.c_str, str);
		GetDiskFreeSpaceExA((LPCSTR)dir_path.c_str(), NULL, NULL, (PULARGE_INTEGER) &bytes);
		return bytes / (1024*1024);
	}

	std::vector<std::string> getVolume()
	{
		std::vector<std::string> volume;
		std::string disk_volume;
		char buffer[100];
		DWORD serial;
		DWORD len;
		DWORD flag;
		char fileSystem[100];
		std::string file_sup_enc = "";
		std::string file_unicode = "";
		for (int i = 0; i < disks.size(); i++)
		{
			GetVolumeInformationA(disks[i].c_str(), buffer, 100,
				&serial, &len, &flag, fileSystem, 100);
			if ((flag && FILE_SUPPORTS_ENCRYPTION) != 0){
				file_sup_enc = "YES";
			}else{
				file_sup_enc = "NO";
			}

			if ((flag && FILE_UNICODE_ON_DISK) != 0){
				file_unicode = "YES";
			}else{
				file_unicode = "NO";
			}
			disk_volume = disks[i] + " \nname: " + buffer + "\n" 
				+ "file system: " + fileSystem + "\n"+
				"serial number: " + std::to_string(serial)
				+ "\nmaximum file name len: " +
				std::to_string(len) + "\n"+
				"EFS :" + file_sup_enc + "\n" + 
				"filename unicode: " + file_unicode + "\n";;
			volume.push_back(disk_volume);
		}
		return volume;
	}

	std::vector<std::string> getDisks()
	{
		//DWORD ftyp;
		this->disks.clear();

		DWORD res = GetLogicalDrives();
		if (res & 0x00000001 != 0)
			this->disks.push_back("A:\\");
		if (res & 0x00000002 != 0)
			this->disks.push_back("B:\\");
		if ((res & 0x00000004) != 0)
			this->disks.push_back("C:\\");
		if ((res & 0x00000008) != 0)
			this->disks.push_back("D:\\");
		if ((res & 16) != 0)
			this->disks.push_back("E:\\");
		if ((res & 32) != 0)
			this->disks.push_back("F:\\");
		if ((res & 64) != 0)
			this->disks.push_back("G:\\");
		if ((res & 128) != 0)
			this->disks.push_back("H:\\");
		/*
		for (int i = 0; i < disks.size(); i++)
		{
			ftyp = GetFileAttributesA(disks[i].c_str());
			if ((ftyp != INVALID_FILE_ATTRIBUTES) && (FILE_ATTRIBUTE_DIRECTORY))
			{
				//disks.push_back()
				int i = 0;
			}
		}
		*/
		//bool ready = std::DirectoryExists(path);
		return this->disks;
	}




private:

	std::vector<std::string> disks;

	LARGE_INTEGER intToLargeInt(int i) {
		LARGE_INTEGER li;
		li.QuadPart = i;
		return li;
	}

};