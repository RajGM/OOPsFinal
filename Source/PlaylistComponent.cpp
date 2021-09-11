/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 16 Aug 2021 12:54:21am
    Author:  rajgm

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,DeckGUI* _deckGUI2, DJAudioPlayer* _dataManagerplayer):deckGUI1(_deckGUI1),deckGUI2(_deckGUI2),  dataManagerplayer(_dataManagerplayer)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    tableComponent.getHeader().addColumn("ID", 1, 25);
    tableComponent.getHeader().addColumn("Track title",2,250);
    tableComponent.getHeader().addColumn("Time", 3, 45);
    tableComponent.getHeader().addColumn("Delete", 4, 45);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchInput);
    addAndMakeVisible(importTrackButton);
    addAndMakeVisible(loadDeck1Button);
    addAndMakeVisible(loadDeck2Button);

    searchInput.addListener(this);
    loadDeck1Button.addListener(this);
    loadDeck2Button.addListener(this);
    importTrackButton.addListener(this);

    searchInput.setTextToShowWhenEmpty("Enter Track Title to Search for", Colours::yellow);
    searchInput.onReturnKey = [this] { searchTrack(searchInput.getText()); };

}

PlaylistComponent::~PlaylistComponent()
{
    //saveLibrary when deconstructing
    saveTrackList();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int rowH= getHeight()/4;
    int colW = getWidth() / 2;
    tableComponent.setBounds(0,0, colW, 4*rowH);
    searchInput.setBounds(colW, 0,colW, rowH);
    importTrackButton.setBounds(colW, rowH, colW, rowH);
    loadDeck1Button.setBounds(colW, rowH * 2, colW, rowH);
    loadDeck2Button.setBounds(colW, rowH * 3, colW, rowH);
    
}

int PlaylistComponent::getNumRows() {
    return trackData.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int 	rowNumber,
    int 	width,
    int 	height,
    bool 	rowIsSelected
) {
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics& g,
    int 	rowNumber,
    int 	columnId,
    int 	width,
    int 	height,
    bool 	rowIsSelected
) {

    if (rowNumber < getNumRows()) {

        if (columnId == 1) {
            g.drawText(std::to_string(rowNumber), 2, 0, width - 4, height, Justification::centredLeft, true);
        }

        if (columnId == 2) {
            g.drawText(trackData[rowNumber].title, 2, 0, width - 4, height, Justification::centredLeft, true);
        }

        if (columnId == 3) {
            g.drawText(trackData[rowNumber].length, 2, 0, width - 4, height, Justification::centredLeft, true);
        }

        if (columnId == 4) {
            g.drawText("Delete", 2, 0, width - 4, height, Justification::centredLeft, true);
        }

    }

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    if(columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Delete" };
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button) {
    
    if (button == &loadDeck1Button) {
        loadToDeck(deckGUI1);
    }else if (button == &loadDeck2Button) {
        loadToDeck(deckGUI2);
    }else if (button == &importTrackButton) {
       loadTrackInList();
       tableComponent.updateContent();
    }
    else {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteTrack(id);
        tableComponent.updateContent();
        DBG(id);
    }
}

void PlaylistComponent::searchTrack(String track) {
    DBG("Searching library for: " << track);

    if (track != "" || track.length()>0 ) {
        int rowID = trackID(track);
        tableComponent.selectRow(rowID);
    }
    else {
        //deslect all row
        //alert not found
        tableComponent.deselectAllRows();
    }

}

int PlaylistComponent::trackID(String trackTitle) {
    for (int i = 0; i < trackData.size(); i++) {
        if (trackData[i].title == trackTitle) {
            return i;
        }
    }

    return -1;
}

void PlaylistComponent::loadTrackInList() {
    DBG("LOAD TRACK IN LIST CALLED");
    FileChooser chooser{ "Select a Track or multiple Tracks..." };
    if (chooser.browseForMultipleFilesToOpen()) {
        
        for (const File &file:chooser.getResults()) {

            String fileName{ file.getFileNameWithoutExtension() };
            if( isTrackPresent(fileName) ) {
                IndiTrack newTrack{ file };
                URL trackURL{ file };
                newTrack.length = getLength(trackURL);
                trackData.push_back(newTrack);
                DBG("New Track Title: " << newTrack.title);
                DBG("New Track Length: " << newTrack.length);
            }
            else {
                //File exists
            }

        }

    }

}

void PlaylistComponent::deleteTrack(int id) {
    trackData.erase(trackData.begin() + id);
}

bool PlaylistComponent::isTrackPresent(String fileName) {
    return "";
}

String PlaylistComponent::getLength(URL trackURL) {
    dataManagerplayer->loadURL(trackURL);
    return dataManagerplayer->getTrackTotalTime();
}

void PlaylistComponent::loadToDeck(DeckGUI* deckGUI) {

    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        //DBG("Adding: " << tracks[selectedRow].title << " to Player");
        deckGUI->loadFile(trackData[selectedRow].url);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }

}

void PlaylistComponent::saveTrackList() {

   std::ofstream myfile("library.txt");
    
    for (int i = 0; i < trackData.size(); i++) {
        myfile<<trackData[i].file.getFullPathName() << "," << trackData[i].length << "\n";
    }

    std::ofstream myLibrary("my-library.csv");

    // save library to file
    for (IndiTrack& t : trackData)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }


}
