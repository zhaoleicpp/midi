#include "pch.h"
#include "timidity_file.h"
#include "MidiToWave.h"

struct timidity_file *open_file(char *name,int decompress,int noise_mode)
{
	timidity_file *ret=new timidity_file();
	ret->url=NULL;//不知道干嘛用的，先给空
	ret->file=new CFile();
	CFile &file=*(CFile*)(ret->file);
	if(strcmp(name,"timidity.cfg")==0||strcmp(name,"/home/web_user:.timidity.cfg")==0)
	{
		file.Open(NULL,FILE_OPEN_MEMORY);
		LPCSTR text=
			"soundfont \"default.sf2\" \n"
			"#extension opt -U\n";
		file.Write(text,strlen(text));
		file.SeekToBegin();
	}
	else if(strcmp(name,"default.sf2")==0)
	{
		file.Attach(*CMidiToWave::m_timber);
		file.SeekToBegin();
	}
	else if(strcmp(name,"input.mid")==0)
	{
		file.Attach(*CMidiToWave::m_midi);
		file.SeekToBegin();
	}
	else
	{
		CString string;
		USE_STRING_CONVERT;
		string.Format(_T("timidity_file.cpp can not open file %s\r\n"),ASCII_TO_TCHAR(name));
		OutputDebugString(string);
		ASSERT(0);
	}
	return ret;
}
struct timidity_file *open_file_r(char *name,int decompress,int noise_mode)
{
	return open_file(name,decompress,noise_mode);
}
struct timidity_file *open_with_mem(char *mem,int memlen,int noise_mode)
{
	return open_file(mem,0,0);
}
void close_file(struct timidity_file *tf)
{
	if(tf==NULL)
		return ;
	CFile &file=*(CFile *)(tf->file);
	file.Close();
	delete (CFile *)(tf->file);
	delete tf;
	tf=NULL;
}
void skip(struct timidity_file *tf,int len)
{
	CFile &file=*(CFile *)(tf->file);
	//不懂什么意思
	file.Seek(len,FILE_SEEK_CURRENT);
}
char *tf_gets(char *buff,int n,struct timidity_file *tf)
{
	CFile &file=*(CFile *)(tf->file);
	for(int index=0;true;index++)
	{
		if(index==n-1)
		{
			buff[index]='\0';
			break;
		}
		char ch;
		if(file.Read(&ch,sizeof(ch))!=sizeof(ch))
		{
			buff[index]='\0';
			if(buff[0]=='\0')
				return NULL;
			break;
		}
		if(ch=='\n')
		{
			buff[index]='\0';
			break;
		}
		buff[index]=ch;
	}
	return buff;
}
int tf_getc(struct timidity_file *tf)
{
	//猜测功能
	CFile &file=*(CFile *)(tf->file);
	unsigned char ch;
	if(file.Read(&ch,sizeof(ch))!=sizeof(ch))
		return EOF;
	return ch;
}
long tf_read(void *buff,int size,int nitems,struct timidity_file *tf)
{
	CFile &file=*(CFile *)(tf->file);
	int ret;
	ret=file.Read(buff,size*nitems);
	ret/=size;
	return ret;
}
long tf_seek(struct timidity_file *tf,long offset,int whence)
{
	CFile &file=*(CFile *)(tf->file);
	file.Seek(offset,whence);
	return file.GetPosition();
}
long tf_tell(struct timidity_file *tf)
{
	CFile &file=*(CFile *)(tf->file);
	return file.GetPosition();
}

intptr_t output_open(char const *const _FileName,int const _OFlag)
{
	CFile *file_pointer=new CFile();
	CFile &file=*file_pointer;
	file.Open(NULL,FILE_OPEN_MEMORY);
	file.Attach(*CMidiToWave::m_raw_wave);
	return (intptr_t)(void *)file_pointer;//64位有问题!!!!
}
int output_close(intptr_t _FileHandle)
{
	CFile &file=*(CFile*)(void *)_FileHandle;
	file.Close();
	delete &file;
	return 0;
}
long output_lseek(intptr_t  _FileHandle,long _Offset,int _Origin)
{
	CFile &file=*(CFile *)(void *)_FileHandle;
	return file.Seek(_Offset,_Origin);
}
int output_std_write(intptr_t fd,const void *buffer,int size)
{
	CFile &file=*(CFile *)(void *)fd;
	return file.Write(buffer,size);
}
