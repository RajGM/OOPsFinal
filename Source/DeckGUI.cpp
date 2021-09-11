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

    volSlider.setRange(0.0, 2.0);
    speedSlider.setRange(0.0, 3.0);
    posSlider.setRange(0.0, 1.0);

    volSlider.setValue(1);
    speedSlider.setValue(1);
    posSlider.setValue(1);

    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow,true,100,20);

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

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    
    volLabel.setText("Volume", dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(Justification::centred);

    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(Justification::centred);

    posLabel.setText("Position", dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    posLabel.setJustificationType(Justification::centred);


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
    playButton.setBounds(0, 0, getWidth()/3, rowH);
    loadButton.setBounds(getWidth()/3, 0, getWidth()/3, rowH );
    stopButton.setBounds(2*getWidth()/3, 0, getWidth()/3, rowH);
    volSlider.setBounds(0, rowH+25, getWidth() / 3, 2*rowH);
    speedSlider.setBounds(getWidth() / 3, rowH+25, getWidth() / 3, 2 * rowH);
    posSlider.setBounds(2 * getWidth() / 3, rowH+25, getWidth()/3, 2 * rowH);
    waveformDisplay.setBounds(0,(rowH*4)-20,getWidth(),2*rowH);
   
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

