cimgui_inc = include_directories('.', 'imgui', 'imgui/backends')

cimgui_lib = static_library(
	'cimgui',
	files(
		'cimgui.cpp',
		'imgui/imgui.cpp',
		'imgui/imgui_demo.cpp',
		'imgui/imgui_draw.cpp',
		'imgui/imgui_tables.cpp',
		'imgui/imgui_widgets.cpp',
		'imgui/backends/imgui_impl_dx11.cpp',
		'imgui/backends/imgui_impl_win32.cpp',
	),
	include_directories: cimgui_inc,
	dependencies: [
		cc.find_library('d3dcompiler'),
		cc.find_library('dwmapi'),
		cc.find_library('imm32'),
		cc.find_library('gdi32'),
	],
	cpp_args: ['-DCIMGUI_NO_EXPORT'],
)
