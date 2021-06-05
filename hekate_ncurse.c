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

char
NCURSE_proc_show        (tPROC *a_proc)
{
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        x_lvl       [LEN_TITLE] = "";
   tEXEC      *x_exec      = NULL;
   char        x_eprint    [LEN_HUND]  = "";
   int         i           = 0;
   /*---(prepare)------------------------*/
   x_exec = a_proc->e_link;
   /*---(executable)---------------------*/
   if (x_exec->e_seq == 0) {
      /*---(prefix)----------------------*/
      snprintf (x_pre, e_len + 3, "%s %s", x_exec->base, S_MID);
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
   printf ("%s%sÏ  ", x_eprint, x_mul);
   /*---(proc-tree defaults)-------------*/
   strcpy (x_lvl, "");
   for (i = 0; i < a_proc->p_lvl; ++i)   strlcat (x_lvl, "²²", LEN_TITLE);
   for (i = a_proc->p_lvl; i < 20; ++i)  p_flag [i] = 0;
   /*---(proc-tree specifics)------------*/
   if (a_proc->p_lvl > 0) {
      if (a_proc->h_next == NULL) {
         x_lvl [(a_proc->p_lvl - 1) * 2] = 'ˆ';
         p_flag [a_proc->p_lvl - 1] = 0;
      } else {
         x_lvl [(a_proc->p_lvl - 1) * 2] = 'Š';
         p_flag [a_proc->p_lvl - 1] = 1;
      }
   }
   /*---(proc-tree fills)----------------*/
   for (i = 0; i < a_proc->p_lvl; ++i) {
      if (p_flag [i] > 0)  x_lvl [i * 2] = '';
   }
   /*---(show proc)----------------------*/
   printf ("%-2.2s  %-5d %2d %2d %s%-40.40s\n", a_proc->hint, a_proc->rpid, a_proc->p_lvl, a_proc->p_seq, x_lvl, a_proc->cmdline);
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
NCURSE_proc_prepare     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
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
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_mul       [LEN_TITLE] = "";
   char        l           =    0;
   char        n           =    0;
   char        x_seq       =    0;
   int         i           =    0;
   /*---(prepare)------------------------*/
   NCURSE_proc_prepare ();
   /*> EXEC_by_cursor (&x_exec, YDLST_DHEAD);                                         <* 
    *> while (x_exec != NULL) {                                                       <* 
    *>    l = strlen (x_exec->base);                                                  <* 
    *>    if (l > e_len)  e_len = l;                                                  <* 
    *>    x_exec->e_col   = 0;                                                        <* 
    *>    x_exec->e_shown = '-';                                                      <* 
    *>    if (x_exec->p_count > 1)  ++e_mult;                                         <* 
    *>    n += x_exec->p_count;                                                       <* 
    *>    x_seq  = 0;                                                                 <* 
    *>    PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DHEAD);                         <* 
    *>    while (x_proc != NULL) {                                                    <* 
    *>       x_proc->e_seq = ++x_seq;                                                 <* 
    *>       PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DNEXT);                      <* 
    *>    }                                                                           <* 
    *>    EXEC_by_cursor (&x_exec, YDLST_DNEXT);                                      <* 
    *> }                                                                              <* 
    *> p_index = 0;                                                                   <* 
    *> for (i = 0; i < 20; ++i) p_flag [i] = 0;                                       <*/
   /*---(title)--------------------------*/
   e_wide = 4 + e_len + 2 + 4;
   snprintf (s, e_wide, "%d exec ············································", e_count);
   snprintf (t, LEN_HUND, "%d proc ············································", p_count);
   printf ("%s %s\n", s, t);
   NCURSE_proc_level (0, p_head);
   printf ("widest = %d, multiples = %d, height = %d\n", e_len, e_mult, n);
   return 0;
}


