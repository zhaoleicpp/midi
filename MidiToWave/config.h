//I added
#pragma once
#ifdef __linux__
#include "pch.h"
#include <ctype.h>
#define __MACOS__ 1//这个模块没有定义linux，只能借用mac os了
#define stat stat_MidiToWave //用空函数
#define usleep usleep_MidiToWave//用空函数
#ifdef st_mtime
#undef st_mtime
#endif
#ifdef st_ctime
#undef st_ctime
#endif
#ifdef EXIT_FAILURE
#undef EXIT_FAILURE
#endif
struct _URL
{
	char no_use;
};
struct MidiEvent
{
	char no_use;
};
struct URL_module
{
	char no_use;
};
#else
inline void ASSERT(x)
{
	if(!(x))
	{
#if __WORDSIZE__ == 64 || defined _M_X64
		char* p = 0;
		*p = 0;
#else
		_asm int 3;
#endif
	}
}
#define stat stat_MidiToWave
#endif
#include "timidity_file.h"
#define HAVE_STRNCASECMP
#define HAVE_VSNPRINTF
#define HAVE_USLEEP
#define HAVE_SNPRINTF
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 0
#define CONFIG_FILE "timidity.cfg"
#define PACKAGE_VERSION "1.2"
#define strerror timidity_strerror
typedef const char *URL;
struct stat {
	short st_mode;
	short st_dev;
	long st_ino;
	unsigned long st_size;
	unsigned long st_mtime,st_ctime,st_btime;
};

#define HAVE_SYS_STAT_H
int stat(const char *filename,struct stat *st);
int usleep(unsigned int x);
long url_read(URL url,void *buff,long n);
URL url_mem_open(char *memory,long memsiz,int autofree);
URL url_inflate_open(URL instream,long compsize,int autoclose);
URL url_buff_open(URL url,int autoclose);
URL url_cache_open(URL url,int autoclose);
void url_cache_detach(URL url);
void url_cache_disable(URL url);
URL url_uudecode_open(URL reader,int autoclose);
URL url_b64decode_open(URL reader,int autoclose);
URL url_hqxdecode_open(URL reader,int dataonly,int autoclose);
URL url_qsdecode_open(URL reader,int autoclose);
URL url_cgi_escape_open(URL reader,int autoclose);
URL url_cgi_unescape_open(URL reader,int autoclose);
char *url_dir_name(URL url);
char *url_newsgroup_name(URL url);
int url_news_connection_cache(int flag);
char *url_expand_home_dir(char *filename);
char *url_unexpand_home_dir(char *filename);
int url_getc(URL url);
void url_close(struct _URL *url);
void url_skip(URL url,long n);
char *url_gets(struct _URL *url,char *buff,int n);
long url_nread(URL url,void *buff,long n);
long url_seek(struct _URL *url,long offset,int whence);
long url_tell(struct _URL *url);
char **expand_archive_names(int *nfiles_in_out,char **files);
int convert_mod_to_midi_file(struct MidiEvent *ev);
int IS_URL_SEEK_SAFE(URL *url);
int get_module_type (char *fn);
int load_module_file (struct timidity_file *tf,int mod_type);
void url_rewind(URL url);
char *get_module_title (struct timidity_file *tf,int mod_type);
int sbk_to_sf2(int oper,int amount);
void *getwd(URL url);//没找到定义
void url_add_module(struct URL_module *m);
void ML_RegisterAllLoaders (void);
void free_archive_files(void);
int get_archive_type(char *archive_name);
