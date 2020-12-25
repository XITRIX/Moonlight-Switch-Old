project('borealis_example', ['c', 'cpp'],
    version: '1.0.0',
    default_options: [ 'buildtype=release', 'strip=true', 'b_ndebug=if-release', 'cpp_std=c++1z' ],
)

subdir('third-party')

src_files = files(
    'src/main.cpp',
)

borealis_example = executable(
    'borealis_example',
    [ src_files, borealis_files, moonlight_wrapper_files, moonlight_files ],
    dependencies : borealis_dependencies,
    install: true,
    include_directories: [ borealis_include, moonlight_wrapper_include, moonlight_include, include_directories('src')],
    cpp_args: [ '-std=c++1z', '-g', '-ffunction-sections', '-O2', '-DBOREALIS_RESOURCES="./resources/"', '-DUSE_OPENSSL_CRYPTO', '-DHAS_SOCKLEN_T', 
    '-DDBUS_COMPILATION', '-DHAS_POLL', '-DHAS_FCNTL', '-D_GNU_SOURCE', '-DMOONLIGHT_VERSION=\"$(MOONLIGHT_VERSION)\"' ]
)
