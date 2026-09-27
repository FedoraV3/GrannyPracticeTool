cjson_inc = include_directories('.')

cjson_lib = static_library(
	'cjson',
	files('cJSON.c'),
	include_directories: cjson_inc,
	# don't dllexport cJSON's functions out of our dll
	c_args: ['-DCJSON_HIDE_SYMBOLS'],
)
