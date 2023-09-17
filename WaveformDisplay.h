/*
  ==============================================================================

    WaveformDisplay.h
    Created: 7 Sep 2023 6:51:54pm
    Author:  cleme

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse, bool mirrorLayout = false);
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    
    bool mirror;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
