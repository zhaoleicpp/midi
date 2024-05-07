#include "pch.h"
#include "Midi.h"
using namespace std;
using namespace smf;
bool CMidiHideTrack9Detail::IsEmpty()
{
	if(m_instrument.GetLength()==0)//先用这个判断。有的音轨可能没有音符，暂时忽略这种情况
		return true;
	return false;
}
void CMidiHideTrack9Detail::Initialize(int track_count,int quarter_note_per_minute)
{
	int user_track_count;
	user_track_count=track_count;
	int actual_track_count;
	actual_track_count=track_count;
	//drum [
	if(actual_track_count<10)
		actual_track_count=10;
	else
		actual_track_count++;//给drum让出一个来
	//drum ]
	if(actual_track_count>1)
		m_outputfile.addTrack(actual_track_count-1);//原来有一个

	{
		vector<uchar> midievent;
		midievent.resize(6);
		midievent[0]=0xff;
		midievent[1]=0x51;
		midievent[2]=0x03;
		int time;
		time=60000000/quarter_note_per_minute;
		midievent[3]=(unsigned char)(time>>16);
		midievent[4]=(unsigned char)(time>>8);
		midievent[5]=(unsigned char)(time>>0);
		for(int track=0;track<track_count;track++)
			m_outputfile.addEvent(track,0,midievent);
	}
	m_outputfile.setTicksPerQuarterNote(m_tick_per_quarter_note);

	m_integral.SetLength(user_track_count);
	for(int track=0;track<user_track_count;track++)
		m_integral[track]=0;
	m_instrument.SetLength(user_track_count);

	m_all_abosulute_start_time.SetLength(actual_track_count);
	m_no_abosulute_start_time_is_0.SetLength(actual_track_count);
	for(int track=0;track<actual_track_count;track++)
	{
		m_all_abosulute_start_time[track]=true;
		m_no_abosulute_start_time_is_0[track]=true;
	}
	//drum [
	m_drum_instrument=26;
	ChangeInstrument(9,m_drum_instrument,true,0);
	//drum ]
}
void CMidiHideTrack9Detail::ChangeInstrument(int track,int instrument,bool internal_,int time_valid_only_when_internal)
{
	int user_track;
	user_track=track;
	int actual_track;
	actual_track=track;
	//drum [
	if(!internal_)
	{
		m_instrument[user_track]=instrument;
		if(actual_track>=9)
			actual_track++;
		if(instrument>=128)
			return ;
	}
	//drum ]
	vector<uchar> midievent;     // temporary storage for MIDI events
	midievent.resize(2);        // set the size of the array to 3 bytes
	midievent[0]=0xC0|(unsigned char)actual_track;//选择乐器
	midievent[1]=(unsigned char)instrument;
	int time;
	if(internal_)
		time=time_valid_only_when_internal;
	else
		time=m_integral[user_track];
	m_outputfile.addEvent(actual_track,time,midievent);
}
void CMidiHideTrack9Detail::AddNode(int track,int pitch/*0~127*/,double duration_by_quarter_note,double strength/*0~1*/,double absolute_time_start_by_quarter_note)
{
	if(pitch==-1)
	{
		m_integral[track]+=Round(duration_by_quarter_note*m_tick_per_quarter_note);
		return ;
	}
	//drum [
	int user_track;
	user_track=track;
	int actual_track;
	actual_track=track;
	{
		int instrument;
		instrument=m_instrument[user_track];
		if(instrument>=128)
		{
			actual_track=9;
			instrument-=128;
			if(instrument!=m_drum_instrument)
			{
				m_drum_instrument=instrument;
				ChangeInstrument(actual_track,instrument,true,m_integral[user_track]);
			}
		}
		else
		{
			if(actual_track>=9)
				actual_track++;
		}
	}
	//drum ]
	vector<uchar> midievent;     // temporary storage for MIDI events
	midievent.resize(3);        // set the size of the array to 3 bytes
	midievent[0]=0x90|(unsigned char)actual_track;//按下键
	midievent[1]=(unsigned char)pitch;
	midievent[2]=(unsigned char)(strength*127);
	int time;
	if(absolute_time_start_by_quarter_note==-1)
	{
		m_all_abosulute_start_time[actual_track]=false;
		time=m_integral[user_track];
	}
	else
	{
		if(absolute_time_start_by_quarter_note!=0)
			m_no_abosulute_start_time_is_0[actual_track]=false;
		time=Round(absolute_time_start_by_quarter_note*m_tick_per_quarter_note);
	}
	m_outputfile.addEvent(actual_track,time,midievent);

	int integer_duration;
	integer_duration=Round(duration_by_quarter_note*m_tick_per_quarter_note);
	if(absolute_time_start_by_quarter_note==-1)
		m_integral[user_track]+=integer_duration;
	midievent[0]=0x80|(unsigned char)actual_track;//松开键
	midievent[1]=(unsigned char)pitch;
	midievent[2]=0;
	if(absolute_time_start_by_quarter_note==-1)
		time=m_integral[user_track];
	else
		time+=integer_duration;
	m_outputfile.addEvent(actual_track,time,midievent);
}
void CMidiHideTrack9Detail::SaveToFile(CFile &file)
{
	//如果时间=0的地方没有音符，他会跳过开始的空白。
	for(int index=0;index<m_all_abosulute_start_time.GetLength();index++)
	{
		if(m_all_abosulute_start_time[index]&&m_no_abosulute_start_time_is_0[index])
		{
			m_no_abosulute_start_time_is_0[index]=false;
			vector<uchar> midievent;     // temporary storage for MIDI events
			midievent.resize(3);        // set the size of the array to 3 bytes
			midievent[0]=0x90|(unsigned char)index;//按下键
			midievent[1]=(unsigned char)0x3c;//pitch
			midievent[2]=(unsigned char)1;//strength
			m_outputfile.addEvent(index,0,midievent);
			midievent[0]=0x80|(unsigned char)index;//松开键
			midievent[1]=(unsigned char)0x3c;//pitch
			midievent[2]=(unsigned char)0;//strength
			m_outputfile.addEvent(index,1,midievent);
		}
	}

	m_outputfile.sortTracks();         // make sure data is in correct order

	class CBuffer:public std::streambuf
	{
	public:
		CFile *file;
		virtual int_type __CLR_OR_THIS_CALL overflow(int_type _Meta) override
		{
			file->Write(&_Meta,1);
			return 1;
		}
	};
	CBuffer buffer;
	buffer.file=&file;
	std::ostream out_stream(&buffer);
	m_outputfile.write(out_stream); // write Standard MIDI File twinkle.mid
}





