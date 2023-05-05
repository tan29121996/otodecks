/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1400, 900);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2); 
    addAndMakeVisible(customControl);

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    Image bg = ImageCache::getFromMemory(BinaryData::dark_png, BinaryData::dark_pngSize);
    g.drawImageWithin(bg, 0, 0, getWidth(), getHeight() / 2, RectanglePlacement::fillDestination, false);

    /*double sizeY = getHeight() / 2;
    double midWidth = sizeY / 1.15;
    double sideWidth = (getWidth() - sizeY / 1.5) / 2;
    Image nex = ImageCache::getFromMemory(BinaryData::nex_png, BinaryData::nex_pngSize);
    g.drawImageWithin(nex, getWidth() - midWidth - sideWidth, getHeight() / 20.9, midWidth, sizeY, RectanglePlacement::centred, false);*/
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double sizeY = getHeight() / 2;
    double midWidth = sizeY / 1.5;
    double sideWidth = (getWidth() - midWidth) / 2;
    deckGUI1.setBounds(0, 0, sideWidth, sizeY);
    deckGUI2.setBounds(sideWidth + midWidth, 0, sideWidth, sizeY);
    customControl.setBounds(sideWidth, 0, midWidth, sizeY);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

