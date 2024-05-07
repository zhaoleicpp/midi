#pragma once
class CMidiToWave
{
public:
	static CFile *m_raw_wave;
	static CFile *m_midi;
	static CFile *m_timber;
    static CFile m_timber_spare_tire;
public:
    static void SetTimberSpareTire(CFile &file);
    static bool IsTimberSpareTireReady();
	static bool Go(CFile &raw_wave,CFile &midi,CString &error=*(CString*)NULL,CFile &timber=*(CFile*)NULL);
    static bool Go(CWave &wave,CFile &midi,CString &error=*(CString *)NULL,CFile &timber=*(CFile *)NULL);
};
/*
#include "Library/MidiToWave/MidiToWave.h"
void main()
{
    CFile midi,raw;
    {
        CFile source;
        source.Open(_T("C:\\Important\\Project\\语音\\Big file\\background_music.mid"),FILE_OPEN_READ);
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
        dest.Open(_T("C:\\Important\\Project\\语音\\Big file\\from_midi.bin"),FILE_OPEN_WRITE|FILE_OPEN_CREATE);
        dest.Write(raw.m_buffer,raw.GetLength());
        dest.Close();
    }
}
*/
