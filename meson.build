project('mycss', [ 'c'], version : '0.1.1')

PROJECT_NAME = 'mycss'
API_VERSION = '0'
PROJECT_VERSION = meson.project_version ()
VERSIONED_PROJECT_NAME = PROJECT_NAME+'-'+API_VERSION
CAMEL_CASE_NAME = 'MyCss'
VERSIONED_CAMEL_CASE_NAME = CAMEL_CASE_NAME +'-'+ API_VERSION

inc_rooth = include_directories ('.')
inc_rooth_dep = declare_dependency (include_directories : inc_rooth)

myhtml_dep = dependency ('myhtml-0')

subdir('source')
