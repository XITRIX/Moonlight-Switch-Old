//
//  SDLAudioRenderer.cpp
//  Moonlight-Switch
//
//  Created by Daniil Vinogradov on 30.12.2020.
//

#include "SDLAudioRenderer.hpp"
#include <SDL2/SDL_audio.h>

#include <stdio.h>
#include <opus/opus.h>
#include <opus/opus_multistream.h>

#define MAX_CHANNEL_COUNT 6
#define FRAME_SIZE 240
#define FRAME_BUFFER 12

static OpusMSDecoder* decoder;
static short pcmBuffer[FRAME_SIZE * MAX_CHANNEL_COUNT];
static SDL_AudioDeviceID dev;
static int channelCount;

int SDLAudioRenderer::init(int audio_configuration, const POPUS_MULTISTREAM_CONFIGURATION opus_config, void *context, int ar_flags) {
  int rc;
    decoder = opus_multistream_decoder_create(opus_config->sampleRate, opus_config->channelCount, opus_config->streams, opus_config->coupledStreams, opus_config->mapping, &rc);

  channelCount = opus_config->channelCount;

  SDL_InitSubSystem(SDL_INIT_AUDIO);

  SDL_AudioSpec want, have;
  SDL_zero(want);
  want.freq = opus_config->sampleRate;
  want.format = AUDIO_S16LSB;
  want.channels = opus_config->channelCount;
  want.samples = 4096;

  dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  if (dev == 0) {
    printf("Failed to open audio: %s\n", SDL_GetError());
    return -1;
  } else {
    if (have.format != want.format)  // we let this one thing change.
      printf("We didn't get requested audio format.\n");
    SDL_PauseAudioDevice(dev, 0);  // start audio playing.
  }

  return 0;
}

void SDLAudioRenderer::cleanup() {
  if (decoder != NULL)
    opus_multistream_decoder_destroy(decoder);

  SDL_CloseAudioDevice(dev);
}

void SDLAudioRenderer::decode_and_play_sample(char *sample_data, int sample_length) {
    int decodeLen = opus_multistream_decode(decoder, (const unsigned char*)sample_data, sample_length, pcmBuffer, FRAME_SIZE, 0);
  if (decodeLen > 0) {
    SDL_QueueAudio(dev, pcmBuffer, decodeLen * channelCount * sizeof(short));
  } else {
    printf("Opus error from decode: %d\n", decodeLen);
  }
}

int SDLAudioRenderer::capabilities() {
    return CAPABILITY_DIRECT_SUBMIT;
}

