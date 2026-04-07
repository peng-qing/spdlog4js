#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "logger.h"
#include "napi.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/hourly_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

using namespace Napi;

Napi::Object LogConfig::Initialize(Napi::Env env, Napi::Object exports)
{
    Napi::Function prototype = DefineClass(
            env,
            "LogConfig",
            {
                    InstanceAccessor<&LogConfig::GetName, &LogConfig::SetName>("name"),
                    InstanceAccessor<&LogConfig::GetSink, &LogConfig::SetSink>("sink"),
                    InstanceAccessor<&LogConfig::GetConsole, &LogConfig::SetConsole>("console"),
                    InstanceAccessor<&LogConfig::GetFilename, &LogConfig::SetFilename>("filename"),
                    InstanceAccessor<&LogConfig::GetLevel, &LogConfig::SetLevel>("level"),
                    InstanceAccessor<&LogConfig::GetPattern, &LogConfig::SetPattern>("pattern"),
                    InstanceAccessor<&LogConfig::GetSetDefault, &LogConfig::SetSetDefault>("setDefault"),
                    InstanceAccessor<&LogConfig::GetMaxFileBackups, &LogConfig::SetMaxFileBackups>("maxFileBackups"),
                    InstanceAccessor<&LogConfig::GetMaxFileSize, &LogConfig::SetMaxFileSize>("maxFileSize"),
                    InstanceAccessor<&LogConfig::GetFlushInterval, &LogConfig::SetFlushInterval>("flushInterval"),
                    InstanceAccessor<&LogConfig::GetAsyncMode, &LogConfig::SetAsyncMode>("asyncMode"),
            });

    exports.Set("LogConfig", prototype);

    return exports;
}

LogConfig::LogConfig(const CallbackInfo &info)
    : Napi::ObjectWrap<LogConfig>(info),
      m_name("root"),
      m_sink("stdout"),
      m_filename("./logs/app.log"),
      m_level("info"),
      m_pattern("[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %@ %v"),
      m_set_default(false),
      m_console(true),
      m_max_file_size(1024 * 1024 * 500),
      m_max_file_backups(50),
      m_flush_interval(3),
      m_async_mode(false)
{
    if (info.Length() <= 0)
    {
        return;
    }
    // 如果有传参 那么使用传参覆盖掉默认值
    if (info[0].IsObject())
    {
        // 支持直接传递目标对象
        Napi::Object opts = info[0].As<Napi::Object>();
        if (opts.Has("name")) m_name = opts.Get("name").As<Napi::String>().Utf8Value();
        if (opts.Has("sink")) m_sink = opts.Get("sink").As<Napi::String>().Utf8Value();
        if (opts.Has("console")) m_console = opts.Get("console").As<Napi::Boolean>().Value();
        if (opts.Has("filename")) m_filename = opts.Get("filename").As<Napi::String>().Utf8Value();
        if (opts.Has("level")) m_level = opts.Get("level").As<Napi::String>().Utf8Value();
        if (opts.Has("pattern")) m_pattern = opts.Get("pattern").As<Napi::String>().Utf8Value();
        if (opts.Has("setDefault")) m_set_default = opts.Get("setDefault").As<Napi::Boolean>().Value();
        if (opts.Has("maxFileBackups")) m_max_file_backups = opts.Get("maxFileBackups").As<Napi::Number>().Uint32Value();
        if (opts.Has("maxFileSize")) m_max_file_size = opts.Get("maxFileSize").As<Napi::Number>().Uint32Value();
        if (opts.Has("flushInterval")) m_flush_interval = opts.Get("flushInterval").As<Napi::Number>().Uint32Value();
        if (opts.Has("asyncMode")) m_async_mode = opts.Get("asyncMode").As<Napi::Boolean>().Value();
    }
    else
    {
        // 报错
        NAPI_THROW(
                Napi::TypeError::New(info.Env(), "parameter is not a valid object"));
    }
}

Napi::Value LogConfig::GetName(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), m_name);
}

Napi::Value LogConfig::GetConsole(const Napi::CallbackInfo &info)
{
    return Napi::Boolean::New(info.Env(), m_console);
}
Napi::Value LogConfig::GetFilename(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), m_filename);
}
Napi::Value LogConfig::GetLevel(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), m_level);
}
Napi::Value LogConfig::GetPattern(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), m_pattern);
}
Napi::Value LogConfig::GetSetDefault(const Napi::CallbackInfo &info)
{
    return Napi::Boolean::New(info.Env(), m_set_default);
}
Napi::Value LogConfig::GetMaxFileBackups(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), m_max_file_backups);
}
Napi::Value LogConfig::GetMaxFileSize(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), m_max_file_size);
}
Napi::Value LogConfig::GetFlushInterval(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), m_flush_interval);
}
Napi::Value LogConfig::GetAsyncMode(const Napi::CallbackInfo &info)
{
    return Napi::Boolean::New(info.Env(), m_async_mode);
}