LPCTSTR CMidi::InstrumentNumberFromInstrumentEnglishName(LPCTSTR english_name)
{
	ASSERT(0);
	return NULL;
}
void CMidi::Initialize(int quarter_note_per_minute)
{
	m_quarter_note_per_minute = quarter_note_per_minute;
	m_musica.RemoveAll();
}
void CMidi::AddNode(LPCTSTR instrument, int pitch, double duration_by_quarter_note, double strength, double absolute_time_start_by_quarter_note)
{
	CString instrument_string;
	{
		instrument_string = instrument;
		LPCTSTR target = _T("MidiInstrument✾defalult✾");
		if (instrument_string.Left(tstrlen(target)) == target)
			instrument_string = instrument_string.Right(instrument_string.GetLength()- tstrlen(target));
		if (!CStringUtil::IsNumber(instrument_string[0]))
			instrument_string = InstrumentNumberFromInstrumentEnglishName(instrument);
	}
	int instrument_integer;
	int fixed_pitch;
	{
		TArray<CString> array;
		CStringUtil::Split(array, instrument_string, _T(','));
		instrument_integer = tstoi(array[0]);
		if (array.GetLength() < 2)//普通乐器
			fixed_pitch = -1;
		else//打击乐器
			fixed_pitch = tstoi(array[1]);
		//钢琴 instrument_integer=0 fixed_pitch=-1
		//嗵鼓 instrument_integer=128+26 fixed_pitch=43
	}
	int track_index;
	for (track_index = 0; track_index < m_musica.GetLength(); track_index++)
		if (m_musica[track_index].instrument == instrument_integer)
			break;
	if (track_index >= m_musica.GetLength())
	{
		track_index = m_musica.GetLength();
		m_musica.New();
	}
	CTrack& track = m_musica[track_index];
	if (track.GetLength() == 0)
		track.instrument = instrument_integer;
	CNote& note = track.New();
	if (fixed_pitch == -1)
		note.pitch = pitch;
	else
		note.pitch = fixed_pitch;
	note.duration_by_quarter_note= duration_by_quarter_note;
	note.strength = strength;
	note.absolute_time_start_by_quarter_note = absolute_time_start_by_quarter_note;
}
void CMidi::SaveToFile(CFile& file)
{
	CMidiHideTrack9Detail::Initialize(m_musica.GetLength(), m_quarter_note_per_minute);
	for (int track_index = 0; track_index < m_musica.GetLength(); track_index++)
	{
		CTrack& track = m_musica[track_index];
		CMidiHideTrack9Detail::ChangeInstrument(track_index, track.instrument);
		for (int note_index = 0; note_index < track.GetLength(); note_index++)
		{
			CNote& note = track[note_index];
			CMidiHideTrack9Detail::AddNode(track_index, note.pitch, note.duration_by_quarter_note, note.strength, note.absolute_time_start_by_quarter_note);
		}
	}
	CMidiHideTrack9Detail::SaveToFile(file);
}
void CMidi::SaveToFile(LPCTSTR path)
{
	CFile file;
	file.Open(path, FILE_OPEN_CREATE | FILE_OPEN_WRITE);
	SaveToFile(file);
	file.Close();
}
void CMidi::Analysis(LPCTSTR string)
{
	TArray<CString> array;
	CStringUtil::Split(array, string, _T('\''));
	if (array.GetLength() < 2)
		return;
	if(array[0]!=_T("midi_string_1_0"))
		return;
	Initialize(tstoi(array[1]));
	for (int note_index = 2; note_index < array.GetLength(); note_index++)
	{
		CString& note_string = array[note_index];
		if (note_string.IsEmpty())
			continue;
		TArray<CString> parameters;
		CStringUtil::Split(parameters, note_string, _T(';'));
		AddNode(parameters[0], tstoi(parameters[1]), tstof(parameters[2]), tstof(parameters[3]), tstof(parameters[4]));
	}
}
