#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <sys/types.h>

#include "napi.h"
#include "spdlog/spdlog.h"

/// @brief 日志配置相关
class LogConfig : public Napi::ObjectWrap<LogConfig>
{
public:
    /**
     * @brief 初始化日志配置类
     * @param env Node.js 环境对象
     * @param exports 导出对象，用于注册模块的 API
     * @return Napi::Object 导出对象
     */
    static Napi::Object Initialize(Napi::Env env, Napi::Object exports);

public:
    /**
     * @brief 构造函数
     * @param info Node.js 回调信息对象
     */
    LogConfig(const Napi::CallbackInfo &info);

    /**
     * @brief 获取日志器配置名
     * @param info Node.js 回调信息对象
     * @return Napi::Value 日志器配置名
     */
    Napi::Value GetName(const Napi::CallbackInfo &info);
    /**
     * @brief 获取日志输出目标
     * @param info Node.js 回调信息对象
     * @return Napi::Value 日志输出目标
     */
    Napi::Value GetSink(const Napi::CallbackInfo &info);
    /**
     * @brief 获取日志输出目标
     * @param info Node.js 回调信息对象
     * @return Napi::Value 日志输出目标
     */
    Napi::Value GetConsole(const Napi::CallbackInfo &info);
    /**
     * @brief 获取输出路径
     * @param info Node.js 回调信息对象
     * @return Napi::Value 输出路径
     */
    Napi::Value GetFilename(const Napi::CallbackInfo &info);
    /**
     * @brief 获取日志级别
     * @param info Node.js 回调信息对象
     * @return Napi::Value 日志级别
     */
    Napi::Value GetLevel(const Napi::CallbackInfo &info);
    /**
     * @brief 获取日志格式
     * @param info Node.js 回调信息对象
     * @return Napi::Value 日志格式
     */
    Napi::Value GetPattern(const Napi::CallbackInfo &info);
    /**
     * @brief 获取是否设置为默认日志器
     * @param info Node.js 回调信息对象
     * @return Napi::Value 是否设置为默认日志器
     */
    /**
     * @brief 获取是否设置为默认日志器
     * @param info Node.js 回调信息对象
     * @return Napi::Value 是否设置为默认日志器
     */
    Napi::Value GetSetDefault(const Napi::CallbackInfo &info);
    /**
     * @brief 获取最大文件大小
     * @param info Node.js 回调信息对象
     * @return Napi::Value 最大文件大小
     */
    Napi::Value GetMaxFileSize(const Napi::CallbackInfo &info);
    /**
     * @brief 获取最大文件备份数
     * @param info Node.js 回调信息对象
     * @return Napi::Value 最大文件备份数
     */
    Napi::Value GetMaxFileBackups(const Napi::CallbackInfo &info);
    /**
     * @brief 获取刷新间隔
     * @param info Node.js 回调信息对象
     * @return Napi::Value 刷新间隔
     */
    Napi::Value GetFlushInterval(const Napi::CallbackInfo &info);
    /**
     * @brief 获取是否异步日志输出
     * @param info Node.js 回调信息对象
     * @return Napi::Value 是否异步日志输出
     */
    Napi::Value GetAsyncMode(const Napi::CallbackInfo &info);
    /**
     * @brief 设置日志器配置名
     * @param info Node.js 回调信息对象
     * @param name 日志器配置名
     */
    void SetName(const Napi::CallbackInfo &info, const Napi::Value &name);
    /**
     * @brief 设置日志输出目标
     * @param info Node.js 回调信息对象
     * @param sink 日志输出目标
     */
    void SetSink(const Napi::CallbackInfo &info, const Napi::Value &sink);
    /**
     * @brief 设置日志输出目标
     * @param info Node.js 回调信息对象
     * @param sink 日志输出目标
     */
    void SetConsole(const Napi::CallbackInfo &info, const Napi::Value &console);
    /**
     * @brief 设置输出路径
     * @param info Node.js 回调信息对象
     * @param filename 输出路径
     */
    void SetFilename(const Napi::CallbackInfo &info, const Napi::Value &filename);
    /**
     * @brief 设置日志级别
     * @param info Node.js 回调信息对象
     * @param level 日志级别
     */
    void SetLevel(const Napi::CallbackInfo &info, const Napi::Value &level);
    /**
     * @brief 设置日志格式
     * @param info Node.js 回调信息对象
     * @param pattern 日志格式
     */
    void SetPattern(const Napi::CallbackInfo &info, const Napi::Value &pattern);
    /**
     * @brief 设置是否设置为默认日志器
     * @param info Node.js 回调信息对象
     * @param setDefault 是否设置为默认日志器
     */
    void SetSetDefault(const Napi::CallbackInfo &info, const Napi::Value &setDefault);
    /**
     * @brief 设置最大文件大小
     * @param info Node.js 回调信息对象
     * @param maxFileSize 最大文件大小
     */
    void SetMaxFileSize(const Napi::CallbackInfo &info, const Napi::Value &maxFileSize);
    /**
     * @brief 设置最大文件备份数
     * @param info Node.js 回调信息对象
     * @param maxFileBackups 最大文件备份数
     */
    void SetMaxFileBackups(const Napi::CallbackInfo &info, const Napi::Value &maxFileBackups);
    /**
     * @brief 设置刷新间隔
     * @param info Node.js 回调信息对象
     * @param flushInterval 刷新间隔
     */
    void SetFlushInterval(const Napi::CallbackInfo &info, const Napi::Value &flushInterval);
    /**
     * @brief 设置是否异步日志输出
     * @param info Node.js 回调信息对象
     * @param asyncMode 是否异步日志输出
     */
    void SetAsyncMode(const Napi::CallbackInfo &info, const Napi::Value &asyncMode);

public:
    /// @brief 日志器配置名
    std::string m_name;
    /// @brief 日志输出目标
    std::string m_sink;
    /// @brief 输出路径
    std::string m_filename;
    /// @brief 日志级别
    std::string m_level;
    /// @brief 日志格式
    std::string m_pattern;
    /// @brief 是否设置为默认日志器
    bool m_set_default;
    /// @brief 是否同时输出到终端
    bool m_console;
    /// @brief 最大文件大小
    size_t m_max_file_size;
    /// @brief 最大文件备份数
    size_t m_max_file_backups;
    /// @brief 刷新间隔 单位：秒
    uint32_t m_flush_interval;
    /// @brief 是否异步日志输出
    bool m_async_mode;
};

