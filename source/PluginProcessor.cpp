#include "PluginProcessor.h"
#include "PluginEditor.h"

/**
 * @brief Represents the core processing component of an audio plugin.
 *
 * This class or object is responsible for processing audio and possibly MIDI
 * data within the plugin. It typically includes methods for managing audio
 * processing blocks, handling parameter changes, and ensuring seamless
 * integration with the host application or digital audio workstation (DAW).
 *
 * @return The plugin processor responsible for managing audio and MIDI processing tasks.
 */
PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
#if !JucePlugin_IsMidiEffect
    #if !JucePlugin_IsSynth
              .withInput ("Input", juce::AudioChannelSet::stereo(), true)
    #endif
              .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
}

/**
 * @brief Processes an audio buffer by applying specific audio transformations or effects.
 *
 * This method manipulates the provided audio buffer, which contains a sequence
 * of floating-point samples. The specific transformations or operations applied may
 * include filtering, amplification, mixing, or other audio processing techniques.
 * The function operates directly on the input buffer, modifying it in place.
 *
 * @param buffer A pointer to the audio buffer containing floating-point sample data to be processed.
 * @param bufferSize The number of samples in the buffer to process.
 */
PluginProcessor::~PluginProcessor()
= default;

/**
 * @brief Retrieves the name associated with the object or entity.
 *
 * This method returns a string representing the name, which can be used for identification
 * or display purposes in various contexts.
 *
 * @return A string containing the name of the object or entity.
 */
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

/**
 * @brief Indicates whether the object or application is configured to accept MIDI input.
 *
 * This function or property typically determines if the current system, application,
 * or object is capable of receiving and processing MIDI (Musical Instrument Digital Interface) data.
 * MIDI input may include signals such as notes, control changes, or other musical events
 * used in digital audio and MIDI workflows.
 *
 * @return True if MIDI input is accepted, false otherwise.
 */
bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

/**
 * @brief Indicates whether the object or application is configured to produce MIDI output.
 *
 * This function or property determines if the current system, application,
 * or object is capable of generating and sending MIDI (Musical Instrument Digital Interface) data.
 * MIDI output may include signals such as notes, control changes, or other musical events
 * relevant to digital audio and MIDI workflows.
 *
 * @return True if MIDI output is produced, false otherwise.
 */
bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

/**
 * @brief Determines if the object or system functions specifically as a MIDI effect.
 *
 * This function indicates whether the current object is designed to process MIDI input data
 * without directly generating or outputting audio. A MIDI effect typically manipulates MIDI
 * events, such as transforming, filtering, or routing MIDI signals, in a non-audio-specific context.
 *
 * @return True if the object operates as a MIDI effect, false otherwise.
 */
bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

/**
 * @brief Retrieves the tail length of the audio processing in seconds.
 *
 * This method provides the duration of the audio tail remaining after the
 * input signal has stopped, commonly used to account for reverb, delay,
 * or other time-based effects that persist after playback has ended.
 *
 * @return The duration of the tail in seconds as a double-precision value.
 */
double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

/**
 * @brief Retrieves the total number of available programs in the plugin.
 *
 * This method provides the count of preset or program slots that the plugin
 * offers, allowing hosts or users to query and iterate through them. Programs
 * typically represent pre-defined settings or configurations within the plugin.
 *
 * @return The number of programs available in the plugin.
 */
int PluginProcessor::getNumPrograms()
{
    return 1;
}

/**
 * @brief Retrieves the index of the currently active program.
 *
 * This method returns the index of the program that is currently selected
 * in the plugin. Programs typically represent presets or specific
 * configurations of parameters that can be saved and recalled.
 *
 * @return An integer representing the index of the currently active program.
 */
int PluginProcessor::getCurrentProgram()
{
    return 0;
}

/**
 * @brief Sets the current program for the plugin.
 *
 * This method allows changing the active program or preset in the plugin.
 * It typically updates the plugin's internal state to correspond to the selected program,
 * ensuring that all parameters reflect the values associated with that program.
 *
 * @param index The index of the program to set as the current program.
 */
