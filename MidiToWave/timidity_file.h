#pragma once
#include <stdint.h> 
struct timidity_file
{
	char *url;
	void *file;
};
#ifdef  __cplusplus
extern "C"
{
#endif
	struct timidity_file *open_file(char *name,int decompress,
		int noise_mode);
	struct timidity_file *open_file_r(char *name,int decompress,
		int noise_mode);
	struct timidity_file *open_with_mem(char *mem,int memlen,
		int noise_mode);
	void close_file(struct timidity_file *tf);
	void skip(struct timidity_file *tf,int len);
	char *tf_gets(char *buff,int n,struct timidity_file *tf);
	int tf_getc(struct timidity_file *tf);
	long tf_read(void *buff,int size,int nitems,struct timidity_file *tf);
	long tf_seek(struct timidity_file *tf,long offset,int whence);
	long tf_tell(struct timidity_file *tf);

	intptr_t output_open(char const *const _FileName,int const _OFlag);
	int output_close(intptr_t _FileHandle);
	long output_lseek(intptr_t  _FileHandle,long _Offset,int _Origin);
	int output_std_write(intptr_t fd,const void *buffer,int size);
	

#ifdef  __cplusplus
};
#endif