/// @brief 日志级别相关
class LogLevel : public Napi::ObjectWrap<LogLevel>
{
public:
    /**
     * @brief 初始化日志级别类
     * @param env Node.js 环境对象
     * @param exports 导出对象，用于注册模块的 API
     * @return Napi::Object 导出对象
     */
    static Napi::Object Initialize(Napi::Env env, Napi::Object exports);

    /**
     * @brief 将字符串转换为 spdlog 日志级别枚举
     * @param level 日志级别字符串
     * @return spdlog::level::level_enum 日志级别枚举
     */
    static spdlog::level::level_enum ToLogLevel(const std::string &level);

    /**
     * @brief 将 spdlog 日志级别枚举转换为字符串
     * @param level 日志级别枚举
     * @return std::string 日志级别字符串
     */
    static std::string ToLogLevelString(const spdlog::level::level_enum &level);
};

/// @brief 日志输出目标相关
class LoggerSink : public Napi::ObjectWrap<LoggerSink>
{
public:
    /**
    * @brief 日志输出目标
    */
    enum class Sink : int32_t
    {
        /// @brief 控制台输出
        CONSOLE = 0,
        /// @brief 文件输出
        FILE = 1,
        /// @brief 旋转文件输出
        ROTATING_FILE = 3,
        /// @brief 每日文件输出
        DAILY_FILE = 4,
        /// @brief 每小时文件输出
        HOURLY_FILE = 5
    };

public:
    /**
     * @brief 初始化日志输出目标类
     * @param env Node.js 环境对象
     * @param exports 导出对象，用于注册模块的 API
     * @return Napi::Object 导出对象
     */
    static Napi::Object Initialize(Napi::Env env, Napi::Object exports);
    /**
     * @brief 将字符串转换为 spdlog 日志输出目标枚举
     * @param sink 日志输出目标字符串
     * @return Sink 日志输出目标枚举
     */
    static Sink ToSink(const std::string &sink);
    /**
     * @brief 将 spdlog 日志输出目标枚举转换为字符串
     * @param sink 日志输出目标枚举
     * @return std::string 日志输出目标字符串
     */
    static std::string ToSinkString(const Sink &sink);
};

