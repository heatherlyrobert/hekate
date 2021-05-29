/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"


/*
 *  get total computer availability for cpu
 *  compare new utime/stime to last to get update
 *  calculate utilization of cpu.
 *
 *  i need cpu units, not real so i can set limits
 *  that do not depend on current machine -- making
 *  my limit flags portable.
 *
 *  then, another function to detirmine usage level
 *
 *  and another, to compare to limit flag
 *
 *  LIKELY need to keep this in yEXEC so khronos and
 *  heracles can use it !!!
 *
 */

/*====================------------------------------------====================*/
/*===----                        data gathering                        ----===*/
/*====================------------------------------------====================*/
static void  o___GATHER__________o () { return; }

char
CPU_detail              (tPROC *a_proc, char *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_land      =  '-';
   tEXEC      *x_exec      = NULL;
   char        x_recd      [LEN_RECD]  = "";
   char        x_name      [LEN_TITLE] = "";
   char       *p           = NULL;
   char       *r           = NULL;
   char        c           =    0;
   int         x_beg       =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)-----------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   /*> sprintf (x_name, "/proc/%d/stat", a_rpid);                                     <*/
   f = fopen (a_file, "rt");
   DEBUG_YEXEC  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)-----------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      fclose (f);
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " ", &r);
   DEBUG_YEXEC  yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      fclose (f);
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  while (p != NULL && c < 48) {
      DEBUG_YEXEC  yLOG_info    ("p"         , p);
      switch (c) {
      case  1 :
         strlcpy (x_name, p + 1, strlen (p) - 1);
         x_exec = a_proc->e_link;
         DEBUG_YEXEC  yLOG_point   ("x_exec"    , x_exec);
         --rce;  if (x_exec == NULL) {
            rc = EXEC_hook (a_proc, x_name);
            if (rc < 0) {
               fclose (f);
               DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
               return rce;
            }
         }
         x_exec = a_proc->e_link;
         break;
      case  2 :
         a_proc->c_state = p [0];
         break;
      case  3 :
         a_proc->ppid    = atoi (p);
         switch (a_proc->ppid) {
         case   0 : x_land = 'k';    break;
         case   2 : x_land = 'k';    break;
         }
         if (a_proc->rpid == 1)  x_land = '-';
         if (x_land == 'k') {
            DEBUG_YEXEC  yLOG_note    ("kernel thread, skipping");
            PROC_unhook (a_proc);
            break;
         }
         break;
      case 13 :
         a_proc->c_utime = atol (p);
         break;
      case 14 :
         a_proc->c_stime = atol (p);
         break;
      case 17 :
         a_proc->c_snice = atoi (p);
         if (a_proc->c_snice < -20)  a_proc->c_snice = -1;
         break;
      case 25 :
         x_beg  = atoi (p);
         break;
      case 26 :
         x_exec->m_code = atoi (p) - x_beg;
         break;
      case 44 :
         x_beg  = atoi (p);
         break;
      case 45 :
         x_exec->m_data = atoi (p) - x_beg;
         break;
      }
      if (x_land != '-')  break;
      ++c;
      p = strtok_r (NULL  , " ", &r);
   }
   DEBUG_YEXEC  yLOG_value   ("c"         , c);
   --rce;  if (c <  45) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
CPU__unit               (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   tPROC      *x_proc      = NULL;
   tEXEC      *x_exec      = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        w           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "CPU              : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "exec"     )      == 0) {
      PROC_by_index (n, &x_proc);
      if (x_proc != NULL)  x_exec = x_proc->e_link;
      if (x_exec != NULL) {
         sprintf  (t, "%2då%.10sæ", strlen (x_exec->name), x_exec->name);
         snprintf (unit_answer, LEN_RECD, "CPU exec    (%2d) : %-14.14s  %7dc %7dd",
               n, t, x_exec->m_code, x_exec->m_data);
      } else {
         snprintf (unit_answer, LEN_RECD, "CPU exec    (%2d) :  -åæ                  -c       -d", n);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "proc"     )      == 0) {
      PROC_by_index (n, &x_proc);
      if (x_proc != NULL) {
         snprintf (unit_answer, LEN_RECD, "CPU proc    (%2d) : %5d %5d  %c %7du %7ds %2dn  %c",
               n, x_proc->rpid, x_proc->ppid, x_proc->c_state, x_proc->c_utime, x_proc->c_stime, x_proc->c_snice, x_proc->c_flag);
      } else {
         snprintf (unit_answer, LEN_RECD, "CPU proc    (%2d) : -     -      -       -u       -s  -n  -", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


