/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"

#define  S_MID  "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€"
#define  S_SPC  "                                                      "


static int  S_COLOR_HCURR      = COLOR_PAIR(33) | A_BOLD;
static int  S_COLOR_HNORM      = COLOR_PAIR(74);
static int  S_COLOR_HUSED      = COLOR_PAIR(43) | A_BOLD;
static int  S_COLOR_FSIMPLE    = COLOR_PAIR(72)           ;
static int  S_COLOR_FLIKE      = COLOR_PAIR(72)           ;
static int  S_COLOR_NUMBER     = COLOR_PAIR(74) | A_BOLD;
static int  S_COLOR_FDANGER    = COLOR_PAIR(71) | A_BOLD;

static int  S_COLOR_CURRENT    = COLOR_PAIR(43) | A_BLINK;
static int  S_COLOR_VISUAL     = COLOR_PAIR(23) | A_BOLD;
static int  S_COLOR_ROOT       = COLOR_PAIR(33) | A_BOLD;
static int  S_COLOR_MARK       = COLOR_PAIR(33) | A_BOLD;
static int  S_COLOR_SEARCH     = COLOR_PAIR(33) | A_BOLD;

static int  S_COLOR_REQS       = COLOR_PAIR(25) | A_BOLD;
static int  S_COLOR_PROS       = COLOR_PAIR(22) | A_BOLD;
static int  S_COLOR_LIKE       = COLOR_PAIR(24) | A_BOLD;

static int  S_COLOR_ERROR      = COLOR_PAIR(61) | A_BOLD;
static int  S_COLOR_WARN       = COLOR_PAIR(61) | A_BOLD;
static int  S_COLOR_FOUND      = COLOR_PAIR(61) | A_BOLD;



/*====================------------------------------------====================*/
/*===----                       program wide                           ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
YVIKEYS_init            (void)
{
   char        rc          =    0;
   /*---(row and column headers)---------*/
   S_COLOR_HCURR      = yCOLOR_curs_value ("h_current");
   S_COLOR_HNORM      = yCOLOR_curs_value ("h_normal" );
   S_COLOR_HUSED      = yCOLOR_curs_value ("h_used"   );
   S_COLOR_FSIMPLE    = yCOLOR_curs_add   ("for"      , 'f', "numeric formula"                                    , 'g' , ' ' , 'y');
   S_COLOR_FLIKE      = yCOLOR_curs_add   ("flike"    , 'l', "numeric formula (copy)"                             , 'w' , ' ' , '-');
   S_COLOR_NUMBER     = yCOLOR_curs_add   ("num"      , 'n', "numeric literal"                                    , 'b' , ' ' , 'y');
   S_COLOR_FDANGER    = yCOLOR_curs_add   ("fdang"    , 'f', "complex numeric formula"                            , 'r' , ' ' , 'y');
   /*---(selection)----------------------*/
   S_COLOR_CURRENT    = yCOLOR_curs_value ("curr"     );
   S_COLOR_ROOT       = yCOLOR_curs_value ("root"     );
   S_COLOR_VISUAL     = yCOLOR_curs_value ("visu"     );
   S_COLOR_MARK       = yCOLOR_curs_value ("mark"     );
   S_COLOR_SEARCH     = yCOLOR_curs_value ("srch"     );
   /*---(dep type)-------*/
   S_COLOR_REQS       = yCOLOR_curs_add   ("reqs"     , ' ', "value required from cell"                           , 'm' , 'm' , 'b');
   S_COLOR_PROS       = yCOLOR_curs_add   ("pros"     , ' ', "value provided to cell"                             , 'g' , 'g' , 'b');
   S_COLOR_LIKE       = yCOLOR_curs_add   ("like"     , ' ', "formula is copy/variation"                          , 'g' , ' ' , '-');
   /*---(trouble)--------*/
   S_COLOR_ERROR      = yCOLOR_curs_value ("error"    );
   S_COLOR_WARN       = yCOLOR_curs_value ("warn"     );
   S_COLOR_FOUND      = yCOLOR_curs_value ("found"    );
   use_legacy_coding (2);
   clear ();
   strlcpy (my.hint, "--", LEN_TERSE);
   rc = yVIKEYS_hint_config (YVIKEYS_hinter);
   rc = yVIKEYS_srch_config (YVIKEYS_searcher , YVIKEYS_unsearcher);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      processing hints                        ----===*/
/*====================------------------------------------====================*/
static void  o___HINTS___________o () { return; }

