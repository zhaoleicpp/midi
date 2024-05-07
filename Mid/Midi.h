#pragma once
#include "include/MidiFile.h"
class CMidiHideTrack9Detail
{
private:
	smf::MidiFile m_outputfile;
	const int m_tick_per_quarter_note=88;
	TArray<int> m_integral;
	TArray<int> m_instrument;
	TArray<bool> m_all_abosulute_start_time;
	TArray<bool> m_no_abosulute_start_time_is_0;
	int m_drum_instrument;
	bool m_use_small_sound_font;
public:
	//本类向调用者隐藏了音轨9是打击乐器这个midi底层的细节
	//要加入嗵鼓仅仅需要:
	//1 ChangeInstrument instrument=128+26 
	//2 AddNode pitch=43
	bool IsEmpty();
	void Initialize(int track_count=1,int quarter_note_per_minute=88);
	void AddNode(int track,int pitch/*0~127*/,double duration_by_quarter_note,double strength/*0~1*/,double absolute_time_start_by_quarter_note=-1);
	void ChangeInstrument(int track,int instrument,bool internal_=false,int time_valid_only_when_internal=0);
	void SaveToFile(CFile &file);
};
class CMidi : public CMidiHideTrack9Detail
{
	struct CNote
	{
		int pitch;
		double duration_by_quarter_note;
		double strength;
		double absolute_time_start_by_quarter_note;
	};
	class CTrack :public TArray<CNote>
	{
	public:
		int instrument;//"0" 钢琴 "128+26"跳舞的乐器
	};
	TArray<CTrack> m_musica;
	int m_quarter_note_per_minute;
public:
	static LPCTSTR InstrumentNumberFromInstrumentEnglishName(LPCTSTR english_name);
	//本类向调用者隐藏音轨，仅仅需要输入
	//1 普通乐器：乐器名+音高  乐器名一个数字，比如 “0”代表钢琴
	//2 打击乐器：乐器名      乐器名两个数字，中间要给逗号，比如“128+26,43” 表示嗵鼓
	void Initialize(int quarter_note_per_minute = 88);
	//instrument = "0" //钢琴
	//instrument = "128+26,43" //嗵鼓
	//instrument = "Acoustic Grand Piano" //钢琴
	//instrument = "High Floor Tom" //嗵鼓
	void AddNode(LPCTSTR instrument/* 如上 */, int pitch/* 0~127 */, double duration_by_quarter_note, double strength/*0~1*/, double absolute_time_start_by_quarter_note = -1);
	void SaveToFile(CFile& file);
	void SaveToFile(LPCTSTR path);
	//midi_string_1_0'quarter_note_per_minute'instrument;pitch;duration_by_quarter_note;strength;absolute_time_start_by_quarter_note
	//midi_string_1_0'88'0;60;0.5;1;2'128+26,43;0;0.5;1;4'Acoustic Grand Piano;60;0.5;1;2'High Floor Tom,43;0;0.5;1;4
	void Analysis(LPCTSTR string);
};
