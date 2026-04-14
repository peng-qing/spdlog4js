# spdlog4js

封装 C++ 的 `spdlog` 提供给 `nodejs` 服务使用。

## 环境需求

1. `node-gyp` 安装
```shell
npm install -g node-gyp

# 验证环境
node-gyp --version
node --version     # 建议 >= 16.x
```

2. 安装依赖 —— node-addon-api 是 N-API 的 C++ 封装层。

> 为什么不使用 `N-API` 
> 
> 裸`N-API`是 `C` 风格接口。 而 `node-addon-api` 是 `N-API` 的 C++ 封装层，提供了相关的封装比如 `Napi::String`，`Napi::Object`等类

```shell
npm install node-addon-api
```

3. 当前项目使用 `git submodule` 管理 `spdlog` 依赖

```shell
# 添加
git submodule add https://github.com/gabime/spdlog.git deps/spdlog

# 更新
git submodule update --init --recursive
```

## 构建流程

```shell
# 构建
node-gyp configure
# 编译
node-gyp build

# 清理 - 构建 - 编译 
# --verbose 会显示编译参数细节 方便定位问题
node-gyp clean && node-gyp configure && node-gyp build --verbose
```
## 简单的性能测试

输出 10000000 条日志
```shell
node examples/bench_example.js

[spdlog sync file]
  Total: 3566.81ms
  Ops/s: 2,803,628
  Avg:   0.36us/op

[spdlog async file]
  Total: 9099.54ms
  Ops/s: 1,098,957
  Avg:   0.91us/op

[Node.js fs.writeSync]
  Total: 9825.25ms
  Ops/s: 1,017,786
  Avg:   0.98us/op

[Node.js fs.appendFileSync]
  Total: 19588.25ms
  Ops/s: 510,510
  Avg:   1.96us/op

[pino sync file]
  Total: 15749.20ms
  Ops/s: 634,953
  Avg:   1.57us/op

[pino async file]
  Total: 20076.09ms
  Ops/s: 498,105
  Avg:   2.01us/op

[pino async (minimal)]
  Total: 25510.36ms
  Ops/s: 391,998
  Avg:   2.55us/op

Done. Check ./logs/ for output files.
```
## 使用

```javascript
const assert = require("assert");
const { Logger, LogConfig, LogLevel } = require("spdlog4js");

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
```