char
YVIKEYS__hint_clear     (void)
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
   yVIKEYS_jump (0, 1, x_proc->f_seq, 0);
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
   EXEC_by_hint (&x_exec, a_hint);
   --rce;  if (x_exec == NULL)  return rce;
   PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DHEAD);
   PROC_by_seq_cursor  (&x_proc, 's');
   while (x_proc != NULL)  {
      SET_HINT (x_proc->p_note);
      PROC_by_exec_cursor (&x_proc, x_exec, YDLST_DNEXT);
   }
   yVIKEYS_jump (0, 1, x_exec->f_seq, 0);
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
   SET_HINT (x_libs->l_note);
   TIES_by_libs_cursor (&x_ties, x_libs, YDLST_DHEAD);
   while (x_ties != NULL)  {
      if (x_proc == NULL)  x_proc = x_ties->p_link;
      if (x_ties->p_link->f_seq < x_proc->f_seq)  x_proc = x_ties->p_link;
      SET_HINT (x_ties->p_link->p_note);
      SET_HINT (x_ties->p_link->e_link->e_note);
      TIES_by_libs_cursor (&x_ties, x_libs, YDLST_DNEXT);
   }
   yVIKEYS_jump (0, 1, x_proc->f_seq, 0);
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
   --rce;  if (a_hint == NULL)                  return rce;
   l = strlen (a_hint);
   --rce;  if (l < 1)                           return rce;
   --rce;  if (a_hint [0] != ';')               return rce;
   /*---(clearing)-----------------------*/
   if (strcmp (a_hint, ";") == 0) {
      YVIKEYS__hint_clear ();
      PROC_by_seq_cursor (&x_proc, 's');
      return 0;
   }
   /*---(cursoring)----------------------*/
   switch (a_hint [1]) {
   case '[' : case '<' : case '.' : case '>' : case ']' :
      rc = PROC_by_seq_cursor (&x_proc, a_hint [1]);
      if (x_proc != NULL) {
         yVIKEYS_jump (0, 1, x_proc->f_seq, 0);
      }
      /*> YVIKEYS__hint_cursor (a_hint [1]);                                          <*/
      return 0;
      break;
   case '+' : case '-' :
      YVIKEYS_update ();
      return 0;
      break;
   }
   /*---(real hints)---------------------*/
   YVIKEYS__hint_clear ();
   PROC_by_seq_cursor (&x_proc, 'r');
   --rce;  if (strncmp (a_hint, ";;", 2) != 0)  return rce;
   strlcpy (my.hint, a_hint + 2, LEN_TERSE);
   rc = YVIKEYS__hint_proc (my.hint);
   if (rc < 0)  rc = YVIKEYS__hint_exec (my.hint);
   if (rc < 0)  rc = YVIKEYS__hint_libs (my.hint);
   return 0;
}

