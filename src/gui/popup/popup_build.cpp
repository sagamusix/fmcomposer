#include "popup.hpp"
#include "../contextmenu/contextmenu.hpp"
#include "../../streamed/streamedExport.h"
#include "../../lang.h"

void setEffectList(List* list)
{
	list->add("A - "+string(lang("popup", "Arpeggio")));
	list->add("B - "+string(lang("popup", "Pattern jump")));
	list->add("C - "+string(lang("popup", "Row jump")));
	list->add("D - "+string(lang("popup", "Delay")));
	list->add("E - "+string(lang("popup", "Portamento up")));
	list->add("F - "+string(lang("popup", "Portamento down")));
	list->add("G - "+string(lang("popup", "Glissando")));
	list->add("H - "+string(lang("popup", "Vibrato")));
	list->add("I - "+string(lang("popup", "Pitch bend")));
	list->add("J - "+string(lang("popup", "Tremolo")));
	list->add("K - "+string(lang("popup", "Instrument control")));
	list->add("L - "+string(lang("popup", "Channel volume")));
	list->add("M - "+string(lang("popup", "Channel volume")));
	list->add("N - "+string(lang("popup", "Channel volume slide")));
	list->add("P - "+string(lang("popup", "Panning slide")));
	list->add("Q - "+string(lang("popup", "Retrigger note")));
	list->add("R - "+string(lang("popup", "Channel reverb")));
	list->add("S - "+string(lang("popup", "Global reverb params")));
	list->add("T - "+string(lang("popup", "Tempo")));
	list->add("V - "+string(lang("popup", "Global volume")));
	list->add("W - "+string(lang("popup", "Global volume slide")));
	list->add("X - "+string(lang("popup", "Channel panning")));
	list->add("Y - "+string(lang("popup", "Sync marker")));
}

