/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tEXEC      *e_head      = NULL;
tEXEC      *e_tail      = NULL;
tEXEC      *e_curr      = NULL;
int         e_count     = 0;



static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*
EXEC__memory            (tEXEC *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "е__._._____.__.___ж", LEN_RECD);
   ++n;  if (a_cur->name [0]    != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
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
   ++n;  if (a_cur->p_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_count     >  0)           s_print [n] = 'X';
   return s_print;
}

char
EXEC_wipe               (tEXEC *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(memory)-------------------------*/
   a_new->m_text   = 0;
   /*---(master)-------------------------*/
   if (a_type == '-')  return 0;
   a_new->name [0] = '\0';
   a_new->inode    = 0;
   /*---(size)---------------------------*/
   a_new->s_total  = 0;
   a_new->s_text   = 0;
   a_new->s_data   = 0;
   a_new->s_bss    = 0;
   a_new->s_disk   = 0;
   /*---(executables)--------------------*/
   a_new->m_prev   = NULL;
   a_new->m_next   = NULL;
   /*---(processes)----------------------*/
   a_new->p_head   = NULL;
   a_new->p_tail   = NULL;
   a_new->p_count  = 0;
   /*---(done)---------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char EXEC_new     (void **a_new) { return SHARE_new   ('E', a_new, '-'); }
char EXEC_force   (void **a_new) { return SHARE_new   ('E', a_new, 'y'); }
char EXEC_free    (void **a_old) { return SHARE_free  ('E', a_old); }
char EXEC_purge   (void)         { return SHARE_purge ('E'); }



/*====================------------------------------------====================*/
/*===----                   hooking and unhooking                      ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
EXEC_hook               (tPROC *a_proc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *e_temp      = NULL;
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
   /*---(pre-check)----------------------*/
   --rce;  if (a_proc->e_link != NULL) {
      if (strcmp (a_proc->e_link->name, a_name) == 0) {
         DEBUG_YEXEC  yLOG_snote   ("already attached correctly");
         DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
         return 2;
      } else {
         DEBUG_YEXEC  yLOG_snote   ("already attached, unhook before repeating");
         DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(walk)---------------------------*/
   e_temp = e_head;
   while (e_temp != NULL) {
      if (strcmp (e_temp->name, a_name) == 0) {
         e_curr = e_temp;
         break;
      }
      e_temp = e_temp->m_next;
   }
   /*---(add if necessary)---------------*/
   --rce;  if (e_temp == NULL) {
      DEBUG_YEXEC  yLOG_snote   ("must add");
      rc = EXEC_new (&e_temp);
      DEBUG_YEXEC  yLOG_sint    (rc);
      if (rc < 0) {
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      e_curr = e_temp;
      strlcpy (e_temp->name, a_name, LEN_TITLE);
      rc = 1;
   } else {
      DEBUG_YEXEC  yLOG_snote   ("existing");
      rc = 0;
   }
   DEBUG_YEXEC  yLOG_spoint  (e_curr);
   /*---(initialize)---------------------*/
   a_proc->p_prev  = a_proc->p_next = NULL;
   /*---(link exec to proc)--------------*/
   if (e_curr->p_head == NULL) {
      DEBUG_DATA   yLOG_snote  ("first");
      e_curr->p_head  = e_curr->p_tail = a_proc;
   } else {
      DEBUG_DATA   yLOG_snote   ("append");
      a_proc->p_prev          = e_curr->p_tail;
      e_curr->p_tail->p_next  = a_proc;
      e_curr->p_tail          = a_proc;
   }
   /*---(tie proc back to exec)----------*/
   a_proc->e_link = e_curr;
   /*---(update count)-------------------*/
   ++e_curr->p_count;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
EXEC_unhook             (tPROC *a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *x_exec      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_exec = a_proc->e_link;
   DEBUG_YEXEC  yLOG_spoint  (x_exec);
   if (x_exec == NULL) {
      DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(unlink exec from proc)----------*/
   if (a_proc->p_next != NULL)  a_proc->p_next->p_prev = a_proc->p_prev;
   else                         x_exec->p_tail         = a_proc->p_prev;
   if (a_proc->p_prev != NULL)  a_proc->p_prev->p_next = a_proc->p_next;
   else                         x_exec->p_head         = a_proc->p_next;
   /*---(update count)-------------------*/
   --(x_exec->p_count);
   /*---(ground)-------------------------*/
   a_proc->e_link = NULL;
   /*---(remove if necesary)-------------*/
   if (x_exec->p_count <= 0)  EXEC_free (&x_exec);
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
EXEC_rehook             (tPROC *a_proc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook)-------------------------*/
   rc = EXEC_unhook (a_proc);
   DEBUG_YEXEC  yLOG_value   ("unhook"    , rc);
   /*---(re-hook)------------------------*/
   rc = EXEC_hook   (a_proc, a_name);
   DEBUG_YEXEC  yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char EXEC_by_cursor  (char a_move, tEXEC **a_curr)  { return SHARE_by_cursor ('E', a_move, a_curr); }
char EXEC_by_index   (int a_index, tEXEC **a_curr)  { return SHARE_by_index  ('E', a_index, a_curr); }
char EXEC_by_inode   (int a_inode, tEXEC **a_curr)  { return SHARE_by_inode  ('E', a_inode, a_curr); }
char EXEC_by_name    (char *a_name, tEXEC **a_curr) { return SHARE_by_name   ('E', a_name, a_curr); }



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
EXEC__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tEXEC      *x_exec      = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        w           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "EXEC             : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_exec = e_head; while (x_exec != NULL) { ++x_fore; x_exec = x_exec->m_next; }
      x_exec = e_tail; while (x_exec != NULL) { ++x_back; x_exec = x_exec->m_prev; }
      snprintf (unit_answer, LEN_RECD, "EXEC count       : num=%4d, fore=%4d, back=%4d", e_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "list"        )   == 0) {
      snprintf (unit_answer, LEN_RECD, "EXEC list        : num=%4d, head=%-10p, tail=%p", e_count, e_head, e_tail);
   }
   else if (strcmp (a_question, "entry"    )      == 0) {
      EXEC_by_index (n, &x_exec);
      if (x_exec != NULL) {
         sprintf  (t, "%2dе%.10sж", strlen (x_exec->name), x_exec->name);
         snprintf (unit_answer, LEN_RECD, "EXEC entry  (%2d) : %-14.14s %-9d   %2d %-10p %p",
               n, t, x_exec->inode, x_exec->p_count, x_exec->p_head, x_exec->p_tail);
      } else {
         snprintf (unit_answer, LEN_RECD, "EXEC entry  (%2d) :  -еж           -            - -          -", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



