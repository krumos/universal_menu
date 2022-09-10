CC := gcc
.PHONY : clean
CFLAGS := -std=gnu11 -I headers/

SRC_DIR := source/
INC_DIR := headers/
OUT_DIR := objects/

SRCS := $(wildcard $(SRC_DIR)*.c)
OBJS := $(patsubst $(SRC_DIR)%.c, $(OUT_DIR)%.o, $(SRCS))

DS := $(patsubst $(SRC_DIR)%.c, $(OUT_DIR)%.d, $(SRCS))
mkdir_out := $(shell mkdir -p $(OUT_DIR))

APP = app.exe

release: $(APP)

$(APP) : $(OBJS)
	$(CC) -o $@ $^

$(OUT_DIR)%.o : $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.d : %.c
	$(CC) -I headers/ -MD $< > $@

clean :
	rm -r $(OUT_DIR)
	rm -f *.exe
	rm -f *.d