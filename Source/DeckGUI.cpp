/*
  ==============================================================================

    DeckGUI.cpp
    Created: 11 Aug 2021 7:55:28pm
    Author:  rajgm

  ==============================================================================
*/
//This CPP file is for DECKGUI

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatMangerToUse,
    AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatMangerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //making all buttons visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
   

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);

    addAndMakeVisible(trackTitleLabel);
    addAndMakeVisible(trackLengthLabel);

    addAndMakeVisible(forwardButton);
    addAndMakeVisible(backwardButton);

    addAndMakeVisible(progressSliderBar);

    //setting up listener on all of the buttons
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    forwardButton.addListener(this);
    backwardButton.addListener(this);

    //setting range of the sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 3.0);
    posSlider.setRange(0.0, 1.0);
    progressSliderBar.setRange(0.0, 1.0);

    //setting default values
    volSlider.setValue(1);
    speedSlider.setValue(1);
    posSlider.setValue(1);

    //Setting up Lables and attaching it to component
    volLabel.setText("Volume", dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(Justification::centred);

    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(Justification::centred);

    posLabel.setText("Position", dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    posLabel.setJustificationType(Justification::centred);

    //Making TrackTitle and Duration lable uneditable by the user
    trackTitleLabel.setText("Track Title",false);
    trackTitleLabel.setReadOnly(true);

    trackLengthLabel.setText("Time",false);
    trackLengthLabel.setReadOnly(true);

    progressSliderBar.setTextBoxStyle(Slider::NoTextBox,true,0,0);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    //paint stuff for the DeckGUI
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);

    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);

    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);

    volSlider.getLookAndFeel().setColour(Slider::thumbColourId, Colours::purple);
    volSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::yellow);

    speedSlider.getLookAndFeel().setColour(Slider::thumbColourId, Colours::purple);
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::yellow);

    posSlider.getLookAndFeel().setColour(Slider::thumbColourId, Colours::purple);
    posSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::yellow);

    progressSliderBar.setSliderStyle(Slider::LinearBar);

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 6;
    double buttonWidth = getWidth() / 5;
    backwardButton.setBounds(0,0, buttonWidth,rowH);
    playButton.setBounds(buttonWidth, 0, buttonWidth, rowH);
    loadButton.setBounds(2*buttonWidth, 0, buttonWidth, rowH );
    stopButton.setBounds(3*buttonWidth, 0, buttonWidth, rowH);
    forwardButton.setBounds(4*buttonWidth, 0, buttonWidth, rowH);
    volSlider.setBounds(0, rowH+25, getWidth() / 3, 2*rowH);
    speedSlider.setBounds(getWidth() / 3, rowH+25, getWidth() / 3, 2 * rowH);
    posSlider.setBounds(2 * getWidth() / 3, rowH+25, getWidth()/3, 2 * rowH);
    waveformDisplay.setBounds(0,(rowH*4)-20,getWidth(),2*rowH);
    trackTitleLabel.setBounds(0, (rowH * 4) - 20 ,100,20);
    trackLengthLabel.setBounds(getWidth()-50, (rowH * 4) - 20,50,20);
    progressSliderBar.setBounds(0, (rowH*6)-20, getWidth(),20);

    startTimer(10);

}

//to detect button clicked and call required functions
void DeckGUI::buttonClicked(Button* button) {
    
    if (button == &loadButton) {
        FileChooser chooser{ "Select a file ..." };
        if (chooser.browseForFileToOpen()) {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });
        }
    }

    if (button == &stopButton) {
        player->stop();
    }

    if (button == &playButton) {
        player->start();
    }

    if (button == &forwardButton) {
        double currentPostion = player->getPosition();
        player->setPositionRelative(currentPostion+0.01);
    }

    if (button == &backwardButton) {
        double currentPostion = player->getPosition();
        player->setPositionRelative(currentPostion - 0.01);
    }

}

//to detect change in value of slider and call required functions
void DeckGUI::sliderValueChanged(Slider* slider) {
    DBG("Slider Value Changed out");

    if (slider == &volSlider) {
        player->setGain(slider->getValue());

        volSlider.setColour(Slider::rotarySliderOutlineColourId,Colours::chocolate);
        volSlider.setColour(Slider::rotarySliderFillColourId,Colours::blanchedalmond);
        volSlider.setColour(Slider::thumbColourId,Colours::black);

    }

    if (slider == &speedSlider) {
       player->setSpeed(slider->getValue());

       speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::chocolate);
       speedSlider.setColour(Slider::rotarySliderFillColourId, Colours::blanchedalmond);
       speedSlider.setColour(Slider::thumbColourId, Colours::black);

    }

    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());

        posSlider.setColour(Slider::rotarySliderOutlineColourId, Colours::chocolate);
        posSlider.setColour(Slider::rotarySliderFillColourId, Colours::blanchedalmond);
        posSlider.setColour(Slider::thumbColourId, Colours::black);
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files) {
    DBG("isInterestedInFileDrag");
    return true;
}

//detect if file is dropped onto the Load button
void DeckGUI::filesDropped(const StringArray& files, int x, int y) {
    DBG("filesDropped");
    if (files.size() == 1) {
        player->loadURL(URL{ File{files[0]} });
    }
}

//update waveform display 
void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPosition());
    if (player->getPosition() > 0 && player->getPosition() < 1) {
        progressSliderBar.setValue(player->getPosition());
    }

}

//load track into the deck, display waveform and metadata
void DeckGUI::loadFile(URL fileURL) {
       
    player->loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);
    trackTitleLabel.setText(fileURL.getFileName());
    trackLengthLabel.setText(player->getTrackTotalTime()+"s");
}

