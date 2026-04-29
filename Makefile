
NAME		=	libui.a

SRC_DIR		= src/
OBJS_DIR	=	obj/
INC_DIR		= inc/
BREW_PREFIX = /opt/homebrew

SDL2_CFLAGS  = $(shell sdl2-config --cflags)
SDL2_LIBS    = $(shell sdl2-config --libs)

INCS =	-I$(BREW_PREFIX)/include \
		-I$(INC_DIR) \
		-I$(SRC_DIR) \
		-I/usr/local/include \
		-I$(HOME)/.local/include

LDFLAGS  =	$(SDL2_LIBS) \
    		-L$(BREW_PREFIX)/lib \
			-L$(HOME)/.local/lib \
    		-lSDL2_image \

SRC		= ui_winCreate.c

SRC := $(addprefix $(SRC_DIR),$(SRC) )

OBJS		=	$(SRC:%.c=$(OBJS_DIR)/%.o)

AR 			=	ar -rcs

CC			=	cc 

CFLAGS		=	-Wall -Wextra -Werror

RM			=	rm -f

all				:	$(NAME) 

$(NAME)			:	$(OBJS)
					$(AR) $(NAME) $^

$(OBJS_DIR)/%.o	: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean	:	
			$(RM) $(OBJS) $(B_OBJS)
			rm -rf $(OBJS_DIR)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
