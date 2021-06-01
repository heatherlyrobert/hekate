/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



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



/*====================------------------------------------====================*/
/*===----                        data gathering                        ----===*/
/*====================------------------------------------====================*/
static void  o___GATHER__________o () { return; }


char
MEM_update              (tPROC *a_proc, char *a_name, int a_line, char *a_addr, int a_inode, char *a_perm, int a_full, int a_rss, int a_pvt)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_part      =  '-';
   long        x_empty     =    0;
   char        x_base      [LEN_TITLE] = "";
   char       *p           = NULL;
   tEXEC      *x_exec      = NULL;
   static tTIES *x_ties    = NULL;
   tLIBS      *x_libs      = NULL;
   long        x_beg       =    0;
   long        x_end       =    0;
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   static int  c           =    0;
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
   DEBUG_ENVI   yLOG_info    ("x_base"    , x_base);
   /*---(parse address)------------------*/
   DEBUG_ENVI   yLOG_point   ("a_addr"    , a_addr);
   if (a_addr != NULL) {
      p = strchr (a_addr, '-');
      if (p != NULL) {
         p [0] = '\0';
         sscanf (a_addr, "%x", &x_beg);
         if (x_beg > 9999999) x_beg = -1;
         sscanf (p + 1 , "%x", &x_end);
         if (x_end > 9999999) x_end = -1;
         if (x_end < 0)  x_beg = -1;
      }
   }
   /*---(special)------------------------*/
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
      x_ties->l_link->inode = a_inode;
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
      /*> a_proc->m_other += a_full - a_pvt;                                          <*/
      break;
   case PROC_HEAP : 
      a_proc->m_full  += a_rss;
      a_proc->m_proc  += a_rss;
      a_proc->m_heap  += a_rss;
      /*> a_proc->m_other += x_empty;                                                 <*/
      break;
   case PROC_STCK : 
      a_proc->m_full  += a_rss;
      a_proc->m_proc  += a_rss;
      a_proc->m_stack += a_rss;
      /*> a_proc->m_other += x_empty;                                                 <*/
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
   /*> printf ("%4d   %7ld %7ld %7ld    %c   %7ld  %7ld %7ld   %7ld %7ld %7ld %7ld   %7ld %7ld   %7ld\n",   <* 
    *>       a_line, a_full, a_rss, a_pvt, x_part,                                                          <* 
    *>       a_proc->m_full, a_proc->m_text, a_proc->m_cons,                                                 <* 
    *>       a_proc->m_data, a_proc->m_stack, a_proc->m_heap, a_proc->m_kern,                               <* 
    *>       x_ties->l_link->m_text, a_proc->m_libs, a_proc->m_other);                                      <*/
   /*> printf ("%4d   %-20.20s %7ld %7d %7d %5d   %7d %7d %7d   %c   %5d %5d %5d   %7d %7d %7d\n",   <* 
    *>       a_line, x_base, x_beg, x_end, x_end - x_beg, (x_end - x_beg) / 1024,                    <* 
    *>       a_full, a_rss, a_pvt, x_part,                                                           <* 
    *>       x_exec->m_text, x_exec->m_cons, x_exec->m_kern,                                         <* 
    *>       a_proc->m_full, a_proc->m_data);                                                        <*/
   if (c % 5 == 0)  printf ("\nline   ---name------------- perm   -full -rss- -pvt-   -   etext econs eheap ekern   pdata pheap stack other   tdata theap   ltext lcons lpriv   -total-\n");
   printf ("%4d   %-20.20s %-4.4s   ", a_line, x_base, a_perm);
   printf ("%5d %5d %5d   %c   "     , a_full, a_rss, a_pvt, x_part);
   printf ("%5d %5d %5d %5d   "      , x_exec->m_text, x_exec->m_cons, x_exec->m_heap, x_exec->m_kern);
   printf ("%5d %5d %5d %5d   "      , a_proc->m_data, a_proc->m_heap, a_proc->m_stack, a_proc->m_other);
   if (x_ties != NULL)  printf ("%5d %5d   "          , x_ties->m_data, x_ties->m_heap);
   else                 printf ("              ");
   if (x_libs != NULL)  printf ("%5d %5d %5d   "      , x_libs->m_text, x_libs->m_cons, x_libs->m_priv);
   else                 printf ("                    ");
   printf ("%7d"                    , a_proc->m_full);
   printf ("\n");
   ++c;
   /*---(complete)-----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MEM_detail              (tPROC *a_proc, char *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   char        x_addr      [LEN_TITLE] = "";
   char        x_perm      [LEN_TERSE] = "";
   long        x_inode     =    0;
   char        x_name      [LEN_HUND]  = "";
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
   char        x_part      =  '-';
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
            x_inode = 0;
            if (p != NULL)  x_inode = atol (p);
            /*---(get name)--------------*/
            p = strtok_r (NULL  , " \t", &r);
            strlcpy (x_name, "", LEN_TITLE);
            if (p != NULL)  strlcpy (x_name, p, LEN_TITLE);
            /*---(assign prog/lib)-------*/
            /*> if (p != NULL)  strltrim (p, ySTR_BOTH, LEN_RECD);                    <* 
             *> printf ("%2då%sæ\n", strlen (p), p);                                  <*/
            x_part = 'd';
            /*---(executables)-----------*/
            /*> if (strncmp (x_perm, "r-x"   , 3) == 0) {                                       <* 
             *>    if (c < 3)                                             x_part = EXEC_TEXT;   <* 
             *>    else if (p != NULL && strcmp  (p, "[vsyscall]") == 0)  x_part = EXEC_KERN;   <* 
             *>    else if (p != NULL && strcmp  (p, "[vdso]"    ) == 0)  x_part = EXEC_KERN;   <* 
             *>    else                                                   x_part = '-';         <* 
             *> }                                                                               <* 
             *> /+---(data segments)---------+/                                                 <* 
             *> else if (c < 3 || p == NULL || strcmp  (p, "") == 0) {                          <* 
             *>    if      (strncmp (x_perm, "r--", 3)  == 0)  x_part = 'd';                    <* 
             *>    else if (strncmp (x_perm, "rw-", 3)  == 0)  x_part = 'd';                    <* 
             *>    else                                        x_part = 'o';                    <* 
             *> }                                                                               <* 
             *> /+---(kernel stuff)----------+/                                                 <* 
             *> else if (strcmp  (p, "[vvar]"    ) == 0)       x_part = EXEC_KERN;              <* 
             *> else if (strcmp  (p, "[heap]"    ) == 0)       x_part = PROC_HEAP;              <* 
             *> else if (strcmp  (p, "[stack]"   ) == 0)       x_part = PROC_STCK;              <* 
             *> /+---(library data)----------+/                                                 <* 
             *> else {                                                                          <* 
             *>    if      (strncmp (x_perm, "r--", 3)  == 0)  x_part = TIES_DATA;              <* 
             *>    else if (strncmp (x_perm, "rw-", 3)  == 0)  x_part = TIES_DATA;              <* 
             *>    else                                        x_part = PROC_OTHR;              <* 
             *> }                                                                               <*/
            /*---(assign segment)--------*/
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
            MEM_update (a_proc, x_name, x_line, x_addr, x_inode, x_perm, x_full, x_rss, x_pvt);
            /*> switch (x_part) {                                                     <* 
             *> case EXEC_TEXT :                                                      <* 
             *>    if (a_max   != NULL)  *a_max   += x_full;                          <* 
             *>    if (a_base  != NULL)  *a_base  += x_full;                          <* 
             *>    if (a_text  != NULL)  *a_text  += x_full;                          <* 
             *>    break;                                                             <* 
             *> case 'd' :                                                            <* 
             *>    if (a_max   != NULL)  *a_max   += x_pvt;                           <* 
             *>    if (a_base  != NULL)  *a_base  += x_pvt;                           <* 
             *>    if (a_data  != NULL)  *a_data  += x_pvt;                           <* 
             *>    if (a_empty != NULL)  *a_empty += x_full - x_pvt;                  <* 
             *>    break;                                                             <* 
             *> case PROC_HEAP :                                                      <* 
             *>    if (a_max   != NULL)  *a_max   += x_rss;                           <* 
             *>    if (a_base  != NULL)  *a_base  += x_rss;                           <* 
             *>    if (a_heap  != NULL)  *a_heap  += x_rss;                           <* 
             *>    if (a_empty != NULL)  *a_empty += x_empty;                         <* 
             *>    break;                                                             <* 
             *> case PROC_STCK :                                                      <* 
             *>    if (a_max   != NULL)  *a_max   += x_rss;                           <* 
             *>    if (a_base  != NULL)  *a_base  += x_rss;                           <* 
             *>    if (a_stack != NULL)  *a_stack += x_rss;                           <* 
             *>    if (a_empty != NULL)  *a_empty += x_empty;                         <* 
             *>    break;                                                             <* 
             *> case EXEC_KERN : /+ mapped kernel space +/                            <* 
             *>    if (a_max   != NULL)  *a_max   += x_full;                          <* 
             *>    if (a_base  != NULL)  *a_base  += x_full;                          <* 
             *>    if (a_kern  != NULL)  *a_kern  += x_full;                          <* 
             *>    break;                                                             <* 
             *> case 'o' :                                                            <* 
             *>    if (a_max   != NULL)  *a_max   += x_full;                          <* 
             *>    if (a_empty != NULL)  *a_empty += x_full;                          <* 
             *>    x_empty = 0;                                                       <* 
             *>    break;                                                             <* 
             *> case TIES_DATA :                                                            <* 
             *>    if (a_max   != NULL)  *a_max   += x_pvt;                           <* 
             *>    if (a_base  != NULL)  *a_base  += x_pvt;                           <* 
             *>    if (a_libs  != NULL)  *a_libs  += x_pvt;                           <* 
             *>    if (a_empty != NULL)  *a_empty += x_full - x_pvt;                  <* 
             *>    break;                                                             <* 
             *> }                                                                     <*/
            /*> printf ("%4d    %7ld %7ld %7ld    %c  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld    %7ld\n",   <* 
             *>       x_line, x_full, x_rss, x_pvt, x_part,                                                              <* 
             *>       *a_max, *a_base, *a_min, *a_text, *a_data, *a_stack,                                               <* 
             *>       *a_heap, *a_kern, *a_libs, *a_empty);                                                              <*/
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



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
MEM__unit               (char *a_question, int n)
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
   strcpy (unit_answer, "MEM              : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "exec"     )      == 0) {
      PROC_by_index (n, &x_proc);
      if (x_proc != NULL)  x_exec = x_proc->e_link;
      if (x_exec != NULL) {
         sprintf  (t, "%2då%.10sæ", strlen (x_exec->base), x_exec->base);
         snprintf (unit_answer, LEN_RECD, "MEM exec    (%2d) : %-14.14s  %7dc %7dd  %7dt %7db %7dk",
               n, t, x_exec->m_code, x_exec->m_data, x_exec->m_text, x_exec->m_cons, x_exec->m_kern);
      } else {
         snprintf (unit_answer, LEN_RECD, "MEM exec    (%2d) :  -åæ                  -c       -d        -t       -b       -k", n);
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




