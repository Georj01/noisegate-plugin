# Noise Gate VST3

A minimalist VST3 Noise Gate plugin. It automatically silences audio signals that drop below a fixed threshold of 0.1f.

## Stack

- C++
- JUCE Framework (v8.0.0)
- CMake

## Installation

```bash
# Clone the repository
git clone https://github.com/Georj01/noisegate-plugin.git
cd noisegate-plugin

# Generate build files
cmake -B build

# Build the plugin
cmake --build build --config Release
```

## Usage

Once built, the plugin can be found in the `build/NoiseGate_artefacts/Release/VST3/` folder. Load the `.vst3` file into any compatible Digital Audio Workstation (DAW). The plugin operates statically; any audio sample with an absolute value below 0.1f will be muted.


