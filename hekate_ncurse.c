/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"

#define  S_MID  "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€"
#define  S_SPC  "                                                      "

char
NCURSE_exec_mult        (tEXEC *a_exec, tPROC *a_proc, char *a_mult)
{
}

char
NCURSE_exec_list        (void)
{
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   char        t           [LEN_HUND]  = "";
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        m           =    0;
   char        l           =    0;
   char        w           =    0;
   char        n           =    0;
   char        x_seq       =    0;
   /*---(prepare)------------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      l = strlen (x_exec->base);
      if (l > w)  w = l;
      x_exec->e_col = 0;
      if (x_exec->p_count > 1) {
         ++m;
         x_exec->e_col = m;
      }
      n += x_exec->p_count;
      x_seq  = 0;
      PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DHEAD);
      while (x_proc != NULL) { 
         x_proc->e_seq = ++x_seq;
         PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DNEXT);
      }
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   snprintf (t, LEN_HUND, "%d exec ············································", e_count);
   printf ("%s\n", t);
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      /*> aa  init €€€€€€€€€€€€€€€€ 1 ²²²²²init [3]                                     ea ²²²2  ƒ€€23€€ libc-2.28.so                ma   <*/
      snprintf (x_pre, w + 3, "%s %s", x_exec->base, S_MID);
      strllower (x_pre, LEN_TITLE);
      snprintf (x_cnt, LEN_TERSE, "-%d%s", x_exec->p_count, S_MID);
      snprintf (x_mul, m * 2, "%s", S_MID);
      if (x_exec->e_col > 0)   x_mul [x_exec->e_col * 2 - 2] = '‰';
      printf ("%-2.2s  %s%-4.4s%s\n", x_exec->hint, x_pre, x_cnt, x_mul);
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   printf ("widest = %d, multiples = %d, height = %d\n", w, m, n);
   return 0;
}

char
NCURSE_proc_list_OLD    (void)
{
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        m           =    0;
   char        l           =    0;
   char        w           =    0;
   char        n           =    0;
   char        x_seq       =    0;
   char        e_wide      =    0;
   /*---(prepare)------------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      l = strlen (x_exec->base);
      if (l > w)  w = l;
      x_exec->e_col = 0;
      if (x_exec->p_count > 1) {
         ++m;
         x_exec->e_col = m;
      }
      n += x_exec->p_count;
      x_seq  = 0;
      PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DHEAD);
      while (x_proc != NULL) { 
         x_proc->e_seq = ++x_seq;
         PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DNEXT);
      }
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(title)--------------------------*/
   e_wide = 4 + w + 3 + 4 + (m * 2) + 2;
   snprintf (s, e_wide, "%d exec ············································", e_count);
   snprintf (t, LEN_HUND, "%d proc ············································", p_count);
   printf ("%s %s\n", s, t);
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL) {
      x_exec = x_proc->e_link;
      if (x_exec->p_head == x_proc) {
         snprintf (x_pre, w + 3, "%s €€€€€€€€€€€€€€€€€€€€€€€€€€€", x_exec->base);
         strllower (x_pre, LEN_TITLE);
         snprintf (x_cnt, LEN_TERSE, "-%d€€€€€€", x_exec->p_count);
         snprintf (x_mul, m * 2, "²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²");
         if (x_exec->e_col > 0)   x_mul [x_exec->e_col * 2 - 2] = '‰';
         printf ("%-2.2s  %s%-4.4s%s    ", x_exec->hint, x_pre, x_cnt, x_mul);
      } else {
         printf ("%*.*s", e_wide, e_wide, "                     ");
      }
      printf ("%-2.2s  %-5d %2d %-40.40s\n", x_proc->hint, x_proc->rpid, x_proc->p_lvl, x_proc->cmdline);
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   printf ("widest = %d, multiples = %d, height = %d\n", w, m, n);
   return 0;
}

static    int      e_len        = 0;
static    int      e_wide       = 0;
static    int      e_mult       = 0;
static    int      e_index      = 0;
static    char     e_flag       [LEN_LABEL];
static    int      p_index      = 0;
static    char     p_flag       [LEN_LABEL];
static    int      l_index      = 0;
static    int      l_show       = 0;
static    int      l_every      = 0;
static    int      l_core       = 0;
static    int      l_multi      = 0;
static    int      l_singles    = 0;

