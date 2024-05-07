#pragma once
//需要Debug版本，并且打开map开关，生成map文件
//GlobalVariableInitialValueCheck.cpp
//文件有调用的代码，放在main.cpp里面即可。
class CGlobalVariableInitialValueCheck
{
	static const int m_length_unkown=-1;
	static const int m_length_can_not_get=0;
	class CItem
	{
	public:
		uintptr_t start_address;
		uintptr_t length;
		int segment;
		CString var_name;
		CString file_name;
		CFile data;
	};
	TArray<CItem>m_array;
	int m_counter=0;
	void Save();
	void Check();
	bool IsInFileList(LPCTSTR file_name);
public:
	void Go();
};
extern CGlobalVariableInitialValueCheck g_global_variable_initial_value_check;

