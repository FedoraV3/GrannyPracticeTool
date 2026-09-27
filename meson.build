
project(
	'grannylegacypracticetool',
	['c', 'cpp'],
	default_options: ['cpp_std=c++20', 'b_vscrt=static_from_buildtype'],
)

cc = meson.get_compiler('c')

subdir('inc/lib/minhook')
subdir('inc/lib/cimgui')
subdir('inc/lib/cjson')

project_inc = include_directories('inc')

# for the gui hook
d3d11_dep = cc.find_library('d3d11')

subdir('src')

# so we can use DEBUG preprocessor flag so that we can debug
debug = get_option('debug') ? ['-DDEBUG'] : []

shared_library(
	'grannylegacypractice',
	src_files,
	include_directories: [project_inc, minhook_inc, cimgui_inc, cjson_inc],
	dependencies: [d3d11_dep],
	link_with: [minhook_lib, cimgui_lib, cjson_lib],
	c_args: ['-DCIMGUI_NO_EXPORT', '-DCIMGUI_DEFINE_ENUMS_AND_STRUCTS', '-DCJSON_HIDE_SYMBOLS'],
	cpp_args: ['-DCIMGUI_NO_EXPORT', '-DCJSON_HIDE_SYMBOLS'],
	link_args: cc.get_argument_syntax() == 'gcc' ? ['-static'] : [],
	install: true
)
