#include "popup.hpp"

#include "../../views/pattern/songEditor.hpp"
extern SongEditor* songEditor;

/* Displays appropriate infos and sliders for each effect in the effect popup */


unsigned char findPrevious(int type, int _order, int _row, int channel, int*foundOrder, int*foundRow)
{
	while (_row < 0)
	{
		_order--;
		if (_order < 0)
		{
			_row = 0;
			_order = 0;
			break;
		}
		_row += fm_getPatternSize(fm, _order);
	}
	int row = _row;
	int order = _order;

	while (1)
	{
		if (type == 0 && fm->pattern[order][row][channel].note < 128
			|| type == 1 && fm->pattern[order][row][channel].instr != 255)
			break;
		row--;
		if (row < 0)
		{
			order--;
			if (order < 0)
			{
				order = 0;
				row = 0;
				break;
			}
			row = fm_getPatternSize(fm, order) - 1;
		}
	}
	if (foundOrder)
		*foundOrder = order;
	if (foundRow)
		*foundRow = row;

	if (type == 0)
		return fm->pattern[order][row][channel].note;
	else
		return fm->pattern[order][row][channel].instr;

}

void Popup::updateEffectDescription()
{
	sliders[1].setVisible(false);
	switch (lists[0].value)
	{
		default:
			texts[0].setString("");
			texts[1].setString("");
			break;
		case 0:{ // Arpeggio
				   sliders[0].setMinMax(0, 255);

				   int pos = fm->order * 128 + fm->row;
				   int maxBackward = 128;
				   while (maxBackward-- > 0 && pos > 0 && fm->pattern[pos / 128][pos % 128][songEditor->selectedChannel].note > 127)
				   {
					   pos--;
				   }
				   if (fm->pattern[pos / 128][pos % 128][songEditor->selectedChannel].note < 128)
				   {
					   int baseNote = fm->pattern[pos / 128][pos % 128][songEditor->selectedChannel].note;

					   texts[0].setString(string(lang("effects", "Play a fast triplet arpeggio"))+" :\n>" + noteName(baseNote) + L"\n> " + noteName(baseNote + sliders[0].value % 16) + " (" + intervals[sliders[0].value % 16] + L")\n> " + noteName(baseNote + sliders[0].value / 16) + " (" + intervals[sliders[0].value / 16] + ") ");
				   }
				   else
				   {
					   texts[0].setString(string(lang("effects", "Play a fast triplet arpeggio"))+" :\n> "+lang("effects", "Base note")+"\n> note+" + int2str[sliders[0].value % 16] + " (" + intervals[sliders[0].value % 16] + ") " + L"\n> note+" + int2str[sliders[0].value / 16] + " (" + intervals[sliders[0].value / 16] + ") ");
				   }
				   sliders[0].name.setString(lang("effects", "A_description"));
				   texts[1].setString("");
				   sprites[0].setColor(Color(0, 0, 0, 0));
		}
			break;
		case 1: // B - Pattern jump
			texts[0].setString(lang("effects", "B_description"));
			texts[1].setString("");
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString(lang("effects", "Pattern number"));
			sprites[0].setColor(Color(0, 0, 0, 0));
			break;
		case 2: // C - Row jump
			texts[0].setString(lang("effects", "C_description"));
			texts[1].setString("");
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString(lang("effects", "Row number"));
			sprites[0].setColor(Color(0, 0, 0, 0));
			break;
		case 3: // D - Delay
			texts[0].setString(lang("effects", "D_description"));
			texts[1].setString(lang("effects", "D_range"));
			sliders[0].setMinMax(0, 7);
			sliders[0].name.setString("");
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 4: // E - Portamento up
			texts[0].setString(lang("effects", "E_description"));
			texts[1].setString(lang("effects", "E_range"));
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString(lang("effects", "Speed"));
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 5: // F - Portamento down
			texts[0].setString(lang("effects", "F_description"));
			texts[1].setString(lang("effects", "F_range"));
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString(lang("effects", "Speed"));
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 6:{ // G - Glissando

				   texts[0].setString(lang("effects", "G_description"));

				   int order, row;
				   unsigned char note1 = findPrevious(0, fm->order, fm->row, songEditor->selectedChannel, &order, &row);

				   if (note1 < 128)
				   {

					   unsigned char note2 = findPrevious(0, order, row - 1, songEditor->selectedChannel, &order, &row);

					   if (note2 < 128)
					   {
						   texts[0].setString(string(lang("effects", "Pitch slide between")) + noteName(note2) + " and " + noteName(note1));
					   }

				   }


				   texts[1].setString(lang("effects", "G_range"));
				   sliders[0].setMinMax(0, 255);
				   sliders[0].name.setString(lang("effects", "speed"));
				   sprites[0].setColor(Color(255, 255, 255, 255));
		}
			break;
		case 7: // H Vibrato
			sliders[1].setVisible(true);
			sliders[0].setMinMax(0, 15);
			sliders[1].setMinMax(0, 15);
			texts[0].setString(lang("effects", "H_description"));
			texts[1].setString("");
			sliders[0].name.setString(lang("effects", "Speed"));
			sliders[1].name.setString(lang("effects", "Depth"));
			sprites[0].setColor(Color(255, 255, 255, 0));
			break;

		case 8: // I Pitch bend
			sliders[0].setMinMax(0, 255);
			texts[0].setString(lang("effects", "I_description"));
			texts[1].setString("-2							0						 +2");
			sliders[0].name.setString("");
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 9: // J Tremolo
			sliders[1].setVisible(true);
			sliders[0].setMinMax(0, 15);
			sliders[1].setMinMax(0, 15);
			texts[0].setString(lang("effects", "J_description"));
			texts[1].setString("");
			sliders[0].name.setString(lang("effects", "Speed"));
			sliders[1].name.setString(lang("effects", "Depth"));
			sprites[0].setColor(Color(255, 255, 255, 0));
			break;
		case 10:{ // K FM parameter select

					unsigned char instr = findPrevious(1, fm->order, fm->row, songEditor->selectedChannel, NULL, NULL);

					if (instr >= fm->instrumentCount)
						instr = 0;

					if (fm->instrument[instrList->value].kfx / 32 == 0)
						texts[0].setString(string(lang("effect", "Current instrument"))+" : " + string(fm->instrument[instr].name) + "\nSet Global " + kfx_globalParams[fm->instrument[instrList->value].kfx % 32]);
					else
						texts[0].setString(string(lang("effect", "Current instrument"))+" : " + string(fm->instrument[instr].name) + "\nSet Operator " + std::to_string(fm->instrument[instrList->value].kfx / 32) + " : " + kfx_operatorParams[fm->instrument[instrList->value].kfx % 32]);



					texts[1].setString("");
					sliders[0].name.setString(lang("global", "Value"));
					sprites[0].setColor(Color(255, 255, 255, 255));



					break;


		}
		case 11:{ // L temperament

			sliders[1].setVisible(true);
			sliders[0].setMinMax(0, 2);
			sliders[1].setMinMax(0, 11);
			texts[0].setString(lang("effects", "L_description"));
			texts[1].setString("");

			 sliders[0].name.setString(lang("instrEditor", "Temperament"));
			  sliders[1].name.setString(lang("popup", "Base note"));

			string temperamentNames[3]={"Equal Tempered", "Werckmeister III", "Meantone"};
			string noteNames[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

			sliders[0].setDisplayedValueOnly(temperamentNames[sliders[0].value]);
			sliders[1].setDisplayedValueOnly(noteNames[sliders[1].value]);

			sprites[0].setColor(Color(255, 255, 255, 0));


					break;


		}
		case 12: // M - Channel volume
			texts[1].setString(lang("effects", "M_range"));
			texts[0].setString(lang("effects", "M_description"));
			sliders[0].setMinMax(0, 99);
			sliders[0].name.setString(lang("global", "Volume"));
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 13:{ // N - Channel volume slide
					sliders[0].setMinMax(0, 255);
					string s;
					if (sliders[0].value < 127)
					{
						s = string(lang("effects", "Decrease"))+" ";
						s += sliders[0].value < 64 ? lang("effects", "Quickly") : lang("effects", "Slowly"); /* Feel free to change every instance of "quickly" with a synonym of your own choice if "quickly" does not feel like that it conveys the same emotion that "fastly" did. */
					}
					else if (sliders[0].value == 127)
					{
						s = lang("effects", "No change");
					}
					else
					{
						s = string(lang("effects", "Increase"))+" ";
						s += sliders[0].value < 192 ? lang("effects", "Slowly") : lang("effects", "Quickly");
					}
					texts[0].setString(string(lang("effects", "N_description"))+" :\n"+s);

					texts[1].setString(lang("effects", "N_range"));
					sliders[0].name.setString("");
					sprites[0].setColor(Color(255, 255, 255, 255));
		}
			break;
		case 14:{ // P Panning slide
					sliders[0].setMinMax(0, 255);
					string s;
					if (sliders[0].value < 127)
					{
						s = string(lang("effects", "To left"))+", ";
						s += sliders[0].value < 64 ? lang("effects", "Fast") : lang("effects", "Slow");
					}
					else if (sliders[0].value == 127)
					{
						s = string(lang("effects", "No change"));
					}
					else
					{
						s = string(lang("effects", "To right"))+", ";
						s += sliders[0].value < 192 ? lang("effects", "Slow") : lang("effects", "Fast");
					}
					texts[0].setString(string(lang("effects", "P_description"))+" :\n"+s);
					texts[1].setString(lang("effects", "P_range"));
					sliders[0].name.setString(lang("effects", "Speed/direction"));
					sprites[0].setColor(Color(255, 255, 255, 255));
		}
			break;
		case 15:{ // Q Note retrigger
					sliders[0].setMinMax(0, 8);
					texts[0].setString(lang("effects", "Q_description"));
					texts[1].setString("");
					sliders[0].name.setString(lang("effects", "Times per row"));
					sprites[0].setColor(Color(255, 255, 255, 255));
		}
			break;
		case 16: // R - Reverb send
			texts[0].setString(lang("effects", "R_description"));
			texts[1].setString(lang("effects", "R_range"));
			sliders[0].setMinMax(0, 99);
			sliders[0].name.setString(lang("effects", "Amount"));
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 17:{ // S - Global reverb params

					string s;
					if (sliders[0].value <= 40)
					{
						s = string(lang("effects", "Room reveberation"))+" : " + std::to_string(sliders[0].value);
					}
					else
					{
						s = string(lang("effects", "Room size"))+" : " + std::to_string(clamp(sliders[0].value - 40, 1, 40));
					}
					texts[0].setString(string(lang("effects", "S_description"))+" :\n"+s);

					texts[1].setString("");
					sliders[0].setMinMax(0, 80);
					sliders[0].name.setString(lang("global", "Value"));
					sprites[0].setColor(Color(255, 255, 255, 255));
					break;
		}
		case 18: // T - Set tempo
			texts[0].setString( lang("effects", "T_description"));
			texts[1].setString(lang("effects", "T_range"));
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString("Tempo");
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 19: // V - Global volume
			texts[0].setString( lang("effects", "V_description"));
			texts[1].setString(lang("effects", "V_range"));
			sliders[0].setMinMax(0, 99);
			sliders[0].name.setString("Volume");
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 20:{ // W - Global volume slide

					sliders[0].setMinMax(0, 255);
					string s;
					if (sliders[0].value < 127)
					{
						s = string(lang("effects", "Decrease"))+" ";
						s += sliders[0].value < 64 ? "quickly" : "slowly";
					}
					else if (sliders[0].value == 127)
					{
						s =lang("effects", "No change");
					}
					else
					{
						s = string(lang("effects", "Increase "))+" ";
						s += sliders[0].value < 192 ? "slowly" : "quickly";
					}
					texts[0].setString(string(lang("effects", "W_description"))+" :\n"+s);
					texts[1].setString(lang("effects", "W_range"));
					sliders[0].name.setString("");
					sprites[0].setColor(Color(255, 255, 255, 255));
					break; }
		case 21: // X - Channel panning
			texts[0].setString(lang("effects", "X_description"));
			texts[1].setString(lang("effects", "X_range"));
			sliders[0].setMinMax(0, 255);
			sliders[0].name.setString(lang("effects", "Pan"));
			sprites[0].setColor(Color(255, 255, 255, 255));
			break;
		case 22: // Y - Sync marker
			sliders[0].setMinMax(0, 255);
			texts[0].setString(lang("effects", "Y_description"));
			sliders[0].name.setString(lang("effects", "Marker ID"));
			texts[1].setString("");
			sprites[0].setColor(Color(0, 0, 0, 0));
			break;
	}
}
