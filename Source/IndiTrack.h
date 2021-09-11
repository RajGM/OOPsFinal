/*
  ==============================================================================

    IndiTrack.h
    Created: 30 Aug 2021 12:26:10pm
    Author:  rajgm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class IndiTrack {
public:
    IndiTrack(File _file);
    File file;
    URL url;
    String title;
    String length;
};