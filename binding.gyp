{
    "targets": [
        {
            "target_name": "spdlog4js",
            "sources": [
                "src/addon.cc",
                "src/logger.h",
                "src/logger.cc",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")", 
                "deps/spdlog/include" 
            ],
            "defines": [
                "NAPI_VERSION=6",
            ],
            "cflags!": [
                "-fno-exceptions"
            ],
            "cflags_cc!": [
                "-fno-exceptions"
            ],
            "cflags_cc": [
                "-std=c++17"
            ],
            "conditions": [
                [
                    "OS=='win'",
                    {
                        "msvs_settings": {
                            "VCCLCompilerTool": {
                                "ExceptionHandling": 1,
                                "AdditionalOptions": ["/std:c++17"]
                            }
                        }
                    }
                ],
                [
                    "OS=='mac'",
                    {
                        "xcode_settings": {
                            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                            "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
                            "CLANG_CXX_LIBRARY": "libc++"
                        }
                    }
                ]
            ]
        }
    ]
}