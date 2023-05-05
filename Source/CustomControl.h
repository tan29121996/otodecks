/*
  ==============================================================================

    CustomControl.h
    Created: 5 Mar 2021 11:45:14am
    Author:  The Ender

  ==============================================================================
*/

#pragma once

#include "myLookAndFeel.h"
#include "DJAudioPlayer.h"

//==============================================================================
class CustomControl   : public juce::AudioAppComponent,
                        public Button::Listener,
                        public Slider::Listener
{
public:
    CustomControl(DJAudioPlayer* player1, 
                  DJAudioPlayer* player2);
    ~CustomControl() override;

    /** The paint() method gets called when a region of a component needs redrawing 
    Inputs: graphics context that must be used to do the drawing operations */
    void CustomControl::paint(juce::Graphics& g); 

    /** Updates angleDelta value */
    void updateAngleDelta();

private:
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    void prepareToPlay(int, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Values used for the synthesiser
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0; 

    myLookAndFeelV1 myLookAndFeelV1;
    myLookAndFeelV2 myLookAndFeelV2;
    myLookAndFeelV3 myLookAndFeelV3;
    myLookAndFeelV4 myLookAndFeelV4;

    Slider Knob1;
    Slider Knob2;
    Slider Knob3;
    Slider Knob4;
    ImageButton imgbtn;

    DJAudioPlayer* player1;
    DJAudioPlayer* player2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomControl) 
};