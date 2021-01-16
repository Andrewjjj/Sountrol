#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);


    // Sliders
    addAndMakeVisible(slider1);
    slider1.setRange(-30.0f, 5.0f, 0.1f);
    slider1.setValue(0.0f);
    slider1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider1.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    addAndMakeVisible(lblSlider1);
    lblSlider1.setText("Slider1", juce::dontSendNotification);
    lblSlider1.attachToComponent(&slider1, false);

    addAndMakeVisible(slider2);
    slider2.setRange(-30.0f, 5.0f, 0.1f);
    slider2.setValue(0.0f);
    slider2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider2.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider2);
    lblSlider2.setText("Slider2", juce::dontSendNotification);
    lblSlider2.attachToComponent(&slider2, false);

    addAndMakeVisible(slider3);
    slider3.setRange(-30.0f, 5.0f, 0.1f);
    slider3.setValue(0.0f);
    slider3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider3.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider3);
    lblSlider3.setText("Slider3", juce::dontSendNotification);
    lblSlider3.attachToComponent(&slider3, false);

    addAndMakeVisible(slider4);
    slider4.setRange(-300.0f, 5.0f, 0.1f);
    slider4.setValue(0.0f);
    slider4.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider4.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    addAndMakeVisible(lblSlider4);
    lblSlider4.setText("Slider4", juce::dontSendNotification);
    lblSlider4.attachToComponent(&slider4, false);


    // Reset Buttons
    addAndMakeVisible(btnResetSlider1);
    addAndMakeVisible(btnResetSlider2);
    addAndMakeVisible(btnResetSlider3);
    addAndMakeVisible(btnResetSlider4);

    // Bypass Buttons
    addAndMakeVisible(btnBypassSlider1);
    addAndMakeVisible(btnBypassSlider2);
    addAndMakeVisible(btnBypassSlider3);
    addAndMakeVisible(btnBypassSlider4);

    // Global Buttons
    addAndMakeVisible(btnOnOff);
    addAndMakeVisible(btnSettings);
    addAndMakeVisible(btnResetAll);
    btnOnOff.setClickingTogglesState(true);
    btnOnOff.onClick = [this] {updateOnOffState(&btnOnOff); };

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
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    filterBand1L.reset();
    auto& gain1L = filterBand1L.get<0>();
    gain1L.setGainDecibels(-20.0f);
    filterBand1L.prepare(spec);

    filterBand1R.reset();
    auto& gain1R = filterBand1R.get<0>();
    gain1R.setGainDecibels(-20.0f);
    filterBand1R.prepare(spec);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    auto activeOutputChannels = device->getActiveOutputChannels();

    auto maxInputChannels = activeInputChannels.getHighestBit() + 1;
    auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

    auto* buffer = bufferToFill.buffer;

    juce::AudioBuffer<float> buffer1;
    buffer1.makeCopyOf(*buffer);
    juce::dsp::AudioBlock<float> block1(buffer1);

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
                if (channel == 0) {
                    filterBand1L.process(pc1);
                }
                else {
                    filterBand1R.process(pc1);
                }
                buffer->addFrom(channel, 0, buffer1, channel, 0, bufferToFill.numSamples, 1);

                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
                    outBuffer[sample] = inBuffer[sample];
                    //DBG(inBuffer[sample]);
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
    slider1.setBounds(getWidth() / 2 - 200, getHeight() / 2 - 100, 45, 150);
    slider2.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 100, 45, 150);
    slider3.setBounds(getWidth() / 2, getHeight() / 2 - 100, 45, 150);
    slider4.setBounds(getWidth() / 2 + 100, getHeight() / 2 - 100, 45, 150);

    btnResetSlider1.setBounds(getWidth() / 2 - 200, getHeight() / 2 - 150, 40, 20);
    btnResetSlider2.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 150, 40, 20);
    btnResetSlider3.setBounds(getWidth() / 2, getHeight() / 2 - 150, 40, 20);
    btnResetSlider4.setBounds(getWidth() / 2 + 100, getHeight() / 2 - 150, 40, 20);

    btnBypassSlider1.setBounds(getWidth() / 2 - 210, getHeight() / 2 + 60, 80, 20);
    btnBypassSlider2.setBounds(getWidth() / 2 - 110, getHeight() / 2 + 60, 80, 20);
    btnBypassSlider3.setBounds(getWidth() / 2 - 10, getHeight() / 2 + 60, 80, 20);
    btnBypassSlider4.setBounds(getWidth() / 2 + 90, getHeight() / 2 + 60, 80, 20);

    btnOnOff.setBounds(getWidth() / 2 - 200, 20, 400, 100);
    btnSettings.setBounds(20, getHeight() - 100, 200, 50);
    btnResetAll.setBounds(getWidth() / 2 - 20, getHeight() - 100, 200, 50);
}
