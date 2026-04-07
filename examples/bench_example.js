const { Logger, LogConfig } = require("../index");
const pino = require("pino");

// 同步
const syncConfig = new LogConfig({
    name: "bench_example_sync",
    sink: "ROTATING_FILE",
    filename: "./logs/bench_example.log",
    level: "DEBUG",
    pattern: "[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %@ %v",
    console: false,
    setDefault: false,
    flushInterval: 3,
});
const syncLogger = new Logger(syncConfig);

// 异步
const asyncConfig = new LogConfig({
    name: "bench_example_async",
    sink: "ROTATING_FILE",
    filename: "./logs/bench_example_async.log",
    level: "DEBUG",
    pattern: "[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %@ %v",
    console: false,
    setDefault: false,
    flushInterval: 3,
    asyncMode: true,
});
const asyncLogger = new Logger(asyncConfig);

// 对比fs
const fs = require("fs");
const fileLogger = fs.openSync("./logs/bench-fs.log", "w");

// ===== pino 文件输出 =====
const pinoFileLogger = pino(
    { level: "debug" },
    pino.destination({ dest: "./logs/bench-pino.log", sync: true })
);
// ===== pino 异步文件输出 =====
const pinoAsyncLogger = pino(
    { level: "debug" },
    pino.destination({ dest: "./logs/bench-pino-async.log", sync: false })
);
// ===== pino 极简模式（去掉序列化开销）=====
const pinoMinLogger = pino(
    {
        level: "debug",
        // 去掉 timestamp 减少序列化
        timestamp: false,
    },
    pino.destination({ dest: "./logs/bench-pino-min.log", sync: false })
);

// ===== 配置 =====
const ITERATIONS = 10000000;
const MESSAGE = "benchmark test message with some payload data 1234567890";
// ===== 工具函数 =====
function bench(name, fn, deferFn) {
    // 预热
    for (let i = 0; i < 1000; i++) fn();
    // 正式测试
    const start = process.hrtime.bigint();
    for (let i = 0; i < ITERATIONS; i++) fn();
    if (deferFn) {
        deferFn();
    }
    const end = process.hrtime.bigint();
    const totalMs = Number(end - start) / 1e6;
    const opsPerSec = Math.round(ITERATIONS / (totalMs / 1000));
    const avgUs = (totalMs / ITERATIONS * 1000).toFixed(2);
    console.log(`[${name}]`);
    console.log(`  Total: ${totalMs.toFixed(2)}ms`);
    console.log(`  Ops/s: ${opsPerSec.toLocaleString()}`);
    console.log(`  Avg:   ${avgUs}us/op`);
    console.log();
}

// 执行压测
bench("spdlog sync file", () => {
    syncLogger.info(MESSAGE);
}, () => {
    syncLogger.flush();
});
bench("spdlog async file", () => {
    asyncLogger.info(MESSAGE);
}, () => {
    asyncLogger.flush();
});
bench("Node.js fs.writeSync", () => {
    fs.writeSync(fileLogger, MESSAGE + "\n");
}, () => {
    fs.closeSync(fileLogger);
});
bench("Node.js fs.appendFileSync", () => {
    fs.appendFileSync("./logs/bench-append.log", MESSAGE + "\n");
});
bench("pino sync file", () => {
    pinoFileLogger.info(MESSAGE);
}, () => {
    pinoFileLogger.flush();
});
bench("pino async file", () => {
    pinoAsyncLogger.info(MESSAGE);
}, () => {
    pinoAsyncLogger.flush();
});
bench("pino async (minimal)", () => {
    pinoMinLogger.info(MESSAGE);
}, () => {
    pinoMinLogger.flush();
});

console.log("Done. Check ./logs/ for output files.");

// pino 异步 destination 需要等待写完
function cleanup() {
    return new Promise((resolve) => {
        let pending = 0;
        const destinations = [pinoFileLogger, pinoAsyncLogger, pinoMinLogger];
        destinations.forEach((logger) => {
            const dest = logger[pino.symbols.streamSym];
            if (dest && typeof dest.end === "function") {
                pending++;
                dest.end(() => {
                    if (--pending === 0) resolve();
                });
            }
        });
        if (pending === 0) resolve();
    });
}
cleanup().then(() => {
    process.exit(0);
});