void LogConfig::SetName(const Napi::CallbackInfo &info, const Napi::Value &name)
{
    m_name = name.As<Napi::String>().Utf8Value();
}
void LogConfig::SetConsole(const Napi::CallbackInfo &info, const Napi::Value &console)
{
    m_console = console.As<Napi::Boolean>().Value();
}
void LogConfig::SetFilename(const Napi::CallbackInfo &info, const Napi::Value &filename)
{
    m_filename = filename.As<Napi::String>().Utf8Value();
}
void LogConfig::SetLevel(const Napi::CallbackInfo &info, const Napi::Value &level)
{
    m_level = level.As<Napi::String>().Utf8Value();
}
void LogConfig::SetPattern(const Napi::CallbackInfo &info, const Napi::Value &pattern)
{
    m_pattern = pattern.As<Napi::String>().Utf8Value();
}
void LogConfig::SetSetDefault(const Napi::CallbackInfo &info, const Napi::Value &setDefault)
{
    m_set_default = setDefault.As<Napi::Boolean>().Value();
}
void LogConfig::SetMaxFileBackups(const Napi::CallbackInfo &info, const Napi::Value &maxFileBackups)
{
    m_max_file_backups = maxFileBackups.As<Napi::Number>().Uint32Value();
}
void LogConfig::SetMaxFileSize(const Napi::CallbackInfo &info, const Napi::Value &maxFileSize)
{
    m_max_file_size = maxFileSize.As<Napi::Number>().Uint32Value();
}
void LogConfig::SetFlushInterval(const Napi::CallbackInfo &info, const Napi::Value &flushInterval)
{
    m_flush_interval = flushInterval.As<Napi::Number>().Uint32Value();
}

Napi::Object LogLevel::Initialize(Napi::Env env, Napi::Object exports)
{
    // 导出日志级别信息
    Napi::Object LevelEnum = Napi::Object::New(env);
    LevelEnum.Set("TRACE", Napi::Number::New(env, SPDLOG_LEVEL_TRACE));
    LevelEnum.Set("DEBUG", Napi::Number::New(env, SPDLOG_LEVEL_DEBUG));
    LevelEnum.Set("INFO", Napi::Number::New(env, SPDLOG_LEVEL_INFO));
    LevelEnum.Set("WARN", Napi::Number::New(env, SPDLOG_LEVEL_WARN));
    LevelEnum.Set("ERROR", Napi::Number::New(env, SPDLOG_LEVEL_ERROR));
    LevelEnum.Set("CRITICAL", Napi::Number::New(env, SPDLOG_LEVEL_CRITICAL));
    LevelEnum.Set("OFF", Napi::Number::New(env, SPDLOG_LEVEL_OFF));

    exports.Set("LogLevel", LevelEnum);

    return exports;
}

spdlog::level::level_enum LogLevel::ToLogLevel(const std::string &level)
{
    // 拷贝一份
    std::string lvStr = level;
    std::transform(lvStr.begin(), lvStr.end(), lvStr.begin(), ::toupper);

    static std::unordered_map<std::string, spdlog::level::level_enum> s_levelMap = {
            {"TRACE", spdlog::level::trace},
            {"DEBUG", spdlog::level::debug},
            {"INFO", spdlog::level::info},
            {"WARN", spdlog::level::warn},
            {"ERROR", spdlog::level::err},
            {"CRITICAL", spdlog::level::critical},
            {"OFF", spdlog::level::off},
    };
    auto itr = s_levelMap.find(lvStr);
    if (itr == s_levelMap.end())
    {
        return spdlog::level::info;
    }
    return itr->second;
}

std::string LogLevel::ToLogLevelString(const spdlog::level::level_enum &level)
{
    static std::unordered_map<spdlog::level::level_enum, std::string> s_levelMap = {
            {spdlog::level::trace, "TRACE"},
            {spdlog::level::debug, "DEBUG"},
            {spdlog::level::info, "INFO"},
            {spdlog::level::warn, "WARN"},
            {spdlog::level::err, "ERROR"},
            {spdlog::level::critical, "CRITICAL"},
            {spdlog::level::off, "OFF"},
    };
    auto itr = s_levelMap.find(level);
    if (itr == s_levelMap.end())
    {
        return "INFO";
    }
    return itr->second;
}

