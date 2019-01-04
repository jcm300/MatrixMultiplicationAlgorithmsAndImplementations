CC=nvcc
CFLAGS=-O2 -std=c++11
SRC_DIR=src
BIN_DIR=bin
DP=dot_product
DP_1=dot_product_1
DP_2_NT=dot_product_2_no_transpose
DP_2_T=dot_product_2_transpose
DEPS=testing_utils
OS:= $(shell uname -s)

.DEFAULT_GOAL = all

$(BIN_DIR)/$(DP_1): $(SRC_DIR)/$(DP_1).cu
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DP_1).cu -o $(DP_1)
	mv $(DP_1) $(BIN_DIR)

checkdirs:
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(DP_1)

clean:
	rm -rf $(BIN_DIR)/
