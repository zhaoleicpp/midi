#include "config.h"
int stat(const char *filename,struct stat *st)
{
	st->st_mode=0;
	st->st_dev=0;
	st->st_ino=0;
	st->st_size=0;
	st->st_mtime=0;
	st->st_ctime=0;
	st->st_btime=0;
	return 0;
}
int usleep(unsigned int x)
{
	return 0;
};
long url_read(URL url,void *buff,long n)
{
	ASSERT(0);
	return 0;
}
URL url_mem_open(char *memory,long memsiz,int autofree)
{
	ASSERT(0);
	return 0;
}
URL url_inflate_open(URL instream,long compsize,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_buff_open(URL url,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_cache_open(URL url,int autoclose)
{
	ASSERT(0);
	return 0;
}
void url_cache_detach(URL url)
{
	ASSERT(0);
}
void url_cache_disable(URL url)
{
	ASSERT(0);
}
URL url_uudecode_open(URL reader,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_b64decode_open(URL reader,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_hqxdecode_open(URL reader,int dataonly,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_qsdecode_open(URL reader,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_cgi_escape_open(URL reader,int autoclose)
{
	ASSERT(0);
	return 0;
}
URL url_cgi_unescape_open(URL reader,int autoclose)
{
	ASSERT(0);
	return 0;
}
char *url_dir_name(URL url)
{
	ASSERT(0);
	return 0;
}
char *url_newsgroup_name(URL url)
{
	ASSERT(0);
	return 0;
}
int url_news_connection_cache(int flag)
{
	ASSERT(0);
	return 0;
}
char *url_expand_home_dir(char *filename)
{
	//就是简单的文件名，没啥
	//函数已完成
	return filename;
}
char *url_unexpand_home_dir(char *filename)
{
	//就是简单的文件名，没啥
	//函数已完成
	return filename;
}
int url_getc(URL url)
{
	ASSERT(0);
	return 0;
}
void url_close(struct _URL *url)
{
	ASSERT(0);
}
void url_skip(URL url,long n)
{
	ASSERT(0);
}
char *url_gets(struct _URL *url,char *buff,int n)
{
	ASSERT(0);
	return 0;
}
long url_nread(URL url,void *buff,long n)
{
	ASSERT(0);
	return 0;
}
long url_seek(struct _URL *url,long offset,int whence)
{
	ASSERT(0);
	return 0;
}
long url_tell(struct _URL *url)
{
	ASSERT(0);
	return 0;
}
char **expand_archive_names(int *nfiles_in_out,char **files)
{
	ASSERT(0);
	return 0;
}
int convert_mod_to_midi_file(struct MidiEvent *ev)
{
	ASSERT(0);
	return 0;
}
int IS_URL_SEEK_SAFE(URL *url)
{
	ASSERT(0);
	return 0;
}
int get_module_type (char *fn)
{
	//没有附加的模块，只能是其他文件
	return 0;//IS_OTHER_FILE;
}
int load_module_file (struct timidity_file *tf,int mod_type)
{
	ASSERT(0);
	return 0;
}
void url_rewind(URL url)
{
	ASSERT(0);
}
char *get_module_title (struct timidity_file *tf,int mod_type)
{
	ASSERT(0);
	return 0;
}
int sbk_to_sf2(int oper,int amount)
{
	ASSERT(0);
	return 0;
}
void *getwd(URL url)//没找到定义
{
	ASSERT(0);
}
void url_add_module(struct URL_module *m)
{
	ASSERT(0);
}
void ML_RegisterAllLoaders (void)
{
	//不需要加载插件，本函数为空函数
}
void free_archive_files(void)
{
	//总是在最后调用
	//没有调用archive相关的函数，也会来到这里。
	//所以本函数为空函数。
}
int get_archive_type(char *archive_name)
{
	ASSERT(0);
	return 0;
}