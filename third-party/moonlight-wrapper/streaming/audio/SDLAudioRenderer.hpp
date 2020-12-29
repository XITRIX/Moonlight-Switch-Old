//
//  SDLAudioRenderer.hpp
//  Moonlight-Switch
//
//  Created by Daniil Vinogradov on 30.12.2020.
//

#pragma once
#include "IAudioRenderer.hpp"

#include <SDL2/SDL.h>
#include <stdbool.h>

class SDLAudioRenderer: public IAudioRenderer {
public:
    SDLAudioRenderer() {};
    ~SDLAudioRenderer() {};
    
    int init(int audio_configuration, const POPUS_MULTISTREAM_CONFIGURATION opus_config, void *context, int ar_flags) override;
    void cleanup() override;
    void decode_and_play_sample(char *sample_data, int sample_length) override;
    int capabilities() override;
};