void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

/**
 * @brief Retrieves the name of the current program or preset.
 *
 * This method returns the name associated with the current program or preset
 * in the plugin. Program names are typically used to manage and identify
 * specific configurations or states within the plugin.
 *
 * @param index The index of the program or preset for which the name is requested.
 * @return The name of the program or preset as a string.
 */
const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

/**
 * @brief Modifies the name of the current program or preset.
 *
 * This method allows renaming a specific program or preset within the plugin.
 * It is typically used to provide a custom name for better identification
 * and organization of presets by the user.
 *
 * @param index The index of the program or preset to be renamed.
 * @param newName The new name to assign to the specified program or preset.
 */
void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

/**
 * @brief Prepares the audio processing environment for playback or rendering.
 *
 * This method is called before audio playback begins or when the audio
 * settings change. It is typically used to allocate resources, initialize
 * internal states, or configure parameters required for processing.
 *
 * @param sampleRate The sample rate in Hz at which the audio processing will operate.
 * @param samplesPerBlock The maximum number of audio samples that will be processed in a single block.
 */
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    oscillator.prepare (sampleRate);
    oscillator.setFrequency (frequency);
    oscillator.setGain (gain);
}

/**
 * @brief Releases any resources allocated during the lifecycle of the application or system.
 *
 * This method is called to free up any resources, such as memory, file handles, or
 * hardware resources, that were allocated and are no longer needed. It ensures
 * proper cleanup and prevents resource leaks.
 *
 * @return Void.
 */
void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

/**
 * @brief Checks if the given buses layout is supported by the audio processor.
 *
 * This method determines whether the specified configuration of input and output
 * audio buses is compatible with the processor. It ensures that the audio
 * processor can handle the provided layout and operate correctly with the
 * requested channel configurations.
 *
 * @param layouts The proposed layout of audio input and output buses.
 *                This may include the number and arrangement of channels
 *                for each bus.
 * @return True if the given buses layout is supported, false otherwise.
 */
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

/**
 * @brief Processes a block of audio and MIDI data.
 *
 * This method is invoked to process a block of audio samples and optionally handle MIDI events.
 * It ensures the audio signal is processed in real-time, applying effects, synthesis, or
 * any other processing as required by the plugin's functionality.
 *
 * @param buffer A reference to the audio buffer containing the input samples to be processed and
 *               where the output samples will be stored.
 * @param midiMessages A reference to a structure containing any incoming MIDI messages that
 *                     need to be processed during this block.
 */
void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    buffer.clear();

    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        float outputSample = 0.0f;

        switch (waveform)
        {
            case Waveform::sine:
                outputSample = oscillator.getNextSineSample();
                break;

            case Waveform::saw:
                outputSample = oscillator.getNextSawSample();
                break;
        }

        for (int channel = 0; channel < numChannels; ++channel)
            buffer.setSample (channel, sampleIndex, outputSample);
    }
}

/**
 * @brief Sets the frequency value for the processing component.
 *
 * This method updates the frequency parameter, which is typically used
 * in audio processing tasks such as oscillation, filtering, or modulation.
 * The provided frequency value should be within the acceptable range of the
 * processing system to ensure accurate and stable operation.
 *
 * @param newFrequency The frequency value to set, expressed in hertz (Hz).
 */
void PluginProcessor::setFrequency (float newFrequency)
{
    frequency = juce::jmax (0.0f, newFrequency);
    oscillator.setFrequency (frequency);
}

/**
 * @brief Sets the gain level for audio processing.
 *
 * This method adjusts the gain level applied to the audio signal. It can be
 * used to control the amplitude of the audio output or to implement volume changes.
 * The value provided as an argument typically affects the strength or loudness
 * of the signal, either increasing or decreasing it as required.
 *
 * @param newGain The new gain level to be set, usually represented as a floating-point
 *             value where the range depends on the specific implementation
 *             (e.g., 0.0 for silence or 1.0 for unity gain).
 */
