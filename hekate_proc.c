/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tPROC      *p_head      = NULL;
tPROC      *p_tail      = NULL;
tPROC      *p_curr      = NULL;
int         p_count     = 0;



static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*
PROC__memory            (tPROC *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "е__.______.___________.___.___._.__.___ж", LEN_RECD);
   ++n;  if (a_cur->rpid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->ppid        >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->c_state     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->c_utime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_stime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_snice     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_max       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_base      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_min       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_heap      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_stack     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_kern      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_libs      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_other     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->d_read      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->d_write     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->d_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->n_read      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->n_write     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->n_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->e_link      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->t_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_count     >  0)           s_print [n] = 'X';
   return s_print;
}

char
PROC_wipe               (tPROC *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(master)-------------------------*/
   if (a_type == '*') {
      a_new->rpid     = 0;
      a_new->ppid     = 0;
   }
   /*---(cpu)----------------------------*/
   a_new->c_state  = '-';
   a_new->c_utime  = 0;
   a_new->c_stime  = 0;
   a_new->c_snice  = 0;
   a_new->c_flag   = '-';
   /*---(memory--------------------------*/
   a_new->m_max    = 0;
   a_new->m_base   = 0;
   a_new->m_min    = 0;
   a_new->m_text   = 0;
   a_new->m_data   = 0;
   a_new->m_heap   = 0;
   a_new->m_stack  = 0;
   a_new->m_kern   = 0;
   a_new->m_libs   = 0;
   a_new->m_other  = 0;
   a_new->m_flag   = '-';
   /*---(disk)---------------------------*/
   a_new->d_read   = 0;
   a_new->d_write  = 0;
   a_new->d_flag   = '-';
   /*---(network)------------------------*/
   a_new->n_read   = 0;
   a_new->n_write  = 0;
   a_new->n_flag   = '-';
   /*---(procs)--------------------------*/
   if (a_type == '*') {
      a_new->e_link   = NULL;
      a_new->m_prev   = NULL;
      a_new->m_next   = NULL;
      a_new->t_head   = NULL;
      a_new->t_tail   = NULL;
      a_new->t_count  = 0;
   }
   /*---(done)---------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char PROC_new     (void **a_new) { return SHARE_new   ('P', a_new, '-'); }
char PROC_force   (void **a_new) { return SHARE_new   ('P', a_new, 'y'); }
char PROC_free    (void **a_old) { return SHARE_free  ('P', a_old); }
char PROC_purge   (void)         { return SHARE_purge ('P'); }



/*====================------------------------------------====================*/
/*===----                   hooking and unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
PROC_hook               (tPROC **a_proc, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_proc = NULL;
   /*---(walk)---------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (p_head);
   x_proc = p_head;
   while (x_proc != NULL) {
      if (x_proc->rpid == a_rpid) {
         DEBUG_YEXEC  yLOG_snote   ("existing");
         p_curr = x_proc;
         DEBUG_YEXEC  yLOG_spoint  (p_curr);
         *a_proc = p_curr;
         DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      x_proc = x_proc->m_next;
   }
   /*---(add if necessary)---------------*/
   DEBUG_YEXEC  yLOG_snote   ("must add");
   rc = PROC_new (&x_proc);
   DEBUG_YEXEC  yLOG_sint    (rc);
   if (rc < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(adding)-------------------------*/
   p_curr = x_proc;
   DEBUG_YEXEC  yLOG_spoint  (p_curr);
   x_proc->rpid = a_rpid;
   *a_proc = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
PROC_unhook             (tPROC **a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (*a_proc);
   --rce;  if (*a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(unlink exec)--------------------*/
   rc = EXEC_unhook (*a_proc);
   rc = LIBS_unhook (*a_proc);
   /*---(remove base)--------------------*/
   rc = PROC_free (a_proc);
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char PROC_by_cursor      (char a_move, tPROC **a_curr)                  { return SHARE_by_cursor ('P', a_move, a_curr); }
char PROC_by_exec_cursor (tEXEC *a_owner, char a_move, tPROC **a_curr)  { return SHARE_cursor_by_owner ('E', a_owner, a_move, a_curr); }
char PROC_by_index       (int a_index, tPROC **a_curr)                  { return SHARE_by_index  ('P', a_index, a_curr); }

char
PROC_by_rpid            (int a_rpid, tPROC **a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_curr != NULL)  *a_curr = NULL;
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (p_head);
   --rce;  if (p_head == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   x_proc = p_head;
   while (x_proc != NULL) {
      if (x_proc->rpid == a_rpid) {
         p_curr = x_proc;
         break;
      }
      x_proc = x_proc->m_next;
   }
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (x_proc);
   --rce;  if (x_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (a_curr != NULL)  *a_curr = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}




/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROC__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tPROC      *x_proc      = NULL;
   tLIBS      *x_lib       = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        w           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "PROC             : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_proc = p_head; while (x_proc != NULL) { ++x_fore; x_proc = x_proc->m_next; }
      x_proc = p_tail; while (x_proc != NULL) { ++x_back; x_proc = x_proc->m_prev; }
      snprintf (unit_answer, LEN_RECD, "PROC count       : num=%4d, fore=%4d, back=%4d", p_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "list"        )   == 0) {
      snprintf (unit_answer, LEN_RECD, "PROC list        : num=%4d, head=%-10p, tail=%p", p_count, p_head, p_tail);
   }
   else if (strcmp (a_question, "entry"    )      == 0) {
      PROC_by_index (n, &x_proc);
      /*> if (n == -1)   x_proc = p_curr;                                             <* 
       *> else {                                                                      <* 
       *>    x_proc = p_head;                                                         <* 
       *>    while (x_proc != NULL) {                                                 <* 
       *>       if (c == n)  break;                                                   <* 
       *>       ++c;                                                                  <* 
       *>       x_proc = x_proc->m_next;                                              <* 
       *>    }                                                                        <* 
       *> }                                                                           <*/
      if (x_proc != NULL) {
         strcpy (t, " -еж");
         strcpy (s, "-");
         strcpy (u, " -еж");
         strcpy (w, " -еж");
         if (x_proc->e_link != NULL) {
            sprintf  (t, "%2dе%.10sж", strlen (x_proc->e_link->name), x_proc->e_link->name);
            sprintf  (s, "%d", x_proc->e_link->inode);
         }
         if (x_proc->t_head)   sprintf  (u, "%2dе%.10sж", strlen (x_proc->t_head->l_link), x_proc->t_head->l_link);
         if (x_proc->t_tail)   sprintf  (w, "%2dе%.10sж", strlen (x_proc->t_tail->l_link), x_proc->t_tail->l_link);
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : %-5d %-14.14s %-9.9s %-5d %c   %2d %-14.14s %s",
               n, x_proc->rpid, t, s, x_proc->ppid, '-',
               x_proc->t_count, u, w);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : -      -еж           -         -     -    -  -еж            -еж", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


