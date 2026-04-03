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
