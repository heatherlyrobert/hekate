/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"

#define  S_MID  "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€"
#define  S_SPC  "                                                      "







static int    s_curx    = 0;
static int    s_cury    = 0;
static tPROC *s_cproc   = NULL;



/*====================------------------------------------====================*/
/*===----                       program wide                           ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
YVIKEYS_init            (void)
{
   char        rc          =    0;
   /*---(row and column headers)---------*/
   use_legacy_coding (2);
   clear ();
   strlcpy (my.hint, "--", LEN_TERSE);
   rc = yVIKEYS_hint_config (YVIKEYS_hinter   , YVIKEYS_unhinter);
   rc = yVIKEYS_srch_config (YVIKEYS_searcher , YVIKEYS_unsearcher);
   yVIKEYS_cmds_direct   (";+");
   yVIKEYS_dump_add ("proc"       , PROC_dump);
   yVIKEYS_view_option (YVIKEYS_OVERLAY, "memory" , YVIKEYS_memory, "current point statistics");
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      processing hints                        ----===*/
/*====================------------------------------------====================*/
static void  o___HINTS___________o () { return; }

char
YVIKEYS_unhinter_full   (void)
{
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tLIBS      *x_libs      = NULL;
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL)  {
      NOT_HINT (x_exec->e_note);
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL)  {
      NOT_HINT (x_proc->p_note);
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   LIBS_by_cursor (&x_libs, YDLST_DHEAD);
   while (x_libs != NULL)  {
      NOT_HINT (x_libs->l_note);
      LIBS_by_cursor (&x_libs, YDLST_DNEXT);
   }
   return 0;
}

char
YVIKEYS__hint_cursor    (char a_move)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_last      = NULL;
   tPROC      *x_proc      = NULL;
   char        x_next      =  '-';
   static tPROC *x_save    = NULL;
   /*---(clear save)---------------------*/
   if (a_move == 'r') {
      x_save = NULL;
      return 0;
   }
   /*---(return current)-----------------*/
   if (a_move == '.') {
      x_proc = x_save;
      yVIKEYS_jump (0, 1, x_proc->f_seq, 0);
      return 0;
   }
   /*---(walk procs)---------------------*/
   PROC_by_seq (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL)  {
      /*---(set save)--------------------*/
      if (a_move == 's' && x_proc->f_seq == g_ymap.gcur) {
         x_save = x_proc;
         break;
      }
      /*---(cursoring)-------------------*/
      if (HAS_HINT (x_proc->p_note)) {
         if (a_move == '[')  break;
         if (x_next == 'y')  break;
         switch (a_move) {
         case '<' :
            if (x_last != NULL) x_proc = x_last;
            break;
         }
         if (a_move == '>') {
            x_next = 'y';
            break;
         }
         x_last = x_proc;
      }
      /*---(next)------------------------*/
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
      /*---(done)------------------------*/
   }
   /*---(last cursor)--------------------*/
   if (a_move == ']')  x_proc = x_last;
   /*---(handle)-------------------------*/
   --rce;  if (x_proc == NULL)  return rce;
   x_save = x_proc;
   yVIKEYS_jump (0, 1, x_proc->f_seq, 0);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__hint_proc      (char *a_hint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   PROC_by_hint (&x_proc, a_hint);
   --rce;  if (x_proc == NULL)  return rce;
   SET_HINT (x_proc->p_note);
   yVIKEYS_hint_found (x_proc->hint, 0, 0, x_proc->f_seq, 0);
   /*> yVIKEYS_jump (0, 1, x_proc->f_seq, 0);                                         <*/
   PROC_by_seq_cursor (&x_proc, 's');
   return 0;
}

char
YVIKEYS__hint_exec      (char *a_hint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   /*---(header)--------------------s----*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   EXEC_by_hint (&x_exec, a_hint);
   DEBUG_SRCH   yLOG_point   ("x_exec"    , x_exec);
   --rce;  if (x_exec == NULL)  return rce;
   /*> PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DHEAD);                            <*/
   PROC_by_seq_cursor  (&x_proc, '[');
   DEBUG_SRCH   yLOG_point   ("x_proc"    , x_proc);
   while (x_proc != NULL)  {
      DEBUG_SRCH   yLOG_complex ("proc"      , "%5.5d, %-10.10p, %-10.10p", x_proc->rpid, x_proc->e_link, x_exec);
      if (x_proc->e_link == x_exec) {
         DEBUG_SRCH   yLOG_note    ("MARKING");
         SET_HINT (x_proc->p_note);
         yVIKEYS_hint_found (x_proc->hint, 0, 0, x_proc->f_seq, 0);
      }
      PROC_by_seq_cursor (&x_proc, '>');
   }
   /*> yVIKEYS_jump (0, 1, x_exec->f_seq, 0);                                         <*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS__hint_libs      (char *a_hint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   tLIBS      *x_libs      = NULL;
   tTIES      *x_ties      = NULL;
   LIBS_by_hint (&x_libs, a_hint);
   --rce;  if (x_libs == NULL)  return rce;
   /*> SET_HINT (x_libs->l_note);                                                     <*/
   TIES_by_libs_cursor (&x_ties, x_libs, YDLST_DHEAD);
   while (x_ties != NULL)  {
      if (x_proc == NULL)  x_proc = x_ties->p_link;
      if (x_ties->p_link->f_seq < x_proc->f_seq)  x_proc = x_ties->p_link;
      SET_HINT (x_ties->p_link->p_note);
      yVIKEYS_hint_found (x_ties->p_link->hint, 0, 0, x_ties->p_link->f_seq, 0);
      /*> SET_HINT (x_ties->p_link->e_link->e_note);                                  <*/
      TIES_by_libs_cursor (&x_ties, x_libs, YDLST_DNEXT);
   }
   /*> yVIKEYS_jump (0, 1, x_proc->f_seq, 0);                                         <*/
   PROC_by_seq_cursor (&x_proc, 's');
   return 0;
}