Napi::Object LoggerSink::Initialize(Napi::Env env, Napi::Object exports)
{
    // 导出日志Sink类型枚举
    Napi::Object SinkEnum = Napi::Object::New(env);
    SinkEnum.Set("CONSOLE", Napi::Number::New(env, static_cast<int32_t>(Sink::CONSOLE)));
    SinkEnum.Set("FILE", Napi::Number::New(env, static_cast<int32_t>(Sink::FILE)));
    SinkEnum.Set("ROTATING_FILE", Napi::Number::New(env, static_cast<int32_t>(Sink::ROTATING_FILE)));
    SinkEnum.Set("DAILY_FILE", Napi::Number::New(env, static_cast<int32_t>(Sink::DAILY_FILE)));
    SinkEnum.Set("HOURLY_FILE", Napi::Number::New(env, static_cast<int32_t>(Sink::HOURLY_FILE)));

    exports.Set("Sink", SinkEnum);

    return exports;
}

LoggerSink::Sink LoggerSink::ToSink(const std::string &sink)
{
    // 拷贝一份
    std::string sinkStr = sink;
    std::transform(sinkStr.begin(), sinkStr.end(), sinkStr.begin(), ::toupper);

    static std::unordered_map<std::string, LoggerSink::Sink> s_sinkMap = {
            {"CONSOLE", LoggerSink::Sink::CONSOLE},
            {"STDOUT", LoggerSink::Sink::CONSOLE},
            {"FILE", LoggerSink::Sink::FILE},
            {"ROTATING_FILE", LoggerSink::Sink::ROTATING_FILE},
            {"DAILY_FILE", LoggerSink::Sink::DAILY_FILE},
            {"HOURLY_FILE", LoggerSink::Sink::HOURLY_FILE},
    };

    auto itr = s_sinkMap.find(sinkStr);
    if (itr == s_sinkMap.end())
    {
        return LoggerSink::Sink::CONSOLE;
    }
    return itr->second;
}

std::string LoggerSink::ToSinkString(const LoggerSink::Sink &sink)
{
    static std::unordered_map<LoggerSink::Sink, std::string> s_sinkMap = {
            {LoggerSink::Sink::CONSOLE, "CONSOLE"},
            {LoggerSink::Sink::FILE, "FILE"},
            {LoggerSink::Sink::ROTATING_FILE, "ROTATING_FILE"},
            {LoggerSink::Sink::DAILY_FILE, "DAILY_FILE"},
            {LoggerSink::Sink::HOURLY_FILE, "HOURLY_FILE"},
    };
    auto itr = s_sinkMap.find(sink);
    if (itr == s_sinkMap.end())
    {
        return "CONSOLE";
    }
    return itr->second;
}

/**
 * @brief 初始化日志记录器
 * @param env Node.js 环境对象
 * @param exports Node.js 导出对象
 * @return Napi::Object 导出对象
 * */
Napi::Object Logger::Initialize(Napi::Env env, Napi::Object exports)
{
    // 导出 Logger
    Napi::Function prototype = DefineClass(
            env,
            "Logger",
            {
                    // 静态函数部分
                    StaticMethod<&Logger::StaticCritical>("critical"),
                    StaticMethod<&Logger::StaticError>("error"),
                    StaticMethod<&Logger::StaticWarn>("warn"),
                    StaticMethod<&Logger::StaticInfo>("info"),
                    StaticMethod<&Logger::StaticDebug>("debug"),
                    StaticMethod<&Logger::StaticTrace>("trace"),
                    StaticMethod<&Logger::StaticFlush>("flush"),
                    StaticMethod<&Logger::StaticFlushOn>("flushOn"),
                    StaticMethod<&Logger::StaticFlushEvery>("flushEvery"),
                    StaticMethod<&Logger::StaticSetDefault>("setDefault"),
                    StaticMethod<&Logger::StaticDropAll>("dropAll"),
                    // 静态成员访问器 getter/setter
                    StaticAccessor<&Logger::StaticGetLevel, &Logger::StaticSetLevel>("level"),
                    StaticAccessor<nullptr, &Logger::StaticSetPattern>("pattern"),
                    // 成员函数部分
                    InstanceMethod<&Logger::Critical>("critical"),
                    InstanceMethod<&Logger::Error>("error"),
                    InstanceMethod<&Logger::Warn>("warn"),
                    InstanceMethod<&Logger::Info>("info"),
                    InstanceMethod<&Logger::Debug>("debug"),
                    InstanceMethod<&Logger::Trace>("trace"),
                    InstanceMethod<&Logger::FlushOn>("flushOn"),
                    InstanceMethod<&Logger::Flush>("flush"),
                    // 成员访问器 getter/setter
                    InstanceAccessor<&Logger::GetLevel, &Logger::SetLevel>("level"),
                    InstanceAccessor<nullptr, &Logger::SetPattern>("pattern"),
            });

    exports.Set("Logger", prototype);

    return exports;
}

