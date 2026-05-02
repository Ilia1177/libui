
NAME		=	libui.a

SRC_DIR		= src/
OBJS_DIR	= obj/
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

SRC		= window/ui_win_create.c\
		  window/ui_win_destroy.c\
		  window/ui_win_add.c\
		  window/ui_win_scale.c\
		  window/ui_win_render.c\
		  window/ui_win_pos.c\
		  window/ui_win_size.c\
		  window/ui_win_update.c\
		  window/ui_win_event.c\
		  global/ui_globalApp.c\
		  ui_init.c\
		  ui_quit.c\
		  ui_is_mouse_in.c\
		  box/ui_box_add.c\
		  box/ui_box_create.c\
		  box/ui_box_event.c\
		  box/ui_box_menu.c\
		  box/ui_box_update.c\
		  box/ui_box_iter.c\
		  box/ui_box_render.c\
		  box/ui_box_destroy.c\

SRC := $(addprefix $(SRC_DIR),$(SRC) )

OBJS		=	$(SRC:%.c=$(OBJS_DIR)/%.o)

AR 			=	ar -rcs

CC			=	cc 

CFLAGS		=	-Wall -Wextra -Werror -g

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
