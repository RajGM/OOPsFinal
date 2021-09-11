/*
==============================================================================

DJAudioPlayer.cpp
Created: 11 Aug 2021 4:02:52pm
Author:  rajgm

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager):formatManager(_formatManager) {

}

DJAudioPlayer::~DJAudioPlayer() {

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)  {
	resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()  {
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) {
		std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
}

void DJAudioPlayer::setGain(double gain) {
	if (gain < 0 || gain > 1) {
		std::cout << "DJ AudioPlayer Gain invalid values" << std::endl;
	}
	else {
		transportSource.setGain(gain);
	}	
}

void DJAudioPlayer::setSpeed(double ratio) {
	if (ratio < 0 || ratio > 100.0) {
		std::cout << "DJ AudioPlayer Speed invalid values" << std::endl;
	}
	else {
		resampleSource.setResamplingRatio(ratio);
	}

}

void DJAudioPlayer::setPosition(double posInSecs) {
	transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
	if (pos < 0 || pos>1.0) {
		std::cout << "DJ AudioPlayer Position invalid values" << std::endl;
	}
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
	}
}

void DJAudioPlayer::start() {
	transportSource.start();
}

void DJAudioPlayer::stop() {
	transportSource.stop();
}

double DJAudioPlayer::getPosition() {
	return transportSource.getCurrentPosition()/transportSource.getLengthInSeconds();
}


String DJAudioPlayer::getTrackTotalTime() {
	double time{ transportSource.getLengthInSeconds() };
	String final = convertSecToMin(time);
	return final;
	//return transportSource.getLengthInSeconds();
}

String DJAudioPlayer::convertSecToMin(double sec) {
	
	int secondsRounded{ int(std::round(sec)) };
	juce::String minutes{ std::to_string(secondsRounded / 60) };
	juce::String seconds{ std::to_string(secondsRounded % 60) };

	if (seconds.length() < 2)
	{
		seconds = seconds.paddedLeft('0', 2);
	}

	return String{ minutes + ":" + seconds };	
}