/**
 * @brief 模块入口文件
 * @author pengqq
 */

#include "logger.h"
#include "napi.h"

/**
 * @brief 模块初始化函数，Node.js 加载 .node 文件时自动调用
 * @param env Node.js 环境对象
 * @param exports 导出对象，用于注册模块的 API
 * @return Napi::Object 导出对象
 */
Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    // 日志配置文件导出
    LogConfig::Initialize(env, exports);
    // 注册日志级别相关信息导出
    LogLevel::Initialize(env, exports);
    // 注册日志输出目标相关信息导出
    LoggerSink::Initialize(env, exports);
    // 注册日志记录器相关信息导出
    Logger::Initialize(env, exports);

    return exports;
}

// 通过宏导出模块的初始化 此处应该和 binding.gyp 中的 target_name 一致
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)