# Version Subystem Build Definition

version_source = custom_target('gen-version',
	output: 'version.c',
	command: [meson.project_source_root() + '/meson/scripts/make_version.sh', '@OUTPUT@'],
	build_always_stale: true
)

version_subsystem_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: version_source,
)
