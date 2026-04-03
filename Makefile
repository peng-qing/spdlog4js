.PHONY: all install submodule clean build rebuild configure test

# 默认目标
all: build

# 安装所有依赖（npm + git submodule）
install: submodule
	npm install

# 初始化并更新 git submodule
submodule:
	git submodule update --init --recursive

# 配置（生成 Makefile）
configure:
	node-gyp configure

# 编译
build: configure
	node-gyp build

# 清理构建缓存
clean:
	node-gyp clean
	rm -rf build
	rm -f compile_commands.json

# 重新构建（清理 + 编译）
rebuild: clean build

# 编译并生成 compile_commands.json（给 clangd 用）
bear: clean configure
	bear -- make -C build BUILDTYPE=Release V=1

# 测试
test: build
	node test.js

