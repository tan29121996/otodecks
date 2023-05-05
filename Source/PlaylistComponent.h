/*
  ==============================================================================

    PlaylistComponent.h
    Created: 2 Mar 2021 6:56:19pm
    Author:  The Ender

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public AudioSource
{
public:
    PlaylistComponent(AudioFormatManager& formatManagerToUse);
    ~PlaylistComponent();

    static std::vector<File> track;

private:
    int rows = 0;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    std::string convertTrackTitle(std::string trackTitle);

    void savePlaylist(); // Saves the playlist data to a text file
    void getPlaylist(); // Gets the playlist data from a text file

    juce::AudioDeviceManager customDeviceManager;
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettings;

    TableListBox tableComponent;
    ImageButton nextButton;
    ImageButton prevButton;
    ImageButton Button1;
    ImageButton Button2;
    ImageButton Button3;
    ImageButton Button4;
    ImageButton settings;

    std::vector<File> trackFiles;
    std::vector<String> trackTitles;
    std::vector<String> trackLength;
    std::vector<int> rowNumbers;

    Label searchLabel;
    TextEditor search;
    TextButton addButton{ "Add to library" };
    TextButton removeButton{ "Delete library" };

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    ScopedPointer<AudioFormatReaderSource> audioSourceReader;
    ScopedPointer<AudioFormatReader> audioFormatReader;
    AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
