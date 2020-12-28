dep_glfw3 = dependency('glfw3', version : '>=3.3')
dep_glm   = dependency('glm', version : '>=0.9.8')

moonlight_wrapper_files = files(
    'crypto/Data.cpp',
    'crypto/MbedTLSCryptoManager.cpp',
    'crypto/OpenSSLCryptoManager.cpp',

    'libgamestream/client.cpp',
    'libgamestream/http.cpp',
    'libgamestream/xml.cpp',

    # 'streaming/audio/AudrenAudioRenderer.cpp',
    'streaming/audio/DebugFileRecorderAudioRenderer.cpp',

    'streaming/ffmpeg/FFmpegVideoDecoder.cpp',

    'streaming/video/GLVideoRenderer.cpp',

    'streaming/GameStreamClient.cpp',
    'streaming/MoonlightSession.cpp',
    'streaming/WakeOnLanManager.cpp',

    'ui/ui_connection.cpp',

    'Async.cpp',
    'EZLogger.cpp',
    'GamepadMapper.cpp',
    'InputController.cpp',
    'Settings.cpp',
)

moonlight_wrapper_include = include_directories(
    '.',
    'crypto',
    'libgamestream',
    'streaming/audio',
    'streaming/ffmpeg',
    'streaming/video',
    'streaming',
    'ui',
)

moonlight_wrapper_dependencies = [ dep_glfw3, dep_glm ]
