/*
  ==============================================================================

    IndiTrack.cpp
    Created: 30 Aug 2021 12:26:10pm
    Author:  rajgm

  ==============================================================================
*/

#include "IndiTrack.h"

IndiTrack::IndiTrack(File _file) : file(_file), title(_file.getFileNameWithoutExtension()), url(URL{_file}) {

}