Logger::Logger(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Logger>(info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsObject())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "Logger constructor requires LogConfig argument"));
    }
    // js 对象解包 LogConfig
    LogConfig *log_conf = Napi::ObjectWrap<LogConfig>::Unwrap(info[0].As<Napi::Object>());
    // 所有的 sinks 用于初始化 logger
    std::vector<spdlog::sink_ptr> sinks;
    // sink 类型
    auto sink_type = LoggerSink::ToSink(log_conf->m_sink);
    auto level = LogLevel::ToLogLevel(log_conf->m_level);
    uint32_t flush_interval = log_conf->m_flush_interval;

    // 文件日志输出
    switch (sink_type)
    {
        case LoggerSink::Sink::FILE:
        {
            auto file_sink_ptr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_conf->m_filename);
            sinks.push_back(file_sink_ptr);
            break;
        }
        case LoggerSink::Sink::ROTATING_FILE:
        {
            auto rotating_sink_ptr = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    log_conf->m_filename,
                    log_conf->m_max_file_size,
                    log_conf->m_max_file_backups);
            sinks.push_back(rotating_sink_ptr);
            break;
        }
        case LoggerSink::Sink::DAILY_FILE:
        {
            auto daily_sink_ptr = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                    log_conf->m_filename, 0, 0);
            sinks.push_back(daily_sink_ptr);
            break;
        }
        case LoggerSink::Sink::HOURLY_FILE:
        {
            auto hourly_sink_ptr = std::make_shared<spdlog::sinks::hourly_file_sink_mt>(
                    log_conf->m_filename);
            sinks.push_back(hourly_sink_ptr);
            break;
        }
        default:
            break;
    }

    // 控制台日志输出 -- 默认彩色
    if (sink_type == LoggerSink::Sink::CONSOLE || log_conf->m_console || sinks.size() <= 0)
    {
        // 没有sink 默认输出到控制台
        auto console_sink_ptr = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        sinks.push_back(console_sink_ptr);
    }

    // 初始化 logger
    if (log_conf->m_async_mode)
    {
        static bool s_async_init;
        if (!s_async_init)
        {
            spdlog::init_thread_pool(8192, 1);
            s_async_init = true;
        }
        m_logger = std::make_shared<spdlog::async_logger>(
                log_conf->m_name,
                sinks.begin(),
                sinks.end(),
                spdlog::thread_pool(),
                spdlog::async_overflow_policy::block);
    }
    else
    {
        m_logger = std::make_shared<spdlog::logger>(
                log_conf->m_name,
                sinks.begin(),
                sinks.end());
    }

    // 设置属性
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "Logger constructor failed"));
    }

    m_logger->set_level(level);
    m_logger->set_pattern(log_conf->m_pattern);
    m_logger->flush_on(spdlog::level::err); // 默认出现 err 需要进行一次刷盘
    spdlog::flush_every(std::chrono::seconds(flush_interval));
    // 是否设置为默认
    if (log_conf->m_set_default)
    {
        spdlog::set_default_logger(m_logger);
    }
}

Logger::~Logger()
{
    if (m_logger == nullptr)
    {
        return;
    }
    try
    {
        m_logger->flush();
        spdlog::drop(m_logger->name());
    }
    catch (...)
    {
        // 静默处理 不在js调用栈 如有必要 输出日志到 stderr
    }
}