char
YVIKEYS_searcher        (char *a_search)
{
   /*---(locals)-----------+------+----+-*/
   char        rce         =   -10;
   char        rc          =     0;
   tPROC      *x_proc      = NULL;
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
         DEBUG_SRCH   yLOG_info    ("->cmdline"      , x_proc->cmdline);
         rc = yREGEX_exec (x_proc->cmdline);
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
   attron  (S_COLOR_HCURR);
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
   /*---(prepare)------------------------*/
   x_pline = a_proc->f_seq - g_ymap.gbeg + 1;
   if (x_pline < 1)  return 0;
   if (x_pline > g_ymap.gend)  return 0;
   l = my.e_wide + my.e_mult * 2 + 1;
   x_conn = my.m_left + l + strlen (a_proc->p_print) + strlen (a_proc->t_print) + 3;
   x_mid  = 2 + my.l_every + 2 + (my.l_core / 2);
   /*---(exec primary)-------------------*/
   x_exec  = a_proc->e_link;
   x_eline = x_exec->f_seq - g_ymap.gbeg + 1;
   if (x_eline > 0) {
      x_exec->f_temp = x_exec->f_seq;
      attron  (S_COLOR_FSIMPLE);
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
         attron  (S_COLOR_FSIMPLE);
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
         attron  (S_COLOR_FSIMPLE);
         mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left, "%s%sÏ", a_proc->e_print, a_proc->m_print);
         attrset (0);
      }
   }
   /*---(proc)---------------------------*/
   if (a_proc->f_seq == g_ymap.gcur)   attron  (S_COLOR_HCURR);
   else if (HAS_HINT (a_proc->p_note)) attron  (S_COLOR_MARK);
   else if (HAS_SRCH (a_proc->p_note)) attron  (S_COLOR_SEARCH);
   else if (HAS_MARK (a_proc->p_note)) attron  (S_COLOR_FOUND);
   else                                attron  (S_COLOR_HNORM);
   mvprintw (my.m_bott - my.m_tall + x_pline, my.m_left + l, " %s·%sÏ", a_proc->p_print, a_proc->t_print);
   attrset (0);
   mvprintw (my.m_bott - my.m_tall + x_pline, x_conn, "   ", a_proc->p_print, a_proc->t_print);
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
   attron  (S_COLOR_FDANGER);
   mvprintw (my.m_bott - my.m_tall + x_line, my.m_left, "%s", x_exec->e_print);
   attrset (0);
   /*---(connector)----------------------*/
   x_link  = x_proc->f_seq - g_ymap.gbeg + 1;
   attron  (S_COLOR_FDANGER);
   mvprintw (66, 120, "proc  %2d  exec  %2d", x_proc->f_seq, x_exec->f_seq);
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
   mvprintw (67, 120, "turn  %2d  %s", x_turn, x_proc->m_print);
   if (x_exec->f_temp != x_proc->f_seq) {
      for (i = x_exec->f_temp; i <= x_proc->f_seq; ++i) {
         PROC_by_seq (&x_other, i);
         switch (x_other->m_print [x_turn]) {
         case '‡' : case '„' : case 'Œ' : case '' :
         case 'Ï' : case '€' : case 'ƒ' : case '‰' :
            mvprintw (i - g_ymap.gbeg + 1, my.e_wide + x_turn, "%c", x_other->m_print [x_turn]);
            break;
         }
         mvprintw (68, 120, "line  %2d  link  %2d  turn  %2d  i     %2d", x_line, x_link, x_turn, i);
      }
      PROC_by_seq (&x_other, x_line + g_ymap.gbeg - 1);
      mvprintw (69, 120, "temp  %2d  other %2d  %s", x_exec->f_temp, x_other->f_seq, x_other->m_print);
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
         attron  (S_COLOR_NUMBER);
         mvprintw (x_vert, x_left, x_libs->l_print);
         x_libs->l_seq = x_vert;
         attrset (0);
         attron  (S_COLOR_FDANGER);
         mvprintw (x_vert, x_left - 2, "Œ ");
         attrset (0);
         break;
      case 'c' :
         ++c;
         x_vert  = my.m_bott - my.m_tall + my.l_every + 3 + c;
         attron  (S_COLOR_NUMBER);
         mvprintw (x_vert, x_left, x_libs->l_print);
         attrset (0);
         attron  (S_COLOR_FDANGER);
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
   attron  (S_COLOR_NUMBER);
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
   attron  (S_COLOR_FDANGER);
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
   attron  (S_COLOR_FDANGER);
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
   attron  (S_COLOR_NUMBER);
   x_vert  = my.m_bott - my.m_tall + 1;
   mvprintw (x_vert, x_left - 1, "EVERY (%d of %d) used in every process", l, my.l_every);
   x_vert += my.l_every + 2;
   mvprintw (x_vert, x_left - 1, "CORE (%d of %d) 10+ procs, 2+ execs", m, my.l_core);
   attrset (0);
   if (c > 0) {
      attron  (S_COLOR_FDANGER);
      mvprintw (x_vert - 1, x_left - 2, "Œ");
      mvprintw (x_vert    , x_left - 2, "Œ");
      attrset (0);
   }
   attron  (S_COLOR_NUMBER);
   x_vert += my.l_core  + 2;
   mvprintw (x_vert, x_left - 1, "OTHER (%d of %d) libraries used", n, l_count - my.l_every - my.l_core);
   attrset (0);
   if (o > 0) {
      attron  (S_COLOR_FDANGER);
      mvprintw (x_vert - 1, x_left - 2, "Œ");
      mvprintw (x_vert    , x_left - 2, "Œ");
      attrset (0);
   } else {
      attron  (S_COLOR_FDANGER);
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
   mvprintw (60, 120, "umin  %2d  gmin  %2d  gamin %2d  glmin %2d", g_ymap.umin, g_ymap.gmin, g_ymap.gamin, g_ymap.glmin);
   mvprintw (61, 120, "ubeg  %2d  gbeg  %2d  gprev %2d", g_ymap.ubeg, g_ymap.gbeg, g_ymap.gprev);
   mvprintw (62, 120, "ucur  %2d  gcur  %2d", g_ymap.ucur, g_ymap.gcur);
   mvprintw (63, 120, "uend  %2d  gend  %2d  gnext %2d", g_ymap.uend, g_ymap.gend, g_ymap.gnext);
   mvprintw (64, 120, "umax  %2d  gmax  %2d  gamax %2d  glmax %2d", g_ymap.umax, g_ymap.gmax, g_ymap.gamax, g_ymap.glmax);
   mvprintw (65, 120, "uava  %2d  utend %2d", g_ymap.uavail, g_ymap.utend);
   mvprintw (70, 120, "hint  %2s", my.hint);
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
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
