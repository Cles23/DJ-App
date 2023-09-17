/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Sep 2023 6:51:54pm
    Author:  cleme

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"


//==============================================================================
//I will store the song info in a struct and then store the struct in a vector
struct SongInfo
{
    String songtitle;
    String artist;

    double length;
    
    File fileLocation;
};

class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener
     
{
public:
    PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground (juce::Graphics& , 
                        int rowNumber, 
                        int width, 
                        int height, 
                        bool rowIsSelected) override;

    void paintCell (juce::Graphics&, 
					    int rowNumber, 
					    int columnId, 
					    int width, 
					    int height, 
					    bool rowIsSelected) override;
    
    Component* refreshComponentForCell (int rowNumber, 
        										int columnId, 
        										bool isRowSelected, 
        										Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    void savePlayList(const File& file);
    void loadSongToPlaylist(const File& file);

    void mouseDown(const MouseEvent& event);


    String formatDuration(double seconds);

    File playlist; // Will be used to store the playlist file

private:

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    TextButton addSongButton{"+"};
    TextButton loadSongButton{"LEFT DECK"};
    TextButton loadSongButton2{"RIGHT DECK"};
    TextButton deleteSong{"DELETE TRACK"};

    FileChooser fChooser{"Select a file..."};

    std::vector<SongInfo> songs;

    File songFilePath;
    File loadedSong;

    
    TableListBox tableComponent; 
    std::vector<std::string> trackLength;
    std::vector<std::string> trackTitles;

    TextEditor searchBar;

    bool isFiltered = false;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