Napi::Value Logger::StaticCritical(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {

        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    // 这里不能使用宏 宏会输出cpp文件位置
    spdlog::critical(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::StaticError(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {

        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    spdlog::error(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::StaticWarn(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {

        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    spdlog::warn(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::StaticInfo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {

        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    spdlog::info(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::StaticDebug(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {

        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    spdlog::debug(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::StaticTrace(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    spdlog::trace(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

void Logger::StaticSetLevel(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    Napi::Env env = info.Env();
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return;
    }
    if (value.IsString())
    {
        auto lvStr = value.As<Napi::String>().Utf8Value();
        auto level = LogLevel::ToLogLevel(lvStr);
        default_logger->set_level(level);
    }
    else if (value.IsNumber())
    {
        auto level = value.As<Napi::Number>().Int32Value();
        default_logger->set_level(static_cast<spdlog::level::level_enum>(level));
    }
    else
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string or number expected"));
    }
}

Napi::Value Logger::StaticGetLevel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return env.Undefined();
    }

    return Napi::Number::New(env, spdlog::default_logger()->level());
}

void Logger::StaticSetPattern(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    Napi::Env env = info.Env();
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return;
    }
    if (value.IsString())
    {
        default_logger->set_pattern(value.As<Napi::String>().Utf8Value());
    }
    else
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"));
    }
}

Napi::Value Logger::StaticFlush(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return env.Undefined();
    }
    default_logger->flush();
    return env.Undefined();
}

Napi::Value Logger::StaticFlushOn(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "level expected"),
                env.Undefined());
    }
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return env.Undefined();
    }
    if (info[0].IsString())
    {
        auto lvStr = info[0].As<Napi::String>().Utf8Value();
        auto level = LogLevel::ToLogLevel(lvStr);
        default_logger->flush_on(level);
    }
    else if (info[0].IsNumber())
    {
        auto level = info[0].As<Napi::Number>().Int32Value();
        default_logger->flush_on(static_cast<spdlog::level::level_enum>(level));
    }
    else
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string or number expected"),
                env.Undefined());
    }
    return env.Undefined();
}

Napi::Value Logger::StaticFlushEvery(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "number expected"),
                env.Undefined());
    }
    auto default_logger = spdlog::default_logger();
    if (!default_logger)
    {
        return env.Undefined();
    }
    auto seconds = info[0].As<Napi::Number>().Int32Value();
    spdlog::flush_every(std::chrono::seconds(seconds));
    return env.Undefined();
}

Napi::Value Logger::StaticSetDefault(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsObject())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "row logger object expected"),
                env.Undefined());
    }
    auto logger = Napi::ObjectWrap<Logger>::Unwrap(info[0].As<Napi::Object>());
    if (!logger->m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    spdlog::set_default_logger(logger->m_logger);
    return env.Undefined();
}

Napi::Value Logger::StaticDropAll(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    auto default_logger = spdlog::default_logger();
    if (default_logger)
    {
        default_logger->flush();
    }
    spdlog::drop_all();
    return env.Undefined();
}

Napi::Value Logger::Critical(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->critical(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::Error(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->error(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::Warn(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->warn(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::Info(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->info(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::Debug(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->debug(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

Napi::Value Logger::Trace(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    if (info.Length() <= 0)
    {
        return env.Undefined();
    }
    if (!info[0].IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"),
                env.Undefined());
    }
    m_logger->trace(info[0].As<Napi::String>().Utf8Value());
    return env.Undefined();
}

void Logger::SetLevel(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"));
    }
    if (value.IsString())
    {
        auto lvStr = value.As<Napi::String>().Utf8Value();
        auto level = LogLevel::ToLogLevel(lvStr);
        m_logger->set_level(level);
    }
    else if (value.IsNumber())
    {
        auto level = value.As<Napi::Number>().Int32Value();
        m_logger->set_level(static_cast<spdlog::level::level_enum>(level));
    }
    else
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "number or string expected"));
    }
}

Napi::Value Logger::GetLevel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "logger not initialized"),
                env.Undefined());
    }
    return Napi::Number::New(env, static_cast<int>(m_logger->level()));
}

void Logger::SetPattern(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    Napi::Env env = info.Env();
    if (!value.IsString())
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string expected"));
        return;
    }
    if (m_logger)
    {
        m_logger->set_pattern(value.As<Napi::String>().Utf8Value());
    }
}

Napi::Value Logger::Flush(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!m_logger)
    {
        return env.Undefined();
    }
    m_logger->flush();
    return env.Undefined();
}

Napi::Value Logger::FlushOn(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() <= 0)
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "level expected"),
                env.Undefined());
    }
    if (!m_logger)
    {
        return env.Undefined();
    }
    if (info[0].IsString())
    {
        auto lvStr = info[0].As<Napi::String>().Utf8Value();
        auto level = LogLevel::ToLogLevel(lvStr);
        m_logger->flush_on(level);
    }
    else if (info[0].IsNumber())
    {
        auto level = info[0].As<Napi::Number>().Int32Value();
        m_logger->flush_on(static_cast<spdlog::level::level_enum>(level));
    }
    else
    {
        NAPI_THROW(
                Napi::TypeError::New(env, "string or number expected"),
                env.Undefined());
    }
    return env.Undefined();
}
