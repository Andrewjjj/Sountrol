#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : audioSetupComp(deviceManager, 0, 256, 0, 256, false, false, false, false)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (450, 600);

    
    // Sliders
    addAndMakeVisible(slider1);
    slider1.setRange(-30.0f, 5.0f, 0.1f);
    slider1.setValue(0.0f);
    slider1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider1.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    addAndMakeVisible(lblSlider1);
    lblSlider1.setText("Bassiness", juce::dontSendNotification);
    lblSlider1.attachToComponent(&slider1, false);

    addAndMakeVisible(slider2);
    slider2.setRange(-30.0f, 5.0f, 0.1f);
    slider2.setValue(0.0f);
    slider2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider2.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider2);
    lblSlider2.setText("Boxiness", juce::dontSendNotification);
    lblSlider2.attachToComponent(&slider2, false);

    addAndMakeVisible(slider3);
    slider3.setRange(-30.0f, 5.0f, 0.1f);
    slider3.setValue(0.0f);
    slider3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider3.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider3);
    lblSlider3.setText("Sharpness", juce::dontSendNotification);
    lblSlider3.attachToComponent(&slider3, false);

    addAndMakeVisible(slider4);
    slider4.setRange(-30.0f, 5.0f, 0.1f);
    slider4.setValue(0.0f);
    slider4.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider4.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider4);
    lblSlider4.setText("Hissssiness", juce::dontSendNotification);
    lblSlider4.attachToComponent(&slider4, false);


    // Reset Buttons
    addAndMakeVisible(btnResetSlider1);
    addAndMakeVisible(btnResetSlider2);
    addAndMakeVisible(btnResetSlider3);
    addAndMakeVisible(btnResetSlider4);

    btnResetSlider1.onClick = [this] {resetSliders(1); };
    btnResetSlider2.onClick = [this] {resetSliders(2); };
    btnResetSlider3.onClick = [this] {resetSliders(3); };
    btnResetSlider4.onClick = [this] {resetSliders(4); };


    // Bypass Buttons
    addAndMakeVisible(btnBypassSlider1);
    addAndMakeVisible(btnBypassSlider2);
    addAndMakeVisible(btnBypassSlider3);
    addAndMakeVisible(btnBypassSlider4);

    // Global Buttons
    addAndMakeVisible(btnOnOff);
    btnOnOff.setColour(0x1000100, juce::Colours::forestgreen);
    btnOnOff.setColour(0x1000101, juce::Colours::indianred);
    //btnOnOff.setSize()
    btnOnOff;
    btnOnOff.setClickingTogglesState(true);
    btnOnOff.onClick = [this] {
        updateOnOffState(&btnOnOff);
    };

    addAndMakeVisible(btnResetAll);
    btnResetAll.onClick = [this] {resetSliders(0); };

    // windows
    addAndMakeVisible(btnSettings);
    btnSettings.onClick = [this] { showWindow(0); };

    addAndMakeVisible(btnPresets);
    btnPresets.onClick = [this] {showWindow(1); };

    addAndMakeVisible(btnSavePreset);
    btnSavePreset.onClick = [this] {showWindow(2); };


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    initializeFrequencies();
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    filterBand1L.reset();
    auto& lowFilter1L = filterBand1L.get<0>();
    auto& highFilter1L = filterBand1L.get<1>();
    auto& gain1L = filterBand1L.get<2>();
    lowFilter1L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq1Low);
    highFilter1L.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq1High);
    gain1L.setGainDecibels(0.0f);
    filterBand1L.prepare(spec);

    filterBand1R.reset();
    auto& lowFilter1R = filterBand1R.get<0>();
    auto& highFilter1R = filterBand1R.get<1>();
    auto& gain1R = filterBand1R.get<2>();
    lowFilter1R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq1Low);
    highFilter1R.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq1High);
    gain1R.setGainDecibels(0.0f);
    filterBand1R.prepare(spec);


    filterBand2L.reset();
    auto& lowFilter2L = filterBand2L.get<0>();
    auto& highFilter2L = filterBand2L.get<1>();
    auto& gain2L = filterBand2L.get<2>();
    lowFilter2L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq2Low);
    highFilter2L.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq2High);
    gain2L.setGainDecibels(0.0f);
    filterBand2L.prepare(spec);

    filterBand2R.reset();
    auto& lowFilter2R = filterBand2R.get<0>();
    auto& highFilter2R = filterBand2R.get<1>();
    auto& gain2R = filterBand2R.get<2>();
    lowFilter2R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq2Low);
    highFilter2R.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq2High);
    gain2R.setGainDecibels(0.0f);
    filterBand2R.prepare(spec);


    filterBand3L.reset();
    auto& lowFilter3L = filterBand3L.get<0>();
    auto& highFilter3L = filterBand3L.get<1>();
    auto& gain3L = filterBand3L.get<2>();
    lowFilter3L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq3Low);
    highFilter3L.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq3High);
    gain3L.setGainDecibels(0.0f);
    filterBand3L.prepare(spec);

    filterBand3R.reset();
    auto& lowFilter3R = filterBand3R.get<0>();
    auto& highFilter3R = filterBand3R.get<1>();
    auto& gain3R = filterBand3R.get<2>();
    lowFilter3R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq3Low);
    highFilter3R.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq3High);
    gain3R.setGainDecibels(0.0f);
    filterBand3R.prepare(spec);


    //filterBand4L.reset();
    //auto& lowFilter4L = filterBand4L.get<0>();
    //auto& highFilter4L = filterBand4L.get<1>();
    //lowFilter4L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq4Low);
    //highFilter4L.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq4High);
    //filterBand4L.prepare(spec);

    //filterBand4R.reset();
    //auto& lowFilter4R = filterBand4R.get<0>();
    //auto& highFilter4R = filterBand4R.get<1>();
    //lowFilter4R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq4Low);
    //highFilter4R.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq4High);
    //filterBand4R.prepare(spec);


    filterBand5L.reset();
    auto& lowFilter5L = filterBand5L.get<0>();
    auto& highFilter5L = filterBand5L.get<1>();
    auto& compressor5L = filterBand5L.get<2>();
    lowFilter5L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq5Low);
    highFilter5L.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq5High);
    compressor5L.setThreshold(0.0f);
    compressor5L.setRatio(10);
    filterBand5L.prepare(spec);

    filterBand5R.reset();
    auto& lowFilter5R = filterBand5R.get<0>();
    auto& highFilter5R = filterBand5R.get<1>();
    auto& compressor5R = filterBand5R.get<2>();
    lowFilter5R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq5Low);
    highFilter5R.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq5High);
    compressor5R.setThreshold(0.0f);
    compressor5R.setRatio(10);
    filterBand5R.prepare(spec);


    filterBand6L.reset();
    auto& lowFilter6L = filterBand6L.get<0>();
    lowFilter6L.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq6Low);
    filterBand6L.prepare(spec);

    filterBand6R.reset();
    auto& lowFilter6R = filterBand6R.get<0>();
    lowFilter6R.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq6Low);
    filterBand6R.prepare(spec);


    //lowCutBtn.onClick = [this, sampleRate] {
    //    auto state = lowCutBtn.getToggleState();
    //    DBG(std::to_string(state == true));
    //    freq1Low = (state == true) ? 0.0f : 100.0f;
    //    auto& lowFilter1LFreq = filterBand1L.get<0>().state;
    //    lowFilter1LFreq = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq1Low);
    //    auto& lowFilter1RFreq = filterBand1R.get<0>().state;
    //    lowFilter1RFreq = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq1Low);
    //};
}