void Popup::show(int _type, int param)
{
	// if popup opened through another popup, call close() before to save the popup state
	if (visible)
	{
		close();
	}
	contextMenu = NULL;
	texts.clear();
	type = _type;
	textures.clear();
	sliders.clear();
	lists.clear();
	sprites.clear();
	buttons.clear();
	shapes.clear();
	checkboxes.clear();
	delay = -1;
	visible = 1;
	switch (type)
	{
		case POPUP_SAVED:{
							 setSize(250, 100);
							 title.setString(lang("popup", "Info"));

							 Sprite s;
							 s.setTexture(*tileset);
							 s.setTextureRect(IntRect(0, 0, 24, 22));
							 sprites.push_back(s);
							 sprites[0].setPosition(80, 34);


							 delay = 1;
							 texts.push_back(Text(lang("popup", "Saved"), font, charSize));
							 texts[0].setColor(colors[BLOCKTEXT]);
							 texts[0].setPosition(110, 40);
							 break;
		}
		case POPUP_WORKING:
			setSize(500, 200);
			title.setString(lang("popup", "Info"));
			texts.push_back(Text(lang("popup", "Rendering"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 20);
			sliders.push_back(DataSlider(80, 70, 99, 0, "%", 99));
			buttons.push_back(Button(50, h - 50, lang("popup", "Abort"), -1, 8));
			break;
		case POPUP_ABOUT:{
			setSize(740, 400);
			title.setString(lang("popup", "About"));

			Sprite s;
			s.setTexture(*tileset);
			s.setTextureRect({ 260, 0, 256, 256 });
			sprites.push_back(s);
			sprites[0].setPosition(10, 10);

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(20, h - 50,lang("popup",  "Go to the website"), -1, 8));
			buttons.push_back(Button(180, h - 50, lang("popup",  "Check for updates"), -1, 8));
			texts.push_back(Text(L"FM Composer, © 2017-2018 Stéphane Damo\n\n--------- Credits ---------\n\nTesting, help and advices :\n		Klairzaki Fil-Xter, Masami Komuro, Isaac Zuniga\n\nLibraries authors :\n		Laurent Gomila & SFML contributors (SFML lib)\n		Guillaume Vareille (tinyfiledialogs lib)\n		Brodie Thiesfield (SimpleIni lib)\n		Ross Bencina/Phil Burk/Roger B. Dannenberg (PortMidi/Audio lib)\n		Yann Collet (LZ4)\n		The LAME MP3 encoder team\n		The Google team (Material Icons)\n		Josh Coalson & Xiph.org foundation (FLAC encoder)", font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(286, 20);

			texts.push_back(Text(string(lang("popup", "Version"))+" " + VERSION + " (" + VERSION_DATE + ")", font, charSize));
			texts[1].setColor(colors[BLOCKTEXT]);
			texts[1].setPosition(100, 276);

		}
			break;

		case POPUP_REPLACE_INSTRUMENT:
			setSize(300, 400);
			title.setString(lang("popup",  "Change instrument"));
			lists.push_back(List(10, 10, 17, 230));

			lists[0].text = instrList->text;
			lists[0].updateSize();

			buttons.push_back(Button(w - 90, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));
			break;
		case POPUP_TRANSPOSE:
			setSize(500, 250);
			title.setString(lang("popup",  "Change note"));

			checkboxes.push_back(Checkbox(20, 20, lang("popup", "Set note")));
			checkboxes.push_back(Checkbox(20, 80, lang("global", "Transpose")));

			checkboxes[0].checked = 1;

			sliders.push_back(DataSlider(160, 20, 128, 0, "", 0, 200, 2));
			sliders.push_back(DataSlider(160, 80, 24, -24, lang("popup", "Semitones"), 0));


			texts.push_back(Text("", font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(160, 110);

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));

			updateIntervalDescription();

			break;
		case POPUP_SETNOTE:
			setSize(300, 150);
			title.setString(lang("popup","Set note"));
			sliders.push_back(DataSlider(20, 20, 128, 0, "", 0, 200, 2));
			sliders[0].setValue(60);

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));

			break;
		case POPUP_FADE:
			setSize(500, 350);
			title.setString(lang("global",  "Volume"));

			checkboxes.push_back(Checkbox(20, 80, lang("popup", "Scale volume")));



			checkboxes.push_back(Checkbox(20, 140, lang("popup", "Fade in/out")));

			texts.push_back(Text(lang("popup", "From\n\nTo"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(160, 140);

			sliders.push_back(DataSlider(210, 140, 99, 0, "", 99));
			sliders.push_back(DataSlider(210, 180, 99, 0, "", 99));

			sliders.push_back(DataSlider(160, 80, 200, 0, "%", 100));

			checkboxes.push_back(Checkbox(20, 20,lang("popup", "Set volume")));

			sliders.push_back(DataSlider(160, 20, 99, 0, "", 99));

			checkboxes[2].checked = 1;

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));
			break;
		case POPUP_EFFECTS:{

							   Sprite s;
							   s.setTexture(*tileset);
							   s.setTextureRect({ 270, 257, 257, 18 });
							   sprites.push_back(s);
							   sprites[0].setPosition(249, 150);

							   setSize(600, 450);
							   title.setString(lang("popup","Add effect"));

							   texts.push_back(Text(lang("popup","Effect type"), font, charSize));
							   texts[0].setColor(colors[BLOCKTEXT]);
							   texts[0].setPosition(10, 10);

							   lists.push_back(List(20, 20, 18, 200));
							   setEffectList(&lists[0]);

							   buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
							   buttons.push_back(Button(20, h - 50, lang("global",  "Cancel"), -1, 8));

							   sliders.push_back(DataSlider(250, 168, 255, 0, lang("popup","Value"), 0, 255));
							   sliders.push_back(DataSlider(250, 188, 255, 0, "", 0, 255));
							   sliders[1].setVisible(false);

							   texts.push_back(Text("", font, charSize));
							   texts[0].setColor(colors[BLOCKTEXT]);
							   texts[0].setPosition(250, 20);

							   texts.push_back(Text("", font, charSize));
							   texts[1].setColor(colors[BLOCKTEXT]);
							   texts[1].setPosition(250, 128);
		}break;

		case POPUP_SEARCH:
			setSize(800, 530);
			title.setString(lang("popup", "Search/Replace"));

			texts.push_back(Text( lang("popup", "Search for"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(20, 20);

			checkboxes.push_back(Checkbox(20, 100, lang("global", "Note")));
			sliders.push_back(DataSlider(150, 100, 128, -1, "", 64, 200, 2));

			checkboxes.push_back(Checkbox(20, 140, lang("global", "Volume")));
			sliders.push_back(DataSlider(150, 140, 99, -1, "", 0, 200));

			checkboxes.push_back(Checkbox(20, 180, lang("global", "Instrument")));
			lists.push_back(List(150, 180, 6, 200));
			lists[0].add( lang("global", "Any"));
			for (int i = 0; i < instrList->text.size(); i++)
			{
				lists[0].add(instrList->text[i].getString().toAnsiString());
			}

			lists[0].updateSize();

			checkboxes.push_back(Checkbox(20, 300, lang("popup", "Effect")));
			lists.push_back(List(150, 300, 5, 200));
			lists[1].add("Any");
			setEffectList(&lists[1]);

			checkboxes.push_back(Checkbox(20, 410, lang("popup", "Effect value")));

			checkboxes.push_back(Checkbox(220, h - 50, lang("popup", "The whole song")));
			checkboxes[5].checked = 1;
			checkboxes.push_back(Checkbox(400, h - 50, lang("popup", "This pattern")));
			checkboxes.push_back(Checkbox(550, h - 50, lang("popup", "Selection")));


			sliders.push_back(DataSlider(150, 410, 255, -1, "", 0, 200));

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));

			shapes.push_back(RectangleShape(Vector2f(1, 410)));
			shapes[0].setPosition(394, 56);
			shapes[0].setFillColor(colors[BLOCKBG]);

			// replace
			checkboxes.push_back(Checkbox(382, 20, lang("global",  "Replace with")));

			checkboxes.push_back(Checkbox(420, 100,lang("global",  "Note")));
			sliders.push_back(DataSlider(550, 100, 128, -1, "", 64, 200, 2));

			checkboxes.push_back(Checkbox(420, 140, lang("global",  "Volume")));
			sliders.push_back(DataSlider(550, 140, 99, -1, "", 0, 200));

			checkboxes.push_back(Checkbox(420, 180, lang("global",  "Instrument")));
			lists.push_back(List(550, 180, 6, 200));
			lists[2].add(lang("global",  "None"));
			for (int i = 0; i < instrList->text.size(); i++)
			{
				lists[2].add(instrList->text[i].getString().toAnsiString());
			}
			lists[2].updateSize();

			checkboxes.push_back(Checkbox(420, 300, lang("popup", "Effect")));
			lists.push_back(List(550, 300, 5, 200));
			lists[3].add(lang("global",  "None"));
			setEffectList(&lists[3]);


			checkboxes.push_back(Checkbox(420, 410, lang("popup",  "Effect value")));
			sliders.push_back(DataSlider(550, 410, 255, -1, "", 0, 200));

			texts.push_back(Text(lang("global",  "Search in :"), font, charSize));
			texts[1].setColor(colors[BLOCKTEXT]);
			texts[1].setPosition(120, h - 50);

			for (int i = 0; i < sliders.size(); i++)
			{
				if (i < 3 && sliders[i].value == -1)
				{
					sliders[i].setDisplayedValueOnly(lang("global",  "Any"));
				}
				else if (i >= 3 && sliders[i].value == -1)
				{
					sliders[i].setDisplayedValueOnly(lang("global",  "None"));
				}
			}

			checkboxes.push_back(Checkbox(420, 60, lang("popup",  "Transposed note")));
			sliders.push_back(DataSlider(600, 60, 24, -24, lang("popup",  "Semitones"), 64, 150));

			break;

		case POPUP_REPLACERESULT:
			setSize(300, 130);
			title.setString(lang("popup",  "Search/Replace"));
			texts.push_back(Text(std::to_string(param) + " occurence(s) replaced", font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(20, 20);
			delay = 60;
			break;

		case POPUP_SAVEFAILED:
			setSize(460, 150);
			title.setString(lang("popup",  "Save action failed"));
			texts.push_back(Text("Can't write data. Check if this file is writable.", font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 50, h - 50,  lang("global",  "OK"), -1, 8));
			break;
		case POPUP_OPENFAILED:
			setSize(460, 150);
			title.setString(lang("popup",  "Open action failed"));
			texts.push_back(Text(lang("popup",  "Missing file"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 50, h - 50,  lang("global",  "OK"), -1, 8));
			break;

		case POPUP_QUITCONFIRM:
			setSize(450, 150);
			title.setString(lang("popup",  "Confirmation"));
			texts.push_back(Text(lang("popup",  "Unsaved"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 160, h - 50, lang("popup",  "Quit without saving"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));
			buttons.push_back(Button(130, h - 50, lang("popup",  "Save and quit"), -1, 8));

			break;

		case POPUP_OPENCONFIRM:
			setSize(450, 150);
			title.setString(lang("popup",  "Confirmation"));
			texts.push_back(Text(lang("popup",  "Unsaved"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 160, h - 50, lang("popup",  "Discard changes"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));
			buttons.push_back(Button(130, h - 50, lang("global",  "Save"), -1, 8));
			break;

		case POPUP_DELETEINSTRUMENT:
			setSize(500, 150);
			title.setString(lang("popup",  "Confirmation"));
			texts.push_back(Text(lang("popup",  "Instrument delete"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 80, h - 50, lang("global",  "Yes"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "No"), -1, 8));
			break;

		case POPUP_NEWFILE:
			setSize(450, 150);
			title.setString(lang("popup",  "Confirmation"));
			texts.push_back(Text(lang("popup",  "Unsaved"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 160, h - 50, lang("popup",  "Discard changes"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));
			buttons.push_back(Button(130, h - 50, lang("global",  "Save"), -1, 8));
			break;

		case POPUP_MIDIEXPORT:{
			int oldSize = midiExportAssoc.size();
			midiExportAssoc.resize(fm->instrumentCount, 0);

			midiExportAssocChannels.resize(fm->instrumentCount, 0);
			for (int i = oldSize; i < midiExportAssocChannels.size(); i++)
			{
				midiExportAssocChannels[i] = i % 16;
				if (i == 9)
					midiExportAssocChannels[i]++;
			}
			setSize(850, 510);
			title.setString(lang("popup", "MIDI export"));

			texts.push_back(Text(lang("popup", "MIDI stuff"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);

			texts.push_back(Text(lang("popup", "Song instrument"), font, charSize));
			texts[1].setColor(colors[TITLE]);
			texts[1].setPosition(10, 45);

			texts.push_back(Text(lang("popup", "MIDI channel"), font, charSize));
			texts[2].setColor(colors[TITLE]);
			texts[2].setPosition(300, 45);

			texts.push_back(Text(lang("popup", "MIDI instrument"), font, charSize));
			texts[3].setColor(colors[TITLE]);
			texts[3].setPosition(600, 45);



			lists.push_back(List(10, 70, 20, 200));
			lists[0].text = instrList->text;
			lists[0].updateSize();

			lists.push_back(List(600, 70, 20, 200));
			setMelodicList();

			lists.push_back(List(300, 70, 20, 200));
			lists[2].add("Ch1 Melodic");
			lists[2].add("Ch2 Melodic");
			lists[2].add("Ch3 Melodic");
			lists[2].add("Ch4 Melodic");
			lists[2].add("Ch5 Melodic");
			lists[2].add("Ch6 Melodic");
			lists[2].add("Ch7 Melodic");
			lists[2].add("Ch8 Melodic");
			lists[2].add("Ch9 Melodic");
			lists[2].add("Ch10 Percussion");
			lists[2].add("Ch11 Melodic");
			lists[2].add("Ch12 Melodic");
			lists[2].add("Ch13 Melodic");
			lists[2].add("Ch14 Melodic");
			lists[2].add("Ch15 Melodic");
			lists[2].add("Ch16 Melodic");


			buttons.push_back(Button(w - 90, h - 50, lang("popup",  "Export"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));

			break;
		}
		case POPUP_STREAMEDEXPORT:
			setSize(620, 380);
			title.setString(lang("popup", "Streamed audio export"));

			texts.push_back(Text(lang("popup", "From"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(20, 20);

			texts.push_back(Text(lang("popup", "To"), font, charSize));
			texts[1].setColor(colors[BLOCKTEXT]);
			texts[1].setPosition(240, 20);

			checkboxes.push_back(Checkbox(20, 100, lang("popup", "Wave")));
			checkboxes.push_back(Checkbox(20, 242, lang("popup", "MP3")));

			checkboxes.push_back(Checkbox(170, 242, lang("popup", "VBR")));
			checkboxes.push_back(Checkbox(170, 282, lang("popup", "CBR")));

			checkboxes[0].checked = 1;

			sliders.push_back(DataSlider(250, 264, 15, 0, "", 0, 200));

			shapes.push_back(RectangleShape(Vector2f(580, 1)));
			shapes[0].setPosition(20, 80);
			shapes[0].setFillColor(colors[BLOCKBG]);

			sliders.push_back(DataSlider(60, 20, fm->patternCount, 0, lang("popup", "Pattern"), 0, 150));
			sliders.push_back(DataSlider(270, 20, fm->patternCount, 0, lang("popup", "Pattern"), fm->patternCount, 150));


			sliders.push_back(DataSlider(60, 50, 10, 0, lang("popup", "Loop"), 0, 80));
			texts.push_back(Text(lang("popup", "Times"), font, charSize));
			texts[2].setColor(colors[BLOCKTEXT]);
			texts[2].setPosition(150, 50);

			checkboxes.push_back(Checkbox(20, 168, lang("popup", "FLAC")));
			sliders.push_back(DataSlider(310, 170, 8, 0, lang("popup", "Compression level"), 0, 200));

			sliders.push_back(DataSlider(170, 102, 4, 0, lang("popup", "Bit depth"), 1,130));
			sliders.push_back(DataSlider(170, 170, 4, 0, lang("popup","Bit depth"), 1,130));

			buttons.push_back(Button(w - 90, h - 50, lang("popup",  "Export"), -1, 8));
			buttons.push_back(Button(50, h - 50, lang("global",  "Cancel"), -1, 8));

			checkboxes.push_back(Checkbox(445, 20, lang("popup", "Multi-track export")));

			break;
		case POPUP_TEMPERAMENT:
			setSize(580, 400);
			title.setString(lang("global", "Edit temperament"));
			texts.push_back(Text(lang("global","Tuning in cents for each note"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(20, 40);
			sliders.push_back(DataSlider(20, 80, 100, -100, "C", 0));
			sliders.push_back(DataSlider(20, 100, 100, -100, "C#", 0));
			sliders.push_back(DataSlider(20, 120, 100, -100, "D", 0));
			sliders.push_back(DataSlider(20, 140, 100, -100, "D# / Eb", 0));
			sliders.push_back(DataSlider(20, 160, 100, -100, "E", 0));
			sliders.push_back(DataSlider(20, 180, 100, -100, "F", 0));
			sliders.push_back(DataSlider(20, 200, 100, -100, "F#", 0));
			sliders.push_back(DataSlider(20, 220, 100, -100, "G", 0));
			sliders.push_back(DataSlider(20, 240, 100, -100, "G# / Ab", 0));
			sliders.push_back(DataSlider(20, 260, 100, -100, "A", 0));
			sliders.push_back(DataSlider(20, 280, 100, -100, "A# / Bb", 0));
			sliders.push_back(DataSlider(20, 300, 100, -100, "B", 0));

			

			buttons.push_back(Button(w - 65, h - 50, lang("global",  "Close"), -1, 8));

			buttons.push_back(Button(250, 88, lang("global",  "Reset"), -1, 8));

			texts.push_back(Text(lang("popup", "Presets"), font, charSize));
			texts[1].setColor(colors[BLOCKTEXT]);
			texts[1].setPosition(320, 40);
			sliders.push_back(DataSlider(320, 70, 2, 0, "", 0));
			sliders.push_back(DataSlider(320, 100, 11, 0, lang("popup","Base note"), 0));

			break;
		case POPUP_INSERTROWS:
			setSize(240, 300);
			title.setString(lang("popup",  "Insert rows"));

			sliders.push_back(DataSlider(20, 40, 128, 1, lang("popup",  "Number of rows"), config->rowHighlight.value));

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));
			break;

		case POPUP_REMOVEROWS:
			setSize(240, 300);
			title.setString(lang("popup",  "Remove rows"));

			sliders.push_back(DataSlider(20, 40, 128, 1, lang("popup",  "Number of rows"), config->rowHighlight.value));

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));
			break;

		case POPUP_FILECORRUPTED:
			setSize(460, 150);
			title.setString(":(");
			texts.push_back(Text(lang("popup",  "Corrupted message"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(10, 10);
			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			break;
		case POPUP_FIRSTSTART:
		{
			setSize(800, 300);
			title.setString(lang("popup",  "First start"));

			Sprite s;
			s.setTexture(*tileset);
			s.setTextureRect({ 260, 0, 256, 256 });
			sprites.push_back(s);
			sprites[0].setPosition(10, 10);

			buttons.push_back(Button(w - 80, h - 50, lang("global",  "Close"), -1, 8));
			buttons.push_back(Button(490, 101, lang("popup", "Online tutorial"), -1, 8));
			texts.push_back(Text(lang("popup",  "Welcome message"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(276, 20);


		}
			break;
		case POPUP_WRONGVERSION:
			setSize(600, 200);
			title.setString(lang("popup",  "Newer file version"));

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(28, 150, lang("popup",  "Go to the download page"), -1, 8));
			texts.push_back(Text(lang("popup",  "Wrong version"), font, charSize));
			texts[0].setColor(colors[BLOCKTEXT]);
			texts[0].setPosition(20, 20);
			break;
		case POPUP_MULTITRACKEXPORT:
			setSize(600, 450);
			title.setString(lang("popup",  "Multi-track window"));
			texts.push_back(Text(lang("popup", "Tracks to export"), font, charSize));
			texts[0].setColor(colors[TITLE]);
			texts[0].setPosition(10, 25);

			texts.push_back(Text(lang("popup", "Channels to mix into"), font, charSize));
			texts[1].setColor(colors[TITLE]);
			texts[1].setPosition(300, 25);

			texts.push_back(Text(lang("popup", "hold CTRL/SHIFT to select multiple"), font, charSize));
			texts[2].setColor(colors[BLOCKTEXT]);
			texts[2].setPosition(300, 45);

			lists.push_back(List(10, 50, 12, 200,4,false));
			lists[0].add("#1");

			lists.push_back(List(300, 70, 20, 200,4,true));

			

			for (unsigned i = 0; i < FM_ch; i++)
			{
				lists[1].add(string(lang("global", "Channel"))+" "+std::to_string(i+1));
			}

			buttons.push_back(Button(20,290, lang("global", "Add"), -1, 8));
			buttons.push_back(Button(70,290, lang("global", "Remove"), -1, 8));

			buttons.push_back(Button(w - 50, h - 50, lang("global",  "OK"), -1, 8));
			buttons.push_back(Button(30, h - 50, lang("global",  "Cancel"), -1, 8));


			if (streamedExport.multitrackAssoc.size() < lists[0].text.size())
			{
				if (streamedExport.multitrackAssoc.size() ==0)
				{
					lists[0].add("#2");
					lists[0].add("#3");
					lists[0].add("#4");
					streamedExport.multitrackAssoc.resize(lists[0].text.size());
					for (unsigned i = 0; i < streamedExport.multitrackAssoc.size(); i++)
					{
						streamedExport.multitrackAssoc[i].resize(1);
						streamedExport.multitrackAssoc[i][0]=0;
					}
				}
				else
				{
					streamedExport.multitrackAssoc.resize(lists[0].text.size());
				}
			}
			if (lists[0].text.size() < streamedExport.multitrackAssoc.size())
			{
				for (unsigned i = lists[0].text.size(); i < streamedExport.multitrackAssoc.size(); i++)
				{
					lists[0].add("#"+std::to_string(lists[0].text.size()+1));
				}
			}


			break;

	}
	view.reset(FloatRect(0.f, 0.f, windowWidth, windowHeight));
	view.setViewport(FloatRect(0, 0, 1, 1));
	view.setCenter((float)windowWidth / 2 - (int)windowWidth / 2 + (float)w / 2, (float)windowHeight / 2 - (int)windowHeight / 2 + (float)h / 2);
	shadow.setSize(Vector2f(w, h + 32));

	// restore widget states if this popup has been opened before
	if (type<savedState.size() && savedState[type].size()>0)
	{
		int item = 0;
		for (int i = 0; i < lists.size(); i++)
		{
			lists[i].select(savedState[type][item]);
			item++;
		}
		for (int i = 0; i < sliders.size(); i++)
		{
			sliders[i].setValue(savedState[type][item]);
			item++;
			if (type == POPUP_SEARCH)
			{
				if (i < 3 && sliders[i].value == -1)
				{
					sliders[i].setDisplayedValueOnly(lang("global", "Any"));
				}
				else if (i >= 3 && sliders[i].value == -1)
				{
					sliders[i].setDisplayedValueOnly(lang("global", "None"));
				}
			}
		}
		for (int i = 0; i < checkboxes.size(); i++)
		{
			checkboxes[i].checked = savedState[type][item];
			item++;
		}
	}

	switch (type)
	{
		case POPUP_EFFECTS:
			updateEffectDescription();
			break;
		case POPUP_TRANSPOSE:
			updateIntervalDescription();
			break;
		case POPUP_STREAMEDEXPORT:
			updateExportSliders();
			updateBitDepthDescription();
			break;
	}


}

void Popup::updateExportSliders()
{
	if (checkboxes[3].checked)
	{
		sliders[0].name.setString(string(lang("popup", "Bitrate"))+" (Kbps)");
		sliders[0].setMinMax(0, 15);
		sliders[0].setDisplayedValueOnly(std::to_string(mp3_bitrates[sliders[0].value]));
	}
	else
	{
		sliders[0].name.setString(lang("popup", "MP3_Quality"));
		sliders[0].setMinMax(0, 9);
	}
}

void Popup::updateWindow()
{
	view.reset(FloatRect(0.f, 0.f, windowWidth, windowHeight));
	view.setViewport(FloatRect(0, 0, 1, 1));
	view.setCenter((float)windowWidth / 2 - (int)windowWidth / 2 + (float)w / 2, (float)windowHeight / 2 - (int)windowHeight / 2 + (float)h / 2);
}
