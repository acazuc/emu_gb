NAME = emu_gb.so

CXX = gcc

CFLAGS = -std=c99 -Wall -Wextra -O3 -pipe -g -fPIC

LD = ld

LDFLAGS = -shared -static-libgcc -Wl,--version-script=link.T -Wl,--no-undefined

INCLUDES_PATH = -I src
INCLUDES_PATH+= -I lib

SRCS_PATH = src/

SRCS_NAME = libretro/libretro.c \
            apu.c \
            gpu.c \
            mem.c \
            rom.c \
            z80.c \
            gb.c \
            z80/instr.c \
            z80/misc.c \
            z80/alu_16.c \
            z80/alu_8.c \
            z80/lsm_16.c \
            z80/lsm_8.c \
            z80/rsb_8.c \
            z80/control.c \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.c=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo "LD $(NAME)"
	@$(LD) -fPIC -shared -o $(NAME) $(OBJS)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES_PATH)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)/libretro
	@mkdir -p $(OBJS_PATH)/z80

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

re: clean all

.PHONY: all clean re odir
