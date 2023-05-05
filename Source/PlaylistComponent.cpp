/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 2 Mar 2021 6:56:19pm
    Author:  The Ender

  ==============================================================================
*/

#include <JuceHeader.h>
#include "juce_audio_formats/juce_audio_formats.h"
#include "PlaylistComponent.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<File> PlaylistComponent::track;

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager& formatManagerToUse)
{   
    formatManager.registerBasicFormats();

    addAndMakeVisible(audioSettings.get());

    // Create columns for the tablelistbox
    tableComponent.getHeader().setStretchToFitActive(true);
    tableComponent.getHeader().addColumn("", 1, 75);
    tableComponent.getHeader().addColumn("Track title", 2, 750);
    tableComponent.getHeader().addColumn("Artist", 3, 400);
    tableComponent.getHeader().addColumn("Album", 4, 400);
    tableComponent.getHeader().addColumn("Duration", 5, 400);
    tableComponent.getHeader().addColumn("Load", 6, 200);
    tableComponent.getHeader().addColumn("Remove", 7, 200);
    tableComponent.setColour(0x1002800, Colours::transparentWhite);
    tableComponent.setRowHeight(30);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    addAndMakeVisible(nextButton);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(settings);
    nextButton.addListener(this);
    prevButton.addListener(this);
    settings.addListener(this);

    addAndMakeVisible(Button1);
    addAndMakeVisible(Button2);
    addAndMakeVisible(Button3);
    addAndMakeVisible(Button4);
    Button1.addListener(this);
    Button2.addListener(this);
    Button3.addListener(this);
    Button4.addListener(this);

    Image normal1 = ImageCache::getFromMemory(BinaryData::next_png, BinaryData::next_pngSize);
    Image over1 = ImageCache::getFromMemory(BinaryData::nextOver_png, BinaryData::nextOver_pngSize);
    Image down1 = ImageCache::getFromMemory(BinaryData::nextOver_png, BinaryData::nextOver_pngSize);
    Image normal2 = ImageCache::getFromMemory(BinaryData::prev_png, BinaryData::prev_pngSize);
    Image over2 = ImageCache::getFromMemory(BinaryData::prevOver_png, BinaryData::prevOver_pngSize);
    Image down2 = ImageCache::getFromMemory(BinaryData::prevOver_png, BinaryData::prevOver_pngSize);
    Image normal3 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image over3 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image normal4 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image over4 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image normal5 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image over5 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image normal6 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image over6 = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    Image normal7 = ImageCache::getFromMemory(BinaryData::settings_png, BinaryData::settings_pngSize);
    Image over7 = ImageCache::getFromMemory(BinaryData::settingsOver_png, BinaryData::settingsOver_pngSize);

    nextButton.setImages(true, true, true, normal1, 0.8f, {}, normal1, 1.0f, {}, down1, 0.5f, {});
    prevButton.setImages(true, true, true, normal2, 0.8f, {}, normal2, 1.0f, {}, down2, 0.5f, {});

    Button1.setImages(true, true, true, normal3, 0.8f, {}, over3, 1.0f, {}, normal3, 0.5f, {});
    Button2.setImages(true, true, true, normal4, 0.8f, {}, over4, 1.0f, {}, normal4, 0.5f, {});
    Button3.setImages(true, true, true, normal5, 0.8f, {}, over5, 1.0f, {}, normal5, 0.5f, {});
    Button4.setImages(true, true, true, normal6, 0.8f, {}, over6, 1.0f, {}, normal6, 0.5f, {});

    settings.setImages(true, true, true, normal7, 0.7f, {}, normal7, 1.0f, {}, normal7, 0.5f, {});

    // Create search label for the search box
    addAndMakeVisible(searchLabel);
    searchLabel.setText("Search:", dontSendNotification);
    searchLabel.attachToComponent(&search, true);
    searchLabel.setColour(Label::textColourId, Colours::silver);
    searchLabel.setJustificationType(juce::Justification::right);

    addAndMakeVisible(search);
    search.setColour(0x1000205, Colours::transparentBlack);
    search.setColour(0x1000206, Colours::transparentBlack);
    search.setColour(0x1000200, Colours::transparentBlack);
    search.onTextChange = [this] { // When the text is changed in search box
        int rowNum = 0; // Row counter
        for (int i = 0; i < trackTitles.size(); i++) // Iterate through all elements in trackTitles
        {
            std::string title = trackTitles[i].toStdString(); // Convert juce::String to std::string
            std::transform(title.begin(), title.end(), title.begin(), ::tolower); // Transform all strings to lower case
            if (title.find(search.getText().toStdString()) != std::string::npos) // If search term is found in vector
            {
                tableComponent.selectRow(rowNum); // Select row that matches search term
                tableComponent.scrollToEnsureRowIsOnscreen(rowNum); // Ensure row is on screen
            }
            rowNum++; // Increment row counter
        }
    };

    addAndMakeVisible(addButton);
    addAndMakeVisible(removeButton);
    addButton.setColour(0x1000102, Colours::white);
    addButton.setColour(0x1000100, Colours::cornflowerblue);
    addButton.setColour(ComboBox::outlineColourId, Colour(40, 40, 40));
    addButton.addListener(this);
    removeButton.setColour(0x1000102, Colours::white);
    removeButton.setColour(0x1000100, Colour(210, 70, 70));
    removeButton.setColour(ComboBox::outlineColourId, Colour(40, 40, 40));
    removeButton.addListener(this);

    tableComponent.setMouseMoveSelectsRows(true);

    getPlaylist(); // Gets the playlist data from a text file
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{

}
void PlaylistComponent::releaseResources()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    double pos = getHeight() / 8;

    g.setFont(Font(Typeface::createSystemTypefaceFor(BinaryData::Septacharge_ttf, BinaryData::Septacharge_ttfSize)));

    g.fillAll (Colours::transparentBlack);

    Image light = ImageCache::getFromMemory(BinaryData::light_png, BinaryData::light_pngSize);
    g.drawImageWithin(light, 0, 40, getWidth(), getHeight() - 40, RectanglePlacement::fillDestination, false);
    g.setColour(Colours::black);
    g.setOpacity(0.8);
    g.fillRect(0, 0, getWidth(), getHeight()); // Playlist background

    g.setColour(Colour(40, 40, 40));
    g.fillRoundedRectangle(60, 10, getWidth() - 275, 20, 5);
    g.setColour(Colour(20, 20, 20));
    g.drawRoundedRectangle(60, 10, getWidth() - 275, 20, 5, 1);

    Image enso = ImageCache::getFromMemory(BinaryData::enso_test_png, BinaryData::enso_test_pngSize);
    double size = getHeight();
    double border = getWidth() / 2 - size;
    g.setOpacity(0.2);
    g.drawImageWithin(enso, 0, 90, getWidth(), getHeight() - 140, RectanglePlacement::centred, false); // Draw design image

    g.setColour(Colour(45, 200, 200));
    g.setFont(36.0f);
    g.setOpacity(0.2);
    g.drawText("Playlist", 0, 70, getWidth(), getHeight() - 100, Justification::centred, true);

    //==============================================================================

    g.setColour(Colour(40, 40, 40));
    g.fillRoundedRectangle(-10, 40, pos + 10, getHeight() - 70, 10);
    g.setColour(Colour(120, 120, 120));
    g.drawRoundedRectangle(-10, 40, pos + 10, getHeight() - 72, 10, 2);

    //==============================================================================

    double sz = pos - 10;
    double block = (getHeight() - 71.5 - 7 * sz) / 8;
    g.setColour(Colour(60, 125, 125));
    g.setOpacity(0.3);
    g.fillRoundedRectangle(4, 40 + block, sz, sz, 5);
    g.fillRoundedRectangle(4, 40 + 2 * block + sz, sz, sz, 5);
    g.fillRoundedRectangle(4, 40 + 7 * block + 6 * sz, sz, sz, 5);
    g.setColour(Colour(120, 185, 80));
    g.fillRoundedRectangle(4, 40 + 3 * block + 2 * sz, sz, sz, 5);
    g.setColour(Colours::cornflowerblue);
    g.fillRoundedRectangle(4, 40 + 4 * block + 3 * sz, sz, sz, 5);
    g.setColour(Colour(230, 150, 0));
    g.fillRoundedRectangle(4, 40 + 5 * block + 4 * sz, sz, sz, 5);
    g.setColour(Colour(210, 70, 70));
    g.fillRoundedRectangle(4, 40 + 6 * block + 5 * sz, sz, sz, 5);
    
    g.setColour(Colour(120, 120, 120));
    g.drawRoundedRectangle(4, 40 + block, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 2 * block + sz, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 7 * block + 6 * sz, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 3 * block + 2 * sz, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 4 * block + 3 * sz, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 5 * block + 4 * sz, sz, sz, 5, 2);
    g.drawRoundedRectangle(4, 40 + 6 * block + 5 * sz, sz, sz, 5, 2);

    Image deck = ImageCache::getFromMemory(BinaryData::playback_png, BinaryData::playback_pngSize);
    g.setOpacity(0.1);
    g.drawImageWithin(deck, -5, 40, pos - 5, getHeight() - 70, RectanglePlacement::stretchToFit, false);

    //==============================================================================

    g.setColour(Colour(65, 175, 175));
    g.fillRoundedRectangle(0, getHeight() - 30, getWidth(), 30, 5);
    g.fillRect(0, getHeight() - 30, getWidth(), 15);
    Image glow = ImageCache::getFromMemory(BinaryData::glow_png, BinaryData::glow_pngSize);
    g.setOpacity(0.3);
    g.drawImageWithin(glow, 0, getHeight() - 30, getWidth(), 30, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(100, 40, 40));
}

