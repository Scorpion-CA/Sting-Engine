#include "Config.h"
#include <fstream>
#include <iostream> 
#include "Window.h"

Config* Config::Get() {
	static Config* self = new Config;
	return self;
}

void Config::AddVar(int* Val, std::string Name) {
	VarInfo_t info;
	info.iVar = Val;
	info.Type = 0;
	m_vVarList.push_back(std::pair(Name, info));
}

void Config::AddVar(POINT* Val, std::string Name) {
	VarInfo_t info;
	info.pVar = Val;
	info.Type = 1;
	m_vVarList.push_back(std::pair(Name, info));
}

//set the value of what the existing variable points to
void Config::SetVar(int Val, std::string Name) {
	*m_vVarList[FindVarByName(Name)].second.iVar = Val;
}

//set the value of what the existing variable points to
void Config::SetVar(POINT Val, std::string Name) {
	*m_vVarList[FindVarByName(Name)].second.pVar = Val;
}

//set the variable to which the config variable points to
void Config::SetVarPtr(int* Val, std::string Name) {
	m_vVarList[FindVarByName(Name)].second.iVar = Val;
}

//set the variable to which the config variable points to
void Config::SetVarPtr(POINT* Val, std::string Name) {
	m_vVarList[FindVarByName(Name)].second.pVar = Val;
}

void Config::Write(std::string FileName) {
	std::ofstream oFile;
	oFile.open(FileName, std::ofstream::trunc);

	std::vector<int> branchSizes;
	int branches = 0;
	for (auto& i : m_vVarList) {
		branches = 0;
		static bool close = false;

		//for (int j = 0; j < branchSizes.size(); j++) {
		int idx = 0;
		for (auto& j : branchSizes) {
			if (j > 0) {
				branches++;
			}
		}

		if (branches > 0) {
			for (int j = 0; j < branches; j++) {
				oFile << char(9);
			}
		}

		//for (auto& j : branchSizes) {
		for (int j = 0; j < branchSizes.size(); j++) {
			if (branchSizes[j] > 0) {
				branchSizes[j]--;
			}

			if (branchSizes[j] == 0) {
				branchSizes.erase(branchSizes.begin() + j);
				close = true;
			}
		}

		if (i.second.Type != 2)
			oFile << i.first << " { [" << i.second.Type << "] [";

		switch (i.second.Type) {
		case 0:			//int
			oFile << *i.second.iVar << "] }" << std::endl;
			break;				

		case 1:			//POINT 
			oFile << i.second.pVar->x << "] [" << i.second.pVar->y << "] }" << std::endl;
			break;

		case 2:			//Branch Identifier
			oFile << i.first << " { [" << i.second.Type << "] } {" << std::endl;
			branchSizes.push_back(i.second.BranchSize);
			break;
		}

		if (close) {
			close = false;

			if (branches > 0) {
				for (int j = 0; j < branches - 1; j++) {
					oFile << char(9);
				}
			}

			oFile << "}" << std::endl;
		}
	}
	 
	oFile.close();
}

int Config::FindVarByName(std::string Name) {
	for (int i = 0; i < m_vVarList.size(); i++)
		if (m_vVarList[i].first == Name)
			return i;
}

bool Config::VarExists(std::string Name) {
	for (auto& i : m_vVarList) 
		if (Name == i.first)
			return true;
	return false;
}

void Config::Read(std::string FileName) {
	std::ifstream iFile;
	iFile.open(FileName);

	std::string in;
	for (auto& i : m_vVarList) {		//just make it so it will not read empty lines thus not fucking up and causing an exception
		std::getline(iFile, in);

		std::string closingBracketCheck = "";
		for (auto& k : in) {
			if (k != char(9))
				closingBracketCheck += k;
		}

		if (closingBracketCheck == "}")
			std::getline(iFile, in);

		std::vector<std::string> elems;
		std::string currElement = "";
		std::string Name;
		int idx = 0;

		for (auto& j : in) {
			if (j == '{')
				idx++;

			if (j != ' ' && idx == 0 && j != '	') {
				Name += j;
				continue;
			}

			if (j == ']') {
				elems.push_back(currElement);
				currElement = "";
				continue;
			}

			if (j != ' ' && j != '[' && j != ']' && j != '{' && j != '}' && j != '	')
				currElement += j;

		}

		if (elems.empty()) {
			if (!iFile.eof())
				continue;

			MessageBox(NULL, "Error Reading Config, Using Default Values", "Error", NULL);
			return;
		}

		switch (std::stoi(elems[0])) {
		case 0:			//int
			if (VarExists(Name))
				Config::Get()->SetVar(std::stoi(elems[1]), Name);
			break;

		case 1:			//POINT
			if (VarExists(Name))
				Config::Get()->SetVar(POINT( std::stoi(elems[1]), std::stoi(elems[2]) ), Name);
			break;

		case 2:			//Branch Identifier
			break;
		}

		

	}

	iFile.close();
}

bool Config::Init(std::string FileName) {
	Config::Get()->AddBranch("General", 0);									//potentially figure out a way to make it so it encompasses all variables automatically?
	Config::Get()->AddVar(&Window::Get()->m_pResolution, "Resolution");

	//sets the General branch to be the size of the list of variables, this is to always be at the end of the initialization list
	m_vVarList[0].second.BranchSize = m_vVarList.size() - 1;

#ifdef _DEBUG
	std::cout << "Config Initialized" << std::endl;
#endif

	std::ifstream f("Global.cfg");
	if (!f.good()) {
		this->Write("Global.cfg");
#ifdef _DEBUG
		std::cout << "Global Config Not Found, creating with default values" << std::endl;
#endif
	}
	this->Read("Global.cfg");
	//this->Write("Global.cfg");		for testing purposes

#ifdef _DEBUG
	std::cout << "Global Config Loaded" << std::endl;
#endif

	return true;
}

void Config::AddBranch(std::string Name, int Size) {
	VarInfo_t info;
	info.Type = 2;
	info.BranchSize = Size;
	info.IsBranch = true;
	m_vVarList.push_back(std::pair(Name, info));
}
