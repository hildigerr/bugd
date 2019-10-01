
#include "config.h"
#include <errno.h>
#include <string.h>

CONF_T config;

char* options[] = {
    /* HIDE_BUTTONS_S  */"hideButtons",
    /* HIDE_MENU_BAR_S */"hideMenuBar",
    /* WINDOW_HEIGHT_S */"windowHeight",
    /* WINDOW_WIDTH_S  */"windowWidth"
};

void set_conf( cfg_t* cfg )
{
    if( cfg ) {
        config.b[HIDE_BUTTONS][SETTING] = cfg_getbool(cfg, options[HIDE_BUTTONS_S]);
        config.b[HIDE_MENU_BAR][SETTING] = cfg_getbool(cfg,options[HIDE_MENU_BAR_S]);
        config.i[WINDOW_HEIGHT][SETTING] = cfg_getint(cfg,options[WINDOW_HEIGHT_S]);
        config.i[WINDOW_WIDTH][SETTING] = cfg_getint(cfg,options[WINDOW_WIDTH_S]);
    } else { /* Settings File Not Parsed */
        config.b[HIDE_BUTTONS][DEFAULT] = config.b[HIDE_BUTTONS][SETTING] = FALSE;
        config.b[HIDE_MENU_BAR][DEFAULT] = config.b[HIDE_MENU_BAR][SETTING] = FALSE;
        config.i[WINDOW_HEIGHT][DEFAULT] = config.i[WINDOW_HEIGHT][SETTING] = 373;
        config.i[WINDOW_WIDTH][DEFAULT] = config.i[WINDOW_WIDTH][SETTING] = 273;
    }
}/* End conf_init Func */

int load_conf( char* config_file )
{
    cfg_opt_t opts[] = {
        CFG_BOOL(options[HIDE_BUTTONS_S], config.b[HIDE_BUTTONS][DEFAULT], CFGF_NONE),
        CFG_BOOL(options[HIDE_MENU_BAR_S], config.b[HIDE_MENU_BAR][DEFAULT], CFGF_NONE),
        CFG_INT(options[WINDOW_HEIGHT_S], config.i[WINDOW_HEIGHT][DEFAULT], CFGF_NONE),
        CFG_INT(options[WINDOW_WIDTH_S], config.i[WINDOW_WIDTH][DEFAULT], CFGF_NONE),
        //CFG_XXX(options[NEW_OPTION_S], config.X[NEW_OPTION][DEFAULT], CFGF_NONE),
        CFG_END()
    };
    cfg_t* cfg = cfg_init(opts, CFGF_NONE);

    if( cfg_parse(cfg, config_file) == CFG_PARSE_ERROR ) return 0;
    else set_conf(cfg);
    cfg_free(cfg);
    return 1;
}/* End load_conf Func */


#define USER_CFG 0
#define ROOT_CFG 1
#define NAMESTRINGS 2

void configure_prefs( void )
{
    char* file[] = { "~/.config/bugd.cfg", "/etc/bugd.cfg" };
    FILE* fh = NULL;
    int i;

    set_conf( NULL );
    for( i = 0; i < NAMESTRINGS; i++ ) {
        if( load_conf(file[i]) ) {
            fprintf( stderr, "Loaded configuration file: \"%s\"\n", file[i] );
            break;
        } else fprintf( stderr, "Did not load preferences [\"%s\"]: %s.\n", file[i], "No such file or directory" );
    }/* End NAMESTRINGS For */

}/* End configure_prefs Func */
