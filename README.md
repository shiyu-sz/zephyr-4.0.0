下载源码：https://github.com/zephyrproject-rtos/zephyr/releases  

下载SDK：https://github.com/zephyrproject-rtos/sdk-ng/releases  

官方教程：https://docs.zephyrproject.org/latest/develop/getting_started/index.html  

安装依赖：sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
  xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1  

创建一个新的虚拟环境：python3 -m venv ~/zephyrproject/.venv  

激活虚拟环境：source ~/zephyrproject/.venv/bin/activate  

安装west：pip install west  

解压源码，使用west init -l 使用本地版本  

更新代码：west update  

导出：Zephyr CMake 包：west zephyr-export  

安装zephyr-sdk：west sdk install  

或下载sdk手动安装：https://github.com/zephyrproject-rtos/sdk-ng/releases  

./setup.sh  

安装python依赖：pip install -r ./scripts/requirements.txt  

加载环境变量：source zephyr-env.sh  

查看支持的板卡：west boards | grep f401  

编译：west build -p always -b stm32f401_mini samples/basic/blinky  

生成的hex文件在：build/zephyr/zephyr.hex  