void PlaylistComponent::resized()
{
    double pos = getHeight() / 8;
    double position = (getHeight() - 6.8 * pos) / 2;
    tableComponent.setBounds(pos + 1, 40, getWidth() - pos - 1, getHeight() - 70);
    search.setBounds(60, 10, getWidth() - 275, 20);
    addButton.setBounds(getWidth() - 210, 5, 100, 30);
    removeButton.setBounds(getWidth() - 105, 5, 100, 30);

    double sz = pos - 10;
    double block = (getHeight() - 72 - 7 * sz) / 8;
    nextButton.setBounds(4, 40 + block, sz, sz);
    prevButton.setBounds(4, 40 + 2 * block + sz, sz, sz);
    Button1.setBounds(4, 40 + 3 * block + 2 * sz, sz, sz);
    Button2.setBounds(4, 40 + 4 * block + 3 * sz, sz, sz);
    Button3.setBounds(4, 40 + 5 * block + 4 * sz, sz, sz);
    Button4.setBounds(4, 40 + 6 * block + 5 * sz, sz, sz);
    settings.setBounds(4, 40 + 7 * block + 6 * sz, sz, sz);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colour(55, 75, 75));
    }
    else 
    {
        g.setColour(Colour(80, 80, 80));
        g.drawRect(getLocalBounds(), 1);
        g.fillAll(Colours::transparentWhite);
    }
}

