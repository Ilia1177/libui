#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "libui.h"

char *ui_get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm = localtime(&tv.tv_sec);
    static char buf[64];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d",
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        tv.tv_usec / 1000  // microseconds → milliseconds
    );
    return buf;
}

void ui_log_eventstr(SDL_Event *e)
{
    if (!e) {
		ui_log("null");
        return;
	}
    switch (e->type)
    {
    case SDL_QUIT:                  ui_log("SDL_QUIT"); break;
    case SDL_KEYDOWN:               ui_log("SDL_KEYDOWN"); break;
    case SDL_KEYUP:                 ui_log("SDL_KEYUP"); break;
    case SDL_MOUSEMOTION:           ui_log("SDL_MOUSEMOTION"); break;
    case SDL_MOUSEBUTTONDOWN:       ui_log("SDL_MOUSEBUTTONDOWN"); break;
    case SDL_MOUSEBUTTONUP:         ui_log("SDL_MOUSEBUTTONUP"); break;
    case SDL_MOUSEWHEEL:            ui_log("SDL_MOUSEWHEEL"); break;
    case SDL_WINDOWEVENT:
        switch (e->window.event)
        {
        case SDL_WINDOWEVENT_SHOWN:         ui_log("SDL_WINDOWEVENT_SHOWN"); break;
        case SDL_WINDOWEVENT_HIDDEN:        ui_log("SDL_WINDOWEVENT_HIDDEN"); break;
        case SDL_WINDOWEVENT_EXPOSED:       ui_log("SDL_WINDOWEVENT_EXPOSED"); break;
        case SDL_WINDOWEVENT_MOVED:         ui_log("SDL_WINDOWEVENT_MOVED"); break;
        case SDL_WINDOWEVENT_RESIZED:       ui_log("SDL_WINDOWEVENT_RESIZED"); break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:  ui_log("SDL_WINDOWEVENT_SIZE_CHANGED"); break;
        case SDL_WINDOWEVENT_MINIMIZED:     ui_log("SDL_WINDOWEVENT_MINIMIZED"); break;
        case SDL_WINDOWEVENT_MAXIMIZED:     ui_log("SDL_WINDOWEVENT_MAXIMIZED"); break;
        case SDL_WINDOWEVENT_RESTORED:      ui_log("SDL_WINDOWEVENT_RESTORED"); break;
        case SDL_WINDOWEVENT_ENTER:         ui_log("SDL_WINDOWEVENT_ENTER"); break;
        case SDL_WINDOWEVENT_LEAVE:         ui_log("SDL_WINDOWEVENT_LEAVE"); break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:  ui_log("SDL_WINDOWEVENT_FOCUS_GAINED"); break;
        case SDL_WINDOWEVENT_FOCUS_LOST:    ui_log("SDL_WINDOWEVENT_FOCUS_LOST"); break;
        case SDL_WINDOWEVENT_CLOSE:         ui_log("SDL_WINDOWEVENT_CLOSE"); break;
        default:                            ui_log("SDL_WINDOWEVENT_UNKNOWN"); break;
        }
        break;
    case SDL_TEXTINPUT:             ui_log("SDL_TEXTINPUT"); break;
    case SDL_TEXTEDITING:           ui_log("SDL_TEXTEDITING"); break;
    case SDL_FINGERDOWN:            ui_log("SDL_FINGERDOWN"); break;
    case SDL_FINGERUP:              ui_log("SDL_FINGERUP"); break;
    case SDL_FINGERMOTION:          ui_log("SDL_FINGERMOTION"); break;
    case SDL_DROPFILE:              ui_log("SDL_DROPFILE"); break;
    case SDL_DROPTEXT:              ui_log("SDL_DROPTEXT"); break;
    case SDL_DROPBEGIN:             ui_log("SDL_DROPBEGIN"); break;
    case SDL_DROPCOMPLETE:          ui_log("SDL_DROPCOMPLETE"); break;
    case SDL_USEREVENT:             ui_log("SDL_USEREVENT"); break;
    default:                        ui_log("SDL_UNKNOWN"); break;
    }
}
void ui_log(const char* msg) {
	printf("%s %s\n", ui_get_time(), msg);
	fflush(stdout);
}
