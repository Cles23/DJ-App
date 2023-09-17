/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 17 Sep 2023 8:21:31pm
    Author:  cleme

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle, Slider& slider)
{

    g.setColour(Colour::fromRGB(14, 17, 18));  // or whatever color you want

    int trackHeight = 2; // Change this to make the track thinner or thicker
    int trackYPos = y + height / 2 - trackHeight / 2;

    g.fillRect(x, y + height / 2 - 2, width, 4);  // Adjust these numbers to change thickness

    g.setColour(slider.findColour(Slider::thumbColourId));     // set thumb color

    int thumbWidth = 10;  // you can customize this
    int thumbPos = static_cast<int> (sliderPos) - thumbWidth / 2;

    g.fillRect(thumbPos, y, thumbWidth, height);  // draw thumb
}

