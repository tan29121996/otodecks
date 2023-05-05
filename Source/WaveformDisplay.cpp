/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

double WaveformDisplay::setPos;

//==============================================================================
WaveformDisplay::WaveformDisplay(DJAudioPlayer* _player,
                                 AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 player(_player),
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);

  Image normal1 = ImageCache::getFromMemory(BinaryData::playhead_png, BinaryData::playhead_pngSize);
  Image over1 = ImageCache::getFromMemory(BinaryData::playheadOver_png, BinaryData::playheadOver_pngSize);
  Image down1 = ImageCache::getFromMemory(BinaryData::playheadOver_png, BinaryData::playheadOver_pngSize);

  tracker.setImages(true, true, true, normal1, 1.0f, {}, over1, 1.0f, {}, down1, 1.0f, {});
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        WaveformDisplay::setPos = position;
        repaint();
    }
}

void WaveformDisplay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    const juce::Rectangle<int> area(4, 4, getWidth() - 8, getHeight() - 8);
    g.fillAll (Colour(40, 40, 40));

    g.setColour(Colour(90, 145, 145));
    g.drawRect(getLocalBounds(), 2);
    g.setColour(Colour(30, 30, 30));
    //g.drawRect(2, 2, getWidth() - 4, getHeight() - 4, 2);

    if(fileLoaded)
    {
        g.setColour(Colour(90, 145, 145));
        g.setOpacity(0.3);
        audioThumb.drawChannel(g, 
        area,
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f);
        
        int playhead = position * getWidth();
        int pos = 1.25 * getHeight();
        double h = 25 * getHeight();
        g.setOpacity(1.0);
        Image deck1 = ImageCache::getFromMemory(BinaryData::playhead_png, BinaryData::playhead_pngSize);
        g.drawImageWithin(deck1, playhead - h / 2, -2 * getHeight(), h, 5 * getHeight(), RectanglePlacement::centred, false);
        g.setColour(Colour(40, 40, 40));
        g.drawRect(0, 2, getWidth(), getHeight() - 4, 2);
        g.setColour(Colour(90, 145, 145));
        g.drawRect(getLocalBounds(), 2);
        /*g.setColour(Colour(90, 145, 145));
        g.drawRect(0, getHeight() / 2 - 1, 4, 2, 2);
        g.drawRect(getWidth() - 4, getHeight() / 2 - 1, 4, 2, 2);*/
    }
    else 
    {
        g.setColour(Colour(100, 40, 40));
        g.drawRect(2, getHeight() / 2, getWidth() - 4, 2, 2);
        g.setColour(Colour(40, 40, 40));
        g.fillRect(getWidth() / 2 - 60, getHeight() / 2 - 20, 120, 40);
        g.setColour(Colour(60, 125, 125));
        g.setFont (21.0f);
        g.drawText ("Waveform", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    WaveformDisplay::setPos = position;
    repaint();
}





