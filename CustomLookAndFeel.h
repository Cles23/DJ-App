/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 17 Sep 2023 8:21:31pm
    Author:  cleme

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomLookAndFeel : public LookAndFeel_V4
{
public:
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle, Slider& slider) override;
};
