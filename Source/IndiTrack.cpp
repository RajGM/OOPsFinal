/*
  ==============================================================================

    IndiTrack.cpp
    Created: 30 Aug 2021 12:26:10pm
    Author:  rajgm

  ==============================================================================
*/
//CPP file for the IndiTrack Class
#include "IndiTrack.h"

IndiTrack::IndiTrack(File _file) : file(_file), title(_file.getFileNameWithoutExtension()), url(URL{_file}) {

}

