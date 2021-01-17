#pragma once

#include <JuceHeader.h>
#include <functional>

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

    Preset(juce::String _name, double v_1, double v_2, double v_3, double v_4) {
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

class LPWComponent : public juce::Component
{
public:
    LPWComponent(std::vector<Preset>* vec, std::function<void(float, float, float, float)> callback)
    {
        btnPreset1.setRadioGroupId(1,juce::dontSendNotification);
        btnPreset2.setRadioGroupId(1, juce::dontSendNotification);
        btnPreset3.setRadioGroupId(1, juce::dontSendNotification);
        btnPreset4.setRadioGroupId(1, juce::dontSendNotification);
        btnPreset5.setRadioGroupId(1, juce::dontSendNotification);

        addAndMakeVisible(btnPreset1);
        addAndMakeVisible(btnPreset2);
        addAndMakeVisible(btnPreset3);
        addAndMakeVisible(btnPreset4);
        addAndMakeVisible(btnPreset5);

        btnPreset1.setClickingTogglesState(true);
        btnPreset2.setClickingTogglesState(true);
        btnPreset3.setClickingTogglesState(true);
        btnPreset4.setClickingTogglesState(true);
        btnPreset5.setClickingTogglesState(true);

        btnPreset1.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
        btnPreset1.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        btnPreset2.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
        btnPreset2.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        btnPreset3.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
        btnPreset3.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        btnPreset4.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
        btnPreset4.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        btnPreset5.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
        btnPreset5.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);

        btnPreset1.onClick = [this] {loadPreset(); };
        btnPreset2.onClick = [this] {loadPreset(); };
        btnPreset3.onClick = [this] {loadPreset(); };
        btnPreset4.onClick = [this] {loadPreset(); };
        btnPreset5.onClick = [this] {loadPreset(); };
    }

    ~LPWComponent() {}

    void resized() override
    {
        btnPreset1.setBounds(0, 0, 300, 50);
        btnPreset2.setBounds(0, 60, 300, 50);
        btnPreset3.setBounds(0, 120, 300, 50);
        btnPreset4.setBounds(0, 180, 300, 50);
        btnPreset5.setBounds(0, 240, 300, 50);
    }
    

private:
    std::vector<Preset>* vecPtr;

    juce::Label lblPreset{ {}, "Please Choose Your Preset" };
    juce::TextButton btnPreset1{ vecPtr->at(0).name },
        btnPreset2{ vecPtr->at(1).name },
        btnPreset3{ vecPtr->at(2).name },
        btnPreset4{ vecPtr->at(3).name },
        btnPreset5{ vecPtr->at(4).name };

    void loadPreset()
    {
        return;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LPWComponent);
};

class LoadPresetWindow : public juce::DocumentWindow
{
public:
    LoadPresetWindow(const juce::String name, std::vector<Preset>* vec, std::function<void(float, float, float, float)> callback)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons), presetVec(vec), lpwComponent(vec, callback)
    {
        setDraggable(false);
        setBounds(getWidth(), getHeight(), 300, 300);
        setResizable(false, false);
        setUsingNativeTitleBar(true);
        setContentOwned(&lpwComponent, false);
    }

    void closeButtonPressed() {
        setVisible(false);
    }

private:
    std::vector<Preset> *presetVec;
    LPWComponent lpwComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoadPresetWindow);
};

class SPWComponent : public juce::Component
{
public:
    SPWComponent(std::vector<Preset>* vec, juce::Slider* slider1, juce::Slider* slider2, juce::Slider* slider3, juce::Slider* slider4)
        : vecPtr(vec), slider1(slider1), slider2(slider2), slider3(slider3), slider4(slider4)
    {
        addAndMakeVisible(txtPresetName);
        addAndMakeVisible(btnSave);
        addAndMakeVisible(btnCancel);
        addAndMakeVisible(lblPresetName);
        lblPresetName.setText("Please Enter Your Preset Name", juce::dontSendNotification);
        lblPresetName.attachToComponent(&txtPresetName, false);

        btnCancel.onClick = [this] {closeWindow(); };
        btnSave.onClick = [this] {
            savePreset(); 
        };
    }

    ~SPWComponent() {}

    void resized() override
    {
        txtPresetName.setBounds(10, 40, 280, 30);
        btnSave.setBounds(45, 100, 100, 30);
        btnCancel.setBounds(getWidth() / 2 + 5, 100, 100, 30);
    }

    void closeWindow()
    {
        return;
    }

    void savePreset()
    {
        Preset ps(txtPresetName.getText(), slider1->getValue(), slider2->getValue(), slider3->getValue(), slider4->getValue());
        if (vecPtr->size() < 5) {
            vecPtr->push_back(ps);
        }
        return;
    }

private:
    std::vector<Preset>* vecPtr;

    juce::Slider* slider1;
    juce::Slider* slider2;
    juce::Slider* slider3;
    juce::Slider* slider4;
    
    juce::Label lblPresetName;
    juce::TextEditor txtPresetName;
    juce::TextButton btnSave{ "Save Preset" };
    juce::TextButton btnCancel{ "Cancel" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SPWComponent);
};

class SavePresetWindow : public juce::DocumentWindow
{

public:
    SavePresetWindow(const juce::String name, std::vector<Preset> *vec, juce::Slider* slider1, juce::Slider* slider2, juce::Slider* slider3, juce::Slider* slider4)
        : DocumentWindow(name,
            juce::Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(juce::ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons), spwComponent(vec, slider1, slider2, slider3, slider4)
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
    void loadPreset(float v1, float v2, float v3, float v4);

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
    std::unique_ptr<LoadPresetWindow> wLoadPreset;
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
            if (wLoadPreset == nullptr) wLoadPreset.reset(new LoadPresetWindow("Presets", &presetVec, loadPreset()));
            wLoadPreset->setVisible(true);
        }
        else if (windowType == 2)
        {
            if (wSavePreset == nullptr) wSavePreset.reset(new SavePresetWindow("Save Preset", &presetVec, &slider1, &slider2, &slider3, &slider4));
            wSavePreset->setVisible(true);
        }
    }

private:
    std::vector<Preset> presetVec = {
        {"Typical Use", -2.0f, -30.0f, -2.0f, 0.0f },
        {"Reduce Hiss", -0.0f, -3.0f, -2.0f, -20.0f },
        {"Boxy", 0.0f, 5.0f, 0.0f, 0.0f},
        {"MAX", 5.0f, 5.0f, 5.0f, 5.0f},
        {"MIN", -30.0f, -30.0f, -30.0f, -30.0f}
    };


private:
    juce::TextEditor savePresetName;
    juce::TextButton savePresetSaveBtn{ "Save" };
    juce::TextButton savePresetCloseBtn{ "Close" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
