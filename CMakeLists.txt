cmake_minimum_required(VERSION 3.20)
project(NoiseGate VERSION 1.0.0)

set(JUCE_DIR "C:/JUCE" CACHE PATH "Path to JUCE library")
if(NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE library not found at ${JUCE_DIR}. Please ensure JUCE is located at C:/JUCE or update the JUCE_DIR variable.")
endif()

add_subdirectory("${JUCE_DIR}" juce)

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