void PlaylistComponent::paintCell(Graphics & g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        g.setFont(17.0f);
        if (columnId == 1)
        {
            if (rowNumbers[rowNumber] % 4 == 1)
            {
                g.setColour(Colour(120, 185, 80));
                g.drawText(" " + String(rowNumbers[rowNumber]),
                    2, 0,
                    width, height,
                    Justification::centred,
                    true);
            }
            else if (rowNumbers[rowNumber] % 4 == 2)
            {
                g.setColour(Colours::cornflowerblue);
                g.drawText(" " + String(rowNumbers[rowNumber]),
                    2, 0,
                    width, height,
                    Justification::centred,
                    true);
            }
            else if (rowNumbers[rowNumber] % 4 == 3)
            {
                g.setColour(Colour(230, 150, 0));
                g.drawText(" " + String(rowNumbers[rowNumber]),
                    2, 0,
                    width, height,
                    Justification::centred,
                    true);
            }
            else if (rowNumbers[rowNumber] % 4 == 0)
            {
                g.setColour(Colour(210, 70, 70));
                g.drawText(" " + String(rowNumbers[rowNumber]),
                    2, 0,
                    width, height,
                    Justification::centred,
                    true);
            }
        }

        g.setFont(Font(Typeface::createSystemTypefaceFor(BinaryData::MontserratRegular_ttf, BinaryData::MontserratRegular_ttfSize)));
        g.setFont(17.0f);
        g.setColour(juce::Colour(230, 230, 230));
        if (columnId == 2)
        {
            g.drawText(" " + trackTitles[rowNumber],
                        2, 0,
                        width, height,
                        Justification::centredLeft,
                        true);
        }

        if (columnId == 3)
        {
            g.drawText("-",
                2, 0,
                width, height,
                Justification::centredLeft,
                true);
        }

        if (columnId == 4)
        {
            g.drawText("-",
                2, 0,
                width, height,
                Justification::centredLeft,
                true);
        }

        if (columnId == 5)
        {
            g.drawText(" " + trackLength[rowNumber],
                        2, 0,
                        width, height,
                        Justification::centredLeft,
                        true);
        }
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    if (columnId == 6)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Select");
            String id{ std::to_string(rowNumber) };
            btn->setColour(0x1000102, Colour(60, 175, 175));
            btn->setColour(0x1000100, Colour(40, 40, 40));
            btn->setColour(ComboBox::outlineColourId, Colour(60, 60, 60));
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    if (columnId == 7)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Remove");
            String id{ std::to_string(rowNumber) };
            btn->setColour(0x1000102, Colour(60, 175, 175));
            btn->setColour(0x1000100, Colour(40, 40, 40));
            btn->setColour(ComboBox::outlineColourId, Colour(60, 60, 60));
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button* button)
{
    int rowNum = 0;

    if (button == &addButton) // If add to playlist button is clicked
    {
        FileChooser fileChooser("Select media files"); // Creates a dialog box to choose a file
        if (fileChooser.browseForMultipleFilesToOpen()) // Shows a dialog box to choose multiple files to open
        {
            for (int i = 0; i < fileChooser.getResults().size(); i++) // For every file selected using fileChooser
            {
                AudioTransportSource audioSource; // Creates an AudioTransportSource

                // Creates a suitable reader for the files selected using fileChooser
                audioFormatReader = formatManager.createReaderFor(fileChooser.getResults().getReference(i).getFullPathName());

                if (audioFormatReader)
                {
                    // Create a new AudioSource to read from the audioFormatReader
                    audioSourceReader = new AudioFormatReaderSource(formatManager.createReaderFor
                                                                   (fileChooser.getResults().getReference(i).getFullPathName()), true);
                    // Sets the new AudioSource that is being used as the input source
                    audioSource.setSource(audioSourceReader);
                    
                    trackFiles.push_back(fileChooser.getResults().getReference(i)); // Insert files selected using fileChooser

                    std::string trackTitle = fileChooser.getResults().getReference(i).getFileNameWithoutExtension().toStdString();
                    trackTitle = convertTrackTitle(trackTitle);
                    trackTitles.push_back(String(trackTitle)); // Insert file names selected using fileChooser

                    int secs = audioSource.getLengthInSeconds();
                    int mins = secs / 60;
                    secs = secs % 60;
                    String duration = String(mins) + " mins " + String(secs) + " secs";
                    trackLength.push_back(duration);

                    rows++;
                    rowNumbers.push_back(rows);
                }
            }
            savePlaylist(); // Saves the playlist data to a text file
        } 
    }
    else if (button == &removeButton) // If remove button is clicked
    {
        trackTitles.clear();
        trackLength.clear();
        trackFiles.clear();
        savePlaylist();
    }
    else if (button == &nextButton)
    {

    }
    else if (button == &prevButton)
    {

    }
    else if (button == &Button1)
    {
        DialogWindow::LaunchOptions profile;
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::white);
        label->setJustificationType(juce::Justification::centred);
        label->setText("Profile A has been loaded.",
            dontSendNotification);
        profile.resizable = false;
        profile.content.setOwned(label);
        Rectangle<int> area(0, 0, 500, 200);
        profile.content->setSize(area.getWidth(), area.getHeight());
        profile.dialogTitle = "Profile";
        profile.dialogBackgroundColour = Colour(76, 76, 76);
        profile.componentToCentreAround;
        profile.useNativeTitleBar = false;
        DialogWindow* dw = profile.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 200);
    }
    else if (button == &Button2)
    {
        DialogWindow::LaunchOptions profile;
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::white);
        label->setJustificationType(juce::Justification::centred);
        label->setText("Profile B has been loaded.",
            dontSendNotification);
        profile.resizable = false;
        profile.content.setOwned(label);
        Rectangle<int> area(0, 0, 500, 200);
        profile.content->setSize(area.getWidth(), area.getHeight());
        profile.dialogTitle = "Profile";
        profile.dialogBackgroundColour = Colour(76, 76, 76);
        profile.componentToCentreAround;
        profile.useNativeTitleBar = false;
        DialogWindow* dw = profile.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 200);
    }
    else if (button == &Button3)
    {
        DialogWindow::LaunchOptions profile;
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::white);
        label->setJustificationType(juce::Justification::centred);
        label->setText("Profile C has been loaded.",
            dontSendNotification);
        profile.resizable = false;
        profile.content.setOwned(label);
        Rectangle<int> area(0, 0, 500, 200);
        profile.content->setSize(area.getWidth(), area.getHeight());
        profile.dialogTitle = "Profile";
        profile.dialogBackgroundColour = Colour(76, 76, 76);
        profile.componentToCentreAround;
        profile.useNativeTitleBar = false;
        DialogWindow* dw = profile.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 200);
    }
    else if (button == &Button4)
    {
        DialogWindow::LaunchOptions profile;
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::white);
        label->setJustificationType(juce::Justification::centred);
        label->setText("Profile D has been loaded.",
            dontSendNotification);
        profile.resizable = false;
        profile.content.setOwned(label);
        Rectangle<int> area(0, 0, 500, 200);
        profile.content->setSize(area.getWidth(), area.getHeight());
        profile.dialogTitle = "Profile";
        profile.dialogBackgroundColour = Colour(76, 76, 76);
        profile.componentToCentreAround;
        profile.useNativeTitleBar = false;
        DialogWindow* dw = profile.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 200);
    }
    else if (button == &settings)
    {
        customDeviceManager.initialise(2, 2, nullptr, true);
        audioSettings.reset(new juce::AudioDeviceSelectorComponent(customDeviceManager,
                            0, 2, 0, 2,
                            true, true, true, true));

        DialogWindow::LaunchOptions settings;
        Label* line = new Label();
        line->setColour(Label::textColourId, Colours::orange);
        line->setJustificationType(juce::Justification::centred);
        settings.content.setOwned(line);
        Rectangle<int> area(0, 0, 500, 260);
        settings.content->setSize(area.getWidth(), area.getHeight());
        settings.dialogTitle = "Settings";
        settings.dialogBackgroundColour = Colour(76, 76, 76);
        settings.componentToCentreAround;
        settings.useNativeTitleBar = false;
        settings.resizable = false;
        DialogWindow* dw = settings.launchAsync();
        dw->setContentNonOwned(audioSettings.get(), false);
        dw->setVisible(true);
        dw->centreWithSize(500, 260);
    }
    else
    {
        rowNum = tableComponent.getSelectedRow();
        if (track.size() > 0) // If track vector is not empty
        {
            track.pop_back(); // Remove last element
            track.push_back(trackFiles[rowNum]); // Insert element corresponding to row number
        }
        else
        {
            track.push_back(trackFiles[rowNum]); // Insert element corresponding to row number
        }
    }

    /*trackTitles.erase(std::next(trackTitles.begin(), rowNum));
    trackLength.erase(std::next(trackLength.begin(), rowNum));
    trackFiles.erase(std::next(trackFiles.begin(), rowNum));
    track.pop_back();
    savePlaylist();*/

    tableComponent.updateContent(); // Update the tablelistbox
}

