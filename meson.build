project('weather_app', 'c')

sources = files(
    'src/main.c',
    'src/renderer.c',
    'libs/microui/microui.c'
)

includes = include_directories(
    'include',
    'libs/fenster',
    'libs/microui',
)

deps = []
c_args = []

cc = meson.get_compiler('c')
deps += cc.find_library('m', required : true)

system = host_machine.system()

if system == 'linux'
  deps += dependency('x11')
  deps += cc.find_library('asound')
  c_args += '-D_POSIX_C_SOURCE=200809L'

elif system == 'darwin'
  deps += dependency('appleframework', modules : ['Cocoa', 'AudioToolbox'])
  c_args += '-D_POSIX_C_SOURCE=200809L'

elif system == 'windows'
  deps += cc.find_library('gdi32')
  deps += cc.find_library('winmm')
endif

executable('weather_app',
            sources,
            include_directories: includes,
            dependencies: deps,
            c_args: c_args
)
