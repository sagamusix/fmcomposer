#include "songEditor.hpp"

void SongEditor::pattern_copy()
{
	copiedPattern.resize(fm->patternSize[fm->order]);
	for (int i = 0; i < copiedPattern.size(); i++)
	{
		copiedPattern[i].resize(FM_ch);
		for (int ch = 0; ch <FM_ch; ch++)
			copiedPattern[i][ch] = fm->pattern[fm->order][i][ch];
	}

	// Store the data as string into clipboard
	string clipboard=to_string(5*FM_ch)+","+to_string(fm->patternSize[fm->order])+",";

	for (int i = 0; i < fm->patternSize[fm->order]; i++)
	{
		for (int j = 0; j < FM_ch; j++)
		{
			clipboard += to_string(fm->pattern[fm->order][i][j].note)+",";
			clipboard += to_string(fm->pattern[fm->order][i][j].instr)+",";
			clipboard += to_string(fm->pattern[fm->order][i][j].vol)+",";
			clipboard += to_string(fm->pattern[fm->order][i][j].fx)+",";
			clipboard += to_string(fm->pattern[fm->order][i][j].fxdata)+",";
		}
	}
	sf::Clipboard::setString(clipboard);

}

void SongEditor::pattern_paste(int insertAfter)
{
	// detect if clipboard has some FM Composer data
	int clipboardWidth=-1;
	int clipboardHeight=-1;
	bool useClipboard=false;
	string clipboard=sf::Clipboard::getString();
	vector<string> splitted;
	if (sf::Clipboard::getString() != "")
	{
		splitted = split(clipboard,",");
		if (splitted.size() > 2)
		{
			clipboardWidth = atoi(splitted[0].c_str());
			clipboardHeight = atoi(splitted[1].c_str());

			if ((clipboardWidth > 0 && clipboardHeight > 0 && clipboardHeight < 256)) {
				useClipboard=true;
			}
		}
					

	}

	if (copiedPattern.size()>0 || useClipboard)
	{
		patternList.insert(fm->order + insertAfter, std::to_string(patternList.elementCount()));
		
		// Paste from clipboard
		if (useClipboard)
		{
			fm_insertPattern(fm, clipboardHeight, fm->order + insertAfter);
			int nbchannels=clipboardWidth/5;
			for (int i = 0; i < clipboardHeight; i++)
			{
				for (int j = 0; j < nbchannels;j++)
				{
					fm->pattern[fm->order + insertAfter][i][j].note = atoi(splitted[2+5*(i*nbchannels+j)+0].c_str());
					fm->pattern[fm->order + insertAfter][i][j].instr = atoi(splitted[2+5*(i*nbchannels+j)+1].c_str());
					fm->pattern[fm->order + insertAfter][i][j].vol = atoi(splitted[2+5*(i*nbchannels+j)+2].c_str());
					fm->pattern[fm->order + insertAfter][i][j].fx = atoi(splitted[2+5*(i*nbchannels+j)+3].c_str());
					fm->pattern[fm->order + insertAfter][i][j].fxdata = atoi(splitted[2+5*(i*nbchannels+j)+4].c_str());
				}
			}
		}
		// Regular paste using internal variables (maybe to be removed)
		else
		{
			fm_insertPattern(fm, copiedPattern.size(), fm->order + insertAfter);
			for (int i = 0; i < copiedPattern.size(); i++)
			{
				for (int ch = 0; ch < FM_ch; ch++)
					fm->pattern[fm->order + insertAfter][i][ch] = copiedPattern[i][ch];
			}
		}
		

		updateFromFM();
		songModified(1);
		history.insert(history.begin() + fm->order, vector<historyElem>());
		currentHistoryPos.insert(currentHistoryPos.begin() + fm->order, 0);
		fm_setPosition(fm, fm->order + insertAfter, fm->row, 2);
	}

	
}
void SongEditor::pattern_delete()
{
	if (fm->patternCount > 1)
	{

		patternList.erase(fm->order);
		history.erase(history.begin() + fm->order);
		currentHistoryPos.erase(currentHistoryPos.begin() + fm->order);

		fm_removePattern(fm, fm->order);


	}
	else
	{
		fm_removePattern(fm, 0);
		history[0].clear();
		currentHistoryPos[0] = 0;
	}
	fm->order = clamp(fm->order, 0, fm->patternCount - 1);
	updateFromFM();
	songModified(1);
}

void SongEditor::pattern_cut()
{

	copiedPattern.resize(fm->patternSize[fm->order]);
	for (int i = 0; i < copiedPattern.size(); i++)
	{
		copiedPattern[i].resize(FM_ch);
		for (int ch = 0; ch < FM_ch; ch++)
			copiedPattern[i][ch] = fm->pattern[fm->order][i][ch];
	}

	pattern_delete();
}

void SongEditor::pattern_duplicate()
{
	pattern_copy();
	pattern_paste(1);
}

void SongEditor::pattern_insert()
{
	patternList.insert(fm->order, std::to_string(patternList.elementCount()));
	fm_insertPattern(fm, patSize.value, fm->order);
	updateFromFM();
	history.insert(history.begin() + fm->order, vector<historyElem>());
	currentHistoryPos.insert(currentHistoryPos.begin() + fm->order, 0);
	songModified(1);
}

void SongEditor::pattern_move(int from, int to)
{
	fm_movePattern(fm, from, to);

	songModified(1);


	patternList.move(from, to);

	history.insert(history.begin() + to+(to>from), history[from]);
	history.erase(history.begin() + from + (to <= from));

	currentHistoryPos.insert(currentHistoryPos.begin() + to+(to>from), currentHistoryPos[from]);
	currentHistoryPos.erase(currentHistoryPos.begin() + from + (to <= from));

}