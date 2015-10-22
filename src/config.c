enum {
    HIDE_BUTTONS,
    HIDE_MENU_BAR,
    OPTION_QT
};

enum { DEFAULT, SETTING };

gboolean config[OPTION_QT][2] = {
    /* HIDE_BUTTONS */ {FALSE,FALSE},
    /* HIDE_MENU_BAR */{FALSE,FALSE}
};

