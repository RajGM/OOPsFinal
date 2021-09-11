/*
  ==============================================================================

    DeckGUI.h
    Created: 11 Aug 2021 7:55:28pm
    Author:  rajgm

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public Button::Listener, public Slider::Listener, public FileDragAndDropTarget, public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,AudioFormatManager &formatMangerToUse,
        AudioThumbnailCache &cacheToUse);
    ~DeckGUI() override;
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button *) override;

    /*Implement slider listener*/
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

private:
    DJAudioPlayer* djAudioPlayerplayer;

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

public:
    void loadFile(URL fileURL);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
