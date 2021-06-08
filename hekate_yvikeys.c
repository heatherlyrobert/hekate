/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"

#define  S_MID  ""
#define  S_SPC  "                                                      "


static int  S_COLOR_HCURR      = COLOR_PAIR(33) | A_BOLD;
static int  S_COLOR_HNORM      = COLOR_PAIR(74);
static int  S_COLOR_HUSED      = COLOR_PAIR(43) | A_BOLD;
static int  S_COLOR_FSIMPLE    = COLOR_PAIR(72)           ;
static int  S_COLOR_FLIKE      = COLOR_PAIR(72)           ;
static int  S_COLOR_NUMBER     = COLOR_PAIR(74) | A_BOLD;
static int  S_COLOR_FDANGER    = COLOR_PAIR(71) | A_BOLD;
static int  S_COLOR_VISUAL     = COLOR_PAIR(23) | A_BOLD;



/*====================------------------------------------====================*/
/*===----                       program wide                           ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
YVIKEYS_init            (void)
{
   /*---(row and column headers)---------*/
   S_COLOR_HCURR      = yCOLOR_curs_value ("h_current");
   S_COLOR_HNORM      = yCOLOR_curs_value ("h_normal" );
   S_COLOR_HUSED      = yCOLOR_curs_value ("h_used"   );
   S_COLOR_FSIMPLE    = yCOLOR_curs_add   ("for"      , 'f', "numeric formula"                                    , 'g' , ' ' , 'y');
   S_COLOR_FLIKE      = yCOLOR_curs_add   ("flike"    , 'l', "numeric formula (copy)"                             , 'w' , ' ' , '-');
   S_COLOR_NUMBER     = yCOLOR_curs_add   ("num"      , 'n', "numeric literal"                                    , 'b' , ' ' , 'y');
   S_COLOR_FDANGER    = yCOLOR_curs_add   ("fdang"    , 'f', "complex numeric formula"                            , 'r' , ' ' , 'y');
   S_COLOR_VISUAL     = yCOLOR_curs_value ("visu"     );
   use_legacy_coding (2);
   clear ();
   /*---(complete)-----------------------*/
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
      x_proc->h_next  = NULL;
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
YVIKEYS__treeify        (char a_level, tPROC *a_parent)
{
   tPROC      *x_proc      = NULL;
   int         c           = 0;
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_value   ("a_parent"  , a_parent->rpid);
   a_parent->f_seq = ++(my.f_seq);
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
            a_parent->h_tail->h_next = x_proc;
            a_parent->h_tail         = x_proc;
            ++(a_parent->h_count);
         }
         x_proc->h_next = NULL;
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
      snprintf  (x_pre, my.e_len + 3, "%s Ο%s", x_exec->base, S_MID);
      strllower (x_pre, LEN_TITLE);
      /*---(count)-----------------------*/
      if (x_exec->p_count > 1)  snprintf (x_cnt, LEN_TERSE, "-%d%s", x_exec->p_count, S_MID);
      else                      snprintf (x_cnt, LEN_TERSE, "%-4.4s", S_MID);
      /*---(multi)-----------------------*/
      snprintf  (x_mprint, my.e_mult * 2 + 1, "%s", S_MID);
      /*---(output)----------------------*/
      sprintf   (x_eprint, "%-2.2s  %s%-4.4s", x_exec->hint, x_pre, x_cnt);
      /*---(update)----------------------*/
      x_exec->f_seq = x_exec->f_temp = a_proc->f_seq;
      strlcpy (x_exec->e_print, x_eprint, LEN_HUND);
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
         if (x_exec->e_seq == 1)  x_mprint [(i * 2) - 2] = '';
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
      x_mprint [x_exec->e_col * 2 - 2] = '';
   } else if (x_exec->e_col > 0 && a_proc->e_seq == x_exec->p_count) {
      x_mprint [x_exec->e_col * 2 - 2] = '';
      my.e_flag [x_exec->e_col] = 0;
   } else if (x_exec->e_col > 0) {
      x_mprint [x_exec->e_col * 2 - 2] = '';
   }
   /*---(multi-use fill-in)-----------*/
   if (x_exec->e_col > 0) {
      for (i = x_exec->e_col; i <= my.e_mult; ++i) {
         x_mprint [i * 2 - 1] = '';
         if (i > x_exec->e_col) {
            if (my.e_flag [i] > 0)  x_mprint [i * 2 - 2] = '';
            else                    x_mprint [i * 2 - 2] = '';
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
      if (a_proc->h_next == NULL) {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ο';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         my.p_flag [a_proc->p_lvl - 1] = 0;
      } else {
         x_lvl [(a_proc->p_lvl - 1) * 2]     = 'Ο';
         x_lvl [(a_proc->p_lvl - 1) * 2 + 1] = ' ';
         my.p_flag [a_proc->p_lvl - 1] = 1;
      }
   }
   /*---(proc-tree fills)----------------*/
   for (i = 0; i < a_proc->p_lvl; ++i) {
      if (my.p_flag [i] > 0 && x_lvl [i * 2] != 'Ο')  x_lvl [i * 2] = '';
   }
   /*---(final)--------------------------*/
   snprintf (x_pprint, 75, "%-5d %-2.2s %s%s······································································", a_proc->rpid, a_proc->hint, x_lvl, a_proc->cmdline);
   snprintf (x_tprint,  6, "%d-", a_proc->t_count);
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
         snprintf (x_cnt , 6, "Ο%d", x_libs->t_count);
         snprintf (x_cnt2, 5, "%d", x_libs->e_count);
         break;
      case '-' : 
         snprintf (x_cnt , 6, "Ο%d", x_libs->t_count);
         snprintf (x_cnt2, 5, "");
         break;
      }
      snprintf (x_lprint, 47, "%s%s %-24.24s  %s", x_cnt, x_cnt2, x_libs->terse, x_libs->hint);
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
      x_proc = x_proc->h_next;
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
   int         x_line      =    0;
   int         l           =    0;
   int         x_conn      =    0;
   int         x_mid       =    0;
   /*---(prepare)------------------------*/
   x_line = a_proc->f_seq - g_ymap.gbeg + 1;
   if (x_line < 1)  return 0;
   if (x_line > g_ymap.gend)  return 0;
   l = my.e_wide + my.e_mult * 2 + 1;
   x_conn = my.m_left + l + strlen (a_proc->p_print) + strlen (a_proc->t_print) + 3;
   x_mid  = 2 + my.l_every + 2 + (my.l_core / 2);
   /*---(exec)---------------------------*/
   attron  (S_COLOR_FSIMPLE);
   mvprintw (my.m_bott - my.m_tall + x_line, my.m_left, "%s%sΟ", a_proc->e_print, a_proc->m_print);
   attrset (0);
   /*---(proc)---------------------------*/
   if (a_proc->f_seq == g_ymap.gcur)   attron  (S_COLOR_HCURR);
   else                                attron  (S_COLOR_HNORM);
   mvprintw (my.m_bott - my.m_tall + x_line, my.m_left + l, " %s·%sΟ", a_proc->p_print, a_proc->t_print);
   attrset (0);
   mvprintw (my.m_bott - my.m_tall + x_line, x_conn, "   ", a_proc->p_print, a_proc->t_print);
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
   x_line = x_exec->f_seq - g_ymap.gbeg + 1;
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
   /*> mvprintw (66, 120, "proc  %2d  exec  %2d", x_proc->f_seq, x_exec->f_seq);      <*/
   mvprintw (x_link, l - 1, "Ο");
   x_len = strlen (x_proc->m_print);
   for (i = 0; i < x_len; ++i) {
      switch (x_proc->m_print [i]) {
      case '' : case '' :
         x_turn = i;
      case 'Ο' : case '' : case '' : case '' :
         mvprintw (x_link, my.e_wide + i, "%c", x_proc->m_print [i]);
         break;
      }
   }
   /*> mvprintw (67, 120, "turn  %2d  %s", x_turn, x_proc->m_print);                  <*/
   if (x_exec->f_seq != x_proc->f_seq) {
      /*> for (i = x_line + 1; i < x_link; ++i) {                                     <*/
      for (i = x_exec->f_seq; i <= x_proc->f_seq; ++i) {
         PROC_by_seq (&x_other, i);
         switch (x_other->m_print [x_turn]) {
         case '' : case '' : case '' : case '' :
         case 'Ο' : case '' : case '' : case '' :
            mvprintw (i - g_ymap.gbeg + 1, my.e_wide + x_turn, "%c", x_other->m_print [x_turn]);
            break;
         }
         /*> mvprintw (68, 120, "line  %2d  link  %2d  turn  %2d  i     %2d", x_line, x_link, x_turn, i);   <*/
      }
      for (i = 0; i <= x_turn; ++i) {
         PROC_by_seq (&x_other, x_line + g_ymap.gbeg - 1);
         switch (x_other->m_print [i]) {
         case '' : case '' :
         case 'Ο' : case '' : case '' : case '' :
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
         mvprintw (x_vert, x_left - 2, " ");
         attrset (0);
         break;
      case 'c' :
         ++c;
         x_vert  = my.m_bott - my.m_tall + my.l_every + 3 + c;
         attron  (S_COLOR_NUMBER);
         mvprintw (x_vert, x_left, x_libs->l_print);
         attrset (0);
         attron  (S_COLOR_FDANGER);
         mvprintw (x_vert, x_left - 2, " ");
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
         if (l == 1) mvprintw (x_ties->l_link->l_seq, x_left - 2, "%s", x_ties->l_link->l_print);
         else        mvprintw (x_ties->l_link->l_seq, x_left - 2, "%s", x_ties->l_link->l_print);
         break;
      case 'c' :
         ++m;
         if (x_ties->l_link->l_seq == x_mid)  x_flag = 'y';
         mvprintw (x_ties->l_link->l_seq, x_left - 2, "%s", x_ties->l_link->l_print);
         break;
      case 'm' : case '-' :
         ++n;
         if (n < o)  mvprintw (x_ties->l_link->l_seq, x_left - 2, "%s", x_ties->l_link->l_print);
         else        mvprintw (x_ties->l_link->l_seq, x_left - 2, "%s", x_ties->l_link->l_print);
         break;
      }
      TIES_by_proc_cursor (&x_ties, x_proc, YDLST_DNEXT);
   }
   attrset (0);
   /*---(proc connector)-----------------*/
   attron  (S_COLOR_FDANGER);
   if (x_flag == 'y')  mvprintw (x_mid, x_left - 2, "");
   else                mvprintw (x_mid, x_left - 2, "");
   if (x_line <  x_mid) {
      mvprintw (x_line, x_conn, "");
      mvprintw (x_mid , x_conn + 1, "");
      for (i = x_line + 1; i < x_mid; ++i)  mvprintw (i , x_conn + 1, "");
   } else if (x_line == x_mid) {
      mvprintw (x_line, x_conn, "");
   } else if (x_line >  x_mid) {
      mvprintw (x_line, x_conn, "");
      mvprintw (x_mid , x_conn + 1, "");
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
      mvprintw (x_vert - 1, x_left - 2, "");
      mvprintw (x_vert    , x_left - 2, "");
      attrset (0);
   }
   attron  (S_COLOR_NUMBER);
   x_vert += my.l_core  + 2;
   mvprintw (x_vert, x_left - 1, "OTHER (%d of %d) libraries used", n, l_count - my.l_every - my.l_core);
   attrset (0);
   if (o > 0) {
      attron  (S_COLOR_FDANGER);
      mvprintw (x_vert - 1, x_left - 2, "");
      mvprintw (x_vert    , x_left - 2, "");
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
      x_proc = x_proc->h_next;
   }
   return 0;
}

char         /*-> update the column labels -----------[ ------ [gz.D91.061.A5]*/ /*-[02.3000.323.!]-*/ /*-[--.---.---.--]-*/
YVIKEYS_main            (void)
{
   /*---(locals)-----------+-----------+-*/
   tPROC      *x_proc      = NULL;
   char        x_lvl       [LEN_TITLE] = "";
   int         c           =    0;
   char        x_print     [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_GRAF  yLOG_enter   (__FUNCTION__);
   /*---(get size)-----------------------*/
   yVIKEYS_view_size     (YVIKEYS_MAIN, &(my.m_left), &(my.m_wide), &(my.m_bott), &(my.m_tall), NULL);
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
