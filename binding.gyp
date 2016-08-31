{
	"variables": {
		"COMMONS_LIB_DIR": "/usr/local/modsecurity"
	},
	"targets": [{
		"target_name": "node_modsecurity",
		"sources": [
			"src/node_modsecurity.hpp",
			"src/node_modsecurity.cpp"
		],
		"cflags!": [
			"-fno-exceptions",
			"-fno-rtti"
		],
		"cflags_cc!": [
			"-fno-exceptions",
			"-fno-rtti"
		],
		"cflags": [
			"-std=c++11"
		],
		"cflags_cc": [
			"-std=c++11"
		],
		"link_settings": {
			"libraries": [
				"-lmodsecurity",
				"-L<(COMMONS_LIB_DIR)/lib"
			]
		},
		"ldflags": ["-Wl,-rpath,<(COMMONS_LIB_DIR)/lib"],
		"conditions": [
			[
				"OS==\"mac\"", {
					"xcode_settings": {
						"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
						"GCC_ENABLE_CPP_RTTI": "YES",
						"OTHER_CPLUSPLUSFLAGS": [
							"-std=c++1y",
							"-stdlib=libc++"
						],
						"OTHER_LDFLAGS": [
							"-stdlib=libc++"
						],
						"MACOSX_DEPLOYMENT_TARGET": "10.7"
					}
				}
			]
		],
		"include_dirs": [
			"<!(node -e \"require('nan')\")",
			"<(COMMONS_LIB_DIR)/include/"
		]
	}]
}
