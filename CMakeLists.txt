cmake_minimum_required(VERSION 3.10)

project(smeagol)

set(C_CXX_SOURCES_DIR "src")
set(C_CXX_HEADERS_DIR "include")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OutputExecutable "${CMAKE_PROJECT_NAME}")


set(SOURCE_CXX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${C_CXX_HEADERS_DIR})
set(SOURCE_CXX_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/${C_CXX_SOURCES_DIR})

# Source Files are Curated Here
file(
    GLOB_RECURSE SOURCE_CXX_FILES
    "${SOURCE_CXX_SRC_DIR}/*.cpp"
)

include_directories(${SOURCE_CXX_INCLUDE_DIR})

add_executable(${OutputExecutable} ${SOURCE_CXX_FILES})

if(UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
    
    # OpenGL
    set(OpenGL_GL_PREFERENCE LEGACY)
    find_package(OpenGL REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIRS})
    target_link_libraries(${OutputExecutable} ${OpenGL_LIBRARIES} OpenGL::GL)

    # X11
    find_package(X11 REQUIRED)
    target_link_libraries(${OutputExecutable} X11::X11)

    include_directories(${X11_INCLUDE_DIRS})

    # Threads
    find_package(Threads REQUIRED)
    target_link_libraries(${OutputExecutable} Threads::Threads)
    include_directories(${Threads_INCLUDE_DIRS})

    # TODO: sanity checks
    
    if(USE_ALSA)
        
        # ALSA
        find_package(ALSA REQUIRED)
        target_link_libraries(${OutputExecutable} ALSA::ALSA)
        include_directories(${ALSA_INCLUDE_DIRS})
        add_compile_definitions(SOUNDWAVE_USING_ALSA=1)
        
    elseif(USE_SDL2_MIXER)

        # Because SDL2_mixer can be used on multiple platforms, we
        # defer it's inclusion until outside of the platform/toolchain
        # selection logic.

    else()  # PulseAudio is Default

        # PulseAudio
        find_package(PulseAudio REQUIRED)
        target_link_libraries(${OutputExecutable} ${PULSEAUDIO_LIBRARY} pulse-simple)
        include_directories(${PULSEAUDIO_INCLUDE_DIR})

        add_compile_definitions(SOUNDWAVE_USING_PULSE=1)

    endif()

    find_package(PNG REQUIRED)
    target_link_libraries(${OutputExecutable} PNG::PNG)
    include_directories(${PNG_INCLUDE_DIRS})

    # stdc++fs
    target_link_libraries(${OutputExecutable} stdc++fs)

endif() # Linux

######################################################################
# Emscripten
######################################################################
if (EMSCRIPTEN)
    
    # Generate an HTML file
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    # Build Cache: SDL2_mixer, libpng, zlib
    execute_process(COMMAND "${EMSCRIPTEN_ROOT_PATH}/embuilder${EMCC_SUFFIX}" build sdl2_mixer libpng zlib)
    
    if(EXISTS "${SOURCE_DATA_DIR}" AND IS_DIRECTORY "${SOURCE_DATA_DIR}")
        target_link_options(
            ${OutputExecutable}
            PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            -sMAX_WEBGL_VERSION=2
            -sMIN_WEBGL_VERSION=2
            -sUSE_LIBPNG=1
            -sUSE_SDL_MIXER=2          # thanks for the s, cstd
            -sLLD_REPORT_UNDEFINED
            --preload-file ${SOURCE_DATA_DIR}@assets)
    else()
        target_link_options(
            ${OutputExecutable}
            PRIVATE
            -sALLOW_MEMORY_GROWTH=1
            -sMAX_WEBGL_VERSION=2
            -sMIN_WEBGL_VERSION=2
            -sUSE_LIBPNG=1
            -sUSE_SDL_MIXER=2          # thanks for the s, cstd
            -sLLD_REPORT_UNDEFINED)
    endif()

endif() # Emscripten