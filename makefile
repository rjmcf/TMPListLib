SUB_PROJECT = untyped_lists
SRC_H_DIR = headers/$(SUB_PROJECT)
SRC_CPP_DIR = src/$(SUB_PROJECT)
TEST_H_DIR = test/headers/$(SUB_PROJECT)
TEST_SHARED_H_DIR = test/headers/shared
TEST_CPP_DIR = test/src/$(SUB_PROJECT)
OBJ_DIR = output
CFLAGS  = -Wall -I$(SRC_H_DIR) -I$(TEST_H_DIR) -I$(TEST_SHARED_H_DIR) -MMD -MP -std=c++14
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

todo:
	grep -rn "TODO" headers src test

-include $(patsubst $(SRC_CPP_DIR)/%,$(OBJ_DIR)/%,$(OBJS:.o=.d))
-include $(patsubst $(TEST_CPP_DIR)/%,$(OBJ_DIR)/%,$(TEST_OBJS:.o=.d))
