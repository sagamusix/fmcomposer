#ifndef NOTEPREVIEW_H
#define NOTEPREVIEW_H
#include "../gui/gui.hpp"
#include "../libs/portmidi/portmidi.h"


void previewNote(int instrument, int id, int volume, int isFromMidi);

void previewNoteStop(int id, int isFromMidi, int midich=-1);

void previewNoteStopAll(int midich=-1);

void previewNoteBend(fmsynth *f, int value, int midich);

void configurePreviewChannel(int channel);

typedef struct saveNote{
	int id;
	int fmChannel;
}saveNote;

extern vector<saveNote> noteActive[16];

#endif