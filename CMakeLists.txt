cmake_minimum_required(VERSION 3.20)
project(NoiseGate VERSION 1.0.0)

# 1. Try finding JUCE locally (Fast, for local development)
find_package(JUCE CONFIG PATHS "C:/JUCE")

# 2. Plan B: If not found locally, fetch it from GitHub
if(NOT TARGET juce::juce_recommended_config_flags)
    message(STATUS "JUCE not found locally. Downloading from GitHub (this will take a while)...")
    
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        8.0.0
    )
    FetchContent_MakeAvailable(JUCE)
endif()

juce_add_plugin(NoiseGate
    COMPANY_NAME "MyCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE MyCo
    PLUGIN_CODE Nsgt
    FORMATS VST3
    PRODUCT_NAME "Noise Gate"
)

target_sources(NoiseGate
    PRIVATE
        src/plugin-processor.cpp
        src/plugin-processor.h
        src/plugin-editor.cpp
        src/plugin-editor.h
)

target_compile_definitions(NoiseGate
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(NoiseGate
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
