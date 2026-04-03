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
        }
    ]
}