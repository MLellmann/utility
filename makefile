BIN ?= bin/release

LIB = libutility.a
SRCS = $(wildcard src/*.cpp)
OBJ = $(addprefix $(BIN)/,$(notdir $(SRCS:.cpp=.o))) 

GCC = g++
WARNING = -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls \
 -Wcast-align -Wunreachable-code -Wmissing-declarations \
 -Wmissing-include-dirs -Wmain \
 -pedantic-errors -pedantic -Wzero-as-null-pointer-constant -Wextra -Wall 
DEBUG ?= -DNDEBUG -O3 -fexpensive-optimizations -fomit-frame-pointer 
STD = -std=c++11
RM = rm -f
AR = ar
AR_OPT = -r

INC = -Iinclude

OPT = $(STD) $(WARNING) $(DEBUG) $(INC)

export BIN
export DEBUG

.PHONY: before clean debug clean-debug

$(BIN)/$(LIB): before $(OBJ) 
	@echo Linking library $(LIB) ...
	@$(RM) $(BIN)/$(LIB)
	@$(AR) $(AR_OPT) -s $(BIN)/$(LIB) $(OBJ)

$(BIN)/%.o: src/%.cpp 
	@echo Compiling $< ...
	@$(GCC) $(OPT) -MMD -c -o $@ $<
	@cp $(BIN)/$*.d $(BIN)/$*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $(BIN)/$*.d >> $(BIN)/$*.P; \
             rm -f $(BIN)/$*.d

-include $(BIN)/*.P

before:
	@mkdir -p bin
	@mkdir -p bin/release
	@mkdir -p bin/debug

clean:
	@$(RM) $(OBJ) $(BIN)/$(LIB) 
	$(MAKE)

debug:
	$(MAKE) BIN="bin/debug" DEBUG="-DDEBUG -g"

clean-debug:
	$(MAKE) clean BIN="bin/debug" DEBUG="-DDEBUG -g"

