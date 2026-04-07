declare module "spdlog4js" {
    export type LogLevelName =
        | "TRACE"
        | "DEBUG"
        | "INFO"
        | "WARN"
        | "ERROR"
        | "CRITICAL"
        | "OFF";

    export type SinkName =
        | "CONSOLE"
        | "STDOUT"
        | "FILE"
        | "ROTATING_FILE"
        | "DAILY_FILE"
        | "HOURLY_FILE";

    export interface LogConfigOptions {
        /** 日志器名称 @default "root" */
        name?: string;
        /** 输出目标 @default "stdout" */
        sink?: string;
        /** 是否同时输出到控制台 @default true */
        console?: boolean;
        /** 日志文件路径 @default "./logs/app.log" */
        filename?: string;
        /** 日志级别 @default "info" */
        level?: string;
        /** 日志格式 @default "[%Y-%m-%d %H:%M:%S.%f] [%^%l%$] %@ %v" */
        pattern?: string;
        /** 是否默认日志器 @default false */
        setDefault?: boolean;
        /** 最大文件备份数 @default 50 */
        maxFileBackups?: number;
        /** 最大文件大小 @default 1024 * 1024 * 500 */
        maxFileSize?: number;
        /** 刷新间隔 @default 3 */
        flushInterval?: number;
        /** 是否异步模式 @default false */
        asyncMode?: boolean;
    }

    export class LogConfig {
        constructor(options?: LogConfigOptions);
        get name(): string;
        set name(value: string);
        get sink(): string;
        set sink(value: string);
        get console(): boolean;
        set console(value: boolean);
        get filename(): string;
        set filename(value: string);
        get level(): string;
        set level(value: string);
        get pattern(): string;
        set pattern(value: string);
        get setDefault(): boolean;
        set setDefault(value: boolean);
        get maxFileBackups(): number;
        set maxFileBackups(value: number);
        get maxFileSize(): number;
        set maxFileSize(value: number);
        get flushInterval(): number;
        set flushInterval(value: number);
        get asyncMode(): boolean;
        set asyncMode(value: boolean);
    }

    export const LogLevel: {
        readonly TRACE: number;
        readonly DEBUG: number;
        readonly INFO: number;
        readonly WARN: number;
        readonly ERROR: number;
        readonly CRITICAL: number;
        readonly OFF: number;
    };

    export const Sink: {
        readonly CONSOLE: number;
        readonly FILE: number;
        readonly ROTATING_FILE: number;
        readonly DAILY_FILE: number;
        readonly HOURLY_FILE: number;
    };

    export class RowLogger {
        constructor(config: LogConfig);

        static critical(message: string): void;
        static error(message: string): void;
        static warn(message: string): void;
        static info(message: string): void;
        static debug(message: string): void;
        static trace(message: string): void;
        static flush(): void;
        static flushOn(level: number | string): void;
        static flushEvery(seconds: number): void;
        static setDefault(logger: Logger): void;
        static dropAll(): void;
        static get level(): number | undefined;
        static set level(value: number | string | undefined);
        static set pattern(value: string);

        critical(message: string): void;
        error(message: string): void;
        warn(message: string): void;
        info(message: string): void;
        debug(message: string): void;
        trace(message: string): void;
        flushOn(level: number | string): void;
        flush(): void;
        get level(): number;
        set level(value: number | string);
        set pattern(value: string);
    }

    export class Logger extends RowLogger {
        constructor(config: LogConfig);

        static critical(...message: string[]): void;
        static error(...message: string[]): void;
        static warn(...message: string[]): void;
        static info(...message: string[]): void;
        static debug(...message: string[]): void;
        static trace(...message: string[]): void;

        critical(...message: string[]): void;
        error(...message: string[]): void;
        warn(...message: string[]): void;
        info(...message: string[]): void;
        debug(...message: string[]): void;
        trace(...message: string[]): void;
    }
}