void MainComponent::initializeFrequencies() {
    freq1Low = 100.0f;
    freq1High = 400.0f;
    freq2Low = 400.0f;
    freq2High = 1000.0f;
    freq3Low = 1000.0f;
    freq3High = 4000.0f;
    freq4Low = 1500.0f;
    freq4High = 4000.0f;
    freq5Low = 4000.0f;
    freq5High = 10000.0f;
    freq6Low = 10000.0f;
}

void MainComponent::updateParameters() {
    auto newValue1 = slider1.getValue();
    auto newValue2 = slider2.getValue();
    auto newValue3 = slider3.getValue();
    auto newValue4 = slider4.getValue();

    // bypass methods
    if (btnOnOff.getToggleState()) {
        newValue1 = 0;
        newValue2 = 0;
        newValue3 = 0;
        newValue4 = 0;
    }
    else 
    {
        if (btnBypassSlider1.getToggleState()) newValue1 = 0;
        if (btnBypassSlider2.getToggleState()) newValue2 = 0;
        if (btnBypassSlider3.getToggleState()) newValue3 = 0;
        if (btnBypassSlider4.getToggleState()) newValue4 = 0;
    }

    auto& gain1L = filterBand1L.get<2>();
    auto& gain1R = filterBand1R.get<2>();

    gain1L.setGainDecibels(newValue1/3);
    gain1R.setGainDecibels(newValue1/3);

    auto& gain2L = filterBand2L.get<2>();
    auto& gain2R = filterBand2R.get<2>();

    gain2L.setGainDecibels(newValue2);
    gain2R.setGainDecibels(newValue2);

    auto& gain3L = filterBand3L.get<2>();
    auto& gain3R = filterBand3R.get<2>();

    gain3L.setGainDecibels(newValue3);
    gain3R.setGainDecibels(newValue3);

    auto& gain4L = filterBand5L.get<2>();
    auto& gain4R = filterBand5R.get<2>();

    gain4L.setThreshold(newValue4*2);
    gain4R.setThreshold(newValue4*2);
}