/// @brief 日志记录器类
class Logger : public Napi::ObjectWrap<Logger>
{
public:
    /**
     * @brief 初始化日志记录器类
     * @param env Node.js 环境对象
     * @param exports 导出对象，用于注册模块的 API
     * @return Napi::Object 导出对象
     */
    static Napi::Object Initialize(Napi::Env env, Napi::Object exports);

public:
    /**
     * @brief 构造函数
     * @param info Node.js 回调信息对象，包含构造函数的参数
     */
    explicit Logger(const Napi::CallbackInfo &info);
    /**
     * @brief 析构函数
     */
    ~Logger();

private:
    /**
     * @brief 记录严重错误日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticCritical(const Napi::CallbackInfo &info);
    /**
     * @brief 记录错误日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticError(const Napi::CallbackInfo &info);
    /**
     * @brief 记录警告日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticWarn(const Napi::CallbackInfo &info);
    /**
     * @brief 记录信息日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticInfo(const Napi::CallbackInfo &info);
    /**
     * @brief 记录调试日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticDebug(const Napi::CallbackInfo &info);
    /**
     * @brief 记录跟踪日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticTrace(const Napi::CallbackInfo &info);
    /**
     * @brief 设置日志级别
     * @param info Node.js 回调信息对象，包含设置日志级别的参数
     * @return Napi::Value 空值
     */
    static void StaticSetLevel(const Napi::CallbackInfo &info, const Napi::Value &value);
    /**
     * @brief 获取日志级别
     * @param info Node.js 回调信息对象，包含获取日志级别的参数
     * @return Napi::Value 日志级别
     */
    static Napi::Value StaticGetLevel(const Napi::CallbackInfo &info);
    /**
     * @brief 设置日志输出模式
     * @param info Node.js 回调信息对象，包含设置日志输出模式的参数
     * @return Napi::Value 空值
     */
    static void StaticSetPattern(const Napi::CallbackInfo &info, const Napi::Value &value);
    /**
     * @brief 刷新日志缓冲区
     * @param info Node.js 回调信息对象，包含刷新日志缓冲区的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticFlush(const Napi::CallbackInfo &info);
    /**
     * @brief 设置刷新日志缓冲区事件
     * @param info Node.js 回调信息对象，包含设置刷新日志缓冲区事件的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticFlushOn(const Napi::CallbackInfo &info);
    /**
     * @brief 设置刷新日志缓冲区事件
     * @param info Node.js 回调信息对象，包含设置刷新日志缓冲区事件的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticFlushEvery(const Napi::CallbackInfo &info);
    /**
     * @brief 设置默认日志记录器
     * @param info Node.js 回调信息对象，包含设置默认日志记录器的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticSetDefault(const Napi::CallbackInfo &info);
    /**
     * @brief 关闭日志记录器
     * @param info Node.js 回调信息对象，包含关闭日志记录器的参数
     * @return Napi::Value 空值
     */
    static Napi::Value StaticDropAll(const Napi::CallbackInfo &info);

private:
    /**
     * @brief 构造函数引用，用于在 Node.js 中创建日志记录器对象引用
     */
    static Napi::FunctionReference constructor;
    /**
     * @brief 记录严重错误日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Critical(const Napi::CallbackInfo &info);
    /**
     * @brief 记录错误日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Error(const Napi::CallbackInfo &info);
    /**
     * @brief 记录警告日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Warn(const Napi::CallbackInfo &info);
    /**
     * @brief 记录信息日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Info(const Napi::CallbackInfo &info);
    /**
     * @brief 记录调试日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Debug(const Napi::CallbackInfo &info);
    /**
     * @brief 记录跟踪日志
     * @param info Node.js 回调信息对象，包含记录日志的参数
     * @return Napi::Value 空值
     */
    Napi::Value Trace(const Napi::CallbackInfo &info);
    /**
     * @brief 设置日志级别
     * @param info Node.js 回调信息对象，包含设置日志级别的参数
     * @return Napi::Value 空值
     */
    void SetLevel(const Napi::CallbackInfo &info, const Napi::Value &value);
    /**
     * @brief 获取日志级别
     * @param info Node.js 回调信息对象，包含获取日志级别的参数
     * @return Napi::Value 日志级别
     */
    Napi::Value GetLevel(const Napi::CallbackInfo &info);
    /**
     * @brief 设置日志格式
     * @param info Node.js 回调信息对象，包含设置日志模式的参数
     * @return Napi::Value 空值
     */
    void SetPattern(const Napi::CallbackInfo &info, const Napi::Value &value);
    /**
     * @brief 开启自动刷新
     * @param info Node.js 回调信息对象，包含开启自动刷新的参数
     * @return Napi::Value 空值
     */
    Napi::Value FlushOn(const Napi::CallbackInfo &info);
    /**
     * @brief 刷新日志缓冲区
     * @param info Node.js 回调信息对象，包含刷新日志缓冲区的参数
     * @return Napi::Value 空值
     */
    Napi::Value Flush(const Napi::CallbackInfo &info);

private:
    /// @brief 日志器
    std::shared_ptr<spdlog::logger> m_logger;
};
