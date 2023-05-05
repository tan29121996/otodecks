/*
  ==============================================================================

    CustomControl.cpp
    Created: 5 Mar 2021 11:45:14am
    Author:  The Ender

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomControl.h"

CustomControl::CustomControl(DJAudioPlayer* _player1,
                             DJAudioPlayer* _player2
                            ): player1(_player1),
                               player2(_player2)
{    
    addAndMakeVisible(Knob1);
    Knob1.setLookAndFeel(&myLookAndFeelV1);
    Knob1.setSliderStyle(Slider::Rotary);
    Knob1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(Knob2);
    Knob2.setLookAndFeel(&myLookAndFeelV2);
    Knob2.setSliderStyle(Slider::Rotary);
    Knob2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(Knob3);
    Knob3.setLookAndFeel(&myLookAndFeelV3);
    Knob3.setSliderStyle(Slider::Rotary);
    Knob3.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(Knob4);
    Knob4.setLookAndFeel(&myLookAndFeelV4);
    Knob4.setSliderStyle(Slider::Rotary);
    Knob4.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(imgbtn);
    Image normal = ImageCache::getFromMemory(BinaryData::icon_png, BinaryData::icon_pngSize);
    Image over = ImageCache::getFromMemory(BinaryData::iconOver_png, BinaryData::iconOver_pngSize);
    Image down = ImageCache::getFromMemory(BinaryData::icon_png, BinaryData::icon_pngSize);
    imgbtn.setImages(true, true, true, normal, 1.0f, {}, over, 1.0f, {}, down, 1.0f, {});
    imgbtn.addListener(this);

    Knob1.setRange(0.0, 5000.0);
    Knob1.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
        {
            updateAngleDelta(); // update angleDelta every time Knob is moved
        }
    };

    Knob2.setRange(0.0, 5000.0);
    Knob2.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
        {
            updateAngleDelta(); // update angleDelta every time Knob is moved
        }
    };

    Knob3.setRange(0.0, 2.0);
    Knob3.setValue(1.0);
    Knob3.addListener(this);

    Knob4.setRange(0.0, 2.0);
    Knob4.setValue(1.0);
    Knob4.addListener(this);

    setAudioChannels(0, 2);
}

CustomControl::~CustomControl()
{
    shutdownAudio();
}

void CustomControl::paint(juce::Graphics& g)
{
    g.setFont(Font(Typeface::createSystemTypefaceFor(BinaryData::Septacharge_ttf, BinaryData::Septacharge_ttfSize)));
    Image deckbg = ImageCache::getFromMemory(BinaryData::deck_png, BinaryData::deck_pngSize);

    double posW = getWidth() / 20;
    double posH = getHeight() / 30;
    double rectH = getHeight() / 8;

    g.fillAll(Colours::aliceblue);

    //==============================================================================
    // Background for custom control
    Image bg = ImageCache::getFromMemory(BinaryData::dark_png, BinaryData::dark_pngSize);
    g.setOpacity(0.95);
    g.drawImageWithin(bg, 0, 0, getWidth(), getHeight(), RectanglePlacement::fillDestination, false);
    g.setColour(Colour(100, 100, 100));
    g.setOpacity(0.2);
    //g.fillRect(0, rectH, getWidth(), 6 * rectH);

    //==============================================================================
    // Bottom rectangle
    g.setColour(Colour(210, 70, 70));
    g.setOpacity(0.9);
    g.fillRoundedRectangle(posH / 2, getHeight() - (rectH - posH / 2), getWidth() - posH, rectH - posH, 5);
    g.setOpacity(0.2);
    g.drawImageWithin(deckbg, posH / 2, getHeight() - (rectH - posH / 2), getWidth() - posH, rectH - posH, RectanglePlacement::stretchToFit, false);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(posH / 2, getHeight() - (rectH - posH / 2), getWidth() - posH, rectH - posH, 5, 2);
    g.setColour(Colour(80, 80, 80));
    g.drawRoundedRectangle(posH / 2 - 1, getHeight() - (rectH - posH / 2) - 1, getWidth() - posH + 2, rectH - posH + 2, 5, 2);
    g.drawRoundedRectangle(0, -10, getWidth(), getHeight() + 10, 10, 1);
    g.setColour(Colour(130, 130, 130));
    g.setOpacity(0.8);
    g.drawRect(posH / 2, 7 * rectH, getWidth() - posH, 2, 2);

    //==============================================================================
    // Top rectangle
    g.setColour(Colours::cornflowerblue);
    g.setOpacity(0.9);
    g.fillRoundedRectangle(posH / 2, posH / 2, getWidth() - posH, rectH - posH, 5);
    g.setOpacity(0.2);
    g.drawImageWithin(deckbg, posH / 2, posH / 2, getWidth() - posH, rectH - posH, RectanglePlacement::stretchToFit, false);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(posH / 2, posH / 2, getWidth() - posH, rectH - posH, 5, 2);
    g.setColour(Colour(80, 80, 80));
    g.drawRoundedRectangle(posH / 2 - 1, posH / 2 - 1, getWidth() - posH + 2, rectH - posH + 2, 5, 2);
    g.setColour(Colour(150, 150, 150));
    g.setOpacity(0.8);
    g.drawRect(posH / 2, rectH, getWidth() - posH, 2, 2);

    //==============================================================================
    // Background for DECK text
    double size = rectH - posH;
    double banner = getWidth() - 2 * posH;
    g.setColour(Colour(40, 40, 40));

    //g.fillEllipse(getWidth() / 2 - 4.5 * posW, getHeight() / 2 - 4.5 * posH, 9 * posW, 9 * posW);
    g.fillRoundedRectangle(posH, getHeight() / 2 - size / 2, banner / 2, size, 5);
    g.fillRoundedRectangle(posH, getHeight() / 2 - size / 2, getWidth() - 2 * posH, size, 5);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(posH, getHeight() / 2 - size / 2, getWidth() - 2 * posH, size, 5, 2);
    g.setColour(Colour(80, 80, 80));
    g.drawRoundedRectangle(posH - 1, getHeight() / 2 - size / 2 - 1, getWidth() - 2 * posH + 2, size + 2, 7, 2);

    //==============================================================================

    g.setFont(21.0f);
    g.setColour(Colour(55, 175, 175));
    g.setOpacity(0.85);
    g.drawText("DECK 1", 1.5 * posW, getHeight() / 2 - posH / 2, posW * 4, posH,
                Justification::centred, true);
    g.drawText("DECK 2", getWidth() - 1.5 * posW - 4 * posH, getHeight() / 2 - posH / 2, posW * 4, posH,
                Justification::centred, true);
    Image deck1 = ImageCache::getFromMemory(BinaryData::deck1W_png, BinaryData::deck1W_pngSize);
    Image deck2 = ImageCache::getFromMemory(BinaryData::deck2W_png, BinaryData::deck2W_pngSize);
    Image deckTop = ImageCache::getFromMemory(BinaryData::deckTop_png, BinaryData::deckTop_pngSize);
    Image deckDown = ImageCache::getFromMemory(BinaryData::deckDown_png, BinaryData::deckDown_pngSize);
    double dist = getWidth() / 2 - 4 * posW - 2 * posH - size / 2;

    g.drawImageWithin(deck1, getWidth() / 2 - dist - 1.75 * posH, getHeight() / 2 - 0.875 * posH, 1.75 * posH, 1.75 * posH, RectanglePlacement::fillDestination, false);
    g.drawImageWithin(deck2, getWidth() / 2 + dist, getHeight() / 2 - 0.875 * posH, 1.75 * posH, 1.75 * posH, RectanglePlacement::fillDestination, false);
    g.drawImageWithin(deckTop, getWidth() / 2 - 0.875 * posH, getHeight() / 2 - dist - 1.75 * posH, 1.75 * posH, 1.75 * posH, RectanglePlacement::fillDestination, false);
    g.drawImageWithin(deckDown, getWidth() / 2 - 0.875 * posH, getHeight() / 2 + dist, 1.75 * posH, 1.75 * posH, RectanglePlacement::fillDestination, false);
    
    double posNew = posW / 1.45;
    g.setColour(Colour(55, 175, 175));
    g.fillEllipse(getWidth() / 2 - 3.5 * posNew, getHeight() / 2 - 3.5 * posNew, 7 * posNew, 7 * posNew);
    g.setColour(Colour(180, 180, 180));
    g.fillEllipse(getWidth() / 2 - 3 * posNew, getHeight() / 2 - 3 * posNew, 6 * posNew, 6 * posNew);
    g.setColour(Colour(150, 150, 150));
    g.fillEllipse(getWidth() / 2 - 2.5 * posNew, getHeight() / 2 - 2.5 * posNew, 5 * posNew, 5 * posNew);
    g.setColour(Colour(120, 120, 120));
    g.fillEllipse(getWidth() / 2 - 2 * posNew, getHeight() / 2 - 2 * posNew, 4 * posNew, 4 * posNew);
    g.setColour(Colour(90, 90, 90));
    g.fillEllipse(getWidth() / 2 - 1.5 * posNew, getHeight() / 2 - 1.5 * posNew, 3 * posNew, 3 * posNew);
    g.setColour(Colour(60, 60, 60));
    g.fillEllipse(getWidth() / 2 - posNew, getHeight() / 2 - posNew, 2 * posW, 2 * posW);
    g.setColour(Colour(30, 30, 30));
    g.fillEllipse(getWidth() / 2 - posNew / 2, getHeight() / 2 - posNew / 2, posNew, posNew);

    g.setColour(Colour(55, 175, 175));
    g.setOpacity(0.6);
    g.fillEllipse(getWidth() / 2 - 3 * posNew, getHeight() / 2 - 3 * posNew, 6 * posNew, 6 * posNew);

    g.setColour(Colour(80, 80, 80));
    g.drawEllipse(getWidth() / 2 - 3.5 * posNew, getHeight() / 2 - 3.5 * posNew, 7 * posNew, 7 * posNew, 2);
    g.setColour(Colour(50, 50, 50));
    //g.drawEllipse(getWidth() / 2 - 3.5 * posNew - 2, getHeight() / 2 - 3.5 * posNew - 2, 7 * posNew + 4, 7 * posNew + 4, 2);

    //==============================================================================

    g.setFont(Font(Typeface::createSystemTypefaceFor(BinaryData::Neon_ttf, BinaryData::Neon_ttfSize)));
    g.setFont(21.0f);
    g.setColour(Colours::white);
    g.drawText("SYNTHESISER", 0, 0, getWidth(), rectH,
                Justification::centred, true);
    g.drawText("SPEED", 0, getHeight() - rectH, getWidth(), rectH,
                Justification::centred, true);

    g.setColour(Colour(50, 50, 50));
    //g.drawRoundedRectangle(0, - 10, getWidth(), getHeight() + 10, 8, 3);
}

void CustomControl::resized()
{
    double posW = getWidth() / 20;
    double posH = getHeight() / 30;
    double knobW = posW * 8;
    double knobH = posH * 7;
    Knob1.setBounds(posW * 1.5, posH * 5, knobW, knobH);
    Knob2.setBounds(getWidth() - posW * 1.5 - knobW, posH * 5, knobW, knobH);
    Knob3.setBounds(posW * 1.5, getHeight() - posH * 5 - knobH, knobW, knobH);
    Knob4.setBounds(getWidth() - posW * 1.5 - knobW, getHeight() - posH * 5 - knobH, knobW, knobH);
    imgbtn.setBounds(getWidth() / 2 - posW * 2.25, getHeight() / 2 - posH * 2.25, posW * 4.5, posH * 4.5);
}

void CustomControl::buttonClicked(Button* button)
{
    // If program icon has been clicked
    if (button == &imgbtn)
    {
        // Launch a dialog window displaying information about the program
        DialogWindow::LaunchOptions about;
        Label* line = new Label();
        line->setColour(Label::textColourId, Colours::white);
        line->setText("Developed by Tan Si Yuan.",
                       dontSendNotification);
        line->setJustificationType(juce::Justification::centred);
        about.content.setOwned(line);
        Rectangle<int> area(0, 0, 500, 300);
        about.content->setSize(area.getWidth(), area.getHeight());
        about.dialogTitle = "About";
        about.dialogBackgroundColour = Colour(76, 76, 76);
        about.componentToCentreAround;
        about.useNativeTitleBar = false;
        about.resizable = false;
        DialogWindow* dw = about.launchAsync();
        dw->setVisible(true);
        dw->centreWithSize(500, 300);
    }
}

void CustomControl::prepareToPlay (int, double sampleRate)
{
    currentSampleRate = sampleRate; // Store a copy of the sample rate value 

    updateAngleDelta(); // Update the value of angleDelta
}

void CustomControl::releaseResources() 
{

}

void CustomControl::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // For each output sample, calculate the sine function for the current angle and increment the angle for the next sample
    auto level = 0.125f;
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        leftBuffer[sample] = currentSample * level;
        rightBuffer[sample] = currentSample * level;
    }
}

void CustomControl::updateAngleDelta() // Update the value of angleDelta
{
    if (Knob1.getValue() > 0 && Knob2.getValue() == 0) // If Knob 1 has its value changed but Knob 2 does not
    {
        // Calculate the number of cycles that need to completed for each output sample in proportion to the value of Knob 1
        auto cyclesPerSample = Knob1.getValue() / currentSampleRate; 

        // Multiply that by the length of a whole sine wave cycle, which is 2pi radians
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi; 
    }
    else if (Knob2.getValue() > 0 && Knob1.getValue() == 0) // If Knob 2 has its value changed but Knob 1 does not
    {
        // Calculate the number of cycles that need to completed for each output sample in proportion to the value of Knob 2
        auto cyclesPerSample = Knob2.getValue() / currentSampleRate;

        // Multiply that by the length of a whole sine wave cycle, which is 2pi radians
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    else if (Knob1.getValue() > 0 && Knob2.getValue() > 0) // If both Knob 1 and Knob 2 has their value changed
    {
        // Calculate the number of cycles that need to completed for each output sample in proportion to the value of Knob 1
        auto cycles1 = Knob1.getValue() / currentSampleRate;

        // Calculate the number of cycles that need to completed for each output sample in proportion to the value of Knob 2
        auto cycles2 = Knob2.getValue() / currentSampleRate;

        // Multiply the sum of both cycles by the length of a whole sine wave cycle, which is 2pi radians
        angleDelta = (cycles1 + cycles2) * 2.0 * juce::MathConstants<double>::pi;
    }
}

void CustomControl::sliderValueChanged(Slider* slider)
{
    if (slider == &Knob3) // If slider value is equal to Knob 3
    {
        player1->setSpeed(slider->getValue()); // adjust playback speed for Deck 1
    }
    
    if (slider == &Knob4) // If slider value is equal to Knob 4
    {
        player2->setSpeed(slider->getValue()); // adjust playback speed for Deck 2
    }
}