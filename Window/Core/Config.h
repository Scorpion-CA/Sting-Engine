#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>

struct VarInfo_t {
	int* iVar;
	POINT* pVar;	//I'm stupid so every type I use will have its own member variable, RIP memory management once there gets to be a lot of variables
	int Type;		//0: int, 1: POINT, 2: Branch Identifier, add more as needed
	int BranchSize;	//these are only used if it is a branch identifier
	bool IsBranch;
};

class Config {
public:
	static Config* Get();
	bool Init(std::string FileName);

	void AddVar(int* Val, std::string Name);
	void AddVar(POINT* Val, std::string Name);

	void SetVar(int Val, std::string Name);
	void SetVar(POINT Val, std::string Name);

	void SetVarPtr(int* Val, std::string Name);
	void SetVarPtr(POINT* Val, std::string Name);

	void AddBranch(std::string Name, int Size);

	bool VarExists(std::string Name);

	void Read(std::string FileName);
	void Write(std::string FileName);

	int FindVarByName(std::string Name);

private:
	std::string FileName;
	std::vector<std::pair<std::string, VarInfo_t>> m_vVarList;
};