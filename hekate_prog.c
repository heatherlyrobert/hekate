/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tMY         my;
char        unit_answer [LEN_RECD];



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char PROG_purge   (void)         { return SHARE_purge ('A'); }



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char       /*----: set up program urgents/debugging --------------------------*/
PROG__unit_quiet        (void)
{
   yLOGS_begin ("hekate", YLOG_SYS, YLOG_QUIET);
   yURG_err_none ();
   yURG_msg_none ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
PROG__unit_loud         (void)
{
   yLOGS_begin   ("hekate", YLOG_SYS, YLOG_NOISE);
   yURG_err_none ();
   yURG_msg_none ();
   /*> DEBUG_YEXEC  yLOG_info     ("hekate"    , PROG_version   ());                  <*/
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
PROG__unit_end          (void)
{
   PROC_purge ();
   yLOGS_end     ();
   return 0;
}

