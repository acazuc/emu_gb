NAME = emu_gb.so

CXX = gcc

CFLAGS = -std=c99 -Wall -Wextra -Ofast -pipe -g -fPIC

LD = ld

LDFLAGS = -shared -static-libgcc -Wl,--version-script=link.T -Wl,--no-undefined

INCLUDES = -I include

SRCS_PATH = src/

SRCS_NAME = libretro/libretro.c \
            apu.c \
            gpu.c \
            mem.c \
            mbc.c \
            cpu.c \
            gb.c \
            cpu/instr.c \
            cpu/misc.c \
            cpu/alu_16.c \
            cpu/alu_8.c \
            cpu/lsm_16.c \
            cpu/lsm_8.c \
            cpu/rsb_8.c \
            cpu/control.c \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.c=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo "LD dmgbios"
	@$(LD) -r -b binary -o dmgbios.o dmgbios.bin
	@echo "LD cgbbios"
	@$(LD) -r -b binary -o cgbbios.o cgbbios.bin
	@echo "LD $(NAME)"
	@$(LD) -fPIC -shared -o $(NAME) $(OBJS) dmgbios.o cgbbios.o

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)/libretro
	@mkdir -p $(OBJS_PATH)/cpu

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

re: clean all

.PHONY: all clean re odir
