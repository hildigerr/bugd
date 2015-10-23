#pragma once
#include <glib.h>
#include <confuse.h>

enum { DEFAULT, SETTING, DEFAULTxSETTING };

enum { HIDE_BUTTONS, HIDE_MENU_BAR, BOOLEAN_OPTION_QT };
enum { WINDOW_HEIGHT, WINDOW_WIDTH, INTEGER_OPTION_QT };
enum { HIDE_BUTTONS_S, HIDE_MENU_BAR_S, WINDOW_HEIGHT_S, WINDOW_WIDTH_S };

typedef struct {
    gboolean b[BOOLEAN_OPTION_QT][DEFAULTxSETTING];
    gint     i[INTEGER_OPTION_QT][DEFAULTxSETTING];
} CONF_T;

inline void configure_prefs( void );
