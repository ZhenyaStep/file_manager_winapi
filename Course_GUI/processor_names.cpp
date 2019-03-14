#include <string>
#include <vector>
#include <fstream>
#include <iostream>


#define FAMILY_6 6
#define PROCESSOR_FAMILY_MAX 6
#define FILE_ITEM_DELIMETER ','

struct Model_Stepping_Name
{
	int family;
	int model;
	int stepping;
	std::string name;
};


class ProcessorsName
{
public:
	ProcessorsName(std::string fileUrl)
	{
		this->fileUrl = fileUrl;
		this->readFile();
	}

	std::string getProcessorNameFromRevision(int family, int model, int stepping)
	{
		std::string ProcessorName = "underfined";

		if ((family >= 0) && (family <= PROCESSOR_FAMILY_MAX))
		{
			for (int i = 0; i < this->Family_Model_Stepping_Name.size(); i++)
			{
				if ((family == this->Family_Model_Stepping_Name[i].family) && 
					(model == this->Family_Model_Stepping_Name[i].model) && 
					(stepping == this->Family_Model_Stepping_Name[i].stepping))
				{
					return this->Family_Model_Stepping_Name[i].name;
				}
			}
		}
		return ProcessorName;
	}

private:
	std::vector<Model_Stepping_Name> Family_Model_Stepping_Name;
	std::string fileUrl;

	void readFile()
	{
		std::ifstream file;
		file.open(this->fileUrl);
		if (!file.is_open())
			return;
		std::string file_string;
		std::vector<std::string> ItemsArr;
		Model_Stepping_Name model_stepping_name;
		while (!file.eof())
		{
			std::getline(file, file_string);
			//file >> file_string;
			ItemsArr = splitString(file_string, FILE_ITEM_DELIMETER);
			model_stepping_name.family = std::stoi(ItemsArr[0]);
			model_stepping_name.model = std::stoi(ItemsArr[1]);
			model_stepping_name.stepping = std::stoi(ItemsArr[2]);
			model_stepping_name.name = ItemsArr[3];
			this->Family_Model_Stepping_Name.push_back(model_stepping_name);
		}
	}

	std::vector<std::string> splitString(std::string string, char splitter)
	{
		int ArrStrIndex = 0;
		std::vector<std::string> arrStrings;
		arrStrings.push_back("");
		for (int i = 0; i < string.size(); i++)
		{
			if (string[i] == splitter)
			{
				arrStrings.push_back("");
				ArrStrIndex++;
			}
			else
			{
				arrStrings[ArrStrIndex] += string[i];
			}
		}
		return arrStrings;
	}
};

