
subdir('declaration')
subdir('media')
subdir('namespace')
subdir('property')
subdir('selectors')
subdir('values')

sources = files ([
	'declaration/default.c',
	'declaration/entry.c',
	'declaration/entry_destroy.c',
	'declaration/init.c',
	'declaration/parser.c',
	'declaration/serialization.c',
	'declaration/state.c',
	'media/init.c',
	'media/state.c',
	'namespace/init.c',
	'namespace/parser.c',
	'namespace/serialization.c',
	'namespace/state.c',
	'property/init.c',
	'property/parser_background.c',
	'property/parser.c',
	'property/parser_image.c',
	'property/parser_text_decoration.c',
	'property/parser_url.c',
	'property/serialization.c',
	'property/shared.c',
	'selectors/function.c',
	'selectors/function_parser.c',
	'selectors/init.c',
	'selectors/list.c',
	'selectors/parser.c',
	'selectors/pseudo.c',
	'selectors/serialization.c',
	'selectors/state.c',
	'selectors/value.c',
	'values/color.c',
	'values/color_parser.c',
	'values/consume.c',
	'values/destroy.c',
	'values/image.c',
	'values/serialization.c',
	'values/units.c',
	'values/values.c',
	'an_plus_b.c',
	'check.c',
	'convert.c',
	'entry.c',
	'mycss.c',
	'mystring.c',
	'parser.c',
	'stack.c',
	'stylesheet.c',
	'tokenizer_end.c',
	'tokenizer_global.c',
	'tokenizer.c'
])
headers = files ([
	'an_plus_b.h',
	'api.h',
	'check.h',
	'convert.h',
	'entry.h',
	'mycss.h',
	'myosi.h',
	'mystring.h',
	'parser.h',
	'stack.h',
	'stylesheet.h',
	'tokenizer_end.h',
	'tokenizer_global.h',
	'tokenizer.h',
	'tokenizer_resource.h'
])

inc_libh = include_directories ('.')
inc_libh_dep = declare_dependency (include_directories : inc_libh)

lib = library(VERSIONED_PROJECT_NAME,
	sources,
	version : PROJECT_VERSION,
	dependencies : [inc_libh_dep, inc_headersh_dep, myhtml_dep],
	install : true,
	install_dir : [
		true,
	])

install_headers(
	headers,
	subdir: VERSIONED_PROJECT_NAME
	)

