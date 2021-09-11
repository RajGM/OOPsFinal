/*
  ==============================================================================

    DeckGUI.cpp
    Created: 11 Aug 2021 7:55:28pm
    Author:  rajgm

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatMangerToUse,
    AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatMangerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 3.0);
    posSlider.setRange(0.0, 1.0);

    volSlider.setValue(1);
    speedSlider.setValue(1);
    posSlider.setValue(1);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 6;
    playButton.setBounds(0, 0, getWidth() / 2, rowH);
    stopButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    volSlider.setBounds(0, rowH, getWidth() / 2, rowH);
    speedSlider.setBounds(getWidth() / 2, rowH, getWidth() / 2, rowH);
    posSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    waveformDisplay.setBounds(0,rowH*3,getWidth(),2*rowH);
    loadButton.setBounds(0, rowH * 5, getWidth(), rowH);

    startTimer(10);

}

void DeckGUI::buttonClicked(Button* button) {
    DBG("Something was clicked");

    if (button == &loadButton) {
        DBG("Load Button was clicked");
        FileChooser chooser{ "Select a file ..." };
        if (chooser.browseForFileToOpen()) {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });
        }
    }

    if (button == &stopButton) {
        DBG("Stop Button was clicked");
        player->stop();
    }

    if (button == &playButton) {
        DBG("Play Button was clicked");
        player->start();
    }

}

void DeckGUI::sliderValueChanged(Slider* slider) {
    DBG("Slider Value Changed out");

    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider) {
       player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files) {
    DBG("isInterestedInFileDrag");
    return true;
}
void DeckGUI::filesDropped(const StringArray& files, int x, int y) {
    DBG("filesDropped");
    if (files.size() == 1) {
        player->loadURL(URL{ File{files[0]} });
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPosition());
}

void DeckGUI::loadFile(URL fileURL) {
       
    player->loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);

}