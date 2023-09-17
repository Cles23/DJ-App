/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 7 Sep 2023 6:51:54pm
    Author:  cleme

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse, bool mirrorLayout) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0),
            		             mirror(mirrorLayout)     
                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{

    g.fillAll(Colour::fromRGB(44, 46, 53));   // clear the background

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


    if (mirror) 
    {
        g.setColour(Colour::fromRGB(0, 110, 173));
    }
    else
    {
        g.setColour(Colour::fromRGB(201, 148, 0));
		
	}
    
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.setColour(Colours::grey);
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
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

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }

  
}