char
NCURSE_proc_showlib     (int a_line, char *a_print)
{
   tLIBS      *x_libs      = NULL;
   int         x_line      =    0;
   int         n           =    0;
   int         c           =    0;
   char        x_cnt       [LEN_TERSE] = "";
   char        x_cnt2      [LEN_TERSE] = "";
   char        x_print     [LEN_HUND] = "";
   /*---(titles)-------------------------*/
   if (a_line == 0) {
      sprintf (a_print, "EVERY (%d) used in every process", l_every);
      return 0;
   }
   else if (a_line == l_every + 2) {
      sprintf (a_print, "CORE (%d) 10+ procs, 2+ execs", l_core);
      return 0;
   }
   else if (a_line == l_every + l_core + 4) {
      sprintf (a_print, "MULTI (%d) more than one exec", l_multi);
      return 0;
   }
   /*---(entries)------------------------*/
   if      (a_line < l_every + 1) {
      n = a_line;
      LIBS_by_cursor (&x_libs, YDLST_DHEAD);
      while (x_libs != NULL) {
         if (x_libs->u_flag == 'e')  ++c;
         if (n == c)  break;
         LIBS_by_cursor (&x_libs, YDLST_DNEXT);
      }
   } else {
      n = a_line - 4;
      LIBS_by_cursor (&x_libs, YDLST_DHEAD);
      while (x_libs != NULL) {
         if (x_libs->u_flag == 'c')  ++c;
         if (n == c)  break;
         LIBS_by_cursor (&x_libs, YDLST_DNEXT);
      }
   }
   /*> else if (n < l_every + l_core)  x_line += 3;                                  <*/

   if (x_libs != NULL) {
      snprintf (x_cnt , 6, "Ï€%d€€", x_libs->t_count);
      snprintf (x_cnt2, 5, "€%d€€", x_libs->e_count);
      snprintf (x_print, 47, "%s%s %s", x_cnt, x_cnt2, x_libs->terse);
      snprintf (a_print, 51, "%-34.34s  %-2.2s", x_print, x_libs->hint);
   } else {
      snprintf (a_print, 51, "%s", S_SPC);
   }
   return 0;
}

