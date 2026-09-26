
project(
	'grannylegacypracticetool',
	['c', 'cpp'],
	default_options: ['cpp_std=c++17', 'b_vscrt=static_from_buildtype'],
)

cc = meson.get_compiler('c')

subdir('inc/lib/minhook')
subdir('inc/lib/cimgui')

project_inc = include_directories('inc')

# for the gui hook
d3d11_dep = cc.find_library('d3d11')

subdir('src')

# so we can use DEBUG preprocessor flag so that we can debug
debug = get_option('debug') ? ['-DDEBUG'] : []

shared_library(
	'grannylegacypractice',
	src_files,
	include_directories: [project_inc, minhook_inc, cimgui_inc],
	dependencies: [d3d11_dep],
	link_with: [minhook_lib, cimgui_lib],
	c_args: ['-DCIMGUI_NO_EXPORT', '-DCIMGUI_DEFINE_ENUMS_AND_STRUCTS'],
	cpp_args: ['-DCIMGUI_NO_EXPORT'],
	link_args: cc.get_argument_syntax() == 'gcc' ? ['-static'] : [],
	install: true
)
