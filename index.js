"use strict";

const nativeBinding = require("bindings")("spdlog4js");
const LogConfig = nativeBinding.LogConfig;
const LogLevel = nativeBinding.LogLevel;
const Sink = nativeBinding.Sink;
const RowLogger = nativeBinding.Logger;

class Logger extends RowLogger {
    constructor(config) {
        super(config);
    }

    static critical(...message) {
        if (message.length <= 0) return;
        RowLogger.critical(message.join());
    }

    static error(...message) {
        if (message.length <= 0) return;
        RowLogger.error(message.join());
    }

    static warn(...message) {
        if (message.length <= 0) return;
        RowLogger.warn(message.join());
    }

    static info(...message) {
        if (message.length <= 0) return;
        RowLogger.info(message.join());
    }

    static debug(...message) {
        if (message.length <= 0) return;
        RowLogger.debug(message.join());
    }

    static trace(...message) {
        if (message.length <= 0) return;
        RowLogger.trace(message.join());
    }

    critical(...message) {
        if (message.length <= 0) return;
        super.critical(message.join());
    }

    error(...message) {
        if (message.length <= 0) return;
        super.error(message.join());
    }

    warn(...message) {
        if (message.length <= 0) return;
        super.warn(message.join());
    }

    info(...message) {
        if (message.length <= 0) return;
        super.info(message.join());
    }

    debug(...message) {
        if (message.length <= 0) return;
        super.debug(message.join());
    }

    trace(...message) {
        if (message.length <= 0) return;
        super.trace(message.join());
    }
}

module.exports = {
    LogConfig,
    LogLevel,
    Sink,
    RowLogger,
    Logger,
};