void MainComponent::savePreset(juce::String name, float v1, float v2, float v3, float v4) {
    Preset p(name, v1, v2, v3, v4);
    presetVec.push_back(p);
}

Preset MainComponent::loadPreset(int index) {
    return presetVec.at(index);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    updateParameters();
    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();

    auto maxInputChannels = activeInputChannels.getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

    auto* buffer = bufferToFill.buffer;

    juce::AudioBuffer<float> buffer1;
    buffer1.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block1(buffer1);

    juce::AudioBuffer<float> buffer2;
    buffer2.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block2(buffer2);

    juce::AudioBuffer<float> buffer3;
    buffer3.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block3(buffer3);

    //juce::AudioBuffer<float> buffer4;
    //buffer4.makeCopyOf(*buffer);
    //juce::dsp::AudioBlock<float> block4(buffer4);

    juce::AudioBuffer<float> buffer5;
    buffer5.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block5(buffer5);

    juce::AudioBuffer<float> buffer6;
    buffer6.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block6(buffer6);

    for (auto channel = 0; channel < maxOutputChannels; ++channel)
    {
        if ((!activeOutputChannels[channel]) || maxInputChannels == 0)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
        }
        else
        {
            auto actualInputChannel = channel % maxInputChannels;

            if (!activeInputChannels[channel])
            {
                bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else
            {
                auto* inBuffer = bufferToFill.buffer->getReadPointer(actualInputChannel, bufferToFill.startSample);
                auto* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

                buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
                
                juce::dsp::ProcessContextReplacing<float>pc1(block1.getSingleChannelBlock(channel));
                juce::dsp::ProcessContextReplacing<float>pc2(block2.getSingleChannelBlock(channel));
                juce::dsp::ProcessContextReplacing<float>pc3(block3.getSingleChannelBlock(channel));
                //juce::dsp::ProcessContextReplacing<float>pc4(block4.getSingleChannelBlock(channel));
                juce::dsp::ProcessContextReplacing<float>pc5(block5.getSingleChannelBlock(channel));
                juce::dsp::ProcessContextReplacing<float>pc6(block6.getSingleChannelBlock(channel));

                if (channel == 0) {
                    filterBand1L.process(pc1);
                    filterBand2L.process(pc2);
                    filterBand3L.process(pc3);
                    //filterBand4L.process(pc4);
                    filterBand5L.process(pc5);
                    filterBand6L.process(pc6);
                }
                else {
                    filterBand1R.process(pc1);
                    filterBand2R.process(pc2);
                    filterBand3R.process(pc3);
                    //filterBand4R.process(pc4);
                    filterBand5R.process(pc5);
                    filterBand6R.process(pc6);
                }
                buffer->addFrom(channel, 0, buffer1, channel, 0, bufferToFill.numSamples, 1.0f);
                buffer->addFrom(channel, 0, buffer2, channel, 0, bufferToFill.numSamples, 1.0f);
                buffer->addFrom(channel, 0, buffer3, channel, 0, bufferToFill.numSamples, 1.0f);
                //buffer->addFrom(channel, 0, buffer4, channel, 0, bufferToFill.numSamples, 1.0f);
                buffer->addFrom(channel, 0, buffer5, channel, 0, bufferToFill.numSamples, 1.0f);
                buffer->addFrom(channel, 0, buffer6, channel, 0, bufferToFill.numSamples, 1.0f);

                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
                    outBuffer[sample] = inBuffer[sample];
                }
            }
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}


//==============================================================================
void MainComponent::updateOnOffState(juce::Button* button) {
    auto state = button->getToggleState();
    juce::String selectedString = state ? "Sountrol Off" : "Sountrol On";
    button->setButtonText(selectedString);
}

void MainComponent::resetSliders(int sliderNumber) {
    if (sliderNumber == 0) {
        slider1.setValue(0.0);
        slider2.setValue(0.0);
        slider3.setValue(0.0);
        slider4.setValue(0.0);
    }
    else if (sliderNumber == 1) slider1.setValue(0.0);
    else if (sliderNumber == 2) slider2.setValue(0.0);
    else if (sliderNumber == 3) slider3.setValue(0.0);
    else if (sliderNumber == 4) slider4.setValue(0.0);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    slider1.setBounds(getWidth() / 2 - 190, getHeight() / 2 - 100, 70, 250);
    slider2.setBounds(getWidth() / 2 - 90, getHeight() / 2 - 100, 70, 250);
    slider3.setBounds(getWidth() / 2 + 10, getHeight() / 2 - 100, 70, 250);
    slider4.setBounds(getWidth() / 2 + 110, getHeight() / 2 - 100, 70, 250);

    btnResetSlider1.setBounds(getWidth() / 2 - 185, getHeight() / 2 - 155, 60, 30);
    btnResetSlider2.setBounds(getWidth() / 2 - 85, getHeight() / 2 - 155, 60, 30);
    btnResetSlider3.setBounds(getWidth() / 2 + 15, getHeight() / 2 - 155, 60, 30);
    btnResetSlider4.setBounds(getWidth() / 2 + 115, getHeight() / 2 - 155, 60, 30);

    btnBypassSlider1.setBounds(getWidth() / 2 - 200, getHeight() / 2 + 150, 80, 40);
    btnBypassSlider2.setBounds(getWidth() / 2 - 100, getHeight() / 2 + 150, 80, 40);
    btnBypassSlider3.setBounds(getWidth() / 2, getHeight() / 2 + 150, 80, 40);
    btnBypassSlider4.setBounds(getWidth() / 2 + 100, getHeight() / 2 + 150, 80, 40);

    //lowCutBtn.setBounds(getWidth() / 2 - 250, getHeight() / 2 + 100, 80, 20);

    btnOnOff.setBounds(getWidth() / 2 - 200, 20, 400, 100);
    btnSettings.setBounds(20, getHeight() - 100, 200, 50);
    btnResetAll.setBounds(getWidth() / 2, getHeight() - 100, 200, 50);
    btnPresets.setBounds(getWidth() / 2, getHeight() - 50, 200, 50);
    btnSavePreset.setBounds(20, getHeight() - 50, 200, 50);
}
