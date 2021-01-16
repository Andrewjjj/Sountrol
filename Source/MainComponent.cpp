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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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
