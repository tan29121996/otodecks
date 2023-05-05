/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource
{
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    /** Reads the URL file
    Inputs: URL file to be read */
    void loadURL(URL audioURL);

    /** Set the volume of the audio 
    Inputs: slider value of the volume slider */
    void setGain(double gain);

    /** Set the speed of the audio
    Inputs: slider value of the speed slider */
    void setSpeed(double ratio);

    /** Get the position in the song 
    Inputs: slider value of the position slider */
    void setPosition(double posInSecs);

    /** Set the relative position of the playhead 
    Inputs: position in the song */
    void setPositionRelative(double pos);

    /** Get the relative position of the playhead */
    double getPositionRelative();

    /** Set Looping mode 
    Inputs: true or false */
    void setLooping(bool shouldLoop);

    /** Start the playback */
    void start();

    /** Stop the playback */
    void stop();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    bool loop = false;
};
