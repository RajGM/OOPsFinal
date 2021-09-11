/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 11 Aug 2021 4:02:52pm
    Author:  rajgm

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    //get relative position of playhead
    double getPosition();
    String  getTrackTotalTime();
    String convertSecToMin(double sec);

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource,false,2 };
};