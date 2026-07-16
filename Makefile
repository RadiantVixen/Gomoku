NAME = Gomoku

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O3 $(shell pkg-config --cflags sdl2)
LIBS = $(shell pkg-config --libs sdl2)

SRC_DIR = src
OBJ_DIR = obj

# Find all .cpp files in src/ and its subdirectories
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/core/Board.cpp \
	$(SRC_DIR)/core/AI.cpp \
       $(SRC_DIR)/gui/GUI.cpp

# Map source files to object files in obj/
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Header dependencies to trigger recompile on header change
DEPS = $(SRC_DIR)/core/Board.hpp \
       $(SRC_DIR)/gui/GUI.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LIBS)

# Rule to compile object files and preserve subdirectory structure inside obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
