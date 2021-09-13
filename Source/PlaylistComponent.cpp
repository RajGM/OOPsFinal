/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 16 Aug 2021 12:54:21am
    Author:  rajgm

  ==============================================================================
*/
//PlayListComponent.CPP handles the library persistence, saving and other stuff related to it.
#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,DeckGUI* _deckGUI2, DJAudioPlayer* _dataManagerplayer):deckGUI1(_deckGUI1),deckGUI2(_deckGUI2),  dataManagerplayer(_dataManagerplayer)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //making components visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchInput);
    addAndMakeVisible(importTrackButton);
    addAndMakeVisible(loadDeck1Button);
    addAndMakeVisible(loadDeck2Button);

    //adding listener to the 
    searchInput.addListener(this);
    loadDeck1Button.addListener(this);
    loadDeck2Button.addListener(this);
    importTrackButton.addListener(this);

    //default search text and behaviour to exectute after hitting enter
    searchInput.setTextToShowWhenEmpty("Enter Track Title to Search for", Colours::white);
    searchInput.onReturnKey = [this] { searchTrack(searchInput.getText()); };

    //table headers
    tableComponent.getHeader().addColumn("ID", 1, 25);
    tableComponent.getHeader().addColumn("Track title", 2, 250);
    tableComponent.getHeader().addColumn("Time", 3, 45);
    tableComponent.getHeader().addColumn("Delete", 4, 45);
    tableComponent.setModel(this);

    //calling load once application starts
    loadSavedTrackList();

}

PlaylistComponent::~PlaylistComponent()
{
    //saveLibrary when deconstructing
    saveTrackList();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    searchInput.setColour(TextEditor::backgroundColourId, Colours::orange);
    tableComponent.setColour(TableListBox::backgroundColourId, Colours::cornsilk);
    loadDeck1Button.setColour(TextButton::buttonColourId, Colours::forestgreen);
    loadDeck2Button.setColour(TextButton::buttonColourId, Colours::limegreen);
    importTrackButton.setColour(TextButton::buttonColourId, Colours::palevioletred);

}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int rowH= getHeight()/4;
    int colW = getWidth() / 2;
    int sectionWidth = 365;
    tableComponent.setBounds(0,0, sectionWidth, 4*rowH);
    searchInput.setBounds(getWidth() - sectionWidth, 0, sectionWidth, rowH);
    importTrackButton.setBounds(getWidth() - sectionWidth, rowH, sectionWidth, rowH);
    loadDeck1Button.setBounds(getWidth()-sectionWidth, rowH * 2, sectionWidth, rowH);
    loadDeck2Button.setBounds(getWidth()-sectionWidth, rowH * 3, sectionWidth, rowH);
    
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

//button clicked behaviour
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

//returns trackID for the track
int PlaylistComponent::trackID(String trackTitle) {

    for (int i = 0; i < trackData.size(); i++) {
        DBG("TrackTitle:" + trackData[i].title);
        if (trackData[i].title == trackTitle) {
            return i;
        }
    }

    return -1;
}

//searchTracks and highlights it 
void PlaylistComponent::searchTrack(String track) {

    if (track != "" || track.length()>0 ) {
        int rowID = trackID(track);
        tableComponent.selectRow(rowID);
    }
    else {
        tableComponent.deselectAllRows();
    }

}

//deleteTrack
void PlaylistComponent::deleteTrack(int id) {
    trackData.erase(trackData.begin() + id);
}

//get length of the track
String PlaylistComponent::getLength(URL trackURL) {
    dataManagerplayer->loadURL(trackURL);
    return dataManagerplayer->getTrackTotalTime();
}

//load track to the audioPlayer
void PlaylistComponent::loadToDeck(DeckGUI* deckGUI) {

    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        deckGUI->loadFile(trackData[selectedRow].url);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Error:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }

}

//load tracks into the library
void PlaylistComponent::loadTrackInList() {

    FileChooser chooser{ "Select a Track or multiple Tracks..." };
    if (chooser.browseForMultipleFilesToOpen()) {

        for (const File& file : chooser.getResults()) {

            String fileName = file.getFileNameWithoutExtension();

            if ( true ) {

                IndiTrack newTrack{ file };
                URL trackURL{ file };
                newTrack.length = getLength(trackURL);
                trackData.push_back(newTrack);

            }
            /*
            else {
                //File exists do nothing
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "File Present Already:",
                    file.getFileNameWithoutExtension() + " is in the Library Already",
                    "OK",
                    nullptr
                );
            }*/

        }

    }

}

//save information about tracks in library to CSV files
void PlaylistComponent::saveTrackList() {

   std::ofstream myfile("library.csv");
    
   for (IndiTrack& t : trackData)
   {
       myfile << t.file.getFullPathName() << "," << t.length << "\n";
   }

    myfile.close();

}

//load tracks from the CSV to the library
void PlaylistComponent::loadSavedTrackList() {

    std::ifstream myfile("library.csv");
    std::string filePath;
    std::string length;

    if (myfile.is_open())
    {
        while (getline(myfile, filePath, ',')) {
            juce::File file{ filePath };
            IndiTrack newTrack{ file };

            getline(myfile, length);
            newTrack.length = length;
            trackData.push_back(newTrack);
        }
    }
    myfile.close();

}