char
YVIKEYS_hinter          (char *a_hint)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        l           =    0;
   tPROC      *x_proc      = NULL;
   /*---(header)--------------------s----*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   DEBUG_SRCH   yLOG_point   ("a_hint"    , a_hint);
   --rce;  if (a_hint == NULL) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_SRCH   yLOG_info    ("a_hint"    , a_hint);
   l = strlen (a_hint);
   DEBUG_SRCH   yLOG_value   ("l"         , l);
   --rce;  if (l < 1) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (strcmp (a_hint, "update") == 0) {
      DEBUG_SRCH   yLOG_note    ("update only");
      YVIKEYS_update ();
      DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(real hints)---------------------*/
   /*> PROC_by_seq_cursor (&x_proc, 'r');                                             <*/
   --rce;  if (strncmp (a_hint, ";;", 2) != 0) {
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (my.hint, a_hint + 2, LEN_TERSE);
   DEBUG_SRCH   yLOG_info    ("my.hint"   , my.hint);
   rc = YVIKEYS__hint_proc (my.hint);
   DEBUG_SRCH   yLOG_value   ("proc"      , rc);
   if (rc < 0) {
      rc = YVIKEYS__hint_exec (my.hint);
      DEBUG_SRCH   yLOG_value   ("exec"      , rc);
   }
   if (rc < 0) {
      rc = YVIKEYS__hint_libs (my.hint);
      DEBUG_SRCH   yLOG_value   ("libs"      , rc);
   }
   YVIKEYS_update ();
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS_unhinter        (int b, int x, int y, int z)
{
   tPROC      *x_proc      = NULL;
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL)  {
      if (x_proc->f_seq == y)  NOT_HINT (x_proc->p_note);
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   YVIKEYS_update ();
   return 0;
}

char
YVIKEYS_searcher        (char *a_search)
{
   /*---(locals)-----------+------+----+-*/
   char        rce         =   -10;
   char        rc          =     0;
   tPROC      *x_proc      = NULL;
   char        t           [LEN_RECD]  = "";
   char        x_label     [LEN_LABEL] = "";
   /*---(header)--------------------s----*/
   DEBUG_SRCH   yLOG_enter   (__FUNCTION__);
   DEBUG_SRCH   yLOG_point   ("a_search"  , a_search);
   /*---(defenses)---------------------------*/
   --rce;  if (a_search == NULL) {
      DEBUG_SRCH   yLOG_note    ("can not use null search");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
   }
   DEBUG_SRCH   yLOG_info    ("a_search"  , a_search);
   rc = yREGEX_comp (a_search + 1);
   DEBUG_SRCH   yLOG_value   ("comp rc"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_SRCH   yLOG_note    ("could not compile search");
      DEBUG_SRCH   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(process range)----------------------*/
   x_proc = h_head;
   while (x_proc != NULL) {
      sprintf (t, "%6d %s", x_proc->rpid, x_proc->cmdline);
      DEBUG_SRCH   yLOG_info    ("t"              , t);
      rc = yREGEX_exec (t);
      DEBUG_SRCH   yLOG_value   ("exec rc"   , rc);
      if (rc > 0) {
         sprintf (x_label, "%d", x_proc->rpid);
         yVIKEYS_srch_found (x_label, 0, 1, x_proc->f_seq, 0);
         SET_SRCH (x_proc->p_note);
      }
      x_proc = x_proc->h_next;
   }
   /*---(complete)---------------------------*/
   DEBUG_SRCH   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS_unsearcher      (int b, int x, int y, int z)
{
   /*---(locals)-----------+------+----+-*/
   char        rce         =   -10;
   char        rc          =     0;
   tPROC      *x_proc      = NULL;
   PROC_by_seq (&x_proc, y);
   NOT_SRCH (x_proc->p_note);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   update for new content                     ----===*/
/*====================------------------------------------====================*/
static void  o___UPDATE__________o () { return; }

char
YVIKEYS__prep_exec      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   char        l           =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.e_len   = 0;
   my.e_wide  = 0;
   my.e_mult  = 0;
   for (i = 0; i < LEN_LABEL; ++i)   my.e_flag [i] = 0;
   /*---(reset exec)---------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      l = strlen (x_exec->base);
      if (l > my.e_len)  my.e_len = l;
      x_exec->f_seq   = 0;
      x_exec->f_temp  = 0;
      x_exec->e_col   = 0;
      x_exec->e_seq   = 0;
      x_exec->e_shown = '-';
      if (x_exec->p_count > 1)  ++my.e_mult;
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   my.e_wide = 4 + my.e_len + 2 + 4;
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS__prep_proc      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tPROC      *x_proc      = NULL;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.p_index = 0;
   for (i = 0; i < LEN_LABEL; ++i)   my.p_flag [i] = 0;
   /*---(reset proc)---------------------*/
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL) { 
      x_proc->e_seq   = 0;
      x_proc->p_lvl   = 0;
      x_proc->p_seq   = 0;
      x_proc->h_head  = NULL;
      x_proc->h_tail  = NULL;
      x_proc->h_count = 0;
      x_proc->h_sibs  = NULL;
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS__prep_lib_cnt   (tLIBS *a_libs)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tTIES      *x_ties      = NULL;
   int         c           =    0;
   /*---(clear count)--------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      NOT_INCL (x_exec->l_note);
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(mark all execs)-----------------*/
   TIES_by_libs_cursor (&x_ties, a_libs, YDLST_DHEAD);
   while (x_ties != NULL) {
      SET_INCL (x_ties->p_link->e_link->l_note);
      TIES_by_libs_cursor (&x_ties, a_libs, YDLST_DNEXT);
   }
   /*---(count execs)--------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      if (HAS_INCL (x_exec->l_note))  c++;
      NOT_INCL (x_exec->l_note);
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(save)---------------------------*/
   a_libs->e_count = c;
   if (c == e_count) {
      ++my.l_every;
      a_libs->u_flag = 'e';
      a_libs->l_seq  = my.l_every;
   } else if (a_libs->t_count >= 20 && a_libs->e_count > 1) {
      ++my.l_core;
      a_libs->u_flag = 'c';
      a_libs->l_seq  = my.l_core;
   } else if (a_libs->e_count > 1) {
      ++my.l_multi;
      a_libs->u_flag = 'm';
   } else {
      ++my.l_singles;
      a_libs->u_flag = '-';
   }
   a_libs->u_line  = 0;
   /*---(complete)-----------------------*/
   return c;
}

char
YVIKEYS__prep_libs      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tLIBS      *x_libs      = NULL;
   int         n           =    0;
   char        x_lprint    [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.l_index   = 0;;
   my.l_show    = 0;
   my.l_every   = 0;
   my.l_core    = 0;
   my.l_multi   = 0;
   my.l_singles = 0;
   /*---(reset libs)---------------------*/
   LIBS_by_cursor (&x_libs, YDLST_DHEAD);
   while (x_libs != NULL) {
      YVIKEYS__prep_lib_cnt (x_libs);
      LIBS_by_cursor (&x_libs, YDLST_DNEXT);
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS__treeify_hook   (tPROC *a_curr)
{
   a_curr->f_seq = ++(my.f_seq);
   if    (h_tail == NULL) { h_head = a_curr; }
   else { a_curr->h_prev  = h_tail; h_tail->h_next = a_curr; }
   h_tail = a_curr;
   return 0;
}

char
YVIKEYS__treeify        (char a_level, tPROC *a_parent)
{
   tPROC      *x_proc      = NULL;
   int         c           = 0;
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_value   ("a_parent"  , a_parent->rpid);
   YVIKEYS__treeify_hook (a_parent);
   x_proc = p_head;
   while  (x_proc != NULL) {
      DEBUG_INPT   yLOG_value   ("x_proc"    , x_proc->rpid);
      if (x_proc->ppid == a_parent->rpid) {
         DEBUG_INPT   yLOG_note   ("found CHILD");
         if (a_parent->h_head == NULL) {
            a_parent->h_head  = x_proc;
            a_parent->h_tail  = x_proc;
            a_parent->h_count = 1;
         } else {
            a_parent->h_tail->h_sibs = x_proc;
            a_parent->h_tail         = x_proc;
            ++(a_parent->h_count);
         }
         x_proc->h_sibs = NULL;
         x_proc->p_lvl = a_level;
         x_proc->p_seq = ++c;
         YVIKEYS__treeify (a_level + 1, x_proc);
      }
      x_proc = x_proc->m_next;
   }
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
YVIKEYS__prep_eprint    (tPROC *a_proc)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   char        x_pre       [LEN_TITLE] = "";
   char        x_cnt       [LEN_TERSE] = "";
   char        x_eprint    [LEN_HUND]  = "";
   char        x_mprint    [LEN_HUND]  = "";
   /*---(defense)------------------------*/
   --rce;  if (a_proc   == NULL)  return rce;
   x_exec = a_proc->e_link;
   --rce;  if (x_exec   == NULL)  return rce;
   /*---(defaults)-----------------------*/
   sprintf   (x_eprint, "%*.*s", my.e_wide, my.e_wide, S_SPC);
   snprintf  (x_mprint, my.e_mult * 2 + 1, "%s", S_SPC);
   /*---(first)--------------------------*/
   if (x_exec->e_seq == 0) {
      /*---(prefix)----------------------*/
      snprintf  (x_pre, my.e_len + 3, "%s Ï%s", x_exec->base, S_MID);
      strllower (x_pre, LEN_TITLE);
      /*---(count)-----------------------*/
      if (x_exec->p_count > 1)  snprintf (x_cnt, LEN_TERSE, "-%d%s", x_exec->p_count, S_MID);
      else                      snprintf (x_cnt, LEN_TERSE, "%-4.4s", S_MID);
      /*---(multi)-----------------------*/
      snprintf  (x_mprint, my.e_mult * 2 + 1, "%s", S_MID);
      /*---(output)----------------------*/
      if (yVIKEYS_hinting ())  sprintf   (x_eprint, "%-2.2s  %s%-4.4s", x_exec->hint, x_pre, x_cnt);
      else                     sprintf   (x_eprint, "%-2.2s  %s%-4.4s", "··"        , x_pre, x_cnt);
      /*---(update)----------------------*/
      x_exec->f_seq = x_exec->f_temp = a_proc->f_seq;
      strlcpy (x_exec->e_print, x_eprint, LEN_HUND);
      strlcpy (x_exec->e_alt  , x_eprint, LEN_HUND);
      /*---(done)------------------------*/
   }
   /*---(save)---------------------------*/
   strlcpy (a_proc->e_print, x_eprint, LEN_HUND);
   strlcpy (a_proc->m_print, x_mprint, LEN_HUND);
   /*---(update)-------------------------*/
   ++(x_exec->e_seq);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__prep_mprint    (tPROC *a_proc)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   int         i           = 0;
   char        x_mprint    [LEN_HUND]  = "";
   /*---(defense)------------------------*/
   --rce;  if (a_proc   == NULL)  return rce;
   x_exec = a_proc->e_link;
   --rce;  if (x_exec   == NULL)  return rce;
   /*---(prepare)---------------------*/
   strlcpy (x_mprint, a_proc->m_print, LEN_HUND);
   /*---(multi-use betweens)----------*/
   for (i = 1; i <= my.e_mult; ++i) {
      if (my.e_flag [i] > 0) {
         if (x_exec->e_seq == 1)  x_mprint [(i * 2) - 2] = 'Œ';
         else                     x_mprint [(i * 2) - 2] = '';
      }
   }
   /*---(assign multi-use column)--------*/
   if (x_exec->p_count > 1) {
      if (x_exec->e_col == 0) {
         x_exec->e_col = ++my.p_index;
      }
      my.e_flag [x_exec->e_col] = x_exec->e_seq;
   }
   a_proc->e_seq = x_exec->e_seq;
   /*---(multi-use ties)--------------*/
   if (x_exec->e_col > 0 && a_proc->e_seq == 1) {
      x_mprint [x_exec->e_col * 2 - 2] = '‰';
   } else if (x_exec->e_col > 0 && a_proc->e_seq == x_exec->p_count) {
      x_mprint [x_exec->e_col * 2 - 2] = '„';
      my.e_flag [x_exec->e_col] = 0;
   } else if (x_exec->e_col > 0) {
      x_mprint [x_exec->e_col * 2 - 2] = '‡';
   }
   /*---(multi-use fill-in)-----------*/
   if (x_exec->e_col > 0) {
      for (i = x_exec->e_col; i <= my.e_mult; ++i) {
         x_mprint [i * 2 - 1] = '€';
         if (i > x_exec->e_col) {
            if (my.e_flag [i] > 0)  x_mprint [i * 2 - 2] = 'Œ';
            else                    x_mprint [i * 2 - 2] = '€';
         }
      }
   }
   /*---(save)---------------------------*/
   strlcpy (a_proc->m_print, x_mprint, LEN_HUND);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__prep_pprint    (tPROC *a_proc)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           = 0;
   char        x_lvl       [LEN_TITLE] = "";
   char        x_pprint    [LEN_HUND]  = "";
   char        x_tprint    [LEN_LABEL] = "";
   /*---(defense)------------------------*/
   --rce;  if (a_proc   == NULL)  return rce;
   /*---(proc-tree defaults)-------------*/
   strcpy (x_lvl, "");
   for (i = 0; i < a_proc->p_lvl; ++i)   strlcat (x_lvl, "··", LEN_TITLE);
   for (i = a_proc->p_lvl; i < 20; ++i)  my.p_flag [i] = 0;
   /*---(proc-tree specifics)------------*/
   if (a_proc->p_lvl > 0) {
      if (a_proc->h_sibs == NULL) {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ï';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         my.p_flag [a_proc->p_lvl - 1] = 0;
      } else {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ï';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         my.p_flag [a_proc->p_lvl - 1] = 1;
      }
   }
   /*---(proc-tree fills)----------------*/
   for (i = 0; i < a_proc->p_lvl; ++i) {
      if (my.p_flag [i] > 0 && x_lvl [i * 2] != 'Ï')  x_lvl [i * 2] = '';
   }
   /*---(final)--------------------------*/
   if (yVIKEYS_hinting ())  snprintf (x_pprint, 75, "%-5d %-2.2s %s%s······································································", a_proc->rpid, a_proc->hint, x_lvl, a_proc->cmdline);
   else                     snprintf (x_pprint, 75, "%-5d %-2.2s %s%s······································································", a_proc->rpid, "··"        , x_lvl, a_proc->cmdline);
   snprintf (x_tprint,  6, "€€%d€€-", a_proc->t_count);
   /*---(save)---------------------------*/
   strlcpy (a_proc->p_print, x_pprint, LEN_HUND);
   strlcpy (a_proc->t_print, x_tprint, LEN_LABEL);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__prep_lprint    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        x_cnt       [LEN_HUND]  = "";
   char        x_cnt2      [LEN_HUND]  = "";
   char        x_lprint    [LEN_HUND]  = "";
   tLIBS      *x_libs      = NULL;
   LIBS_by_cursor (&x_libs, YDLST_DHEAD);
   while (x_libs != NULL) {
      switch (x_libs->u_flag) {
      case 'e' : case 'c' : case 'm' :
         snprintf (x_cnt , 6, "Ï€%d€€", x_libs->t_count);
         snprintf (x_cnt2, 5, "€%d€€€", x_libs->e_count);
         break;
      case '-' : 
         snprintf (x_cnt , 6, "Ï€%d€€", x_libs->t_count);
         snprintf (x_cnt2, 5, "€€€€€€");
         break;
      }
      if (yVIKEYS_hinting ())  snprintf (x_lprint, 47, "%s%s %-24.24s  %s", x_cnt, x_cnt2, x_libs->terse, x_libs->hint);
      else                     snprintf (x_lprint, 47, "%s%s %-24.24s  %s", x_cnt, x_cnt2, x_libs->terse, "··"        );
      strlcpy (x_libs->l_print, x_lprint, LEN_HUND);
      LIBS_by_cursor (&x_libs, YDLST_DNEXT);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__prep_print     (char a_lvl, tPROC *a_proc)
{
   char        x_lvl       [LEN_TITLE] = "";
   tPROC      *x_proc      = NULL;
   if (a_proc == NULL)  return 0;
   YVIKEYS__prep_eprint (a_proc);
   YVIKEYS__prep_mprint (a_proc);
   YVIKEYS__prep_pprint (a_proc);
   x_proc = a_proc->h_head;
   while (x_proc != NULL) {
      YVIKEYS__prep_print (a_lvl + 1, x_proc);
      x_proc = x_proc->h_sibs;
   }
   return 0;
}

char
YVIKEYS_update          (void)
{
   YVIKEYS__prep_exec   ();
   YVIKEYS__prep_proc   ();
   YVIKEYS__prep_libs   ();
   my.f_seq = 0;
   h_head = h_tail = NULL;
   YVIKEYS__treeify     (1, p_head);
   YVIKEYS__prep_print  (1, p_head);
   YVIKEYS__prep_lprint ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     simple drawing handlers                  ----===*/
/*====================------------------------------------====================*/
static void  o___SIMPLE__________o () { return; }

char         /*-> update the column labels -----------[ ------ [gz.D91.061.A5]*/ /*-[02.3000.323.!]-*/ /*-[--.---.---.--]-*/
YVIKEYS_xaxis           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         x_left      = 0;
   int         x_wide      = 0;
   int         x_bott      = 0;
   char        x_proc      [LEN_HUND]  = "";
   char        x_mult      [LEN_HUND]  = "";
   char        x_exec      [LEN_HUND]  = "";
   char        x_libs      [LEN_HUND]  = "";
   char        x_print     [LEN_RECD]  = "";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(get size)-----------------------*/
   yVIKEYS_view_size     (YVIKEYS_XAXIS, &x_left, &x_wide, &x_bott, NULL, NULL);
   /*---(whole title line)---------------*/
   snprintf (x_exec, my.e_wide, "[ %d execs····································", e_count);
   l = strlen (x_exec);
   x_exec [l - 1] = ']';
   snprintf (x_mult, my.e_mult * 2 + 3, "[ %d multi···············", my.e_mult);
   l = strlen (x_mult);
   x_mult [l - 1] = ']';
   snprintf (x_proc, 77, "[·· %d procs············································································", p_count);
   l = strlen (x_proc);
   x_proc [l - 1] = ']';
   snprintf (x_libs, 50, "[··ties··][·· %d libs·············································································", l_count);
   l = strlen (x_libs);
   x_libs [l - 1] = ']';
   snprintf (x_print, x_wide, "%s%s%s%s", x_exec, x_mult, x_proc, x_libs);
   yCOLOR_curs ("h_curr");
   mvprintw (x_bott, x_left, "%s", x_print);
   attrset (0);
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     main drawing handlers                    ----===*/
/*====================------------------------------------====================*/
static void  o___MAIN____________o () { return; }

char
YVIKEYS__proc_show      (tPROC *a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tPROC      *x_disp      = NULL;
   int         x_pline     =    0;
   int         x_eline     =    0;
   int         l           =    0;
   int         x_conn      =    0;
   int         x_mid       =    0;
   int         i           =    0;
   int         x_len       =    0;
   char        x_turn      =   -1;
   char        x_label     [LEN_LABEL] = "";
   char        c           =    0;
   /*---(prepare)------------------------*/
   x_pline = a_proc->f_seq - g_ymap.gbeg + 1;
   if (x_pline < 1)  return 0;
   if (x_pline > g_ymap.uavail)  return 0;
   l = my.e_wide + my.e_mult * 2 + 1;
   x_conn = my.m_left + l + strlen (a_proc->p_print) + strlen (a_proc->t_print) + 3;
   x_mid  = 2 + my.l_every + 2 + (my.l_core / 2);
   /*---(exec primary)-------------------*/
   x_exec  = a_proc->e_link;
   x_eline = x_exec->f_seq - g_ymap.gbeg + 1;
   if (x_eline > 0) {
      x_exec->f_temp = x_exec->f_seq;
      yCOLOR_curs ("9_form");
      mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left, "%s%sÏ", a_proc->e_print, a_proc->m_print);
      attrset (0);
   }
   /*---(exec alternate)-----------------*/
   else {
      x_eline = x_exec->f_temp - g_ymap.gbeg + 1;
      if (x_eline < 0) {
         x_exec->f_temp = a_proc->f_seq;
         x_eline = x_pline;
      }
      if (x_eline == x_pline) {
         yCOLOR_curs ("9_form");
         mvprintw (my.m_bott - my.m_tall + x_eline, my.m_left, "%s%sÏ", x_exec->e_print, a_proc->m_print);
         x_len = strlen (a_proc->m_print);
         for (i = 0; i < x_len; ++i) {
            switch (a_proc->m_print [i]) {
            case ' ' :
               mvprintw (x_eline, my.e_wide + i, "€");
               break;
            case '' : case 'Œ' :
               mvprintw (x_eline, my.e_wide + i, "Œ");
               break;
            case '‡' :
               mvprintw (x_eline, my.e_wide + i, "Š");
               x_turn = i;
               break;
            case '„' :
               mvprintw (x_eline, my.e_wide + i, "ˆ");
               x_turn = i;
               break;
            default  :
               x_turn = i;
               break;
            }
            if (x_turn >= 0) break;
         }
         attrset (0);
      } else {
         yCOLOR_curs ("9_form");
         mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left, "%s%sÏ", a_proc->e_print, a_proc->m_print);
         attrset (0);
      }
   }
   /*---(proc)---------------------------*/
   if (a_proc->f_seq == g_ymap.gcur) {
      yCOLOR_curs ("v_curr");
      s_cproc = a_proc;
   }
   else if (HAS_SRCH (a_proc->p_note)) yCOLOR_curs ("m_srch");
   else if (HAS_HINT (a_proc->p_note)) yCOLOR_curs ("m_hint");
   else if (HAS_MARK (a_proc->p_note)) yCOLOR_curs ("m_mark");
   else                                yCOLOR_curs ("h_norm");
   mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left + l, " %s·%sÏ", a_proc->p_print, a_proc->t_print);
   attrset (0);
   /*---(marks)--------------------------*/
   sprintf (x_label, "%d", a_proc->rpid);
   c = yVIKEYS_marking (x_label);
   if (c > 0) {
      yCOLOR_curs ("m_temp");
      mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left + l + 7, "'%c", c);
      attrset (0);
   }
   /*> mvprintw (my.m_bott - my.m_tall + x_pline, x_conn, "   ", a_proc->p_print, a_proc->t_print);   <*/
   mvprintw (my.m_bott - my.m_tall + x_pline, x_conn, "   ");
   if (a_proc->f_seq == g_ymap.gcur) {
      s_curx = my.m_left + l;
      s_cury = my.m_bott - my.m_tall + x_pline;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__main_exec      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tPROC      *x_proc      = NULL;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_other     = NULL;
   int         x_line      =    0;
   int         l           =    0;
   int         x_link      =    0;
   int         i           =    0;
   int         x_turn      =   -1;
   int         x_len       =    0;
   /*---(find proc)----------------------*/
   PROC_by_seq (&x_proc, g_ymap.gcur);
   if (x_proc == NULL)  x_proc = p_head;
   x_exec = x_proc->e_link;
   /*---(prepare)------------------------*/
   x_line = x_exec->f_temp - g_ymap.gbeg + 1;
   if (x_line < 1)  return 0;
   if (x_line > g_ymap.gend)  return 0;
   l = my.e_wide + my.e_mult * 2 + 1;
   /*---(exec)---------------------------*/
   yCOLOR_curs ("9_dang");
   mvprintw (my.m_bott - my.m_tall + x_line, my.m_left, "%s", x_exec->e_print);
   attrset (0);
   /*---(connector)----------------------*/
   x_link  = x_proc->f_seq - g_ymap.gbeg + 1;
   yCOLOR_curs ("9_dang");
   /*> mvprintw (66, 120, "proc  %2d  exec  %2d", x_proc->f_seq, x_exec->f_seq);      <*/
   mvprintw (x_link, l - 1, "Ï");
   x_len = strlen (x_proc->m_print);
   for (i = 0; i < x_len; ++i) {
      switch (x_proc->m_print [i]) {
      case '‡' : case '„' :
         x_turn = i;
      case 'Ï' : case '€' : case 'ƒ' : case '‰' :
         mvprintw (x_link, my.e_wide + i, "%c", x_proc->m_print [i]);
         break;
      }
   }
   /*> mvprintw (67, 120, "turn  %2d  %s", x_turn, x_proc->m_print);                  <*/
   for (i = x_exec->f_temp; i <= x_proc->f_seq; ++i) {
      PROC_by_seq (&x_other, i);
      switch (x_other->m_print [x_turn]) {
      case '‡' : case '„' : case 'Œ' : case '' :
      case 'Ï' : case '€' : case 'ƒ' : case '‰' :
         mvprintw (i - g_ymap.gbeg + 1, my.e_wide + x_turn, "%c", x_other->m_print [x_turn]);
         break;
      }
      /*> mvprintw (68, 120, "line  %2d  link  %2d  turn  %2d  i     %2d", x_line, x_link, x_turn, i);   <*/
   }
   PROC_by_seq (&x_other, x_line + g_ymap.gbeg - 1);
   /*> mvprintw (69, 120, "temp  %2d  other %2d  %s", x_exec->f_temp, x_other->f_seq, x_other->m_print);   <*/
   for (i = 0; i <= x_turn; ++i) {
      switch (x_other->m_print [i]) {
      case ' ' :
         mvprintw (x_line, my.e_wide + i, "€");
         break;
      case '‡' : case '„' :
      case 'Ï' : case '€' : case 'ƒ' : case '‰' :
         mvprintw (x_line, my.e_wide + i, "%c", x_other->m_print [i]);
         break;
      }
   }
   attrset (0);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__main_libs      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tPROC      *x_proc      = NULL;
   tLIBS      *x_libs      = NULL;
   tTIES      *x_ties      = NULL;
   tEXEC      *x_exec      = NULL;
   int         x_left      =  123;
   int         x_vert      =    0;
   int         e           =    0;
   int         c           =    0;
   int         o           =    0;
   int         n           =    0;
   int         m           =    0;
   int         l           =    0;
   int         i           =    0;
   int         x_mid       =    0;
   int         x_conn      =    0;
   int         x_flag      =  '-';
   int         x_line      =    0;
   /*---(find proc)----------------------*/
   PROC_by_seq (&x_proc, g_ymap.gcur);
   if (x_proc == NULL)  x_proc = p_head;
   /*---(prepare)------------------------*/
   x_left  = my.m_left + 123;
   x_mid   = 2 + my.l_every + 2 + (my.l_core / 2);
   x_conn  = my.m_left + my.e_wide + my.e_mult * 2 + 1 + strlen (x_proc->p_print) + strlen (x_proc->t_print) + 3;
   x_line  = x_proc->f_seq - g_ymap.gbeg + 1;
   /*---(every and core libs)------------*/
   LIBS_by_cursor (&x_libs, YDLST_DHEAD);
   while (x_libs != NULL) {
      switch (x_libs->u_flag) {
      case 'e' :
         ++e;
         x_vert  = my.m_bott - my.m_tall + 1 + e;
         yCOLOR_curs ("9_norm");
         mvprintw (x_vert, x_left, x_libs->l_print);
         x_libs->l_seq = x_vert;
         attrset (0);
         yCOLOR_curs ("9_dang");
         mvprintw (x_vert, x_left - 2, "Œ ");
         attrset (0);
         break;
      case 'c' :
         ++c;
         x_vert  = my.m_bott - my.m_tall + my.l_every + 3 + c;
         yCOLOR_curs ("9_norm");
         mvprintw (x_vert, x_left, x_libs->l_print);
         attrset (0);
         yCOLOR_curs ("9_dang");
         mvprintw (x_vert, x_left - 2, "Œ ");
         attrset (0);
         x_libs->l_seq = x_vert;
         break;
      default  :
         x_libs->l_seq = 0;
      }
      attrset (0);
      LIBS_by_cursor (&x_libs, YDLST_DNEXT);
   }
   /*---(major and single libs)----------*/
   x_vert  = my.m_bott - my.m_tall + my.l_every + my.l_core + 4;
   for (i = x_vert; i < g_ymap.uavail; ++i) {
      mvprintw (i, x_left - 3, "%*.*s", 41, 41, S_SPC);
   }
   yCOLOR_curs ("9_norm");
   TIES_by_proc_cursor (&x_ties, x_proc, YDLST_DHEAD);
   while (x_ties != NULL) {
      switch (x_ties->l_link->u_flag) {
      case 'm' : case '-' :
         ++o;
         x_vert  = my.m_bott - my.m_tall + my.l_every + my.l_core + 5 + o;
         mvprintw (x_vert, x_left, x_ties->l_link->l_print);
         x_ties->l_link->l_seq = x_vert;
         break;
      }
      TIES_by_proc_cursor (&x_ties, x_proc, YDLST_DNEXT);
   }
   attrset (0);
   /*---(major and single libs)----------*/
   yCOLOR_curs ("9_dang");
   TIES_by_proc_cursor (&x_ties, x_proc, YDLST_DHEAD);
   while (x_ties != NULL) {
      switch (x_ties->l_link->u_flag) {
      case 'e' :
         ++l;
         if (l == 1) mvprintw (x_ties->l_link->l_seq, x_left - 2, "ƒ€%s", x_ties->l_link->l_print);
         else        mvprintw (x_ties->l_link->l_seq, x_left - 2, "‡€%s", x_ties->l_link->l_print);
         break;
      case 'c' :
         ++m;
         if (x_ties->l_link->l_seq == x_mid)  x_flag = 'y';
         mvprintw (x_ties->l_link->l_seq, x_left - 2, "‡€%s", x_ties->l_link->l_print);
         break;
      case 'm' : case '-' :
         ++n;
         if (n < o)  mvprintw (x_ties->l_link->l_seq, x_left - 2, "‡€%s", x_ties->l_link->l_print);
         else        mvprintw (x_ties->l_link->l_seq, x_left - 2, "„€%s", x_ties->l_link->l_print);
         break;
      }
      TIES_by_proc_cursor (&x_ties, x_proc, YDLST_DNEXT);
   }
   attrset (0);
   /*---(proc connector)-----------------*/
   yCOLOR_curs ("9_dang");
   if (x_flag == 'y')  mvprintw (x_mid, x_left - 2, "Š");
   else                mvprintw (x_mid, x_left - 2, "†");
   if (x_line <  x_mid) {
      mvprintw (x_line, x_conn, "€‚");
      mvprintw (x_mid , x_conn + 1, "„€");
      for (i = x_line + 1; i < x_mid; ++i)  mvprintw (i , x_conn + 1, "");
   } else if (x_line == x_mid) {
      mvprintw (x_line, x_conn, "€€€");
   } else if (x_line >  x_mid) {
      mvprintw (x_line, x_conn, "€…");
      mvprintw (x_mid , x_conn + 1, "ƒ€");
      for (i = x_mid + 1; i < x_line; ++i)  mvprintw (i , x_conn + 1, "");
   }
   attrset (0);
   /*---(titles)-------------------------*/
   yCOLOR_curs ("9_norm");
   x_vert  = my.m_bott - my.m_tall + 1;
   mvprintw (x_vert, x_left - 1, "EVERY (%d of %d) used in every process", l, my.l_every);
   x_vert += my.l_every + 2;
   mvprintw (x_vert, x_left - 1, "CORE (%d of %d) 10+ procs, 2+ execs", m, my.l_core);
   attrset (0);
   if (c > 0) {
      yCOLOR_curs ("9_dang");
      mvprintw (x_vert - 1, x_left - 2, "Œ");
      mvprintw (x_vert    , x_left - 2, "Œ");
      attrset (0);
   }
   yCOLOR_curs ("9_norm");
   x_vert += my.l_core  + 2;
   mvprintw (x_vert, x_left - 1, "OTHER (%d of %d) libraries used", n, l_count - my.l_every - my.l_core);
   attrset (0);
   if (o > 0) {
      yCOLOR_curs ("9_dang");
      mvprintw (x_vert - 1, x_left - 2, "Œ");
      mvprintw (x_vert    , x_left - 2, "Œ");
      attrset (0);
   } else {
      yCOLOR_curs ("9_dang");
      mvprintw (x_vert - 1, x_left - 2, "Œ");
      mvprintw (x_vert    , x_left - 2, "Ï");
      attrset (0);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__main_proc      (char a_lvl, tPROC *a_proc)
{
   char        x_lvl       [LEN_TITLE] = "";
   tPROC      *x_proc      = NULL;
   YVIKEYS__proc_show (a_proc);
   x_proc = a_proc->h_head;
   while (x_proc != NULL) {
      YVIKEYS__main_proc (a_lvl + 1, x_proc);
      x_proc = x_proc->h_sibs;
   }
   return 0;
}

char         /*-> update the column labels -----------[ ------ [gz.D91.061.A5]*/ /*-[02.3000.323.!]-*/ /*-[--.---.---.--]-*/
YVIKEYS_main            (void)
{
   /*---(locals)-----------+-----------+-*/
   tPROC      *x_proc      = NULL;
   tEXEC      *x_exec      = NULL;
   char        x_lvl       [LEN_TITLE] = "";
   int         c           =    0;
   char        x_print     [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(get size)-----------------------*/
   yVIKEYS_view_size     (YVIKEYS_MAIN, &(my.m_left), &(my.m_wide), &(my.m_bott), &(my.m_tall), NULL);
   /*---(cleanse)------------------------*/
   EXEC_by_cursor (&x_exec, YDLST_DHEAD);
   while (x_exec != NULL) {
      x_exec->f_temp  = 0;
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   /*---(dislay procs)-------------------*/
   YVIKEYS__main_proc  (0, p_head);
   YVIKEYS__main_libs  ();
   YVIKEYS__main_exec  ();
   /*> mvprintw (60, 120, "umin  %2d  gmin  %2d  gamin %2d  glmin %2d", g_ymap.umin, g_ymap.gmin, g_ymap.gamin, g_ymap.glmin);   <* 
    *> mvprintw (61, 120, "ubeg  %2d  gbeg  %2d  gprev %2d", g_ymap.ubeg, g_ymap.gbeg, g_ymap.gprev);                            <* 
    *> mvprintw (62, 120, "ucur  %2d  gcur  %2d", g_ymap.ucur, g_ymap.gcur);                                                     <* 
    *> mvprintw (63, 120, "uend  %2d  gend  %2d  gnext %2d", g_ymap.uend, g_ymap.gend, g_ymap.gnext);                            <* 
    *> mvprintw (64, 120, "umax  %2d  gmax  %2d  gamax %2d  glmax %2d", g_ymap.umax, g_ymap.gmax, g_ymap.gamax, g_ymap.glmax);   <* 
    *> mvprintw (65, 120, "avail %2d  utend %2d", g_ymap.uavail, g_ymap.utend);                                                  <* 
    *> mvprintw (66, 120, "bott  %2d  tall  %2d  left  %2d  wide  %2d", my.m_bott, my.m_tall, my.m_left, my.m_wide);             <*/
   move (s_cury, s_curx);
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       drawing overlay                        ----===*/
/*====================------------------------------------====================*/
static void  o___OVERLAY_________o () { return; }

char
YVIKEYS__memory_exec    (tEXEC *a_exec, int a_left, int *a_epss)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =    0;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_LABEL] = "";
   /*---(background)---------------------*/
   yCOLOR_curs ("9_norm");
   mvprintw ( 9, a_left,   "ƒ²²²²²²exec²²²²²²²‚²²²²²²²²²‚");
   for (i = 10; i < 32; ++i) {
      mvprintw (i, a_left, "Œ                           Œ");
   }
   mvprintw (32, a_left,   "„²²²²²²²²²²²²²²²²²…²²²²²²²²²…");
   /*---(data)---------------------------*/
   a_left += 2;  /* 18 characters */
   n       = 10;
   mvprintw (n  , a_left,  "                    per");
   mvprintw (++n, a_left, "%-16.16s"        , a_exec->base);
   ++n;
   mvprintw (++n, a_left, "inode : %.10d"   , a_exec->inode);
   mvprintw (++n, a_left, "hint  : %-2.2s"  , a_exec->hint);
   ++n;
   strl4comma  (a_exec->m_full                   , s, 0, 'c', '-', LEN_LABEL);
   strl4comma  (a_exec->m_full  / a_exec->p_count, t, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "total : %7s Œ %7s", s, t);
   strl4comma  (a_exec->m_text                   , s, 0, 'c', '-', LEN_LABEL);
   strl4comma  (a_exec->m_text  / a_exec->p_count, t, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "text  : %7s Œ %7s", s, t);
   strl4comma  (a_exec->m_cons                   , s, 0, 'c', '-', LEN_LABEL);
   strl4comma  (a_exec->m_cons  / a_exec->p_count, t, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "cons  : %7s Œ %7s", s, t);
   strl4comma  (a_exec->m_heap                   , s, 0, 'c', '-', LEN_LABEL);
   strl4comma  (a_exec->m_heap  / a_exec->p_count, t, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "heap  : %7s Œ %7s", s, t);
   strl4comma  (a_exec->m_kern                   , s, 0, 'c', '-', LEN_LABEL);
   strl4comma  (a_exec->m_kern  / a_exec->p_count, t, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "kern  : %7s Œ %7s", s, t);
   ++n;
   strl4comma  (a_exec->m_code                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "code  : %7s", s);
   strl4comma  (a_exec->m_data                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "data  : %7s", s);
   ++n;
   strl4comma  (a_exec->s_total                  , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "total : %7s", s);
   strl4comma  (a_exec->s_text                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "text  : %7s", s);
   strl4comma  (a_exec->s_data                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "data  : %7s", s);
   strl4comma  (a_exec->s_bss                    , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "bss   : %7s", s);
   ++n;
   mvprintw (++n, a_left, "procs : %d"      , a_exec->p_count);
   if (a_epss != NULL)  *a_epss = a_exec->m_full / a_exec->p_count;
   attrset (0);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__memory_proc    (tPROC *a_proc, int a_left, int a_epss, int a_lpss)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =    0;
   char        s           [LEN_LABEL] = "";
   int         a           =    0;
   /*---(background)---------------------*/
   yCOLOR_curs ("9_norm");
   mvprintw ( 9, a_left,   "ƒ²²²²²²proc²²²²²²²‚");
   for (i = 10; i < 32; ++i) {
      mvprintw (i, a_left, "Œ                 Œ");
   }
   mvprintw (32, a_left,   "„²²²²²²²²²²²²²²²²²…");
   /*---(data)---------------------------*/
   a_left += 2;  /* 18 characters */
   n       = 10;
   mvprintw (++n, a_left, "%-10.10s"        , a_proc->shown);
   ++n;
   mvprintw (++n, a_left, "rpid  : %d"      , a_proc->rpid);
   mvprintw (++n, a_left, "ppid  : %d"      , a_proc->ppid);
   mvprintw (++n, a_left, "hint  : %-2.2s"  , a_proc->hint);
   ++n;
   strl4comma  (a_proc->m_full                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "full  : %7s"    , s);
   ++n;
   strl4comma  (a_proc->m_proc                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "proc  : %7s"    , s);
   strl4comma  (a_proc->m_data                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "data  : %7s"    , s);
   strl4comma  (a_proc->m_heap                   , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "heap  : %7s"    , s);
   strl4comma  (a_proc->m_stack                  , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "stack : %7s"    , s);
   strl4comma  (a_proc->m_other                  , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "other : %7s"    , s);
   ++n;
   mvprintw (++n, a_left, "libs  : %-6d"    , a_proc->t_count);
   ++n;
   a = a_proc->m_proc + a_epss + a_lpss;
   strl4comma  (a                                , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (++n, a_left, "pss   : %7s"    , s);
   attrset (0);
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS__memory_libone  (int n, int l, tTIES *a_ties, tLIBS *a_libs, int *a_lpss)
{
   int         a           =    0;
   int         b           =    0;
   char        s           [LEN_LABEL] = "";
   mvprintw (n, l          , "%-15.15s %3d", a_libs->terse, a_libs->t_count);
   strl4comma  (a_ties->m_data , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 20    , "%8s", s);
   strl4comma  (a_ties->m_heap , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   b = a_ties->m_data + a_ties->m_heap;
   mvprintw (n, l += 9     , "%8s", s);
   strl4comma  (a_libs->m_text , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   strl4comma  (a_libs->m_cons , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   strl4comma  (a_libs->m_priv , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   a = a_libs->m_full / a_libs->t_count;
   strl4comma  (a            , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   strl4comma  (a + b        , s, 0, 'c', '-', LEN_LABEL);
   mvprintw (n, l += 9     , "%8s", s);
   if (a_lpss != NULL)  *a_lpss += a + b;
}

char
YVIKEYS__memory_libs    (tPROC *a_proc, int a_left, int *a_lpss)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         n           =    0;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   char        s           [LEN_LABEL] = "";
   int         l           =    0;
   int         a           =    0;
   int         b           =    0;
   int         c           =    0;
   int         x_line      =    0;
   /*---(background)---------------------*/
   n = a_proc->t_count + (int) (a_proc->t_count / 5);
   if (a_proc->t_count % 5 == 0) --n;
   yCOLOR_curs ("9_norm");
   mvprintw ( 9, a_left,   "ƒ²²²²²libs²²²²²²²²²²²²‚²²²²²²²tie²²²²²²²‚²²²²²²²²²²²²²²²²lib²²²²²²²²²²²²²²²²‚²²²²²²²pss²²²²²²²²‚");
   for (i = 10; i < 10 + n + 2; ++i) {
      mvprintw (i, a_left, "Œ                                                                                              Œ");
   }
   mvprintw (10 + n + 2, a_left,   "„²²²²²²²²²²²²²²²²²²²²²…²²²²²²²²²²²²²²²²²…²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²…²²²²²²²²²²²²²²²²²²…");
   attrset (0);
   /*---(data)---------------------------*/
   a_left += 2;  /* 18 characters */
   n       =  9;
   if (a_lpss != NULL)  *a_lpss = 0;
   yCOLOR_curs ("#_form");
   TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DHEAD);
   while (x_ties != NULL) {
      if (x_ties->l_link->u_flag == 'e') {
         if (x_line % 5 == 0) ++n;
         ++x_line;
         ++n;
         YVIKEYS__memory_libone (n, a_left, x_ties, x_ties->l_link, a_lpss);
      }
      TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DNEXT);
   }
   attrset (0);
   yCOLOR_curs ("p_rang");
   TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DHEAD);
   while (x_ties != NULL) {
      if (x_ties->l_link->u_flag == 'c') {
         if (x_line % 5 == 0) ++n;
         ++x_line;
         ++n;
         YVIKEYS__memory_libone (n, a_left, x_ties, x_ties->l_link, a_lpss);
      }
      TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DNEXT);
   }
   attrset (0);
   yCOLOR_curs ("9_norm");
   TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DHEAD);
   while (x_ties != NULL) {
      if (strchr ("m-", x_ties->l_link->u_flag) != NULL) {
         if (x_line % 5 == 0) ++n;
         ++x_line;
         ++n;
         YVIKEYS__memory_libone (n, a_left, x_ties, x_ties->l_link, a_lpss);
      }
      TIES_by_proc_cursor (&x_ties, a_proc, YDLST_DNEXT);
   }
   attrset (0);
   /*> if (a_lpss != NULL)  *a_lpss  = c;                                             <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS_memory          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tLIBS      *x_libs      = NULL;
   tTIES      *x_ties      = NULL;
   int         x_vert      =    0;
   int         i           =    0;
   int         x_lpss      =    0;
   int         x_epss      =    0;
   /*---(prepare)------------------------*/
   x_proc  = s_cproc;
   x_exec  = x_proc->e_link;
   /*---(draw exec)----------------------*/
   YVIKEYS__memory_exec (x_exec, my.m_left +   7, &x_epss);
   YVIKEYS__memory_libs (x_proc, my.m_left +  55, &x_lpss);
   YVIKEYS__memory_proc (x_proc, my.m_left +  36, x_epss, x_lpss);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

char
YVIKEYS_row_mapping     (tMAPPED *a_map, char a_req)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         x_tall      =    0;
   /*---(height)-------------------------*/
   yVIKEYS_view_bounds (YVIKEYS_MAIN, NULL, NULL, NULL, NULL, NULL, &x_tall);
   a_map->uavail = a_map->utend = x_tall;
   /*---(absolutes)----------------------*/
   if (a_req == YVIKEYS_INIT) {
      a_map->ubeg = a_map->gbeg = 1;
      a_map->ucur = a_map->gcur = 1;
      a_map->uend = a_map->gend = 1;
   }
   /*---(absolutes)----------------------*/
   a_map->umin = a_map->gmin = a_map->gamin = a_map->glmin = 1;
   a_map->umax = a_map->gmax = a_map->gamax = a_map->glmax = p_count;
   /*---(jumps)--------------------------*/
   if (a_map->gcur <= 1      )  a_map->gprev = 1;
   else                         a_map->gprev = a_map->gcur - 1;
   if (a_map->gcur <  p_count)  a_map->gnext = a_map->gcur + 1;
   else                         a_map->gnext = p_count;
   /*---(mapping)------------------------*/
   for (i = 1; i <= p_count; ++i)  a_map->map  [i] = i;
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS_mapper          (char a_req)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL, &g_xmap.uavail, NULL, &g_ymap.uavail, NULL);   <*/
   /*---(remap)----------------*/
   if (a_req == YVIKEYS_INIT) {
      yVIKEYS_clear_mapping (&g_bmap);
      yVIKEYS_clear_mapping (&g_xmap);
      yVIKEYS_clear_mapping (&g_ymap);
      g_ymap.gcur = 1;
      yVIKEYS_clear_mapping (&g_zmap);
   }
   YVIKEYS_row_mapping (&g_ymap, a_req);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> break label into coordinates -------[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
YVIKEYS_locator         (char *a_label, int *b, int *x, int *y, int *z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   /*---(default)------------------------*/
   if (b != NULL)  *b = 0;
   if (x != NULL)  *x = 0;
   if (y != NULL)  *y = 0;
   if (z != NULL)  *z = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_label == NULL)  return rce;
   /*---(find process)-------------------*/
   PROC_by_rpid (&x_proc, atoi (a_label));
   --rce;  if (x_proc  == NULL)  return rce;
   /*---(save back)----------------------*/
   if (y != NULL)  *y = x_proc->f_seq;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> return address for coordinates -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
YVIKEYS_addressor       (char *a_label, int b, int x, int y, int z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tPROC      *x_proc      = NULL;
   /*---(defense)------------------------*/
   --rce;  if (a_label == NULL)  return rce;
   strlcpy (a_label, "", LEN_LABEL);
   /*---(find process)-------------------*/
   PROC_by_seq  (&x_proc, y);
   --rce;  if (x_proc  == NULL)  return rce;
   /*---(save back)----------------------*/
   sprintf (a_label, "%d", x_proc->rpid);
   /*---(complete)-----------------------*/
   return 0;
}
