const assert = require("assert");
const { Logger, LogConfig, LogLevel } = require("../index");

function runExample() {
    const config = new LogConfig({
        name: "example",
        sink: "ROTATING_FILE",
        filename: "./logs/example.log",
        level: "DEBUG",
        pattern: "[%Y-%m-%d %H:%M:%S] [%^%l%$] %@ %v",
        console: true,
        setDefault: false,
        flushInterval: 3,
    });

    const logger = new Logger(config);
    assert.ok(logger, "Logger should be created");

    // 实例日志
    logger.critical("instance critical");
    logger.error("instance error");
    logger.warn("instance warn");
    logger.info("instance info");
    logger.debug("instance debug");
    logger.trace("instance trace");
    logger.flushOn(LogLevel.ERROR);
    logger.flush();
    logger.level = "warn";
    logger.critical("instance critical");
    logger.error("instance error");
    logger.warn("instance warn");
    logger.info("instance info");
    logger.debug("instance debug");
    logger.trace("instance trace");
    logger.warn();

    // 静态日志（默认 logger）
    Logger.level = "INFO";
    Logger.pattern = "[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %@ %v";
    Logger.trace("static trace");
    Logger.debug("static debug");
    Logger.info("static info");
    Logger.warn("static warn");
    Logger.error("static error");
    Logger.critical("static critical");
    Logger.flushOn(LogLevel.ERROR);
    Logger.flush();

    const level = Logger.level;
    assert.strictEqual(typeof level, "number");
    console.log(level);

    Logger.dropAll();
    console.log("example.js test passed");
}

runExample();
