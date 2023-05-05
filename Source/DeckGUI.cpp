/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================

DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(_player, formatManagerToUse, cacheToUse)
{
    formatManager.registerBasicFormats();

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    addAndMakeVisible(loadPlaylist);

    addAndMakeVisible(info);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(volSlider);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(slider1);
    slider1.setLookAndFeel(&myLookAndFeelV5);
    slider1.setAlpha(0.9);
    slider1.setSliderStyle(Slider::Rotary);
    slider1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(slider2);
    slider2.setLookAndFeel(&myLookAndFeelV6);
    slider2.setAlpha(0.9);
    slider2.setSliderStyle(Slider::Rotary);
    slider2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);

    loadPlaylist.addListener(this);

    info.addListener(this);
    prevButton.addListener(this);
    nextButton.addListener(this);

    volSlider.addListener(this);
    posSlider.addListener(this);

    slider1.addListener(this);
    slider2.addListener(this);

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1.0);
    posSlider.setRange(0.0, 1.0);

    slider1.setRange(0.0, 1.0);
    slider2.setRange(0.0, 1.0);

    volSlider.setColour(0x1001300, Colour(55, 175, 175));
    volSlider.setColour(0x1001200, Colours::transparentBlack);
    volSlider.setColour(0x1001310, Colours::transparentBlack);
    posSlider.setColour(0x1001200, Colours::transparentBlack);
    posSlider.setColour(0x1001300, Colours::transparentBlack);
    posSlider.setColour(0x1001310, Colours::transparentBlack);

    startTimer(500);

    // Get the images for the Play, Stop, Load and Loop buttons
    Image normal1 = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    Image over1 = ImageCache::getFromMemory(BinaryData::playOver_png, BinaryData::playOver_pngSize);
    Image down1 = ImageCache::getFromMemory(BinaryData::playOver_png, BinaryData::playOver_pngSize);
    Image normal2 = ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
    Image over2 = ImageCache::getFromMemory(BinaryData::stopOver_png, BinaryData::stopOver_pngSize);
    Image down2 = ImageCache::getFromMemory(BinaryData::stopOver_png, BinaryData::stopOver_pngSize);
    Image normal3 = ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize);
    Image over3 = ImageCache::getFromMemory(BinaryData::loadOver_png, BinaryData::loadOver_pngSize);
    Image down3 = ImageCache::getFromMemory(BinaryData::loadOver_png, BinaryData::loadOver_pngSize);
    Image normal4 = ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
    Image over4 = ImageCache::getFromMemory(BinaryData::loopOver_png, BinaryData::loopOver_pngSize);
    Image down4 = ImageCache::getFromMemory(BinaryData::loopOver_png, BinaryData::loopOver_pngSize);

    // Set the images for the Play, Stop, Load and Loop buttons
    playButton.setImages(true, true, true, normal1, 1.0f, {}, over1, 1.0f, {}, down1, 0.5f, {});
    stopButton.setImages(true, true, true, normal2, 1.0f, {}, over2, 1.0f, {}, down2, 0.5f, {});
    loadButton.setImages(true, true, true, normal3, 1.0f, {}, over3, 1.0f, {}, down3, 0.5f, {});
    loopButton.setImages(true, true, true, normal4, 1.0f, {}, over4, 1.0f, {}, down4, 0.5f, {});

    // Get the image for the Playlist Load button
    Image normal5 = ImageCache::getFromMemory(BinaryData::headset_png, BinaryData::headset_pngSize);
    loadPlaylist.setImages(true, true, true, normal5, 0.8f, {}, normal5, 1.0f, {}, normal5, 0.5f, {});

    Image normal6 = ImageCache::getFromMemory(BinaryData::info_png, BinaryData::info_pngSize);
    Image normal7 = ImageCache::getFromMemory(BinaryData::deck1B_png, BinaryData::deck1B_pngSize);
    Image normal8 = ImageCache::getFromMemory(BinaryData::deck2B_png, BinaryData::deck2B_pngSize);
    info.setImages(true, true, true, normal6, 0.8f, {}, normal6, 1.0f, {}, normal6, 0.5f, {});
    prevButton.setImages(true, true, true, normal7, 0.6f, {}, normal7, 1.0f, {}, normal7, 0.5f, {});
    nextButton.setImages(true, true, true, normal8, 0.6f, {}, normal8, 1.0f, {}, normal8, 0.5f, {});
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.setFont(Font(customLookAndFeel.getCustomFont()));
    g.fillAll (Colours::transparentBlack);

    double pos = getHeight() / 8; 
    double border = getHeight() / 50;
    double btn = 1.85 * pos - border;
    double s_space = getWidth() - pos * 2;
    double s_border = pos / 2 - 7.5;

    //==============================================================================
    // Background for playback controls
    g.setColour(Colour(55, 90, 90));
    g.fillRect(0, -2, getWidth() + 2, pos + 2);
    
    g.setColour(Colour(50, 70, 70));
    g.drawRect(pos, 7, pos, pos - 14, 2);
    g.setColour(Colour(70, 95, 95));
    g.drawRect(pos + 2, 7, pos, pos - 14, 2);
    g.setColour(Colour(55, 90, 90));
    g.fillRect(pos, -2, pos - 1.5, pos + 2);

    Image bannerbg = ImageCache::getFromMemory(BinaryData::deck_png, BinaryData::deck_pngSize);
    g.setOpacity(0.1);
    g.drawImageWithin(bannerbg, pos, 0, pos * 10, pos, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(90, 90, 90));
    g.drawRect(0, -2, getWidth() + 2, pos + 3, 2);

    //==============================================================================
    // Loop
    double posH = getHeight() / 30;
    double n_border = posH * 9;

    Image patternL = ImageCache::getFromMemory(BinaryData::patternL_png, BinaryData::patternL_pngSize);
    Image patternR = ImageCache::getFromMemory(BinaryData::patternR_png, BinaryData::patternR_pngSize);
    g.setOpacity(0.4);
    g.drawImageWithin(patternL, 2 * pos + posH / 2, posH / 2, n_border - 1.2 * posH, pos - posH, RectanglePlacement::stretchToFit, false);
    g.drawImageWithin(patternR, getWidth() - (n_border - posH) - 0.8 * posH / 2, posH / 2, n_border - 1.2 * posH, pos - posH, RectanglePlacement::stretchToFit, false);

    g.setColour(Colour(50, 50, 50));
    g.fillRoundedRectangle(2 * pos + n_border, posH / 2, getWidth() - 2 * (pos + n_border), pos - posH, 5);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(2 * pos + n_border, posH / 2, getWidth() - 2 * (pos + n_border), pos - posH, 5, 2);
    g.setColour(Colour(80, 80, 80));
    g.drawRoundedRectangle(2 * pos + n_border - 1, posH / 2 - 1, getWidth() - 2 * (pos + n_border) + 2, pos - posH + 2, 7, 2);

    // Loop text
    g.setFont(18.0f);
    if (loop) // If loop is set to true
    {
        // Green
        g.setColour(Colour(120, 185, 80));
        g.fillRoundedRectangle(2, pos * 2 - 7, pos - 2, pos + 7, 7);
        g.setColour(Colours::silver);
        // Display fitted text showing "Song is set to loop"
        g.drawFittedText("Loop is active", pos * 2, 0, getWidth() - 2 * pos, pos,
            Justification::centred, true);
    }
    else // If loop is set to false
    {
        // Orange
        g.setColour(Colour(230, 150, 0));
        g.fillRoundedRectangle(2, pos * 2 - 7, pos - 2, pos + 7, 7);
        g.setColour(Colours::silver);
        // Display fitted text showing "Song is not set to loop"
        g.drawFittedText("Loop is not active", pos * 2, 0, getWidth() - 2 * pos, pos,
            Justification::centred, true);
    }

    // Red
    g.setColour(Colour(210, 70, 70));
    g.fillRect(2, pos, pos - 2, pos);
    
    g.setColour(Colour(120, 120, 120));
    g.drawRoundedRectangle(1, -5, pos, pos * 3 + 5, 7, 2);

    // Blue
    g.setColour(Colours::cornflowerblue);
    g.fillRect(2, 0, pos - 1, pos);

    Image bg = ImageCache::getFromMemory(BinaryData::playback_png, BinaryData::playback_pngSize);
    g.setOpacity(0.35);
    g.drawImageWithin(bg, 2, 0, pos - 2, pos * 3, RectanglePlacement::stretchToFit, false);

    //==============================================================================
    // Background for volume and position sliders
    g.setColour(Colour(40, 40, 40));
    g.setOpacity(0.7);
    g.fillRoundedRectangle(pos * 2, pos, getWidth(), pos * 2, 10);
    Image deckbg = ImageCache::getFromMemory(BinaryData::volBack_png, BinaryData::volBack_pngSize);
    g.setOpacity(0.05);
    g.drawImageWithin(deckbg, pos * 2, 2 * pos, getWidth(), pos * 2, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(90, 145, 145));
    g.drawRoundedRectangle(pos * 2 + 1, pos + 2, getWidth(), pos * 2, 10, 2);

    // Background for Song Details section
    g.setColour(Colour(40, 40, 40));
    g.fillRoundedRectangle(0, pos * 5.5, getWidth(), pos * 2.5, 10);
    g.setColour(Colour(100, 40, 40));
    g.fillRect(0, pos * 5.5, pos * 5, 3);

    // Create volume slider shape
    g.setColour(Colour(55, 105, 105));
    g.setOpacity(0.5);
    g.fillRoundedRectangle(2 * pos + s_border, pos * 2.5 - 7.5, s_space - 2.05 * s_border, 15, 8);
    g.setOpacity(0.1);
    g.drawImageWithin(deckbg, 2 * pos + s_border, pos * 2.5 - 7.5, s_space - 2.05 * s_border, 15, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(30, 30, 30));
    g.drawRoundedRectangle(2 * pos + s_border, pos * 2.5 - 7.5, s_space - 2.05 * s_border , 15, 8, 2);
    g.setColour(Colour(90, 105, 105));
    g.drawRoundedRectangle(2 * pos + s_border - 2, pos * 2.5 - 7.5 - 2, s_space - 2.05 * s_border + 4, 19, 10, 2);

    //==============================================================================

    double circle = 1.05 * btn;
    g.setColour(Colour(40, 40, 40));
    g.setOpacity(0.5);
    g.fillEllipse(getWidth() / 2 - circle / 2, pos * 3 + border, circle, circle);
    Image ellipse = ImageCache::getFromMemory(BinaryData::circle_png, BinaryData::circle_pngSize);
    g.setOpacity(0.2);
    g.drawImageWithin(ellipse, getWidth() / 2 - 0.4 * circle, pos * 3 + 2.5 * border, 0.8 * circle, 0.75 * circle, RectanglePlacement::centred, false);
    g.setOpacity(0.7);
    Image enso = ImageCache::getFromMemory(BinaryData::enso_png, BinaryData::enso_pngSize);
    g.drawImageWithin(enso, getWidth() / 2 - circle / 2, pos * 3 + border, circle, circle, RectanglePlacement::centred, false);

    //==============================================================================
    // Song Details section
    g.setColour(Colour(60, 125, 125));
    g.fillRoundedRectangle(getWidth() / 2 - 75, pos * 5.1, 150, 0.8 * pos, 10);
    Image deck = ImageCache::getFromMemory(BinaryData::deck_png, BinaryData::deck_pngSize);
    g.setOpacity(0.1);
    g.drawImageWithin(deck, getWidth() / 2 - 75, pos * 5.1, 150, 0.8 * pos, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(40, 40, 40));
    g.drawRoundedRectangle(getWidth() / 2 - 75, pos * 5.1, 150, 0.8 * pos, 10, 2);
    g.setColour(Colour(70, 70, 70));
    g.drawRoundedRectangle(getWidth() / 2 - 77, pos * 5.1 - 2, 154, 0.8 * pos + 4, 12, 2);

    //==============================================================================

    double circleH = (pos * 2.5 - 40) / 5;
    g.setColour(Colour(90, 90, 90));
    g.drawRect(0, pos * 5.5 + 20, pos + 2, pos * 2.5 - 40, 2);
    g.setColour(Colour(40, 40, 40));
    g.fillRect(0, pos * 5.5 + 2, pos, pos * 2.5 - 10);

    g.setOpacity(0.05);
    g.drawImageWithin(bg, 0, pos * 5.5, pos, pos * 2.5, RectanglePlacement::stretchToFit, false);
    g.setColour(Colour(120, 185, 80));
    g.fillRoundedRectangle(pos / 2 - 10, pos * 5.5 + circleH, 20, 10, 5);
    g.setColour(Colours::cornflowerblue);
    g.fillRoundedRectangle(pos / 2 - 10, pos * 5.5 + 2 * circleH + 10, 20, 10, 5);
    g.setColour(Colour(230, 150, 0));
    g.fillRoundedRectangle(pos / 2 - 10, pos * 5.5 + 3 * circleH + 20, 20, 10, 5);
    g.setColour(Colour(210, 70, 70));
    g.fillRoundedRectangle(pos / 2 - 10, pos * 5.5 + 4 * circleH + 30, 20, 10, 5);

    //==============================================================================

    double textPos = pos * 1.35;
    g.setColour(Colour(50, 50, 50));
    g.setFont(21.0f);
    g.drawText("Song Details", 0, pos * 5, getWidth(), pos,
                Justification::centred, true);
    g.setColour(Colours::silver);
    g.setFont(18.0f);
    g.drawText("Title: ", pos + 22.5, pos * 5.5, 52.5, 1.03 * textPos,
                Justification::bottomLeft, true);
    g.drawText("Length: ", pos + 22.5, pos * 5.5 + 0.95 * textPos, 75, textPos,
                Justification::left, true);

    g.setColour(Colours::white);
    g.drawText(trackTitle, pos + 70, pos * 5.5, getWidth() - 5.25 * pos, 1.03 * textPos,
        Justification::bottomLeft, true);
    g.drawText(duration, pos + 92.5, pos * 5.5 + 0.95 * textPos, getWidth() - 4 * pos, textPos,
        Justification::left, true);

    //==============================================================================
    // Set up images for volume and position icons
    Image volImg = ImageCache::getFromMemory(BinaryData::volume_png, BinaryData::volume_pngSize);
    Image posImg = ImageCache::getFromMemory(BinaryData::position_png, BinaryData::position_pngSize);
    g.drawImageWithin(volImg, pos, 2.1 * pos, pos, 0.9 * pos, RectanglePlacement::centred, false); // Draw volume image
    g.drawImageWithin(posImg, pos, pos, pos, pos, RectanglePlacement::stretchToFit, false); // Draw position image

    //==============================================================================
    
    double waveSize = (getWidth() - 6 * border - 3 * btn) / 2;
    double space = getWidth() / 2 - 1.5 * btn - border;
    double wavePos = (space - waveSize) / 2;
    g.setOpacity(0.5);
    Image wave1 = ImageCache::getFromMemory(BinaryData::waveLeftW_png, BinaryData::waveLeftW_pngSize);
    Image wave2 = ImageCache::getFromMemory(BinaryData::waveRightW_png, BinaryData::waveRightW_pngSize);
    g.drawImageWithin(wave1, border + btn + wavePos, pos * 3 + border, waveSize, btn, RectanglePlacement::stretchToFit, false);
    g.drawImageWithin(wave2, getWidth() - border - btn - wavePos - waveSize, pos * 3 + border, waveSize, btn, RectanglePlacement::stretchToFit, false);

    //==============================================================================

    Image design = ImageCache::getFromMemory(BinaryData::design_png, BinaryData::design_pngSize); // Set up image for design
    g.drawImageWithin(design, -5, pos * 6, getWidth(), pos * 2, RectanglePlacement::centred, false); // Draw design image
    Image icon = ImageCache::getFromMemory(BinaryData::iconG_png, BinaryData::iconG_pngSize); 
    g.setOpacity(0.07);
    g.drawImageWithin(icon, getWidth() / 2 - pos / 2, pos * 6.35, pos, pos, RectanglePlacement::centred, false);

    //==============================================================================

    float button = 1.665 * pos - border;
    double imgSize = 0.9 * btn;
    int size = getHeight() - 3.5 * border - button - 5.5 * pos;

    // Album cover background
    Path n;
    n.addTriangle(getWidth() - 0.75 * button, pos * 8, getWidth() - 3.55 * pos, pos * 5.5 - 1, getWidth(), pos * 5.5 - 1);
    g.setColour(Colour(53, 53, 53));
    g.fillPath(n);
    Path p;
    p.addTriangle(getWidth() - 2 * button, pos * 8, getWidth() - button , pos * 5.5, getWidth() - button, pos * 8);
    g.setColour(Colour(55, 105, 105));
    g.fillPath(p);
    g.fillRoundedRectangle(getWidth() - border - button, pos * 5.5, button +  border, pos * 2.5, 10);
    Image effect = ImageCache::getFromMemory(BinaryData::effect_png, BinaryData::effect_pngSize);
    g.setOpacity(0.15);
    g.drawImageWithin(effect, getWidth() - 2 * button, pos * 5.5, 2 * button, pos * 3, RectanglePlacement::centred, false);

    Path t;
    t.addTriangle(getWidth() - 2 * button, pos * 5.5, getWidth(), pos * 5.5, getWidth() - 6.5 * border - button, pos * 5.5 + size);
    g.setColour(Colour(150, 150, 150));
    g.fillPath(t);
    const juce::Rectangle<int> banner(getWidth() - 6.5 * border - button, pos * 5.5 + 0.5, 7 * border + button, size);
    g.fillRect(banner);
    g.setColour(Colour(60, 60, 60));
    g.setFont(18.0f);
    g.drawText("Album Cover", banner, Justification::centred, true);

    // Album cover
    g.setColour(Colour(40, 40, 40));   
    g.fillRoundedRectangle(getWidth() - 1.5 * border - button, getHeight() - 2 * border - button, button, button, 10);
    g.setColour(Colour(20, 20, 20));
    g.drawRoundedRectangle(getWidth() - 1.5 * border - button, getHeight() - 2 * border - button, button, button, 10, 2);
    g.setColour(Colour(90, 125, 125));
    g.drawRoundedRectangle(getWidth() - 1.5 * border - button - 2, getHeight() - 2 * border - button - 2, button + 4, button + 4, 12, 2);

    // Music image
    g.setOpacity(0.3);
    Image itunes = ImageCache::getFromMemory(BinaryData::iTunes_png, BinaryData::iTunes_pngSize);
    g.drawImageWithin(itunes, getWidth() - 1.5 * border - imgSize, getHeight() - 2 * border - imgSize, imgSize, imgSize, RectanglePlacement::centred, false);

    //==============================================================================
    
    g.setFont(18.0f);
    g.setColour(Colours::silver);
    g.drawFittedText("Equalizer", 1.5 * border, pos * 4.55, btn, pos,
        Justification::centred, true);
    g.drawFittedText("Equalizer", getWidth() - 1.5 * border - btn, pos * 4.55, btn, pos,
        Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    double border = getHeight() / 50;
    double btn = 1.85 * rowH - border;
    playButton.setBounds(1, 0, rowH, rowH);
    stopButton.setBounds(1, rowH, rowH, rowH);
    loadButton.setBounds(rowH, -2, rowH, 1.1 * rowH);
    loopButton.setBounds(1, rowH * 2, rowH, rowH);

    loadPlaylist.setBounds(getWidth() / 2 - 0.2 * btn, rowH * 3 + 4.6 * border, 0.4 * btn, 0.35 * btn);

    double value = (2.25 * rowH + 22.5) - (rowH + 17.5 + 0.4 * btn);
    info.setBounds(rowH + 17.5 + value / 2 + 0.3 * btn, rowH * 5.825, 0.2 * btn, 0.2 * btn);
    prevButton.setBounds(rowH + 17.5, rowH * 5.65, 0.4 * btn, 0.4 * btn);
    nextButton.setBounds(2.25 * rowH + 22.5, rowH * 5.65, 0.4 * btn, 0.4 * btn);

    double s_space = getWidth() - rowH * 2;
    double s_border = rowH / 2 - 11.5;
    volSlider.setBounds(2 * rowH + s_border, 2 * rowH, s_space - 2 * s_border, rowH);
    posSlider.setBounds(2 * rowH - 12, rowH, s_space + 24, rowH);

    double newBorder = 1.2 * border;
    double wheel = 0.9 * btn;
    slider1.setBounds(1.5 * newBorder, rowH * 3 + 1.5 * newBorder, wheel, 0.9 * wheel);
    slider2.setBounds(getWidth() - 1.5 * newBorder - wheel, rowH * 3 + 1.5 * newBorder, wheel, 0.9 * wheel);

    waveformDisplay.setBounds(2 * rowH, rowH, s_space, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton) // If play button is clicked
    {
        if (playing == true)
        {
            std::cout << "Play button was clicked " << std::endl;
            player->start(); // Call start function in DJAudioPlayer
            playing = false;
        }
        else
        {
            std::cout << "Stop button was clicked " << std::endl;
            player->stop(); // Call stop function in DJAudioPlayer
            playing = true;
        }
    }

     if (button == &stopButton) // If stop button is clicked
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->setPositionRelative(0.0);
        player->stop();
        playing = true;
    }

    if (button == &loadButton) // If load button is clicked
    {
        FileChooser chooser{"Select a file..."}; // Creates a dialog box to choose a file
        if (chooser.browseForFileToOpen()) // Shows a dialog box to choose multiple files to open
        {
            player->loadURL(URL{chooser.getResult()}); // Loads the file to the player
            waveformDisplay.loadURL(URL{chooser.getResult()}); // Display the audio as a waveform
        }

        AudioTransportSource audioSource; // Creates an AudioTransportSource

        // Create a new AudioSource to read from the audioFormatReader
        audioSourceReader = new AudioFormatReaderSource(formatManager.createReaderFor(chooser.getResult().getFullPathName()), true);

        // Sets the new AudioSource that is being used as the input source
        audioSource.setSource(audioSourceReader);

        // Convert the song length into minutes and seconds
        int secs = audioSource.getLengthInSeconds();
        int mins = secs / 60;
        secs = secs % 60;

        // Set duration and track title
        duration = String(mins) + " mins " + String(secs) + " secs"; 
        std::string title = chooser.getResult().getFileNameWithoutExtension().toStdString();
        title = convertTrackTitle(title);
        trackTitle = String(title);

        repaint();
    }

    if (button == &loopButton) // If loop button is clicked
    {
        if (loop) // If loop is true
        {
            loop = false; // Switch to false
            player->setLooping(false); // Set Looping mode to false
        }
        else
        {
            loop = true;
            player->setLooping(true); // Set Looping mode to true
        }  

        repaint();
    }

    if (button == &loadPlaylist) // If playlist load button is clicked
    {
        if (PlaylistComponent::track.size() == 0) // if tracks have not been selected from the playlist
        {
            // Launch a dialog window 
            DialogWindow::LaunchOptions search;
            Label* label = new Label();
            label->setColour(Label::textColourId, Colours::white);
            label->setJustificationType(juce::Justification::centred);
            label->setText("You have not selected a song from the playlist.",
                            dontSendNotification);
            search.resizable = false;
            search.content.setOwned(label);
            Rectangle<int> area(0, 0, 500, 200);
            search.content->setSize(area.getWidth(), area.getHeight());
            search.dialogTitle = "Playlist Load";
            search.dialogBackgroundColour = Colour(76, 76, 76);
            search.componentToCentreAround;
            search.useNativeTitleBar = false;
            DialogWindow* dw = search.launchAsync();
            dw->setVisible(true);
            dw->centreWithSize(500, 200);
        }
        else // if tracks have been selected from the playlist
        {
            playing = true;

            File track = PlaylistComponent::track[0]; // Get the track file
            player->loadURL(URL{ track }); // Load the file to the player
            waveformDisplay.loadURL(URL{ track }); // Display the audio as a waveform

            AudioTransportSource audioSource;
            audioSourceReader = new AudioFormatReaderSource(formatManager.createReaderFor(track.getFullPathName()), true);
            audioSource.setSource(audioSourceReader);
            int secs = audioSource.getLengthInSeconds();
            int mins = secs / 60;
            secs = secs % 60;
            duration = String(mins) + " mins " + String(secs) + " secs";

            std::string title = track.getFileNameWithoutExtension().toStdString();
            title = convertTrackTitle(title);
            trackTitle = String(title);

            repaint(); 
        }
    }
    else if (button == &info)
    {
        DialogWindow::LaunchOptions search;
        Label* label = new Label();
        label->setColour(Label::textColourId, Colours::white);
        label->setJustificationType(juce::Justification::centredLeft);
        label->setText("Song title: " + trackTitle + "\n\nSong artist: " +
            "\n\nSong album: " + "\n\nSong length: " + duration,
            dontSendNotification);
        search.resizable = false;
        search.content.setOwned(label);
        Rectangle<int> area(0, 0, 500, 200);
        search.content->setSize(area.getWidth(), area.getHeight());
        search.dialogTitle = "Information";
        search.dialogBackgroundColour = Colour(76, 76, 76);
        search.componentToCentreAround;
        search.useNativeTitleBar = false;
        DialogWindow* dw = search.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 200);
    }
    else if (button == &prevButton)
    {

    }
    else if (button == &nextButton)
    {

    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &slider1)
    {
        
    }

    if (slider == &slider2)
    {
        
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

std::string DeckGUI::convertTrackTitle(std::string trackTitle)
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

void setPositionSlider(double pos)
{

}
    

