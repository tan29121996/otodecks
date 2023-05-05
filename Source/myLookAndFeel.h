/*
  ==============================================================================

    myLookAndFeel.h
    Created: 6 Mar 2021 5:35:07pm
    Author:  The Ender

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class myLookAndFeelV1 : public LookAndFeel_V4
{
public:
    myLookAndFeelV1();

    /** Draws a rotary slider with custom graphics 
    Inputs: graphics file for the rotary slider, 
    x and y coordinates of the rotary slider,
    width and height of the rotary slider, 
    position of the slider for the rotary slider, 
    start angle of the rotary slider, end angle of the rotary slider,
    Slider object
    */
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img1;
};

class myLookAndFeelV2 : public LookAndFeel_V4
{
public:
    myLookAndFeelV2();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img2;
};

class myLookAndFeelV3 : public LookAndFeel_V4
{
public:
    myLookAndFeelV3();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img3;
};

class myLookAndFeelV4 : public LookAndFeel_V4
{
public:
    myLookAndFeelV4();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img4;
};

class myLookAndFeelV5 : public LookAndFeel_V4
{
public:
    myLookAndFeelV5();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img5;
};

class myLookAndFeelV6 : public LookAndFeel_V4
{
public:
    myLookAndFeelV6();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img6;
};