char
NCURSE_proc_show        (tPROC *a_proc)
{
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_cnt2      [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        x_lvl       [LEN_TITLE] = "";
   tEXEC      *x_exec      = NULL;
   tLIBS      *x_libs      = NULL;
   char        x_eprint    [LEN_HUND]  = "";
   char        x_pprint    [LEN_HUND]  = "";
   char        x_tprint    [LEN_HUND]  = "";
   char        x_lprint    [LEN_HUND]  = "";
   int         i           = 0;
   static int  x_line      = 0;
   /*---(prepare)------------------------*/
   if (a_proc == p_head)  x_line = 0;
   else                   ++x_line;
   x_exec = a_proc->e_link;
   /*---(executable)---------------------*/
   if (x_exec->e_seq == 0) {
      /*---(prefix)----------------------*/
      snprintf (x_pre, e_len + 3, "%s Ï%s", x_exec->base, S_MID);
      strllower (x_pre, LEN_TITLE);
      /*---(count)-----------------------*/
      if (x_exec->p_count > 1)  snprintf (x_cnt, LEN_TERSE, "-%d%s", x_exec->p_count, S_MID);
      else                      snprintf (x_cnt, LEN_TERSE, "%-4.4s", S_MID);
      /*---(multi)-----------------------*/
      snprintf (x_mul, e_mult * 2 + 1, "%s", S_MID);
      /*---(output)----------------------*/
      sprintf (x_eprint, "%-2.2s  %s%-4.4s", x_exec->hint, x_pre, x_cnt);
   } else {
      snprintf (x_mul, e_mult * 2 + 1, "%s", S_SPC);
      sprintf (x_eprint, "%*.*s", e_wide, e_wide, "                          ");
   }
   ++(x_exec->e_seq);
   /*---(multi-use betweens)----------*/
   for (i = 1; i <= e_mult; ++i) {
      if (e_flag [i] > 0) {
         if (x_exec->e_seq == 1)  x_mul [(i * 2) - 2] = 'Œ';
         else                     x_mul [(i * 2) - 2] = '';
      }
   }
   /*---(assign multi-use column)--------*/
   if (x_exec->p_count > 1) {
      if (x_exec->e_col == 0) {
         x_exec->e_col = ++p_index;
      }
      e_flag [x_exec->e_col] = x_exec->e_seq;
   }
   a_proc->e_seq = x_exec->e_seq;
   /*---(multi-use ties)--------------*/
   if (x_exec->e_col > 0 && a_proc->e_seq == 1) {
      x_mul [x_exec->e_col * 2 - 2] = '‰';
   } else if (x_exec->e_col > 0 && a_proc->e_seq == x_exec->p_count) {
      x_mul [x_exec->e_col * 2 - 2] = '„';
      e_flag [x_exec->e_col] = 0;
   } else if (x_exec->e_col > 0) {
      x_mul [x_exec->e_col * 2 - 2] = '‡';
   }
   /*---(multi-use fill-in)-----------*/
   if (x_exec->e_col > 0) {
      for (i = x_exec->e_col; i <= e_mult; ++i) {
         x_mul [i * 2 - 1] = '€';
         if (i > x_exec->e_col) {
            if (e_flag [i] > 0)  x_mul [i * 2 - 2] = 'Œ';
            else                 x_mul [i * 2 - 2] = '€';
         }
      }
   }
   /*---(show exec)-------------------*/
   /*> printf ("%sÏ %2d %2d %sÏ\n", x_eprint, x_exec->e_col, x_exec->e_seq, x_mul);   <*/
   printf ("%s%sÏ ", x_eprint, x_mul);
   /*---(proc-tree defaults)-------------*/
   strcpy (x_lvl, "");
   for (i = 0; i < a_proc->p_lvl; ++i)   strlcat (x_lvl, "··", LEN_TITLE);
   for (i = a_proc->p_lvl; i < 20; ++i)  p_flag [i] = 0;
   /*---(proc-tree specifics)------------*/
   if (a_proc->p_lvl > 0) {
      if (a_proc->h_next == NULL) {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ï';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         p_flag [a_proc->p_lvl - 1] = 0;
      } else {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ï';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         p_flag [a_proc->p_lvl - 1] = 1;
      }
   }
   /*---(proc-tree fills)----------------*/
   for (i = 0; i < a_proc->p_lvl; ++i) {
      if (p_flag [i] > 0 && x_lvl [i * 2] != 'Ï')  x_lvl [i * 2] = '';
   }
   /*---(show proc)----------------------*/
   /*> printf ("%-2.2s  %-5d %2d %2d %s%-40.40s\n", a_proc->hint, a_proc->rpid, a_proc->p_lvl, a_proc->p_seq, x_lvl, a_proc->cmdline);   <*/
   snprintf (x_pprint, 75, "%-5d %-2.2s %s%s······································································", a_proc->rpid, a_proc->hint, x_lvl, a_proc->cmdline);
   snprintf (x_tprint,  6, "€€%d€€-", a_proc->t_count);
   /*---(show libs)----------------------*/
   NCURSE_proc_showlib     (x_line, x_lprint);
   /*> LIBS_by_index (&x_libs, l_index++);                                                      <* 
    *> while (x_libs != NULL && (x_libs->t_count <= 9 || x_libs->e_count <= 1)) {               <* 
    *>    LIBS_by_index (&x_libs, l_index++);                                                   <* 
    *> }                                                                                        <* 
    *> if (x_libs != NULL) {                                                                    <* 
    *>    snprintf (x_cnt , 6, "Ï€%d€€", x_libs->t_count);                                      <* 
    *>    snprintf (x_cnt2, 5, "€%d€€", x_libs->e_count);                                       <* 
    *>    snprintf (x_lprint, 47, "%s%s%c %s", x_cnt, x_cnt2, x_libs->u_flag, x_libs->terse);   <* 
    *> } else {                                                                                 <* 
    *>    snprintf (x_lprint, 47, "%s", S_SPC);                                                 <* 
    *> }                                                                                        <*/
   printf ("%s·%sÏ     %s\n", x_pprint, x_tprint, x_lprint);
   /*---(done)---------------------------*/
   return 0;
}

char
NCURSE_proc_level       (char a_lvl, tPROC *a_proc)
{
   char        x_lvl       [LEN_TITLE] = "";
   tPROC      *x_proc      = NULL;
   NCURSE_proc_show (a_proc);
   x_proc = a_proc->h_head;
   while (x_proc != NULL) {
      NCURSE_proc_level (a_lvl + 1, x_proc);
      x_proc = x_proc->h_next;
   }
   return 0;
}

char
NCURSE_proc__libcnt     (tLIBS *a_libs)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tTIES      *x_ties      = NULL;
   int         c           =    0;
   /*---(clear count)--------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      x_exec->l_note = '-';
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(mark all execs)-----------------*/
   TIES_by_libs_cursor (&x_ties, a_libs, YDLST_DHEAD);
   while (x_ties != NULL) {
      x_ties->p_link->e_link->l_note = 'y';
      TIES_by_libs_cursor (&x_ties, a_libs, YDLST_DNEXT);
   }
   /*---(count execs)--------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      if (x_exec->l_note != '-')  c++;
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(save)---------------------------*/
   a_libs->e_count = c;
   if (c == e_count) {
      ++l_every;
      a_libs->u_flag = 'e';
   } else if (a_libs->t_count > 10 && a_libs->e_count > 1) {
      ++l_core;
      a_libs->u_flag = 'c';
   } else if (a_libs->e_count > 1) {
      ++l_multi;
      a_libs->u_flag = 'm';
   } else {
      ++l_singles;
      a_libs->u_flag = '-';
   }
   a_libs->u_line  = 0;
   /*---(complete)-----------------------*/
   return c;
}

