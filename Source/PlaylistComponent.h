/*
  ==============================================================================

    PlaylistComponent.h
    Created: 16 Aug 2021 12:54:21am
    Author:  rajgm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "IndiTrack.h"

//==============================================================================//

class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener, public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,DeckGUI* _deckGUI2, DJAudioPlayer* _dataManagerplayer);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&,
        int 	rowNumber,
        int 	width,
        int 	height,
        bool 	rowIsSelected
    ) override;

    void paintCell(Graphics&,
        int 	rowNumber,
        int 	columnId,
        int 	width,
        int 	height,
        bool 	rowIsSelected
    ) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

private:
    TableListBox tableComponent;
    std::vector<IndiTrack> trackData;

    //test below
    TextButton importTrackButton{"IMPORT TRACK"};
    TextEditor searchInput;

    TextButton loadDeck1Button{ "LOAD TO DECK 1" };
    TextButton loadDeck2Button{ "LOAD TO DECK 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* dataManagerplayer;

    String getLength(URL trackURL);

    void searchTrack(String track);
    void deleteTrack(int id);
    int trackID(String trackTitle);
    void loadTrackInList();

    void saveTrackList();
    void loadSavedTrackList();
    
    void loadToDeck(DeckGUI *deckGUI);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
