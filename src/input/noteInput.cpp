#include "../gui/gui.hpp"
#include "../input/noteInput.hpp"
#include "../views/pattern/songEditor.hpp"
#include "../globalFunctions.hpp"
#include "../state.hpp"
extern List *instrList;


static int noteChn[128];
static int noteChn2[FM_ch];
static int noteChn3[FM_ch];
static int noteMidiCh[16];

vector<saveNote> noteActive[16];

static vector<int> recordChannels;

void configurePreviewChannel(int channel)
{
	fm->ch[channel].muted = 0;
	fm->ch[channel].vol = 1;
	fm->ch[channel].reverbSend = fm_volumeToExp(config->previewReverb.value);
	fm->ch[channel].destPan = fm->ch[channel].pan = 127;
}

void previewNote(int instrument, int id, int volume, int isFromMidi)
{
	int channel = -1;


	if (!isFromMidi)
	{
		for (unsigned i = 0; i < FM_ch; i++)
		{
			if (id == noteChn2[i] - 1)
			{
				return;
			}
		}
	}

	// if the same note is already playing, overwrite it
	for (unsigned i = 0; i < FM_ch; i++)
	{

		if (id == noteChn3[i] - 1)
		{
			channel = i;
			break;
		}
	}
	// 
	if (channel == -1)
	{
		int nbTries = 0;
		do
		{
			channel = (channel + 1) % FM_ch;
			nbTries++;
		} while (fm->ch[channel].active && nbTries<FM_ch);
	}

	if (channel == -1)
	{
		int nbTries = 0;
		do
		{
			channel = (channel + 1) % FM_ch;
			nbTries++;
		} while ((noteChn2[channel] > 0) && nbTries < FM_ch);
		return;
	}

	recordChannels.clear();

	for (unsigned i = 0; i < FM_ch; i++)
	{
		if (songEditor->channelHead[i].record.selected)
		{
			recordChannels.push_back(i);
		}
	}

	configurePreviewChannel(channel);
	fm_playNote(fm, instrument, id, channel, volume);
	noteChn[id] = channel;
	noteChn3[channel] = id + 1;

	if (isFromMidi)
		noteActive[instrument].push_back({id,channel});

	// song editor : record notes
	if (state == songEditor && !instrList->selected && recordChannels.size() > 0)
	{
		channel = 0;
		while ((noteChn2[channel] > 0 || !songEditor->channelHead[channel].record.selected) && channel < FM_ch - 1)
		{
			channel++;

		}


		if (channel>recordChannels[recordChannels.size() - 1])
		{
			channel = recordChannels[recordChannels.size() - 1];
		}

		songEditor->recordFromKeyboard(id, volume, channel, isFromMidi);

	}

	noteChn2[channel] = id + 1;




}

void previewNoteBend(fmsynth *f, int value, int midich)
{
	for (int i = 0; i < noteActive[midich].size(); i++)
	{
		f->ch[noteActive[midich][i].fmChannel].pitchBend = 1 - (float)(128 - value) * 9.2852373168154813872606848242328e-4;
	}
}

void previewNoteStop(int id, int isFromMidi, int midich)
{
	if (isFromMidi)
	{
		for (int i = 0; i < noteActive[midich].size(); i++)
		{
			if (id == noteActive[midich][i].id)
			{
				fm_stopNote(fm, noteActive[midich][i].fmChannel);
				if (state == songEditor)
				{
					songEditor->recordFromKeyboard(128, 0, noteActive[midich][i].fmChannel, isFromMidi); // 128 = note off
				}
				noteActive[midich].erase(noteActive[midich].begin()+i);
				
				break;
			}
		}
		
	}
	else {
		fm_stopNote(fm, noteChn[id]);
		for (unsigned i = 0; i < FM_ch; i++)
		{
			if (id == noteChn2[i] - 1)
			{

				if (state == songEditor)
					songEditor->recordFromKeyboard(128, 0, i, isFromMidi); // 128 = note off
				noteChn2[i] = 0;
			}
		}
	}


}

void previewNoteStopAll(int midich)
{
	if (midich >= 0)
	{
		for (int i = 0; i < noteActive[midich].size(); i++)
		{
			
			fm_stopNote(fm, noteActive[midich][i].fmChannel);
			if (state == songEditor)
			{
				songEditor->recordFromKeyboard(128, 0, noteActive[midich][i].fmChannel, 1); // 128 = note off
			}
			noteActive[midich].erase(noteActive[midich].begin()+i);
			i--;
		}
	}
	else
	{
		for (unsigned i = 0; i < FM_ch; i++)
		{
			noteChn2[i] = 0;
			fm_stopNote(fm, i);
		}
	}
	
}