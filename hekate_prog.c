/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tMY         my;
char        unit_answer [LEN_RECD];



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char PROG_purge   (void)         { return SHARE_purge ('A'); }

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
      if (strchr (LTRS_NUMBER, x_file->d_name [0]) == NULL) {
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
      x_new = NULL;
      rc = PROC_hook (&x_new, x_rpid);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("can not add process");
         continue;
      }
      /*---(gather)----------------------*/
      printf ("%d\n", x_rpid);
      sprintf (x_name, "/proc/%d/stat", x_rpid);
      rc = CPU_detail (x_new, x_name);
      if (rc < 0)  {
         PROC_unhook (&x_new);
         continue;
      }
      sprintf (x_name, "/proc/%d/smaps", x_rpid);
      rc = MEM_detail (x_new, x_name);
      if (rc < 0)  continue;
      /*---(summary)---------------------*/
      printf ("\n\n\n");
      /*> if (x_count % 5 == 0)  printf ("\n#rpid  ---name--------- -ppid l   s -utime- -stime- sn   --text- --data-   --heap- -stack-\n");   <*/
      /*> printf ("%5d  %-16.16s %5d %c   %c %7ld %7ld %2d   %7ld %7ld   %7ld %7ld\n",   <* 
       *>       x_new->rpid, x_new->name, x_new->ppid, x_new->land,                      <* 
       *>       x_new->c_state, x_new->c_utime, x_new->c_stime, x_new->c_snice,          <* 
       *>       x_new->m_text , x_new->m_data , x_new->m_heap , x_new->m_stack);         <*/
      /*> printf ("%s\n",                                                             <* 
       *>       x_file->d_name);                                                      <*/
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

