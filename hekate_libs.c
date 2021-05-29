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
   strlcpy (s_print, "�___.___._____.__.___�", LEN_RECD);
   ++n;  if (a_cur->terse   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->name    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_full      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->s_total     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_bss       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_disk      >  0)           s_print [n] = 'X';
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
   a_new->m_data   = 0;
   /*---(size)---------------------------*/
   a_new->s_total  = 0;
   a_new->s_text   = 0;
   a_new->s_data   = 0;
   a_new->s_bss    = 0;
   a_new->s_disk   = 0;
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

char LIBS_new     (void **a_new) { return SHARE_new   ('L', a_new, '-'); }
char LIBS_force   (void **a_new) { return SHARE_new   ('L', a_new, 'y'); }
char LIBS_free    (void **a_old) { return SHARE_free  ('L', a_old); }
char LIBS_purge   (void)         { return SHARE_purge ('L'); }



/*====================------------------------------------====================*/
/*===----                   hooking and unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
LIBS_hook               (tPROC *a_proc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTIES      *t_temp      = NULL;
   tLIBS      *l_temp      = NULL;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_snote   (a_name);
   /*---(walk)---------------------------*/
   t_temp = a_proc->t_head;
   while (t_temp != NULL) {
      if (t_temp->l_link != NULL && strcmp (t_temp->l_link->name, a_name) == 0) {
         t_curr = t_temp;
         DEBUG_YEXEC  yLOG_snote   ("existing tie/lib");
         DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      t_temp = t_temp->l_next;
   }
   /*---(add tie)------------------------*/
   DEBUG_YEXEC  yLOG_snote   ("add tie");
   rc = TIES_new (&t_temp);
   DEBUG_YEXEC  yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   t_temp->p_link = a_proc;
   t_curr = t_temp;
   rc = 1;
   DEBUG_YEXEC  yLOG_spoint  (t_curr);
   /*---(walk)---------------------------*/
   l_temp = l_head;
   while (l_temp != NULL) {
      if (strcmp (l_temp->name, a_name) == 0) {
         l_curr = l_temp;
         break;
      }
      l_temp = l_temp->m_next;
   }
   DEBUG_YEXEC  yLOG_spoint  (l_curr);
   /*---(add lib)------------------------*/
   if (l_temp == NULL) {
      DEBUG_YEXEC  yLOG_snote   ("add lib");
      rc = LIBS_new (&l_temp);
      DEBUG_YEXEC  yLOG_sint    (rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      l_curr = l_temp;
   }
   /*---(add name)-----------------------*/
   DEBUG_YEXEC  yLOG_spoint  (l_curr);
   strlcpy (l_curr->name , a_name, LEN_TITLE);
   p = strrchr (a_name, '/');
   if (p == NULL)  p = a_name;
   else            ++p;
   strlcpy (l_curr->terse, p, LEN_TITLE);
   rc = 1;
   /*---(hook tie to proc)---------------*/
   if (a_proc->t_head == NULL) {
      DEBUG_DATA   yLOG_snote  ("first");
      a_proc->t_head  = a_proc->t_tail = t_temp;
   } else {
      DEBUG_DATA   yLOG_snote   ("append");
      t_temp->p_prev          = a_proc->t_tail;
      a_proc->t_tail->p_next  = t_temp;
      a_proc->t_tail          = t_temp;
   }
   ++(a_proc->t_count);
   /*---(hook tie to lib)----------------*/
   t_temp->l_link = l_temp;
   if (l_curr->t_head == NULL) {
      DEBUG_DATA   yLOG_snote  ("first");
      l_curr->t_head  = l_curr->t_tail = t_temp;
   } else {
      DEBUG_DATA   yLOG_snote   ("append");
      t_temp->l_prev          = l_curr->t_tail;
      l_curr->t_tail->l_next  = t_temp;
      l_curr->t_tail          = t_temp;
   }
   ++(l_curr->t_count);
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
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
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_tie  = a_proc->t_head;
   DEBUG_YEXEC  yLOG_spoint  (x_tie);
   if (x_tie == NULL) {
      DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
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
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char LIBS_by_cursor  (char a_move, tLIBS **a_curr)  { return SHARE_by_cursor ('L', a_move, a_curr); }
char LIBS_by_index   (int a_index, tLIBS **a_curr)  { return SHARE_by_index  ('L', a_index, a_curr); }
char LIBS_by_inode   (int a_inode, tLIBS **a_curr)  { return SHARE_by_inode  ('L', a_inode, a_curr); }
char LIBS_by_name    (char *a_name, tLIBS **a_curr) { return SHARE_by_name   ('L', a_name, a_curr); }



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
   tLIBS      *x_lib       = NULL;
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
      x_lib = l_head; while (x_lib != NULL) { ++x_fore; x_lib = x_lib->m_next; }
      x_lib = l_tail; while (x_lib != NULL) { ++x_back; x_lib = x_lib->m_prev; }
      snprintf (unit_answer, LEN_RECD, "LIBS count       : num=%4d, fore=%4d, back=%4d", l_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "list"        )   == 0) {
      snprintf (unit_answer, LEN_RECD, "LIBS list        : num=%4d, head=%-10p, tail=%p", l_count, l_head, l_tail);
   }
   else if (strcmp (a_question, "entry"    )      == 0) {
      LIBS_by_index (n, &x_lib);
      if (x_lib != NULL) {
         sprintf  (t, "%2d�%.10s�", strlen (x_lib->terse), x_lib->terse);
         sprintf  (u, "%2d�%.20s�", strlen (x_lib->name ), x_lib->name);
         snprintf (unit_answer, LEN_RECD, "LIBS entry  (%2d) : %-14.14s %-24.24s  %-9ld  %2d  %7d",
               n, t, u, x_lib->inode, x_lib->t_count, x_lib->m_text);
      } else {
         snprintf (unit_answer, LEN_RECD, "LIBS entry  (%2d) :  -��            -��                      -           -        -", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


