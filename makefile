# 定义目标文件
TARGET = rand
OBJ = main.o

# 编译器
CXX = clang++
CXXFLAGS = -std=c++17

# 默认目标
all: $(TARGET)

# 目标文件的生成规则
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# main.o 的编译规则
main.o: ../main.cpp
	$(CXX) -c ../main.cpp -Oz -o main.o $(CXXFLAGS)

# 清理目标
clean:
	rm -rf $(OBJ) $(TARGET)

# 声明伪目标
.PHONY: all clean
