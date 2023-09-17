/*
  ==============================================================================

    DeckGUI.cpp
    Created: 7 Sep 2023 6:51:54pm
    Author:  cleme

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include <thread>

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse,
                bool mirrorLayout
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse, mirrorLayout),
               mirror(mirrorLayout)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    
       
    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    


    //VolSlider
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabelNumber);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.2);
    volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    volSlider.setColour(Slider::thumbColourId, mirror ? Colour::fromRGB(0, 110, 173) : Colour::fromRGB(201, 148, 0));
    volSlider.setColour(Slider::trackColourId, Colour::fromRGB(14, 17, 18));
    volSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(14, 17, 18));
    volLabelNumber.setText(String("VOL"), dontSendNotification);

    //PosSlider
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0, 0.001);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    posSlider.setColour(Slider::thumbColourId, mirror ? Colour::fromRGB(0, 110, 173) : Colour::fromRGB(201, 148, 0));
    posSlider.setColour(Slider::rotarySliderOutlineColourId,mirror ? Colour::fromRGB(33, 65, 84) : Colour::fromRGB(102, 89, 52));
    posSlider.setColour(Slider::rotarySliderFillColourId, mirror ? Colour::fromRGB(17, 97, 143) : Colour::fromRGB(163, 124, 11));
    

    


    //SpeedSlider
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabelNumber);
    speedSlider.addListener(this);
    speedSlider.setRange(0, 100, 1);
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setRange(0, 100.0, 0.1);
    speedSlider.setValue(50);
    speedSlider.setColour(Slider::thumbColourId, mirror ? Colour::fromRGB(0, 110, 173) : Colour::fromRGB(201, 148, 0));
    speedSlider.setColour(Slider::trackColourId, Colour::fromRGB(14, 17, 18));
    speedSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(14, 17, 18));
    speedLabelNumber.setText(String("BPM"), dontSendNotification);
    

    startTimer(50);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
  

    g.fillAll(Colour::fromRGB(31, 36, 43));
    

    g.setColour(Colours::white);
    g.setFont(14.0f);

    
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 14;

    //Right layout
    if (mirror)
    {
        //VolSlider
        volSlider.setBounds(getWidth() - 30, getHeight() / 1.9, 20, getHeight() / 3);
        volLabelNumber.setBounds(getWidth() - 37, 12 + getHeight() / 3 + getHeight() / 1.9 - 20, 40, 20);

        //SpeedSlider
        speedSlider.setBounds(getWidth() - 30, getHeight() / 5, 20, getHeight() / 3);
        speedLabelNumber.setBounds(getWidth() - 37, getHeight() / 5 - 20, 40, 20);

        //StopButton
        stopButton.setBounds(getWidth() - 25 - ((getWidth() / 9) * 3), getHeight() / 1.1, getWidth() / 9, rowH);

        //PlayButton
        playButton.setBounds(getWidth() - 15 - (getWidth() / 4.5), getHeight() / 1.1, getWidth() / 9, rowH);

        //LoadButton
        loadButton.setBounds(getWidth() - 10 - (getWidth()/9), getHeight() / 1.1, getWidth() / 9, rowH);
  
    }
    //Left layout
    else
    {
        //VolSlider
        volSlider.setBounds(10, getHeight() / 1.9 , 20, getHeight() / 3);
        volLabelNumber.setBounds(5, 12 + getHeight() / 3 + getHeight() / 1.9 - 20, 40, 20);

        //SpeedSlider
        speedSlider.setBounds(10, getHeight() / 5, 20, getHeight() / 3);
        speedLabelNumber.setBounds(5, getHeight() / 5 - 20, 40, 20);

        //StopButton
        stopButton.setBounds(10, getHeight() / 1.1, getWidth() / 9, rowH);

        //PlayButton
        playButton.setBounds(15 + (getWidth() / 9), getHeight() / 1.1, getWidth() / 9, rowH);

        //LoadButton
        loadButton.setBounds(20 + (getWidth() / 4.5), getHeight() / 1.1, getWidth() / 9, rowH);
    }

    //This stay the same in both layouts
    //PosSlider
    posSlider.setBounds(getWidth() / 3, getHeight() / 4, getWidth() / 3, getWidth() / 3);

    

    //WaveformDisplay
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 1.3);

    

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                player->loadURL(URL{ chooser.getResult() });
                // and now the waveformDisplay as well
                waveformDisplay.loadURL(URL{ chooser.getResult() });
            });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        double internalValue = speedSlider.getValue();  // This will be between 0 and 100
        double displayedValue = (internalValue * 2) - 100;  // This will be between -100 and 100


        if (internalValue < 50) {
			internalValue = 0.5 + (internalValue / 50) * 0.5;
		}
        else
        {
			internalValue = 1.0 + ((internalValue - 50) / 50) * 1.0;
		}
        player->setSpeed(internalValue);
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    URL audioURL = URL{File{files[0]}};
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL); 
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());

    if (player && player->isMediaLoaded())
    {
        double currentPosition = player->getPositionRelative();
        posSlider.setValue(currentPosition, dontSendNotification);
    }

}
void DeckGUI::loadSong(const File& file) {
    player->loadURL(URL{ file });
    waveformDisplay.loadURL(URL{ file });
}

    