void PluginProcessor::setGain (float newGain)
{
    gain = juce::jlimit (0.0f, 1.0f, newGain);
    oscillator.setGain (gain);
}

/**
 * @brief Sets the waveform type for audio processing.
 *
 * This method specifies the waveform type to be used in the audio signal generation
 * or manipulation process. It adjusts the behavior of the audio processing component
 * according to the selected waveform.
 *
 * @param newWaveform A string or identifier representing the desired waveform type (e.g., "sine", "square", "triangle").
 */
void PluginProcessor::setWaveform (Waveform newWaveform)
{
    waveform = newWaveform;
}

/**
 * @brief Retrieves the frequency value associated with a specific operation or parameter.
 *
 * This method is typically used to obtain the frequency value for purposes such
 * as audio processing, signal analysis, or generating sound waves. The frequency
 * value may be in hertz (Hz) or another relevant unit, depending on the context.
 *
 * @return The frequency value as a numeric representation, typically in hertz (Hz).
 */
float PluginProcessor::getFrequency() const
{
    return frequency;
}

/**
 * @brief Retrieves the current gain value.
 *
 * This method returns the gain level currently applied, which is typically used
 * to control the amplitude or volume of an audio signal. It may be used in audio
 * processing workflows to adjust or monitor the signal's output level.
 *
 * @return The current gain value as a floating-point number.
 */
float PluginProcessor::getGain() const
{
    return gain;
}

/**
 * @brief Retrieves the waveform representation of the audio signal.
 *
 * This method provides access to the waveform data, which typically represents
 * the amplitude variations of an audio signal over time. It can be used for
 * visualizations or further audio processing tasks.
 *
 * @return The waveform data associated with the audio signal.
 */
PluginProcessor::Waveform PluginProcessor::getWaveform() const
{
    return waveform;
}

/**
 * @brief Indicates whether the plugin includes a user interface editor.
 *
 * This method determines if the audio plugin provides a graphical editor
 * for user interaction, allowing for parameter adjustments and other
 * visual controls.
 *
 * @return True if the plugin has an editor; false otherwise.
 */
bool PluginProcessor::hasEditor() const
{
    return true;
}

/**
 * @brief Creates and returns the editor component for a plugin.
 *
 * This method is responsible for creating the graphical user interface (GUI) component
 * of the plugin. The editor provides users with a way to interact with the plugin,
 * such as adjusting parameters or visualizing audio processing states.
 * It ensures that the editor is properly initialized and ready for use.
 *
 * @return A pointer to the editor component that allows user interaction with the plugin.
 */
juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

/**
 * @brief Retrieves the current state of the processor or plugin.
 *
 * This method is responsible for saving the internal state of the processor or plugin
 * in order to allow restoring it at a later point. The state is typically stored in
 * a data structure that can be serialized and deserialized, enabling a consistent
 * and seamless user experience across sessions.
 *
 * @param destData A reference to a memory block where the serialized state
 *                 information will be stored. This memory block can then be used
 *                 to persist the state or transmit it to the host application.
 */
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

/**
 * @brief Restores the state of the plugin from provided data.
 *
 * This method is used to load the saved state of the plugin, allowing it to reconstruct
 * its parameters and settings from a binary data blob. It is typically called by the host
 * when restoring the state of a plugin instance.
 *
 * @param data A pointer to the binary data containing the state information.
 * @param sizeInBytes The size of the binary data pointed to by `data`, measured in bytes.
 */
void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

/**
 * @brief Creates and returns a new instance of the plugin's audio processor.
 *
 * This function acts as a factory method for generating the audio processor
 * that defines the behavior and functionality of the plugin. It ensures the
 * plugin's processing logic is encapsulated and properly initialized for use
 * within the host environment.
 *
 * @return A newly created audio processor instance for the plugin.
 */
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
