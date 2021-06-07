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
   strlcpy (s_print, "е____._____._______.___.___._.__.___ж", LEN_RECD);
   ++n;  if (a_cur->rpid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->rpid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->shown [0]   != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->cmdline [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->hint [0]    != '\0')        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->c_state     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->c_utime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_stime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_snice     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_full      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_proc      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_heap      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_stack     >  0)           s_print [n] = 'X';
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
   ++n;  if (a_cur->h_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->h_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->h_count     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->t_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_count     >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->note        != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->f_seq       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->e_seq       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->p_lvl       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->p_seq       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->e_print [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->m_print [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->p_print [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->t_print [0] != '\0')        s_print [n] = 'X';
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
      a_new->rpid        = 0;
      a_new->ppid        = 0;
      a_new->shown [0]   = '\0';
      a_new->cmdline [0] = '\0';
      a_new->hint [0]    = '\0';
   }
   /*---(cpu)----------------------------*/
   a_new->c_state  = '-';
   a_new->c_utime  = 0;
   a_new->c_stime  = 0;
   a_new->c_snice  = 0;
   a_new->c_flag   = '-';
   /*---(memory--------------------------*/
   a_new->m_full   = 0;
   a_new->m_proc   = 0;
   a_new->m_data   = 0;
   a_new->m_heap   = 0;
   a_new->m_stack  = 0;
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
      a_new->h_head   = NULL;
      a_new->h_next   = NULL;
      a_new->h_tail   = NULL;
      a_new->h_count  = 0;
      a_new->t_head   = NULL;
      a_new->t_tail   = NULL;
      a_new->t_count  = 0;
   }
   /*---(working)------------------------*/
   a_new->note      = '-';
   a_new->f_seq     = 0;
   a_new->e_seq     = 0;
   a_new->p_lvl     = 0;
   a_new->p_seq     = 0;
   a_new->e_print [0] = '\0';
   a_new->m_print [0] = '\0';
   a_new->p_print [0] = '\0';
   a_new->t_print [0] = '\0';
   /*---(done)---------------------------*/
   return 0;
}


/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char PROC_new     (void **a_new) { return SHARE_new   (TYPE_PROC, a_new, '-'); }
char PROC_force   (void **a_new) { return SHARE_new   (TYPE_PROC, a_new, 'y'); }
char PROC_free    (void **a_old) { return SHARE_free  (TYPE_PROC, a_old); }
char PROC_purge   (void)         { return SHARE_purge (TYPE_PROC); }



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
   DEBUG_NORM   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_point   ("a_proc", a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_proc = NULL;
   /*---(walk)---------------------------*/
   DEBUG_NORM   yLOG_point   ("p_head", p_head);
   x_proc = p_head;
   while (x_proc != NULL) {
      if (x_proc->rpid == a_rpid) {
         DEBUG_NORM   yLOG_note    ("existing");
         p_curr = x_proc;
         DEBUG_NORM   yLOG_point   ("p_curr", p_curr);
         *a_proc = p_curr;
         DEBUG_NORM   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_proc = x_proc->m_next;
   }
   /*---(add if necessary)---------------*/
   DEBUG_NORM   yLOG_note    ("must add");
   rc = PROC_new (&x_proc);
   DEBUG_NORM   yLOG_value   ("rc", rc);
   if (rc < 0) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(adding)-------------------------*/
   p_curr = x_proc;
   DEBUG_NORM   yLOG_point   ("p_curr", p_curr);
   x_proc->rpid = a_rpid;
   *a_proc = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
PROC_unhook             (tPROC **a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_point   ("a_proc", a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_NORM   yLOG_point   ("*a_proc", *a_proc);
   --rce;  if (*a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unlink exec)--------------------*/
   rc = EXEC_unhook (*a_proc);
   rc = LIBS_unhook (*a_proc);
   /*---(remove base)--------------------*/
   rc = PROC_free (a_proc);
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char PROC_by_cursor      (tPROC **r_curr, char a_move)                  { return SHARE_by_cursor       (TYPE_PROC, r_curr, a_move); }
char PROC_by_exec_cursor (tPROC **r_curr, tEXEC *a_owner, char a_move)  { return SHARE_cursor_by_owner (TYPE_EXEC, r_curr, a_owner, a_move); }
char PROC_by_index       (tPROC **r_curr, int a_index)                  { return SHARE_by_index        (TYPE_PROC, r_curr, a_index); }

char
PROC_by_rpid            (tPROC **r_curr, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (p_head);
   --rce;  if (p_head == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
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
   DEBUG_NORM   yLOG_spoint  (x_proc);
   --rce;  if (x_proc == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (r_curr != NULL)  *r_curr = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PROC_by_seq             (tPROC **r_curr, int a_seq)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *x_proc      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (p_head);
   --rce;  if (p_head == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   x_proc = p_head;
   while (x_proc != NULL) {
      if (x_proc->f_seq == a_seq) {
         p_curr = x_proc;
         break;
      }
      x_proc = x_proc->m_next;
   }
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (x_proc);
   --rce;  if (x_proc == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (r_curr != NULL)  *r_curr = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
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
   char        r           [LEN_RECD]  = "";
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
      PROC_by_index (&x_proc, n);
      if (x_proc != NULL) {
         strcpy (t, " -еж");
         strcpy (s, "  -еж");
         strcpy (r, " -еж");
         strcpy (u, " -еж");
         strcpy (w, " -еж");
         if (x_proc->e_link != NULL) {
            sprintf  (t, "%2dе%.10sж", strlen (x_proc->e_link->base), x_proc->e_link->base);
            sprintf  (s, "%3dе%.10sж", strlen (x_proc->cmdline), x_proc->cmdline);
            sprintf  (r, "%2dе%.10sж", strlen (x_proc->shown), x_proc->shown);
         }
         if (x_proc->t_head)   sprintf  (u, "%2dе%.10sж", strlen (x_proc->t_head->l_link), x_proc->t_head->l_link);
         if (x_proc->t_tail)   sprintf  (w, "%2dе%.10sж", strlen (x_proc->t_tail->l_link), x_proc->t_tail->l_link);
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : %-5d %-14.14s %-5d %2d %-14.14s %-14.14s%-15.15s %s",
               n, x_proc->rpid, t, x_proc->ppid,
               x_proc->t_count, u, w, s, r);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC entry  (%2d) : -      -еж           -      -  -еж            -еж            -еж            -еж", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