char
NCURSE_proc_prepare     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tLIBS      *x_libs      = NULL;
   char        l           =    0;
   char        n           =    0;
   int         i           =    0;
   /*---(reset exec)---------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      l = strlen (x_exec->base);
      if (l > e_len)  e_len = l;
      x_exec->e_col   = 0;
      x_exec->e_seq   = 0;
      x_exec->e_shown = '-';
      if (x_exec->p_count > 1)  ++e_mult;
      n += x_exec->p_count;
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(reset proc)---------------------*/
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL) { 
      x_proc->e_seq = 0;
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   /*---(reset libs)---------------------*/
   l_index   = 0;
   l_show    = 0;
   l_every   = 0;
   l_core    = 0;
   l_multi   = 0;
   l_singles = 0;
   LIBS_by_index (&x_libs, l_index++);
   while (x_libs != NULL) {
      NCURSE_proc__libcnt (x_libs);
      LIBS_by_index (&x_libs, l_index++);
   }
   l_index = 0;
   /*---(reset flags)--------------------*/
   e_index = 0;
   p_index = 0;
   for (i = 0; i < 20; ++i) {
      e_flag [i] = 0;
      p_flag [i] = 0;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
NCURSE_proc_list        (void)
{
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   char        r           [LEN_HUND]  = "";
   char        u           [LEN_HUND]  = "";
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        l           =    0;
   char        n           =    0;
   char        x_seq       =    0;
   int         i           =    0;
   /*---(prepare)------------------------*/
   NCURSE_proc_prepare ();
   /*---(title)--------------------------*/
   e_wide = 4 + e_len + 2 + 4;
   snprintf (r, e_wide, "[ %d execs····································", e_count);
   l = strlen (r);
   r [l - 1] = ']';
   snprintf (s, e_mult * 2 + 3, "[ %d multi···············", e_mult);
   l = strlen (s);
   s [l - 1] = ']';
   snprintf (t, 77, "[·· %d procs············································································", p_count);
   l = strlen (t);
   t [l - 1] = ']';
   snprintf (u, 50, "[·· %d libs·············································································", l_count);
   l = strlen (u);
   u [l - 1] = ']';
   printf ("%s%s%s%s\n", r, s, t, u);
   NCURSE_proc_level (0, p_head);
   /*> printf ("widest = %d, multiples = %d, height = %d\n", e_len, e_mult, n);       <*/
   return 0;
}


