# 编译器和编译选项
CXX = g++
CXXFLAGS = -Wall -std=c++14
DEPENDLIB = -lcurl -lmodbus -lmosquittopp -lmosquitto -pthread -g

# 源文件和目标文件
COMMON_SRCS = enn_gen2_lightcontrol.cpp modbus_RTU.cpp mqtt_controller.cpp
MAIN_SRCS = main.cpp $(COMMON_SRCS)
#CONF_SRCS = enn_factory_init.cpp $(COMMON_SRCS)

MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
#CONF_OBJS = $(CONF_SRCS:.cpp=.o)

# 可执行文件名称
TARGET = lightController
#CONF_TARGET = enn_light_configutation

# 查找所有头文件
HEADERS = $(wildcard *.h)

# 默认目标
all: $(TARGET) $(CONF_TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPENDLIB)

#$(CONF_TARGET): $(CONF_OBJS)
#	$(CXX) $(CXXFLAGS) -o $@ $^ $(DEPENDLIB)

# 编译源文件生成目标文件，并处理头文件依赖
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(DEPENDLIB)

# 清理生成的文件
conf_clean:
#	rm -f $(CONF_OBJS) $(CONF_TARGET)

main_clean:
	rm -f $(MAIN_OBJS) $(TARGET)

all_clean: conf_clean main_clean

clean: all_clean
    
