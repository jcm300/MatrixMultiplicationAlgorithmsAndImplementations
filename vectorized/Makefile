CC=icpc
CFLAGS=-O2 -qopt-report=3
SRC_DIR=src
BIN_DIR=bin
DP=dot_product
DP_1=dot_product_1
DP_2_NT=dot_product_2_no_transpose
DP_2_T=dot_product_2_transpose
DEPS=testing_utils
OS:= $(shell uname -s)

$(DEPS): $(wildcard $(SRC_DIR)/$(DEPS).[hc])
.DEFAULT_GOAL = all

$(BIN_DIR)/$(DP): $(SRC_DIR)/$(DP).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DP).c $(SRC_DIR)/$(DEPS).c -o $(DP)
	mv $(DP) $(BIN_DIR)

$(BIN_DIR)/$(DP_1): $(SRC_DIR)/$(DP_1).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DP_1).c $(SRC_DIR)/$(DEPS).c -o $(DP_1)
	mv $(DP_1) $(BIN_DIR)

$(BIN_DIR)/$(DP_2_NT): $(SRC_DIR)/$(DP_2_NT).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DP_2_NT).c $(SRC_DIR)/$(DEPS).c -o $(DP_2_NT)
	mv $(DP_2_NT) $(BIN_DIR)

$(BIN_DIR)/$(DP_2_T): $(SRC_DIR)/$(DP_2_T).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DP_2_T).c $(SRC_DIR)/$(DEPS).c -o $(DP_2_T)
	mv $(DP_2_T) $(BIN_DIR)

checkdirs:
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(DP) $(BIN_DIR)/$(DP_1) $(BIN_DIR)/$(DP_2_NT) $(BIN_DIR)/$(DP_2_T) 

clean:
	rm -rf $(BIN_DIR)/
