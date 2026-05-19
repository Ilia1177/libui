
NAME		=	libui.a

SRC_DIR		= src/
OBJS_DIR	= obj/
INC_DIR		= inc/
BREW_PREFIX = /opt/homebrew

SDL2_CFLAGS  = $(shell sdl2-config --cflags)
SDL2_CFLAGS += $(shell pkg-config --cflags SDL2_ttf)
SDL2_LIBS    = $(shell sdl2-config --libs)
SDL2_LIBS   += $(shell pkg-config --libs SDL2_ttf)

INCS =	-I$(BREW_PREFIX)/include \
		-I$(BREW_PREFIX)/include/SDL2 \
		-I$(INC_DIR) \
		-I$(SRC_DIR) \
		-I../libft/include \
		-I/usr/local/include \
		-I$(HOME)/.local/include

LDFLAGS  =	$(SDL2_LIBS) \
    		-L$(BREW_PREFIX)/lib \
			-L$(HOME)/.local/lib \
    		-lSDL2_image \
			-L../libft/bin -lft \

# CFLAGS += -fsanitize=address -fno-omit-frame-pointer
# LDFLAGS += -fsanitize=address

SRC		= window/ui_win_create.c\
		  window/ui_win_destroy.c\
		  window/ui_win_add.c\
		  window/ui_win_scale.c\
		  window/ui_win_pos.c\
		  window/ui_win_size.c\
		  window/ui_text.c\
		  elem/ui_elem.c\
		  elem/ui_belem_slider.c\
		  global/ui_globalApp.c\
		  hook/ui_bhook_mousemotion_default.c\
		  hook/ui_bhook_clickdown_default.c\
		  hook/ui_bhook_clickup_default.c\
		  hook/ui_bhook_update_default.c\
		  hook/ui_bhook_render_default.c\
		  hook/ui_bhook_windowevent_default.c\
		  hook/ui_bhook_mousewheel_default.c\
		  hook/ui_bhook.c\
		  hook/ui_bhook_utils.c\
		  hook/ui_bhook_default.c\
		  hook/ui_whook.c\
		  hook/ui_whook_utils.c\
		  hook/ui_whook_update_default.c\
		  hook/ui_whook_render_default.c\
		  hook/ui_whook_clickup_default.c\
		  hook/ui_whook_clickdown_default.c\
		  hook/ui_whook_keydown_default.c\
		  hook/ui_whook_mousemotion_default.c\
		  hook/ui_whook_mousewheel_default.c\
		  hook/ui_whook_windowevent_default.c\
		  ui_init.c\
		  ui_quit.c\
		  utils.c\
		  ui_get_time.c\
		  ui_is_mouse_in.c\
		  box/ui_box_utils.c\
		  box/ui_box_add.c\
		  box/ui_box_create.c\
		  box/ui_box_menu.c\
		  box/ui_box_iter.c\
		  box/ui_box_destroy.c\
		  layer/ui_layer.c\

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

test	: re
	$(CC) $(CFLAGS) $(SDL2_CFLAG) $(INCS) src/test.c -o test -L. -lui $(LDFLAGS)
	./test

.PHONY	:	all clean fclean re test
