SRC_H_DIR = headers
SRC_CPP_DIR = src
TEST_H_DIR = test/headers
TEST_CPP_DIR = test/src
OBJ_DIR = output
CFLAGS  = -Wall -I$(SRC_H_DIR) -I$(TEST_H_DIR) -std=c++14
SRCS = $(wildcard $(SRC_CPP_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_CPP_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_CPP_DIR)/%,$(OBJ_DIR)/%,$(SRCS:.cpp=.o))
TEST_OBJS = $(patsubst $(TEST_CPP_DIR)/%,$(OBJ_DIR)/%,$(TEST_SRCS:.cpp=.o))
ENTRY_POINT = main
TEST_ENTRY_POINT = testEntry

run: allObjs
	$(CXX) $(CFLAGS) $(OBJS) -o $(OBJ_DIR)/$(ENTRY_POINT)
	$(OBJ_DIR)/$(ENTRY_POINT)

allObjs: $(OBJS)

runTests: allObjs allTestObjs
	$(CXX) $(CFLAGS) $(filter-out $(OBJ_DIR)/$(ENTRY_POINT).o,$(OBJS)) $(TEST_OBJS) -o $(OBJ_DIR)/$(TEST_ENTRY_POINT)
	$(OBJ_DIR)/$(TEST_ENTRY_POINT)

allTestObjs: $(TEST_OBJS)

$(OBJ_DIR)/%.o: $(SRC_CPP_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(TEST_CPP_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm $(OBJ_DIR)/*
