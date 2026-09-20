minhook_inc = include_directories('include')

minhook_lib = static_library(
	'minhook',
	files(
		'src/buffer.c',
		'src/hook.c',
		'src/trampoline.c',
		'src/hde/hde32.c',
		'src/hde/hde64.c',
	),
	include_directories: minhook_inc,
	c_args: ['-Wno-unused-parameter'],
)
