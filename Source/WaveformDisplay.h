/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class WaveformDisplay : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    /** Reads the URL file
    Inputs: URL file to be read */
    void loadURL(URL audioURL);

    /** Set the relative position of the playhead
    Inputs: position in the song */
    void setPositionRelative(double pos);

    static double setPos;

private:
    void paint(Graphics&) override;

    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    DJAudioPlayer* player;

    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    ImageButton tracker;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
