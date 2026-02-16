#!/bin/bash
set -e  # 遇到错误立即退出

# 获取脚本所在目录的绝对路径，确保从任何位置运行脚本都能正确找到项目根目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="${SCRIPT_DIR}/.."

BUILD_TYPE="Debug"
DO_CLEAN=0

# 解析参数
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
    -r|--release)
      BUILD_TYPE="Release"
      shift
      ;;
    -c|--clean)
      DO_CLEAN=1
      shift
      ;;
    -h|--help)
      echo "Usage: $0 [-r|--release] [-c|--clean]"
      exit 0
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

# 如果指定了清理
if [ $DO_CLEAN -eq 1 ]; then
    echo "Cleaning build artifacts..."
    rm -rf "${PROJECT_ROOT}/build"
    rm -rf "${PROJECT_ROOT}/bin"
fi

echo "Building in ${BUILD_TYPE} mode..."

# 创建必要的目录
mkdir -p "${PROJECT_ROOT}/build"
mkdir -p "${PROJECT_ROOT}/bin"

# 进入构建目录
cd "${PROJECT_ROOT}/build"

# 配置 CMake
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..

# 构建
cmake --build . -- -j$(nproc)

echo "Build finished successfully! Executables are in bin/"
