#include "pch.h"
#include "MidiToWave.h"
extern "C"
{
    int TimidityGo(int argc,const char **argv);
};
CFile * CMidiToWave::m_raw_wave=NULL;
CFile * CMidiToWave::m_midi=NULL;
CFile * CMidiToWave::m_timber=NULL;
CFile CMidiToWave::m_timber_spare_tire;
void  CMidiToWave::SetTimberSpareTire(CFile &file)
{
    m_timber_spare_tire.Open(NULL,FILE_OPEN_MEMORY);
    m_timber_spare_tire.SetLength(file.GetLength());
    file.SeekToBegin();
    file.Read(m_timber_spare_tire.m_buffer,m_timber_spare_tire.GetLength());
}
bool CMidiToWave::IsTimberSpareTireReady()
{
    if(m_timber_spare_tire.m_is_memory&&m_timber_spare_tire.m_buffer&&m_timber_spare_tire.m_file_length)
        return true;
    return false;
}
bool CMidiToWave::Go(CFile &raw_wave,CFile &midi,CString &error,CFile &timber)
{
    if (&timber == NULL)
    {
        if (!IsTimberSpareTireReady())
        {
            CString path;
            g_configure.GetTemporaryPath(path);
            path += _T("乐器库/资源/MidiTimber.sf2");
            CFile file;
            if (!file.Open(path, FILE_OPEN_READ))
            {
                CString string;
                string.Format(_T("Can not load timber file @ %s\r\n"), (LPCTSTR)path);
                OutputDebugString(string);
                if (&error)
                    error = _T("No timber file to load.");
                return false;
            }
            CMidiToWave::SetTimberSpareTire(file);
            file.Close();
        }
        m_timber = &m_timber_spare_tire;
    }
    else
        m_timber=&timber;
    if(m_timber->GetLength()==0)
    {
        if(&error)
            error=_T("No timber file load.");
        return false;
    }
    raw_wave.SetLength(0);
    m_raw_wave=&raw_wave;
    midi.SeekToBegin();
    m_midi=&midi;
    //const char *argv[]={"timidity++.exe","--help"};
    //const char *argv[]={"timidity++.exe","-Ow","-o","output.wav","input.mid"};
    const char *argv[]={"timidity++.exe","--verbose=-10","-OrM1","-o","output.raw","-s","16000","input.mid"};
    //const char *argv[]={"timidity++.exe","-OrM1","-o","output.raw","-s","16000","input.mid"};
    //OutputDebugString(_T("Output 16000Hz, 1 channal, 16bit sample\r\n"));
    TimidityGo(sizeof(argv)/sizeof(argv[0]),argv);
    m_timber=NULL;
    {
        int last_zero;
        for(last_zero=0;last_zero<raw_wave.GetLength()/2;last_zero++)
        {
            raw_wave.Seek(raw_wave.GetLength()-(last_zero+1)*2,FILE_SEEK_BEGIN);
            short sample;
            raw_wave.Read(&sample,sizeof(sample));
            if(sample<-4||sample>4)
            {
                last_zero--;
                break;
            }
        }
        raw_wave.SetLength(raw_wave.GetLength()-(last_zero+1)*2);
    }
    return true;
}
bool CMidiToWave::Go(CWave &wave,CFile &midi,CString &error,CFile &timber)
{
    CFile dest;
    dest.Open(NULL,FILE_OPEN_MEMORY);
    if(!CMidiToWave::Go(dest,midi,error,timber))
    {
        dest.Close();
        return false;
    }
    wave.SetLength(dest.GetLength()/sizeof(short));
    dest.SeekToBegin();
    for(int index=0;index<wave.GetLength();index++)
    {
        short x;
        dest.Read(&x,sizeof(x));
        double normaled;
        normaled=x;
        normaled*=5.5;//输出的声音就是比较小
        normaled/=0x7fff;
        wave[index]=normaled;
    }
    return true;
}