std::string PlaylistComponent::convertTrackTitle(std::string trackTitle)
{
    // Replace all underscore with space
    std::replace(trackTitle.begin(), trackTitle.end(), '_', ' ');

    // Remove all brackets
    int i = 0;
    while (i < trackTitle.length())
    {
        if (trackTitle[i] == '(' || trackTitle[i] == ')')
        {
            trackTitle.erase(i, 1);
        }
        else {
            i++;
        }
    }

    // Convert first letter of each word to uppercase
    trackTitle[0] = toupper(trackTitle[0]);
    for (int i = 1; i < trackTitle.length(); i++)
    {
        if (trackTitle[i - 1] == ' ')
            trackTitle[i] = toupper(trackTitle[i]);
    }

    return trackTitle;
}

void PlaylistComponent::savePlaylist() 
{
    std::ofstream trackData{ "PlaylistTracks.txt" };
    for (int i = 0; i < trackFiles.size(); i++)
    {
        trackData << trackFiles[i].getFullPathName().toStdString() << std::endl;
    }
}

void PlaylistComponent::getPlaylist()
{
    std::string line;
    std::ifstream trackData{ "PlaylistTracks.txt" };

    if (trackData.is_open())
    {
        while (getline(trackData, line))
        {
            File track(line);
            auto* reader = formatManager.createReaderFor(URL{track}.createInputStream(false));
            if (reader != nullptr)
            {
                std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                readerSource.reset(newSource.release());
            }
            // Insert files into trackFiles vector
            trackFiles.push_back(track);

            std::string trackTitle = track.getFileNameWithoutExtension().toStdString();
            trackTitle = convertTrackTitle(trackTitle);

            // Insert file names into trackTitles vector
            trackTitles.push_back(trackTitle);

            AudioTransportSource audioSource; // Creates an AudioTransportSource

            // Create a new AudioSource to read from the audioFormatReader
            audioSourceReader = new AudioFormatReaderSource(formatManager.createReaderFor(track.getFullPathName()), true);

            // Sets the new AudioSource that is being used as the input source
            audioSource.setSource(audioSourceReader);

            // Convert the song length into minutes and seconds
            int secs = audioSource.getLengthInSeconds();
            int mins = secs / 60;
            secs = secs % 60;

            // Set duration and track title
            String duration = String(mins) + " mins " + String(secs) + " secs";

            // Insert duration into trackLength vector
            trackLength.push_back(duration);

            rows++;
            rowNumbers.push_back(rows);
        }
        trackData.close();
    }
}