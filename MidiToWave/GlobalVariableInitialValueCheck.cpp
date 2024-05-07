#include "pch.h"
#include "GlobalVariableInitialValueCheck.h"
/*
	{
		for(int loop=0;loop<2;loop++)
		{
			CFile midi,raw;
			{
				CFile source;
				//CString path;
				//path.Format(_T("C:\\Important\\Project\\语音\\Big file\\debug_midi_%d.mid"),loop);
				//source.Open(path,FILE_OPEN_READ);
				source.Open(_T("C:\\Important\\Project\\语音\\Big file\\debug_midi_1.mid"),FILE_OPEN_READ);
				midi.Open(NULL,FILE_OPEN_MEMORY);
				midi.SetLength(source.GetLength());
				source.Read(midi.m_buffer,source.GetLength());
				source.Close();
				midi.SeekToBegin();
			}
			raw.Open(NULL,FILE_OPEN_MEMORY);
			CMidiToWave::Go(raw,midi);
			{
				CFile dest;
				CString path;
				path.Format(_T("C:\\Important\\Project\\语音\\Big file\\from_midi_%d.bin"),loop);
				dest.Open(path,FILE_OPEN_WRITE|FILE_OPEN_CREATE);
				dest.Write(raw.m_buffer,raw.GetLength());
				dest.Close();
			}
		}
		return ;
	}
*/

extern "C"
{
	void g_global_variable_initial_value_check_Go()
	{
		g_global_variable_initial_value_check.Go();
	};
}
CGlobalVariableInitialValueCheck g_global_variable_initial_value_check;
void CGlobalVariableInitialValueCheck::Go()
{
	if(m_counter==0)
		Save();
	else
		Check();
	m_counter++;
}
bool CGlobalVariableInitialValueCheck::IsInFileList(LPCTSTR file_name)
{
	LPCTSTR file_list[]={_T("aq"),_T("audio_cnv"),_T("bitset"),_T("common"),_T("controls"),_T("dumb_c"),_T("effect"),_T("fft"),_T("fft4g"),_T("filter"),_T("freq"),_T("getopt"),_T("instrum"),_T("list_a"),_T("loadtab"),_T("mblock"),_T("memb"),_T("mfi"),_T("miditrace"),_T("mix"),_T("mt19937ar"),_T("optcode"),_T("output"),_T("playmidi"),_T("quantity"),_T("raw_a"),_T("rcp"),_T("readmidi"),_T("recache"),_T("resample"),_T("reverb"),_T("sffile"),_T("sfitem"),_T("smplfile"),_T("sndfont"),_T("strtab"),_T("support"),_T("tables"),_T("timer"),_T("timidity"),_T("version"),_T("wave_a"),_T("wrdt"),_T("wrd_read")};
	for(LPCTSTR current: file_list)
		if(tstricmp(current,file_name)==0)
			return true;
	return false;
}
void CGlobalVariableInitialValueCheck::Save()
{
	m_array.RemoveAll();
#ifndef _DEBUG
	return ;
#endif
	uintptr_t load_base_address;
	{
		MEMORY_BASIC_INFORMATION membinfo;
		static int ibase=0;//这里要是静态变量，否则失败
		VirtualQuery(&ibase,&membinfo,sizeof(membinfo));
		load_base_address=(uintptr_t)(membinfo.AllocationBase);
	}


	CString map;
	if(!CFileUtil::ReadWholeTextFile(map,_T("C:\\Important\\Project\\语音\\Vocality\\Debug\\Vocality.map")))
	{
		ASSERT(0);//打开map开关，生成map文件
	}
	for(int index_map=0;true;index_map++)
	{
		CString line;
		CStringUtil::GetLine(line,map,index_map,index_map);
		if(line.IsEmpty())
			break;
		CString segment,var_name,offset,last_but_two,last_but_one,last;
		for(int index_line=0,word_counter=0;true;index_line++,word_counter++)
		{
			CString word;
			CStringUtil::GetWord(word,line,index_line,index_line);
			if(word.IsEmpty())
				break;
			if(word_counter==0)
				segment=word;
			if(word_counter>2)
				if(word.GetLength()==8)
					if(CStringUtil::IsNumber(word[0]))
					{
						var_name=last;
						offset=word;
					}
			last_but_two=last_but_one;
			last_but_one=last;
			last=word;
		}
		if(offset.GetLength()!=8||!CStringUtil::IsNumber(offset[0]))
		{
			if(m_array.GetLength())
			{
				CItem &last=m_array[m_array.GetLength()-1];
				if(last.length==m_length_unkown)
					last.length=m_length_can_not_get;
			}
			continue;
		}
		uintptr_t offset_integer;
		tsscanf(offset,_T("%p"),&offset_integer);
		if(m_array.GetLength())
		{
			CItem &last=m_array[m_array.GetLength()-1];
			if(last.length==m_length_unkown)
			{
				last.length=offset_integer-last.start_address;
				last.start_address+=load_base_address-0x00400000;
				last.data.Write((void *)(last.start_address),last.length);
			}
		}
		if(segment==_T("0002")||segment==_T("0001"))
			continue;//Text
		if(last_but_one!=_T(".")||last!=_T("obj"))
			continue;
		if(!IsInFileList(last_but_two))
			continue;
		CItem &new_=m_array.New();
		new_.start_address=offset_integer;
		new_.length=m_length_unkown;
		new_.var_name=var_name;
		new_.file_name=last_but_two;
		tsscanf(segment,_T("%x"),&new_.segment);
		new_.data.Open(NULL,FILE_OPEN_MEMORY);
	}
}
void CGlobalVariableInitialValueCheck::Check()
{
	for(int index=0;index<m_array.GetLength();index++)
	{
		CItem &item=m_array[index];
		if(item.length<=0)
			continue;
		if(memcmp((void *)(item.start_address),item.data.m_buffer,item.data.GetLength())!=0)
		{
			CString string;
			string.Format(_T("CGlobalVariableInitialValueCheck::Check var name=%s file_name=%s length=%d\r\n"),(LPCTSTR)item.var_name,(LPCTSTR)item.file_name,item.length);
			OutputDebugString(string);
		}
	}
}
