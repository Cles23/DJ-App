/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Sep 2023 6:51:54pm
    Author:  cleme

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : deckGUI1(_deckGUI1),
    deckGUI2(_deckGUI2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    tableComponent.getHeader().addColumn("ID", 1, 50);
    tableComponent.getHeader().addColumn("SONG TITLE", 2, 500);
    tableComponent.getHeader().addColumn("ARTIST", 3, 250);
    tableComponent.getHeader().addColumn("LENGTH", 4, 200);

    tableComponent.setModel(this);
    tableComponent.addMouseListener(this, true);
    tableComponent.setColour(ListBox::ColourIds::backgroundColourId, Colour::fromRGB(31, 36, 43));

    addAndMakeVisible(tableComponent);
    
    addAndMakeVisible(addSongButton);
    addAndMakeVisible(loadSongButton);
    addAndMakeVisible(loadSongButton2);
    addAndMakeVisible(deleteSong);

    addSongButton.addListener(this);
    loadSongButton.addListener(this);
    loadSongButton2.addListener(this);
    deleteSong.addListener(this);
    

    playlist = File::getSpecialLocation(File::userDesktopDirectory).getChildFile("playlist.xml");


}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll(Colour::fromRGB(31, 36, 43));

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    if (songs.empty())
    {
        g.drawText("Add tracks to start playing", getWidth() / 2 - 100, getHeight() * 8 / 10, getWidth() / 2, getHeight() / 10,
            juce::Justification::left, true);
    }
    else {
        g.drawText("Load them to a deck to start playing", getWidth() / 2 - 100, getHeight() * 8 / 10, getWidth() / 2, getHeight() / 10,
            juce::Justification::left, true);
    }
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 30, getWidth(), getHeight());

    addSongButton.setBounds(0, 2, 30, 25);
    loadSongButton.setBounds(30, 2, 90, 25);
    loadSongButton2.setBounds(120, 2, 90, 25);
    deleteSong.setBounds(getWidth() - 100, 2, 100, 25);


}

int PlaylistComponent::getNumRows()
{
	return songs.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
		g.fillAll(juce::Colours::orange);
	}
    else
    {
		g.fillAll(juce::Colours::darkgrey);
	}

}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    //add columns and rows according to the lenght and content of the tracks vector containing the files
    auto& track = songs[rowNumber];

    if (columnId == 1) // ID column
    {
		g.drawText(String(rowNumber + 1),
            			2, 0,
            			width - 4, height,
            			juce::Justification::centredLeft,
            			true);
	}
	else

    if (columnId == 2) // Track title column
    {
        g.drawText(track.songtitle,
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
    else if (columnId == 3) // artist column
    {
        g.drawText(track.artist,
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
    else if (columnId == 4) // Duration column
    {
        g.drawText(formatDuration(track.length),
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }

}


Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                    int columnId,
                    bool isRowSelected,
                    Component *existingComponentToUpdate)
{
  
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &addSongButton)
    {

        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();

                // Check if file is already in playlist
                bool duplicateFound = false;
                for (auto& track : songs)
                {
                    if (chosenFile.getFileName().toStdString() == track.songtitle)
                    {
                        duplicateFound = true;
                        break;
                    }
                }

                if (duplicateFound)
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                        "Duplicate song",
                        "The selected song is already in the playlist.");
                }
                else
                {
                    // Create a new TrackInfo object for the selected file
                    SongInfo newTrack;
                    newTrack.songtitle = chosenFile.getFileName().toStdString();
                    // Set the file path to the chosen file
                    newTrack.fileLocation = chosenFile;

                    // Get the file duration and BPM
                    juce::AudioFormatManager formatManager;
                    formatManager.registerBasicFormats();
                    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(chosenFile));
                    if (reader.get() != nullptr)
                    {
                        newTrack.length = reader->lengthInSamples / static_cast<double>(reader->sampleRate);
                        juce::String bpmString = reader->metadataValues.getValue("BPM", "");

                        juce::String artistString = reader->metadataValues.getValue("ARTIST", "");
                        if (!artistString.isEmpty())
                            newTrack.artist = artistString.toStdString();
                    }

                    // Add the new track to the tracks vector
                    songs.push_back(newTrack);

                    // Update the table
                    tableComponent.updateContent();
                    repaint(); // Add this to update the UI

                }
            });
    }
    if(button == &loadSongButton)
    {
        DBG("leftDeck button clicked");
        // Load the selected track to the left deck
        if (songFilePath.existsAsFile())
        {
            // Use the selectedFilePath variable to load the track to the left deck
            deckGUI1->player->loadURL(juce::URL{ songFilePath });
            deckGUI1->waveformDisplay.loadURL(juce::URL{ songFilePath });
        }
    }
    else if (button == &loadSongButton2)
    {
        DBG("rightDeck button clicked");

        // Load the selected track to the right deck
        if (songFilePath.existsAsFile())
        {
            // Use the selectedFilePath variable to load the track to the right deck
            deckGUI2->player->loadURL(juce::URL{ songFilePath });
            deckGUI2->waveformDisplay.loadURL(juce::URL{ songFilePath });
        }
    }

    // Handle the deleteSong click
    if (button == &deleteSong)
    {

        // Remove the selected track from the tracks vector
        int selectedRow = tableComponent.getSelectedRow();
        DBG("Selected row: " << selectedRow); // Debug statement

        if (selectedRow >= 0 && selectedRow < songs.size())
        {
            songs.erase(songs.begin() + selectedRow);
            tableComponent.updateContent();
            tableComponent.repaint();

        }
    }
}


String PlaylistComponent::formatDuration(double seconds)
{
    int minutes = static_cast<int>(seconds) / 60;
    int remainingSeconds = static_cast<int>(seconds) % 60;

    return juce::String::formatted("%02d:%02d", minutes, remainingSeconds);
}

void PlaylistComponent::mouseDown(const juce::MouseEvent& event)
{
    // Get the selected row number from the tableComponent
    const int selectedRow = tableComponent.getSelectedRow();

    // Check if a row is actually selected
    if (selectedRow >= 0)
    {
        // Retrieve the file path of the selected track
        juce::String filePathString = songs[selectedRow].fileLocation.getFullPathName();

        // Create a juce::File object from the file path string
        songFilePath = File::createFileWithoutCheckingPath(filePathString);
    }
}