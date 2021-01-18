# Sountrol
## Welcome to Soundtrol!
Sountrol aims to enhance the end-user audio experience.
It is easy-to use and therefore, no Audio knowledge is required.

### Background
Since COVID-19 hit, everything has changed to online: all lectures, job meetings, etc. 
However, not all speakers in those meetings / lectures don't have $1000 mics and awesome vocal techniques to produce the best sound. 
Hence, we decided to create an app that can help the speaker and listener have a better audio experience.

## How to Run
### Running Sountrol as a **Speaker** (Windows)
1. Download Sountrol Executable from the [Release Page](https://github.com/Andrewjjj/Sountrol/releases/tag/0.1)
1. You need to download and install virtual cable. One that works: https://vb-audio.com/Cable/
2. Open up Sountrol and navigate to 'Sound Settings'.
3. For **audio input**, select your **microphone**.
4. For **audio output**, select your **virtual cable input**.
5. In the platform you are speaking in, make sure that the sound input is coming out of your **virtual cable output**.

### Running Sountrol as a **Listener** (Windows)
1. You need to download and install virtual cable. One that works: https://vb-audio.com/Cable/
2. Open up the sound setting in your windows.
3. Set your **virtual cable input** as a default output.
4. Open up Sountrol and navigate to 'Sound Settings'.
5. For **audio input**, select your **virtual cable output**.
6. For **audio output**, select your **[default]** **audio output**.


## How to use
It enhances the audio experience by the listener and / or speaker. We discovered that the audio discomforts can be categorized into the following:
1. **Basiness / Boominess**. These types have a lot of 'Bass' in the Audio. This happens frequency due to the speaker's natural voice more than the mic qualities. The sound becomes 'muffled' that it is often hard to hear what they are saying.
2. **Boxiness**. These are the MOST COMMON audio discomforts. It happens a lot especially with a labtop built-in mics. It is also caused mostly due to a poor acoustic treatment of the room. You can hear a lot of this 'boxiness' in places like ETLC lecture rooms.
3. **Sharpness**. These are common when the volume becomes loud. This is mostly common with labtop built-in mics as well. Long exposure to this 'Sharpness' can hurt your ears a lot.
4. **Hissness**. These are the uncomfortable sound that we can hear when someone says an 'sssss' sound.

## How we built it
Since we needed to have this running real-time and performance was crucial, we could not use languages like Python. 
Hence, we built it with c++ with a JUCE Framework.
Then, we processed the Audio Buffer with our algorithm so that these uncomfortable sounds can be reduced / eliminiated.

## What's next for Sountrol
We want to integrate WASAPI (Window Audio Session API) for windows and also integrate machine learning into this app so that it would automatically process the audio to the user's needs.
