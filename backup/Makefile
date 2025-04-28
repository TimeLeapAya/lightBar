# 编译器和编译选项
CXX = g++
CXXFLAGS = -Wall -std=c++11

# 源文件和目标文件
# enn_factory_init.cpp  enn_gen2_lightcontrol.cpp  main.cpp  modbus_RTU.cpp
COMMON_SRCS = enn_gen2_lightcontrol.cpp modbus_RTU.cpp

MAIN_SRCS = main.cpp $(COMMON_SRCS)

CONF_SRCS = enn_factory_init.cpp $(COMMON_SRCS)

MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
CONF_OBJS = $(CONF_SRCS:.cpp=.o)

# 可执行文件名称
TARGET = lightController
CONF_TARGET = enn_light_configutation

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(MAIN_OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

$(CONF_TARGET): $(CONF_OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^


# 编译源文件生成目标文件
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
conf_clean:
	rm -f $(CONF_OBJS) $(CONF_TARGET)

all_clean:
	rm -f $(CONF_OBJS) $(CONF_TARGET) $(MAIN_OBJS) $(TARGET)
   
clean:
	$(MAIN_OBJS) $(TARGET)


