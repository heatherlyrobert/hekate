/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"

#define  S_MID  "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€"
#define  S_SPC  "                                                      "


static int  S_COLOR_HCURR      = COLOR_PAIR(33) | A_BOLD;
static int  S_COLOR_HNORM      = COLOR_PAIR(74);



char
YVIKEYS_init            (void)
{
   /*---(row and column headers)---------*/
   S_COLOR_HCURR      = yCOLOR_curs_value ("h_current");
   S_COLOR_HNORM      = yCOLOR_curs_value ("h_normal" );
   use_legacy_coding (2);
   clear ();
   /*---(complete)-----------------------*/
   return 0;
}

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
   snprintf (x_libs, 50, "[·· %d libs·············································································", l_count);
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

char
YVIKEYS__main_prepare   (void)
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
      if (l > my.e_len)  my.e_len = l;
      x_exec->e_col   = 0;
      x_exec->e_seq   = 0;
      x_exec->e_shown = '-';
      if (x_exec->p_count > 1)  ++my.e_mult;
      n += x_exec->p_count;
      EXEC_by_cursor (&x_exec, YDLST_DNEXT);
   }
   my.e_wide = 4 + my.e_len + 2 + 4;
   /*---(reset proc)---------------------*/
   PROC_by_cursor (&x_proc, YDLST_DHEAD);
   while (x_proc != NULL) { 
      x_proc->e_seq = 0;
      PROC_by_cursor (&x_proc, YDLST_DNEXT);
   }
   /*---(reset libs)---------------------*/
   my.l_index   = 0;;
   my.l_show    = 0;
   my.l_every   = 0;
   my.l_core    = 0;
   my.l_multi   = 0;
   my.l_singles = 0;
   LIBS_by_index (&x_libs, my.l_index++);
   while (x_libs != NULL) {
      YVIKEYS__lib_count (x_libs);
      LIBS_by_index (&x_libs, my.l_index++);
   }
   my.l_index = 0;
   /*---(reset flags)--------------------*/
   my.p_index = 0;
   for (i = 0; i < 20; ++i) {
      my.e_flag [i] = 0;
      my.p_flag [i] = 0;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
YVIKEYS_update          (void)
{
   YVIKEYS__main_prepare ();
   return 0;
}

char
YVIKEYS__proc_show      (tPROC *a_proc)
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
   char        x_print     [LEN_RECD]  = "";
   int         i           = 0;
   static int  x_line      = 0;
   /*---(prepare)------------------------*/
   if (a_proc == p_head)  x_line = 0;
   else                   ++x_line;
   x_exec = a_proc->e_link;
   /*---(executable)---------------------*/
   if (x_exec->e_seq == 0) {
      /*---(prefix)----------------------*/
      snprintf (x_pre, my.e_len + 3, "%s Ï%s", x_exec->base, S_MID);
      strllower (x_pre, LEN_TITLE);
      /*---(count)-----------------------*/
      if (x_exec->p_count > 1)  snprintf (x_cnt, LEN_TERSE, "-%d%s", x_exec->p_count, S_MID);
      else                      snprintf (x_cnt, LEN_TERSE, "%-4.4s", S_MID);
      /*---(multi)-----------------------*/
      snprintf (x_mul, my.e_mult * 2 + 1, "%s", S_MID);
      /*---(output)----------------------*/
      sprintf (x_eprint, "%-2.2s  %s%-4.4s", x_exec->hint, x_pre, x_cnt);
   } else {
      snprintf (x_mul, my.e_mult * 2 + 1, "%s", S_SPC);
      sprintf (x_eprint, "%*.*s", my.e_wide, my.e_wide, "                          ");
   }
   ++(x_exec->e_seq);
   /*---(multi-use betweens)----------*/
   for (i = 1; i <= my.e_mult; ++i) {
      if (my.e_flag [i] > 0) {
         if (x_exec->e_seq == 1)  x_mul [(i * 2) - 2] = 'Œ';
         else                     x_mul [(i * 2) - 2] = '';
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
      x_mul [x_exec->e_col * 2 - 2] = '‰';
   } else if (x_exec->e_col > 0 && a_proc->e_seq == x_exec->p_count) {
      x_mul [x_exec->e_col * 2 - 2] = '„';
      my.e_flag [x_exec->e_col] = 0;
   } else if (x_exec->e_col > 0) {
      x_mul [x_exec->e_col * 2 - 2] = '‡';
   }
   /*---(multi-use fill-in)-----------*/
   if (x_exec->e_col > 0) {
      for (i = x_exec->e_col; i <= my.e_mult; ++i) {
         x_mul [i * 2 - 1] = '€';
         if (i > x_exec->e_col) {
            if (my.e_flag [i] > 0)  x_mul [i * 2 - 2] = 'Œ';
            else                 x_mul [i * 2 - 2] = '€';
         }
      }
   }
   /*---(show exec)-------------------*/
   /*> printf ("%sÏ %2d %2d %sÏ\n", x_eprint, x_exec->e_col, x_exec->e_seq, x_mul);   <*/
   /*> printf ("%s%sÏ ", x_eprint, x_mul);                                            <*/
   /*---(proc-tree defaults)-------------*/
   strcpy (x_lvl, "");
   for (i = 0; i < a_proc->p_lvl; ++i)   strlcat (x_lvl, "··", LEN_TITLE);
   for (i = a_proc->p_lvl; i < 20; ++i)  my.p_flag [i] = 0;
   /*---(proc-tree specifics)------------*/
   if (a_proc->p_lvl > 0) {
      if (a_proc->h_next == NULL) {
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
   /*---(show proc)----------------------*/
   /*> printf ("%-2.2s  %-5d %2d %2d %s%-40.40s\n", a_proc->hint, a_proc->rpid, a_proc->p_lvl, a_proc->p_seq, x_lvl, a_proc->cmdline);   <*/
   snprintf (x_pprint, 75, "%-5d %-2.2s %s%s······································································", a_proc->rpid, a_proc->hint, x_lvl, a_proc->cmdline);
   snprintf (x_tprint,  6, "€€%d€€-", a_proc->t_count);
   /*---(show libs)----------------------*/
   /*> NCURSE_proc_showlib     (x_line, x_lprint);                                    <*/
   /*> LIBS_by_index (&x_libs, my.l_index++);                                                      <* 
    *> while (x_libs != NULL && (x_libs->t_count <= 9 || x_libs->e_count <= 1)) {               <* 
    *>    LIBS_by_index (&x_libs, my.l_index++);                                                   <* 
    *> }                                                                                        <* 
    *> if (x_libs != NULL) {                                                                    <* 
    *>    snprintf (x_cnt , 6, "Ï€%d€€", x_libs->t_count);                                      <* 
    *>    snprintf (x_cnt2, 5, "€%d€€", x_libs->e_count);                                       <* 
    *>    snprintf (x_lprint, 47, "%s%s%c %s", x_cnt, x_cnt2, x_libs->u_flag, x_libs->terse);   <* 
    *> } else {                                                                                 <* 
    *>    snprintf (x_lprint, 47, "%s", S_SPC);                                                 <* 
    *> }                                                                                        <*/
   /*> printf ("%s·%sÏ     %s\n", x_pprint, x_tprint, x_lprint);                      <*/

   snprintf (x_print, my.m_wide, "%s%sÏ %s·%sÏ     %s", x_eprint, x_mul, x_pprint, x_tprint, x_lprint);
   attron  (S_COLOR_HNORM);
   mvprintw (my.m_bott - my.m_tall + x_line + 1, my.m_left, "%s", x_print);
   attrset (0);
   /*---(done)---------------------------*/
   return 0;
}

char
YVIKEYS__lib_count      (tLIBS *a_libs)
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
   } else if (a_libs->t_count > 10 && a_libs->e_count > 1) {
      ++my.l_core;
      a_libs->u_flag = 'c';
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
   mvprintw (10, 10, "hello world");
   YVIKEYS__main_proc (0, p_head);
   /*> x_proc = p_head;                                                                                                                                                  <* 
    *> while (x_proc != NULL) {                                                                                                                                          <* 
    *>    ++c;                                                                                                                                                           <* 
    *>    snprintf (x_print, 75, "%-5d %-2.2s %s······································································", x_proc->rpid, x_proc->hint, x_proc->cmdline);   <* 
    *>    mvprintw (x_bott - x_tall + c, x_left, "%s", x_print);                                                                                                         <* 
    *>    x_proc = x_proc->m_next;                                                                                                                                       <* 
    *>    if (c > 20)  break;                                                                                                                                            <* 
    *> }                                                                                                                                                                 <*/
   /*---(complete)-----------------------*/
   DEBUG_GRAF  yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

char
YVIKEYS_row_mapping     (tMAPPED *a_map)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(absolutes)----------------------*/
   a_map->umin = a_map->gmin = a_map->gamin = a_map->glmin = 1;
   a_map->umax = a_map->gmax = a_map->gamax = a_map->glmax = p_count;
   /*---(jumps)--------------------------*/
   if (a_map->gcur <= 1      )  a_map->gprev = 1;
   else                         a_map->gprev = a_map->gcur - 1;
   if (a_map->gcur <  p_count)  a_map->gnext = a_map->gcur + 1;
   else                         a_map->gnext = p_count;
   /*---(mapping)------------------------*/
   for (i = 0; i < p_count; ++i)  a_map->map  [i] = i;
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
      yVIKEYS_clear_mapping (&g_zmap);
   }
   YVIKEYS_row_mapping (&g_ymap);



   /*> TAB_map_update   (a_req);                                                      <*/
   /*> COL_mapper       (a_req);                                                      <*/
   /*> ROW_mapper       (a_req);                                                      <*/
   /*> if (a_req == YVIKEYS_INIT) {                                                   <* 
    *>    LOC__mapper   ('T');                                                        <* 
    *>    LOC__mapper   ('C');                                                        <* 
    *>    LOC__mapper   ('R');                                                        <* 
    *> } else {                                                                       <* 
    *>    LOC__mapper   ('t');                                                        <* 
    *>    LOC__mapper   ('c');                                                        <* 
    *>    LOC__mapper   ('r');                                                        <* 
    *> }                                                                              <*/
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}
