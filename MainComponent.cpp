/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 540);

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

    addAndMakeVisible(playlistComponent);


    formatManager.registerBasicFormats();

    //Crossfade Slider
    crossFadeSlider.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(crossFadeSlider);
    crossFadeSlider.setRange(0.0, 100.0, 1);
    crossFadeSlider.setValue(50.0);
    crossFadeSlider.addListener(this);
    crossFadeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    crossFadeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crossFadeSlider.setColour(Slider::thumbColourId, Colours::grey);
    crossFadeSlider.setColour(Slider::trackColourId, Colour::fromRGB(14, 17, 18));
    crossFadeSlider.setColour(Slider::backgroundColourId, Colour::fromRGB(14, 17, 18));
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    crossFadeSlider.setLookAndFeel(nullptr);
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
    //g.fillAll(Colour::fromRGB(44, 46, 53));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 1.5);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()  /2, getHeight() / 1.5);

    crossFadeSlider.setBounds(getWidth() / 2.4, getHeight() / 1.7, getWidth() / 6, 30);

    playlistComponent.setBounds(0, getHeight() / 1.5, getWidth(), getHeight() / 3);

}

void MainComponent::sliderValueChanged(juce::Slider* slider) {

    if (slider == &crossFadeSlider)
    {
        // Get the current volume levels from each deck
        float deck1Volume = deckGUI1.getCurrentVolume();
        float deck2Volume = deckGUI2.getCurrentVolume();

        // Get and normalize the crossfade slider value
        float sliderValue = static_cast<float>(crossFadeSlider.getValue());
        float sliderNormalized = sliderValue / 100.0f;

        // Calculate the gains for each deck based on the crossfader
        float leftGain = (1.0f - sliderNormalized) * deck1Volume;
        float rightGain = sliderNormalized * deck2Volume;

        // Apply the gains to each deck
        deckGUI1.player->setGain(leftGain);
        deckGUI2.player->setGain(rightGain);
    }
}

