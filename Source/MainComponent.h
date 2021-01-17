#pragma once

#include <JuceHeader.h>


struct Preset {
    juce::String name;
    float v1;
    float v2;
    float v3;
    float v4;
    Preset(juce::String _name, float v_1, float v_2, float v_3, float v_4) {
        name = _name;
        v1 = v_1;
        v2 = v_2;
        v3 = v_3;
        v4 = v_4;
    }
};

class SettingsWindow : public juce::DocumentWindow
{ 
public:
    SettingsWindow(const juce::String name, juce::AudioDeviceSelectorComponent* audioSetupComp)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setContentNonOwned(audioSetupComp, false);
        audioSetupComp->setBounds(0, 0, 400, 200);

        setDraggable(false);
        setBounds(getWidth(), getHeight(), 500, 300);
        setResizable(true, false);
        setUsingNativeTitleBar(true);
    }

    void closeButtonPressed() {
        setVisible(false);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsWindow);
};

class PWComponent : public juce::Component
{
public:
    PWComponent()
    {

    }

    ~PWComponent() {}

    void resized() override
    {

    }

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PWComponent);
};

class PresetWindow : public juce::DocumentWindow
{
public:
    PresetWindow(const juce::String name)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setDraggable(false);
        setBounds(getWidth(), getHeight(), 400, 400);
        setResizable(false, false);
        setUsingNativeTitleBar(true);

        setContentOwned(&pwComponent, false);
    }


    void closeButtonPressed() {
        setVisible(false);
    }

private:
    PWComponent pwComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetWindow);
};

class SPWComponent : public juce::Component
{
public:
    SPWComponent() {
        addAndMakeVisible(txtPresetName);
        addAndMakeVisible(btnSave);
        addAndMakeVisible(btnCancel);
        addAndMakeVisible(lblPresetName);
        lblPresetName.setText("Please Enter Your Preset Name", juce::dontSendNotification);
        lblPresetName.attachToComponent(&txtPresetName, false);

        btnCancel.onClick = [this] {closeWindow(); };
        btnSave.onClick = [this] {savePreset(); };
    }

    ~SPWComponent() {}

    void resized() override
    {
        txtPresetName.setBounds(10, 40, 280, 30);
        btnSave.setBounds(45, 100, 100, 30);
        btnCancel.setBounds(getWidth() / 2 + 5, 100, 100, 30);
    }

private:
    void closeWindow()
    {
        return;
    }
    
    void savePreset()
    {
        return;
    }

    juce::Label lblPresetName;
    juce::TextEditor txtPresetName;
    juce::TextButton btnSave{ "Save Preset" };
    juce::TextButton btnCancel{ "Cancel" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SPWComponent);
};

class SavePresetWindow : public juce::DocumentWindow
{
public:
    SavePresetWindow(const juce::String name)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setDraggable(false);
        setBounds(getWidth(), getHeight(), 300, 150);
        setResizable(false, false);
        setUsingNativeTitleBar(true);

        setContentOwned(&spwComponent, false);
    }

    void closeButtonPressed() {
        setVisible(false);
    }

private:
    SPWComponent spwComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SavePresetWindow);
};

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
    void resetSliders(int sliderNumber);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void initializeFrequencies();
    void updateParameters();

    void savePreset(juce::String name, float v1, float v2, float v3, float v4);
    Preset loadPreset(int index);

    void MainComponent::colourAllComponent();
    //void MainComponent::showSavePresetWindow();

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Array<Component::SafePointer<Component>> windows;

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


    juce::TextButton btnOnOff{ "Sountrol On" };
    juce::TextButton btnSettings{ "Sound Settings" };
    juce::TextButton btnResetAll{ "Reset All" };
    juce::TextButton btnPresets{ "Load Presets" };
    juce::TextButton btnSavePreset{ "Save Preset" };

    //juce::ToggleButton lowCutBtn{ "Cut Unncessary Sounds" };


private:
    float freq1Low, freq1High, freq2Low, freq2High, freq3Low, freq3High,
        freq4Low, freq4High, freq5Low, freq5High, freq6Low;

    using GainProcessor = juce::dsp::Gain<float>;
    using HLFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using CompressorProcessor = juce::dsp::Compressor<float>;

    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand1L;
    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand1R;

    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand2L;
    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand2R;

    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand3L;
    juce::dsp::ProcessorChain<HLFilter, HLFilter, GainProcessor> filterBand3R;

    //juce::dsp::ProcessorChain<HLFilter, HLFilter> filterBand4L;
    //juce::dsp::ProcessorChain<HLFilter, HLFilter> filterBand4R;

    juce::dsp::ProcessorChain<HLFilter, HLFilter, CompressorProcessor> filterBand5L;
    juce::dsp::ProcessorChain<HLFilter, HLFilter, CompressorProcessor> filterBand5R;

    juce::dsp::ProcessorChain<HLFilter> filterBand6L;
    juce::dsp::ProcessorChain<HLFilter> filterBand6R;

    //juce::dsp::ProcessorChain<GainProcessor> filterBand1L;
    //juce::dsp::ProcessorChain<GainProcessor> filterBand1R;


private:
    juce::AudioDeviceSelectorComponent audioSetupComp;

    std::unique_ptr<SettingsWindow> wSettings;
    std::unique_ptr<PresetWindow> wPreset;
    std::unique_ptr<SavePresetWindow> wSavePreset;

     //0:settings, 1:Preset, 2: Save Preset
    void showWindow(int windowType)
    {
        if (windowType == 0)
        {
            if (wSettings == nullptr) wSettings.reset(new SettingsWindow("Sound Settings", &audioSetupComp));
            wSettings->setVisible(true);
        }
        else if (windowType == 1)
        {
            if (wPreset == nullptr) wPreset.reset(new PresetWindow("Presets"));
            wPreset->setVisible(true);
        }
        else if (windowType == 2)
        {
            if (wSavePreset == nullptr) wSavePreset.reset(new SavePresetWindow("Save Preset"));
            wSavePreset->setVisible(true);
        }
    }

private:
    std::vector<Preset> presetVec = {
        {"Typical Use", -2.0f, -30.0f, -2.0f, 0.0f },
        {"Reduce Hiss", -0.0f, -3.0f, -2.0f, -20.0f }
    };


private:
    juce::TextEditor savePresetName;
    juce::TextButton savePresetSaveBtn{ "Save" };
    juce::TextButton savePresetCloseBtn{ "Close" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
