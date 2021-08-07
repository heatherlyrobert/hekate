/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tMY         my;
char        unit_answer [LEN_RECD];



/*====================------------------------------------====================*/
/*===----                        support functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char      verstring    [500];

char*        /*-> return library versio --------------[ leaf   [gs.420.012.00]*/ /*-[00.0000.012.!]-*/ /*-[--.---.---.--]-*/
PROG_version            (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang      ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}



/*====================------------------------------------====================*/
/*===----                      program startup                         ----===*/
/*====================------------------------------------====================*/
static void  o___STARTUP_________o () { return; }

char         /*-> very first setup -------------------[ shoot  [gz.633.201.0A]*/ /*-[00.0000.121.!]-*/ /*-[--.---.---.--]-*/
PROG__init              (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("hekete"  , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info     ("yVIKEYS" , yVIKEYS_version ());
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   /*---(yvikeys config)-----------------*/
   rc = yVIKEYS_init         (MODE_MAP);
   if (rc == 0)  rc = yVIKEYS_whoami       (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, P_SUFFIX, P_CONTENT, NULL, NULL, NULL);
   /*> if (rc == 0)  rc = yVIKEYS_srch_config  (api_yvikeys_searcher , api_yvikeys_unsearcher);   <*/
   DEBUG_PROG   yLOG_value    ("yvikeys"   , rc);
   if (rc <  0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rc);
      return rc;
   }
   my.mem_dump = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char         /*-> process the command line args ------[ ------ [gz.952.251.B4]*/ /*-[01.0000.121.!]-*/ /*-[--.---.---.--]-*/
PROG__args              (int a_argc, char *a_argv[])
{
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(locals)-------------------------*/
   char        rc          = 0;
   int         i           = 0;
   char       *a           = NULL;
   int         x_total     = 0;
   int         x_args      = 0;
   char        x_name      [LEN_FULL]   = "";
   char        t           [LEN_FULL]   = "";
   /*---(begin)--------------------------*/
   /*> FILE_rename ("");                                                              <*/
   rc = yVIKEYS_args (a_argc, a_argv);
   /*---(process)------------------------*/
   for (i = 1; i < a_argc; ++i) {
      a = a_argv[i];
      ++x_total;
      if (a[0] == '@')  continue;
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      ++x_args;
      if      (strcmp  (a, "--memory"     ) == 0)  {
         my.mem_dump = fopen ("/tmp/hekate_mem.dump", "wt");
      }
      /*> if      (strncmp (a, "-f"        ,10) == 0)  strlcpy (x_name , a_argv[++i], LEN_RECD);   <* 
       *> else if (strncmp (a, "-h"        ,10) == 0)  PROG_usage();                               <* 
       *> else if (strncmp (a, "--help"    ,10) == 0)  PROG_usage();                               <*/
      /*---(prefixes)--------------------*/
      /*> else if (strncmp (a, "--formula-"          , 10) == 0)  PROG_layout_set ("cli", "formula"  , a + 10);   <* 
       *> else if (strncmp (a, "--status-"           ,  9) == 0)  PROG_layout_set ("cli", "status"   , a +  9);   <* 
       *> else if (strncmp (a, "--command-"          , 10) == 0)  PROG_layout_set ("cli", "command"  , a + 10);   <* 
       *> else if (strncmp (a, "--layout-"           ,  9) == 0)  PROG_layout_set ("cli", "layout"   , a +  9);   <* 
       *> else if (strncmp (a, "--function-list"     ,  9) == 0)  CALC_func_list  ();                             <*/
      /*---(other)-----------------------*/
      /*> else if (a[0] != '-'                     )   strlcpy (x_name , a_argv[i]  , LEN_RECD);   <*/
   }
   DEBUG_ARGS  yLOG_value  ("entries"   , x_total);
   DEBUG_ARGS  yLOG_value  ("arguments" , x_args);
   if (x_args == 0) {
      DEBUG_ARGS  yLOG_note   ("no arguments identified");
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   return 0;
}

char         /*-> initialize program and variables ---[ ------ [gz.741.041.07]*/ /*-[00.0000.121.!]-*/ /*-[--.---.---.--]-*/
PROG__begin             (void)
{
   DEBUG_PROG  yLOG_enter (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         i, j, k;
   char        tmp         [100];
   /*---(overall)------------------------*/
   yVIKEYS_view_config   (P_NAMESAKE, P_VERNUM, YVIKEYS_CURSES, 0, 0, 0);
   yVIKEYS_map_config    (YVIKEYS_OFFICE, YVIKEYS_mapper, YVIKEYS_locator, YVIKEYS_addressor);
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   return 0;
}

char         /*-> initialize program and variables ---[ ------ [gz.421.001.08]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG__visual            (void)
{
   DEBUG_PROG  yLOG_enter (__FUNCTION__);
   YVIKEYS_init          ();
   yVIKEYS_view_basic    (YVIKEYS_MAIN , YVIKEYS_FLAT, YVIKEYS_TOPLEF, 0, YVIKEYS_main);
   yVIKEYS_view_simple   (YVIKEYS_XAXIS    , 0, YVIKEYS_xaxis);
   yVIKEYS_cmds_direct   (":layout min");
   yVIKEYS_cmds_direct   (":xaxis show");
   yVIKEYS_cmds_direct   (":status show");
   yVIKEYS_cmds_direct   (":command show");
   yVIKEYS_cmds_direct   (":modes show");
   yVIKEYS_cmds_direct   (":keys show");
   /*---(complete)-----------------------*/
   DEBUG_PROG  yLOG_exit  (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv[])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__init   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__args   (a_argc, a_argv);
   if (rc >= 0)  rc = PROG__begin  ();
   if (rc >= 0)  rc = PROG__visual ();
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      program driver                          ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVING________o () { return; }

char
PROG_driver             (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   YVIKEYS_mapper (YVIKEYS_INIT);
   yVIKEYS_map_refresh ();
   rc = yVIKEYS_main  ("keys", "every", NULL);
   return rc;
}

/*====================------------------------------------====================*/
/*===----                      program shutdown                        ----===*/
/*====================------------------------------------====================*/
static void  o___SHUTDOWN_______o () { return; }

char
PROG__cloak             (void)
{
   yVIKEYS_wrap ();
   return 0;
}

char
PROG__end               (void)
{
   /*---(header)-------------------------*/
   yLOG_enter (__FUNCTION__);
   /*---(stop logging)-------------------*/
   yLOG_info  ("logger",   "shutting down logger");
   yLOG_exit  (__FUNCTION__);
   yLOGS_end   ();
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG_shutdown           (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(shutdown)-----------------------*/
   if (rc >= 0)  rc = PROG__cloak  ();
   if (rc >= 0)  rc = PROG__end    ();
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char PROG_purge   (void)         { return SHARE_purge (TYPE_ALLS); }

char
PROG_review             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_rpid      =    0;
   tPROC      *x_new       = NULL;
   int         x_total     =    0;
   int         x_count     =    0;
   int         x_pass      =    0;
   char        x_name      [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   x_dir = opendir ("/proc");
   DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);
   --rce;  if (x_dir == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through processes");
   while ((x_file = readdir (x_dir)) != NULL) {
      DEBUG_INPT   yLOG_complex ("ENTRY"     , "%3d, %2då%sæ", x_total, strlen (x_file->d_name), x_file->d_name);
      ++x_total;
      rc = 0;
      /*---(simple filtering)-------------------*/
      if (strchr (YSTR_NUMBER, x_file->d_name [0]) == NULL) {
         DEBUG_INPT   yLOG_note    ("not leading number");
         continue;
      }
      x_rpid = atoi (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <= 0) {
         DEBUG_INPT   yLOG_note    ("not a process entry");
         continue;
      }
      /*---(create)----------------------*/
      rc = DATA_driver (x_rpid, &x_new, '-');
      DEBUG_INPT   yLOG_value   ("driver"    , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("not a user-land entry");
         continue;
      }
      /*---(total)-----------------------*/
      ++x_count;
      if (rc >= 0)  ++x_pass;
      DEBUG_INPT   yLOG_complex ("counts"    , "%d total, %d count, %d pass", x_total, x_count, x_pass);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_INPT   yLOG_value   ("found"     , x_total);
   DEBUG_INPT   yLOG_value   ("processed" , x_count);
   DEBUG_INPT   yLOG_value   ("passed"    , x_pass);
   /*---(close)--------------------------*/
   rc = closedir (x_dir);
   DEBUG_INPT   yLOG_point   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}



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
   PROG_purge ();
   yLOGS_end     ();
   return 0;
}

