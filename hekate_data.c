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


#define    EXEC_TEXT   'T'
#define    EXEC_CONS   'C'
#define    EXEC_HEAP   'H'
#define    EXEC_KERN   'K'

#define    PROC_DATA   'd'
#define    PROC_HEAP   'h'
#define    PROC_STCK   's'
#define    PROC_OTHR   'o'

#define    TIES_DATA   'D'
#define    TIES_HEAP   '+'

#define    LIBS_TEXT   't'
#define    LIBS_CONS   'c'
#define    LIBS_PRIV   'p'



tDATA    *d_head  = NULL;
tDATA    *d_tail  = NULL;
tDATA    *d_curr  = NULL;
int       d_count = 0;;



/*====================------------------------------------====================*/
/*===----                          pre-work                            ----===*/
/*====================------------------------------------====================*/
static void  o___PREWORK_________o () { return; }

char
DATA__exename           (char *a_file, int *a_inode, char *a_base, char *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   char       *p           = NULL;
   char       *q           = NULL;
   char       *r           = NULL;
   int        i            =    0;
   int        l            =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_base != NULL)  strlcpy (a_base, "", LEN_TITLE);
   if (a_full != NULL)  strlcpy (a_full, "", LEN_RECD);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get first line)-----------------*/
   fgets (x_recd, LEN_RECD, f);
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   /*---(open proc)----------------------*/
   p = strtok_r (x_recd, " \t", &r);
   --rce;  for (i = 0; i < 4; ++i) {
      p = strtok_r (NULL, " \t", &r);
      if (p == NULL) {
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(inode)--------------------------*/
   if (a_inode != NULL)  *a_inode = atol (p);
   /*---(save back)----------------------*/
   p = strtok_r (NULL, " \t", &r);
   if (p == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("a_full"    , p);
   if (a_full != NULL)  strlcpy (a_full, p, LEN_RECD);
   /*---(parse name)---------------------*/
   q = strrchr (p, '/');
   if (q != NULL) ++q;
   else           q = p;
   DEBUG_ENVI   yLOG_info    ("a_base"    , q);
   if (a_base != NULL)  strlcpy (a_base, q, LEN_TITLE);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA__pubname           (char *a_file, char *a_public)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         l           =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_public != NULL)  strlcpy (a_public, "", LEN_TITLE);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(saveback)----------------------*/
   if (a_public != NULL)  strlcpy (a_public, x_recd, LEN_TITLE);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA__cmdline           (char *a_file, char *a_cmdline)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_cmdline != NULL)  strlcpy (a_cmdline, "", LEN_RECD);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   while (1) {
      if (x_recd [i] == '\0' && x_recd [i + 1] == '\0')  break;
      if (x_recd [i] == '\0')  x_recd [i] = ' ';
      ++i;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(saveback)----------------------*/
   if (a_cmdline != NULL)  strlcpy (a_cmdline, x_recd, LEN_RECD);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA__size              (tPROC *a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_cmd       [LEN_RECD]  = "";
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   tEXEC      *x_exec      = NULL;
   char       *p           = NULL;
   char       *r           = NULL;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_exec = a_proc->e_link;
   DEBUG_ENVI   yLOG_point   ("x_exec"    , x_exec);
   --rce;  if (x_exec == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_point   ("->full"    , x_exec->full);
   --rce;  if (strlen (x_exec->full) <= 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove file)--------------------*/
   sprintf (x_cmd, "rm tmp/size.tmp  2> /dev/null");
   rc = system (x_cmd);
   DEBUG_ENVI   yLOG_value   ("rm"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run command)--------------------*/
   sprintf (x_cmd, "size %s  > /tmp/size.tmp  2> /dev/null", x_exec->full);
   rc = system (x_cmd);
   DEBUG_ENVI   yLOG_value   ("size"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open proc)----------------------*/
   f = fopen ("/tmp/size.tmp", "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " \t", &r);
   for (i = 0; i < 4; ++i) {
      if (p == NULL)  break;
      switch (i) {
      case 0 :  x_exec->s_text  = atoi (p) / 1024;  break;
      case 1 :  x_exec->s_data  = atoi (p) / 1024;  break;
      case 2 :  x_exec->s_bss   = atoi (p) / 1024;  break;
      case 3 :  x_exec->s_total = atoi (p) / 1024;  break;
      }
      p = strtok_r (NULL, " \t", &r);
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA_prework            (int a_rpid, tPROC **a_proc, char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   char        x_base      [LEN_TITLE] = "";
   char        x_full      [LEN_RECD]  = "";
   char        x_public    [LEN_TITLE] = "";
   char        x_cmdline   [LEN_RECD]  = "";
   long        x_inode     =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*> printf ("%d\n", a_rpid);                                                       <*/
   /*---(exename)---------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/smaps", a_rpid);
   else                sprintf (x_file, "./testdata/%d_smaps", a_rpid);
   rc = DATA__exename (x_file, &x_inode, x_base, x_full);
   DEBUG_ENVI   yLOG_value   ("exename"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("can not gather exec name");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(public name)-----------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/comm", a_rpid);
   else                sprintf (x_file, "./testdata/%d_comm", a_rpid);
   rc = DATA__pubname (x_file, x_public);
   DEBUG_ENVI   yLOG_value   ("pubname"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("can not gather public name");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(command line)----------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/cmdline", a_rpid);
   else                sprintf (x_file, "./testdata/%d_cmdline", a_rpid);
   rc = DATA__cmdline (x_file, x_cmdline);
   DEBUG_ENVI   yLOG_value   ("cmdline"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("can not gather command line");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create process)----------------*/
   rc = PROC_hook (a_proc, a_rpid);
   DEBUG_ENVI   yLOG_value   ("proc"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("can not add process");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create process)----------------*/
   rc = EXEC_hook (*a_proc, x_base);
   DEBUG_ENVI   yLOG_value   ("exec"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("can not add executable");
      PROC_unhook (a_proc);
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add prework)-----------------*/
   strlcpy ((*a_proc)->e_link->full, x_full   , LEN_RECD);
   strlcpy ((*a_proc)->shown       , x_public , LEN_TITLE);
   strlcpy ((*a_proc)->cmdline     , x_cmdline, LEN_RECD);
   (*a_proc)->e_link->inode        = x_inode;
   /*---(gather cpu)------------------*/
   rc = DATA__size  (*a_proc);
   DEBUG_ENVI   yLOG_value   ("size"      , rc);
   --rce;  if (rc < 0) {
      PROC_unhook (a_proc);
      DEBUG_INPT   yLOG_note    ("can not gather size data");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        data gathering                        ----===*/
/*====================------------------------------------====================*/
static void  o___GATHER__________o () { return; }

char
DATA_cpu                (tPROC *a_proc, char *a_file)
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
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   /*> sprintf (x_name, "/proc/%d/stat", a_rpid);                                     <*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      fclose (f);
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " ", &r);
   DEBUG_ENVI   yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      fclose (f);
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  while (p != NULL && c < 48) {
      DEBUG_ENVI   yLOG_info    ("p"         , p);
      switch (c) {
      case  1 :
         strlcpy (x_name, p + 1, strlen (p) - 1);
         x_exec = a_proc->e_link;
         DEBUG_ENVI   yLOG_point   ("x_exec"    , x_exec);
         --rce;  if (x_exec == NULL) {
            rc = EXEC_hook (a_proc, x_name);
            DEBUG_ENVI   yLOG_value   ("hook"      , rc);
            if (rc < 0) {
               fclose (f);
               DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
               return rce;
            }
         }
         x_exec = a_proc->e_link;
         if (strcmp (x_exec->base, x_name) != 0 && strcmp (a_proc->shown, x_name) != 0) {
            DEBUG_ENVI   yLOG_note    ("name change");
            rc = EXEC_rehook (a_proc, x_name);
            DEBUG_ENVI   yLOG_value   ("rehook"    , rc);
            if (rc < 0) {
               fclose (f);
               DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
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
            DEBUG_ENVI   yLOG_note    ("kernel thread, skipping");
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
         x_exec->m_code = (atoi (p) - x_beg) / 1024;
         break;
      case 44 :
         x_beg  = atoi (p);
         break;
      case 45 :
         x_exec->m_data = (atoi (p) - x_beg) / 1024;
         break;
      }
      if (x_land != '-')  break;
      ++c;
      p = strtok_r (NULL  , " ", &r);
   }
   DEBUG_ENVI   yLOG_value   ("c"         , c);
   --rce;  if (c <  45) {
      rc = fclose (f);
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA__mem_update        (tPROC *a_proc, char *a_name, int a_line, char *a_addr, int a_inode, char *a_perm, int a_full, int a_rss, int a_pvt)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_part      =  '-';
   long        x_empty     =    0;
   char        x_base      [LEN_TITLE] = "";
   char        x_addr      [LEN_TITLE] = "";
   char       *p           = NULL;
   tEXEC      *x_exec      = NULL;
   static tTIES *x_ties    = NULL;
   tLIBS      *x_libs      = NULL;
   long        x_beg       =    0;
   long        x_end       =    0;
   static int  c           =    0;
   /*---(header)-------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   /*---(reset)--------------------------*/
   if (a_line == 1) {
      c      = 0;
      x_ties = NULL;
   }
   /*---(parse name)---------------------*/
   DEBUG_ENVI   yLOG_point   ("a_name"    , a_name);
   if (a_name != NULL) {
      p = strrchr (a_name, '/');
      if (p == NULL)  p = a_name;
      else            ++p;
      strlcpy (x_base, p, LEN_TITLE);
   }
   DEBUG_ENVI   yLOG_info    ("a_name"    , a_name);
   DEBUG_ENVI   yLOG_info    ("x_base"    , x_base);
   /*---(parse address)------------------*/
   DEBUG_ENVI   yLOG_point   ("a_addr"    , a_addr);
   if (a_addr != NULL) {
      strlcpy (x_addr, a_addr, LEN_TITLE);
      DEBUG_ENVI   yLOG_info    ("x_addr"    , x_addr);
      p = strchr (x_addr, '-');
      DEBUG_ENVI   yLOG_char    ("p [0]"     , p [0]);
      DEBUG_ENVI   yLOG_point   ("p"         , p);
      if (p != NULL) {
         p [0] = '\0';
         sscanf (x_addr, "%x", &x_beg);
         if (x_beg > 9999999) x_beg = -1;
         sscanf (p + 1 , "%x", &x_end);
         if (x_end > 9999999) x_end = -1;
         if (x_end < 0)  x_beg = -1;
      }
   }
   /*---(special)------------------------*/
   DEBUG_ENVI   yLOG_note    ("starting part logic");
   if (a_name [0] == '[') {
      DEBUG_ENVI   yLOG_note    ("special");
      x_ties = NULL;
      if      (strncmp (a_name, "[stack" , 6) == 0)  x_part = PROC_STCK;
      else if (strcmp  (a_name, "[heap]"    ) == 0)  x_part = PROC_HEAP;
      else if (strcmp  (a_name, "[vvar]"    ) == 0)  x_part = EXEC_KERN;
      else if (strcmp  (a_name, "[vsyscall]") == 0)  x_part = EXEC_KERN;
      else if (strcmp  (a_name, "[vdso]"    ) == 0)  x_part = EXEC_KERN;
      else                                           x_part = '[';
   }
   /*---(executable)---------------------*/
   else if (strcmp (x_base, a_proc->e_link->base) == 0)  {
      DEBUG_ENVI   yLOG_note    ("base program");
      a_proc->e_link->inode = a_inode;
      if      (strncmp (a_perm, "r-x", 3    ) == 0)  x_part = EXEC_TEXT;
      else if (strncmp (a_perm, "r--", 3    ) == 0)  x_part = EXEC_CONS;
      else if (strncmp (a_perm, "rw-", 3    ) == 0)  x_part = PROC_DATA;
      else                                           x_part = PROC_OTHR;
   }
   else if (strncmp (a_name, "/lib", 4) == 0 ||
         strncmp (a_name, "/usr/lib", 8) == 0 || 
         strncmp (a_name, "/usr/local/lib", 14) == 0) {
      DEBUG_ENVI   yLOG_note    ("library");
      rc = LIBS_hook (a_proc, a_name, &x_ties);
      DEBUG_ENVI   yLOG_value   ("hook"      , rc);
      if (x_ties->l_link->inode == 0)  x_ties->l_link->inode = a_inode;
      DEBUG_ENVI   yLOG_point   ("x_ties"    , x_ties);
      if      (strncmp (a_perm, "r-x", 3    ) == 0)  x_part = LIBS_TEXT;
      else if (strncmp (a_perm, "r--", 3    ) == 0)  x_part = LIBS_CONS;
      else if (strncmp (a_perm, "rw-", 3    ) == 0)  x_part = TIES_DATA;
      else                                           x_part = LIBS_PRIV;
   } else {
      if (x_ties == NULL)  x_part = EXEC_HEAP;
      else                 x_part = TIES_HEAP;
   }
   DEBUG_ENVI   yLOG_char    ("x_part"    , x_part);
   /*---(calculate empty)----------------*/
   x_empty = a_full - a_rss;
   if (x_empty < 0)  x_empty = 0;
   /*---(assign values)------------------*/
   x_exec = a_proc->e_link;
   if (x_ties != NULL)  x_libs = x_ties->l_link;
   /*---(executable)---------------------*/
   switch (x_part) {
   case EXEC_TEXT :  /* exec code base */
      x_exec->m_text  += a_full;
      x_exec->m_full  += a_full;
      a_proc->m_full  += a_full;
      break;
   case EXEC_CONS :  /* exec const data */
      x_exec->m_cons  += a_full;
      x_exec->m_full  += a_full;
      a_proc->m_full  += a_full;
      break;
   case EXEC_KERN : /* mapped kernel space */
      x_exec->m_kern  += a_full;
      x_exec->m_full  += a_full;
      a_proc->m_full  += a_full;
      break;
   case EXEC_HEAP :  /* exec shared heap */
      a_proc->m_full  += a_full;
      x_exec->m_full  += a_full;
      x_exec->m_heap  += a_full;
      break;
   }
   /*---(process)------------------------*/
   switch (x_part) {
   case PROC_DATA :  /* variable data */
      a_proc->m_full  += a_pvt;
      a_proc->m_proc  += a_pvt;
      a_proc->m_data  += a_pvt;
      break;
   case PROC_HEAP : 
      a_proc->m_full  += a_rss;
      a_proc->m_proc  += a_rss;
      a_proc->m_heap  += a_rss;
      break;
   case PROC_STCK : 
      a_proc->m_full  += a_rss;
      a_proc->m_proc  += a_rss;
      a_proc->m_stack += a_rss;
      break;
   case PROC_OTHR : 
      a_proc->m_full  += a_full;
      a_proc->m_other += a_full;
      x_empty = 0;
      break;
   }
   /*---(files)--------------------------*/
   /*---(ties)---------------------------*/
   switch (x_part) {
   case TIES_DATA :  /* libs data */
      a_proc->m_full  += a_pvt;
      x_ties->m_data  += a_pvt;
      break;
   case TIES_HEAP :  /* libs heap */
      a_proc->m_full  += a_full;
      x_ties->m_heap  += a_full;
      break;
   }
   /*---(libraries)----------------------*/
   switch (x_part) {
   case LIBS_TEXT : /* libs code base */
      a_proc->m_full  += a_full;
      x_libs->m_text  += a_full;
      break;
   case LIBS_CONS : /* libs constants */
      a_proc->m_full  += a_full;
      x_libs->m_cons  += a_full;
      break;
   case LIBS_PRIV : /* libs unknown */
      a_proc->m_full  += a_full;
      x_libs->m_priv  += a_full;
      break;
   }
   /*---(reporting)----------------------*/
   /*> if (c % 5 == 0)  printf ("\nline   ---name------------- perm   -full -rss- -pvt-   -   etext econs eheap ekern   pdata pheap stack other   tdata theap   ltext lcons lpriv   -total-\n");   <*/
   /*> printf ("%4d   %-20.20s %-4.4s   ", a_line, x_base, a_perm);                   <*/
   /*> printf ("%5d %5d %5d   %c   "     , a_full, a_rss, a_pvt, x_part);             <*/
   /*> printf ("%5d %5d %5d %5d   "      , x_exec->m_text, x_exec->m_cons, x_exec->m_heap, x_exec->m_kern);   <*/
   /*> printf ("%5d %5d %5d %5d   "      , a_proc->m_data, a_proc->m_heap, a_proc->m_stack, a_proc->m_other);   <*/
   /*> if (x_ties != NULL)  printf ("%5d %5d   "          , x_ties->m_data, x_ties->m_heap);                   <* 
    *> else                 printf ("              ");                                                         <* 
    *> if (x_libs != NULL)  printf ("%5d %5d %5d   "      , x_libs->m_text, x_libs->m_cons, x_libs->m_priv);   <* 
    *> else                 printf ("                    ");                                                   <* 
    *> printf ("%7d"                    , a_proc->m_full);                                                     <* 
    *> printf ("\n");                                                                                          <*/
   ++c;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA_mem                (tPROC *a_proc, char *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   char        x_addr      [LEN_TITLE] = "";
   char        x_perm      [LEN_TERSE] = "";
   long        x_inode     =    0;
   char        x_name      [LEN_RECD]  = "";
   char        x_base      [LEN_HUND]  = "";
   int         c           =    0;
   int         i           =    0;
   int         l           =    0;
   char       *p           = NULL;
   char       *r           = NULL;
   long        x_full      =    0;
   long        x_rss       =    0;
   long        x_pvt       =    0;
   long        x_empty     =    0;
   int         n           =    1;
   int         x_line      =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(open proc)----------------------*/
   /*> sprintf (x_name, "/proc/%d/smaps", a_rpid);                                    <*/
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   PROC_wipe (a_proc, '-');
   /*---(read stanza)-----------------*/
   while (1) {
      /*> if (c % 5 == 0)  printf ("\nline    --full-  --rss--    p  --max--  --base-  --min--  --text-  --data-  -stack-  --heap-  --kern-  --libs-    -empty-\n");   <*/
      x_line = n;
      for (i = 0; i < 16; ++i) {
         fgets (x_recd, LEN_RECD, f);
         if (feof (f))  break;
         l = strlen (x_recd);
         if (l > 0 && x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
         ++n;
         /*> printf ("%2d, %s\n", i, x_recd);                                         <*/
         /*---(address)------------------*/
         p = strtok_r (x_recd, " \t", &r);
         if (p == NULL) break;
         if (i == 0) strlcpy (x_addr, p, LEN_TITLE);
         /*---(permissions)--------------*/
         p = strtok_r (NULL  , " \t", &r);
         if (p == NULL) break;
         if (i == 0)  strlcpy (x_perm, p, LEN_TERSE);
         /*---(handle)-------------------*/
         switch (i) {
         case  0 : /* permissions */
            /*---(spacers)---------------*/
            p = strtok_r (NULL  , " \t", &r);
            p = strtok_r (NULL  , " \t", &r);
            /*---(get inode)-------------*/
            p = strtok_r (NULL  , " \t", &r);
            /*> printf ("x_node = å%sæ\n", p);                                        <*/
            if (p != NULL)  x_inode = atol (p);
            /*---(get name)--------------*/
            p = strtok_r (NULL  , " \t", &r);
            if (p != NULL)  strlcpy (x_name, p, LEN_RECD);
            /*---(assign prog/lib)-------*/
            /*> if (p != NULL)  strltrim (p, ySTR_BOTH, LEN_RECD);                    <* 
             *> printf ("%2då%sæ\n", strlen (p), p);                                  <*/
            break;
         case  1 : /* rss           */
            x_full  = atol (p);
            break;
         case  2 : /* rss           */
            x_rss   = atol (p);
            x_empty = x_full - x_rss;
            if (x_empty < 0)  x_empty = 0;
            break;
         case  6 : /* private clean */
            x_pvt   = atol (p);
            break; 
         case  7 : /* private dirty */
            x_pvt  += atol (p);
            break; 
         case 15 :
            DATA__mem_update (a_proc, x_name, x_line, x_addr, x_inode, x_perm, x_full, x_rss, x_pvt);
            break;
         }
      }
      if (feof (f))  break;
      ++c;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("fclose"    , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA_driver             (int a_rpid, tPROC **a_proc, char a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(pre-work)--------------------*/
   rc = DATA_prework (a_rpid, a_proc, a_rpid);
   DEBUG_ENVI   yLOG_value   ("prework"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(gather cpu)------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/stat", a_rpid);
   else                sprintf (x_file, "./testdata/%d_stat", a_rpid);
   DEBUG_ENVI   yLOG_info    ("x_file"    , x_file);
   rc = DATA_cpu  (*a_proc, x_file);
   DEBUG_ENVI   yLOG_value   ("cpu"       , rc);
   --rce;  if (rc < 0) {
      PROC_unhook (a_proc);
      DEBUG_INPT   yLOG_note    ("can not gather cpu data");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(gather memory)---------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/smaps", a_rpid);
   else                sprintf (x_file, "./testdata/%d_smaps", a_rpid);
   DEBUG_ENVI   yLOG_info    ("x_file"    , x_file);
   rc = DATA_mem (*a_proc, x_file);
   DEBUG_ENVI   yLOG_value   ("memory"    , rc);
   --rce;  if (rc < 0) {
      PROC_unhook (a_proc);
      DEBUG_INPT   yLOG_note    ("can not gather memory data");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char  DATA_new     (void **a_new) { return SHARE_new   (TYPE_DATA, a_new, '-'); }
char  DATA_force   (void **a_new) { return SHARE_new   (TYPE_DATA, a_new, 'y'); }
char  DATA_free    (void **a_old) { return SHARE_free  (TYPE_DATA, a_old); }
char  DATA_purge   (void)         { return SHARE_purge (TYPE_DATA); }



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char  DATA_by_cursor      (tDATA **r_curr, char a_move)                  { return SHARE_by_cursor (TYPE_DATA, r_curr, a_move); }
char  DATA_by_index       (tDATA **r_curr, int a_index)                  { return SHARE_by_index  (TYPE_DATA, r_curr, a_index); }

char* DATA_key            (tDATA *a_curr) { if (a_curr != NULL)  return a_curr->key;  return "999999"; }

char
DATA_hook               (tDATA *a_next, tDATA *a_curr)
{
   /*---(at head)------------------------*/
   if (a_next->m_prev == NULL) {
      a_next->m_prev = a_curr;
      a_curr->m_next = a_next;
      d_head         = a_curr;
      ++d_count;
   }
   /*---(else)---------------------------*/
   else {
      a_next->m_prev->m_next = a_curr;
      a_curr->m_prev         = a_next->m_prev;
      a_next->m_prev         = a_curr;
      a_curr->m_next         = a_next;
      ++d_count;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
DATA_unhook             (tDATA *a_curr)
{
   if (a_curr->m_next != NULL)  a_curr->m_next->m_prev  = a_curr->m_prev;
   else                         d_tail                  = a_curr->m_prev;
   if (a_curr->m_prev != NULL)  a_curr->m_prev->m_next  = a_curr->m_next;
   else                         d_head                  = a_curr->m_next;
   a_curr->m_prev = NULL;
   a_curr->m_next = NULL;
   --d_count;
   return 0;
}




/*====================------------------------------------====================*/
/*===----                     /proc gathering                          ----===*/
/*====================------------------------------------====================*/
static void  o___REVIEW__________o () { return; }

char
DATA_gather             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_rpid      =    0;
   tDATA      *x_new       = NULL;
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
      rc = DATA_new (&x_new);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("can not add process");
         continue;
      }
      /*---(populate)--------------------*/
      x_new->rpid = x_rpid;
      sprintf (x_new->key, "%-6", x_new->rpid);
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
static void  o___UNITTEST________o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
DATA__unit              (char *a_question, int n)
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
   strcpy (unit_answer, "DATA             : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "cpu"      )      == 0) {
      PROC_by_index (&x_proc, n);
      if (x_proc != NULL) {
         snprintf (unit_answer, LEN_RECD, "DATA cpu    (%2d) : %5d %5d  %c %7du %7ds %2dn  %c",
               n, x_proc->rpid, x_proc->ppid, x_proc->c_state, x_proc->c_utime, x_proc->c_stime, x_proc->c_snice, x_proc->c_flag);
      } else {
         snprintf (unit_answer, LEN_RECD, "DATA cpu    (%2d) : -     -      -       -u       -s  -n  -", n);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "emem"     )      == 0) {
      EXEC_by_index (&x_exec, n);
      if (x_exec != NULL) {
         sprintf  (t, "%2då%.10sæ", strlen (x_exec->base), x_exec->base);
         snprintf (unit_answer, LEN_RECD, "DATA emem   (%2d) : %-14.14s  %4dc %4dd   %4df %4dt %3dc %4dh %2dk   %4dt %4dd %3db %4dt",
               n, t, x_exec->m_code, x_exec->m_data, x_exec->m_full, x_exec->m_text, x_exec->m_cons, x_exec->m_heap, x_exec->m_kern, x_exec->s_text, x_exec->s_data, x_exec->s_bss, x_exec->s_total);
      } else {
         snprintf (unit_answer, LEN_RECD, "DATA emem   (%2d) :  -åæ               -c    -d      -f    -t   -c    -h  -k      -t    -d   -b    -t", n);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "esize"    )      == 0) {
      EXEC_by_index (&x_exec, n);
      if (x_exec != NULL) {
         sprintf  (t, "%2då%.10sæ", strlen (x_exec->base), x_exec->base);
         snprintf (unit_answer, LEN_RECD, "DATA esize  (%2d) : %-14.14s  %5dt %5dd %5db %5dt",
               n, t, x_exec->s_text, x_exec->s_data, x_exec->s_bss, x_exec->s_total);
      } else {
         snprintf (unit_answer, LEN_RECD, "DATA esize  (%2d) :  -åæ                -t     -d     -b     -t", n);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "pmem"     )      == 0) {
      PROC_by_index (&x_proc, n);
      if (x_proc != NULL) {
         x_exec = x_proc->e_link;
         if (x_exec != NULL) sprintf  (t, "%2då%.10sæ", strlen (x_exec->base), x_exec->base);
         snprintf (unit_answer, LEN_RECD, "DATA pmem   (%2d) : %-14.14s %5df %5dp %4dd %5dh %3ds %3do - %4dr %4dw - %4dr %4dw -",
               n, t, x_proc->m_full, x_proc->m_proc, x_proc->m_data, x_proc->m_heap, x_proc->m_stack, x_proc->m_other, x_proc->d_read, x_proc->d_write, x_proc->n_read, x_proc->n_write);
      } else {
         snprintf (unit_answer, LEN_RECD, "DATA pmem   (%2d) :  -åæ               -f     -p    -d     -h   -s   -o -    -r    -w -    -r    -w -", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


