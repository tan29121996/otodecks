/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "myLookAndFeel.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager & 	formatManagerToUse,
            AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

private:
    void paint(Graphics&) override;

    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;

    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

    std::string convertTrackTitle(std::string trackTitle);

    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> audioSourceReader;
    ScopedPointer<AudioFormatReader> audioFormatReader;

    bool playing = true;

    ImageButton playButton;
    ImageButton stopButton;
    ImageButton loadButton;
    ImageButton loopButton;
    
    ImageButton loadPlaylist;

    ImageButton info;
    ImageButton prevButton;
    ImageButton nextButton;
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    Slider slider1;
    Slider slider2;

    WaveformDisplay waveformDisplay;

    myLookAndFeelV5 myLookAndFeelV5;
    myLookAndFeelV6 myLookAndFeelV6;

    DJAudioPlayer* player; 

    String trackTitle = " ";
    String duration = " ";

    bool loop = false;

    class CustomFontLookAndFeel : public LookAndFeel_V4
    {
    public:
        CustomFontLookAndFeel()
        {
            // without this custom Fonts won't work!!
            LookAndFeel::setDefaultLookAndFeel(this);

            // This can be used as one way of setting a default font
            //setDefaultSansSerifTypeface (getCustomFont().getTypeface());
        }

        static const Font getCustomFont()
        {
            static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::Neon_ttf, BinaryData::Neon_ttfSize);
            return Font(typeface);
        }

        Typeface::Ptr getTypefaceForFont(const Font& f) override
        {
            // This can be used to fully change/inject fonts.
            // For example: return different TTF/OTF based on weight of juce::Font (bold/italic/etc)
            return getCustomFont().getTypeface();
        }

        /*Button* createDocumentWindowButton(int 	buttonType)
        {
            if (buttonType)
            {
                ImageButton* btn = new ImageButton();
                Image normal = ImageCache::getFromMemory(BinaryData::iconG_png, BinaryData::iconG_pngSize);
                File icon = File::getSpecialLocation(File::userMusicDirectory).getChildFile("icon.png");
                Image ver = ImageFileFormat::loadFrom(icon);
                Image over = ImageCache::getFromMemory(BinaryData::icon_png, BinaryData::icon_pngSize);
                btn->setImages(true, true, true, ver, 1.0f, {}, over, 1.0f, {}, over, 0.5f, {});
                return btn;
            }
        }*/

    private:


    } 
    customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};




