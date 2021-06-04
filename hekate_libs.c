/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tLIBS      *l_head      = NULL;
tLIBS      *l_tail      = NULL;
tLIBS      *l_curr      = NULL;
int         l_count     = 0;



static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*
LIBS__memory            (tLIBS *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "е___.____.____.__.___ж", LEN_RECD);
   ++n;  if (a_cur->terse   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->name    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_full      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_cons      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_priv      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->s_total     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_bss       >  0)           s_print [n] = 'X';
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
LIBS_wipe               (tLIBS *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(master)-------------------------*/
   if (a_type == '*') {
      a_new->terse [0] = '\0';
      a_new->name [0]  = '\0';
      a_new->inode     = 0;
   }
   /*---(memory)-------------------------*/
   a_new->m_full   = 0;
   a_new->m_text   = 0;
   a_new->m_cons   = 0;
   a_new->m_priv   = 0;
   /*---(size)---------------------------*/
   a_new->s_total  = 0;
   a_new->s_text   = 0;
   a_new->s_data   = 0;
   a_new->s_bss    = 0;
   /*---(links)--------------------------*/
   if (a_type == '*') {
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

char LIBS_new     (void **a_new) { return SHARE_new   (TYPE_LIBS, a_new, '-'); }
char LIBS_force   (void **a_new) { return SHARE_new   (TYPE_LIBS, a_new, 'y'); }
char LIBS_free    (void **a_old) { return SHARE_free  (TYPE_LIBS, a_old); }
char LIBS_purge   (void)         { return SHARE_purge (TYPE_LIBS); }



/*====================------------------------------------====================*/
/*===----                   hooking and unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
LIBS_hook               (tPROC *a_proc, char *a_name, tTIES **a_ties)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_point   ("a_name", a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    (a_name);
   /*---(walk)---------------------------*/
   x_ties = a_proc->t_head;
   while (x_ties != NULL) {
      if (x_ties->l_link != NULL && strcmp (x_ties->l_link->name, a_name) == 0) {
         t_curr = x_ties;
         DEBUG_YEXEC  yLOG_note    ("existing tie/lib");
         if (a_ties != NULL)  *a_ties = x_ties;
         DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      x_ties = x_ties->p_next;
   }
   /*---(add tie)------------------------*/
   DEBUG_YEXEC  yLOG_note    ("add tie");
   rc = TIES_new (&x_ties);
   DEBUG_YEXEC  yLOG_value   ("rc", rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_ties->p_link = a_proc;
   t_curr = x_ties;
   rc = 1;
   DEBUG_YEXEC  yLOG_point   ("t_curr", t_curr);
   /*---(walk)---------------------------*/
   x_libs = l_head;
   while (x_libs != NULL) {
      if (strcmp (x_libs->name, a_name) == 0) {
         l_curr = x_libs;
         break;
      }
      x_libs = x_libs->m_next;
   }
   DEBUG_YEXEC  yLOG_point   ("l_curr", l_curr);
   /*---(add lib)------------------------*/
   if (x_libs == NULL) {
      DEBUG_YEXEC  yLOG_note    ("add lib");
      rc = LIBS_new (&x_libs);
      DEBUG_YEXEC  yLOG_value   ("rc", rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      l_curr = x_libs;
   }
   /*---(add name)-----------------------*/
   DEBUG_YEXEC  yLOG_point   ("l_curr", l_curr);
   strlcpy (l_curr->name , a_name, LEN_RECD);
   p = strrchr (a_name, '/');
   if (p == NULL)  p = a_name;
   else            ++p;
   strlcpy (l_curr->terse, p, LEN_TITLE);
   rc = 1;
   /*---(hook tie to proc)---------------*/
   if (a_proc->t_head == NULL) {
      DEBUG_DATA   yLOG_note   ("first in proc");
      a_proc->t_head  = a_proc->t_tail = x_ties;
   } else {
      DEBUG_DATA   yLOG_note    ("append to proc");
      x_ties->p_prev          = a_proc->t_tail;
      a_proc->t_tail->p_next  = x_ties;
      a_proc->t_tail          = x_ties;
   }
   ++(a_proc->t_count);
   /*---(hook tie to lib)----------------*/
   x_ties->l_link = x_libs;
   if (l_curr->t_head == NULL) {
      DEBUG_DATA   yLOG_note   ("first for lib");
      l_curr->t_head  = l_curr->t_tail = x_ties;
   } else {
      DEBUG_DATA   yLOG_note    ("append to lib");
      x_ties->l_prev          = l_curr->t_tail;
      l_curr->t_tail->l_next  = x_ties;
      l_curr->t_tail          = x_ties;
   }
   ++(l_curr->t_count);
   /*---(save back)----------------------*/
   if (a_ties != NULL)  *a_ties = x_ties;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
LIBS_unhook             (tPROC *a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTIES      *x_tie       = NULL;
   tLIBS      *x_lib       = NULL;
   tTIES      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_proc", a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_tie  = a_proc->t_head;
   DEBUG_YEXEC  yLOG_point   ("x_tie", x_tie);
   if (x_tie == NULL) {
      DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(cycle ties)---------------------*/
   while (x_tie != NULL) {
      /*---(save pointers)---------------*/
      x_next = x_tie->p_next;
      x_lib  = x_tie->l_link;
      /*---(remove tie from proc)--------*/
      if (x_tie->p_next != NULL)  x_tie->p_next->p_prev  = x_tie->p_prev;
      else                        a_proc->t_tail         = x_tie->p_prev;
      if (x_tie->p_prev != NULL)  x_tie->p_prev->p_next  = x_tie->p_next;
      else                        a_proc->t_head         = x_tie->p_next;
      x_tie->p_link = NULL;
      --(a_proc->t_count);
      /*---(remove tie from lib)---------*/
      if (x_tie->l_next != NULL)  x_tie->l_next->l_prev  = x_tie->l_prev;
      else                        x_lib->t_tail          = x_tie->l_prev;
      if (x_tie->l_prev != NULL)  x_tie->l_prev->l_next  = x_tie->l_next;
      else                        x_lib->t_head          = x_tie->p_next;
      x_tie->p_link = NULL;
      --(x_lib->t_count);
      /*---(remove tie from master)------*/
      rc = TIES_free (&x_tie);
      /*---(check on library)------------*/
      if (x_lib->t_count <= 0)  LIBS_free (&x_lib);
      /*---(next)------------------------*/
      x_tie = x_next;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char LIBS_by_cursor  (tLIBS **r_curr, char a_move)  { return SHARE_by_cursor (TYPE_LIBS, r_curr, a_move); }
char LIBS_by_index   (tLIBS **r_curr, int a_index)  { return SHARE_by_index  (TYPE_LIBS, r_curr, a_index); }
char LIBS_by_inode   (tLIBS **r_curr, int a_inode)  { return SHARE_by_inode  (TYPE_LIBS, r_curr, a_inode); }
char LIBS_by_name    (tLIBS **r_curr, char *a_name) { return SHARE_by_name   (TYPE_LIBS, r_curr, a_name); }



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
LIBS__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tLIBS      *x_libs      = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        w           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "LIBS             : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_libs = l_head; while (x_libs != NULL) { ++x_fore; x_libs = x_libs->m_next; }
      x_libs = l_tail; while (x_libs != NULL) { ++x_back; x_libs = x_libs->m_prev; }
      snprintf (unit_answer, LEN_RECD, "LIBS count       : num=%4d, fore=%4d, back=%4d", l_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "list"        )   == 0) {
      snprintf (unit_answer, LEN_RECD, "LIBS list        : num=%4d, head=%-10p, tail=%p", l_count, l_head, l_tail);
   }
   else if (strcmp (a_question, "entry"    )      == 0) {
      LIBS_by_index (&x_libs, n);
      if (x_libs != NULL) {
         sprintf  (t, "%2dе%.10sж", strlen (x_libs->terse), x_libs->terse);
         sprintf  (u, "%2dе%.20sж", strlen (x_libs->name ), x_libs->name);
         snprintf (unit_answer, LEN_RECD, "LIBS entry  (%2d) : %-14.14s %-24.24s  %-9ld  %2d",
               n, t, u, x_libs->inode, x_libs->t_count);
      } else {
         snprintf (unit_answer, LEN_RECD, "LIBS entry  (%2d) :  -еж            -еж                      -           -", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


