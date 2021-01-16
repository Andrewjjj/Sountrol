#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void updateOnOffState(juce::Button* button);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
<<<<<<< HEAD
    juce::Slider slider1;
    juce::Slider slider2;
    juce::Slider slider3;
    juce::Slider slider4;
    juce::Label lblSlider1;
    juce::Label lblSlider2;
    juce::Label lblSlider3;
    juce::Label lblSlider4;

    juce::TextButton btnResetSlider1{ "Reset" };
    juce::TextButton btnResetSlider2{ "Reset" };
    juce::TextButton btnResetSlider3{ "Reset" };
    juce::TextButton btnResetSlider4{ "Reset" };
    
    juce::ToggleButton btnBypassSlider1{ "Bypass" };
    juce::ToggleButton btnBypassSlider2{ "Bypass" };
    juce::ToggleButton btnBypassSlider3{ "Bypass" };
    juce::ToggleButton btnBypassSlider4{ "Bypass" };
=======
    float freq1Low, freq1High, freq2Low, freq2High, freq3Low, freq3High,
        freq4Low, freq4High, freq5Low, freq5High, freq6Low;

    using GainProcessor = juce::dsp::Gain<float>;
    //
    juce::dsp::ProcessorChain<GainProcessor> filterBand1L;
    juce::dsp::ProcessorChain<GainProcessor> filterBand1R;
>>>>>>> b1663f845679e869379c9c56c86559dfc4fc5391

    juce::TextButton btnOnOff{ "Sountrol On" };
    juce::TextButton btnSettings{ "Sound Settings" };
    juce::TextButton btnResetAll{ "Reset All" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
