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
   strlcpy (s_print, "е____.____.__.____.__.___ж", LEN_RECD);
   ++n;  if (a_cur->base [0]    != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->full [0]    != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->hint [0]    != '\0')        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_cons      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_heap      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_kern      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_code      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->s_total     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_bss       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->p_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_count     >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->e_note      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->e_col       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->e_seq       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->e_shown     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->e_print [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->e_alt   [0] != '\0')        s_print [n] = 'X';
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
   a_new->m_cons   = 0;
   a_new->m_heap   = 0;
   a_new->m_kern   = 0;
   a_new->m_code   = 0;
   a_new->m_data   = 0;
   /*---(master)-------------------------*/
   if (a_type == '-')  return 0;
   a_new->base [0] = '\0';
   a_new->full [0] = '\0';
   a_new->inode    = 0;
   a_new->hint [0] = '\0';
   /*---(size)---------------------------*/
   a_new->s_total  = 0;
   a_new->s_text   = 0;
   a_new->s_data   = 0;
   a_new->s_bss    = 0;
   /*---(executables)--------------------*/
   a_new->m_prev   = NULL;
   a_new->m_next   = NULL;
   /*---(processes)----------------------*/
   a_new->p_head   = NULL;
   a_new->p_tail   = NULL;
   a_new->p_count  = 0;
   /*---(working)------------------------*/
   a_new->e_note   = 0;
   a_new->f_seq    = 0;
   a_new->f_temp   = 0;
   a_new->e_col    = 0;
   a_new->e_seq    = 0;
   a_new->e_shown  = '-';
   a_new->e_print [0] = '\0';
   a_new->e_alt   [0] = '\0';
   /*---(done)---------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char EXEC_new     (void **a_new) { return SHARE_new   (TYPE_EXEC, a_new, '-'); }
char EXEC_force   (void **a_new) { return SHARE_new   (TYPE_EXEC, a_new, 'y'); }
char EXEC_free    (void **a_old) { return SHARE_free  (TYPE_EXEC, a_old); }
char EXEC_purge   (void)         { return SHARE_purge (TYPE_EXEC); }



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
   DEBUG_NORM   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_point   ("a_proc", a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_NORM   yLOG_point   ("a_name", a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_NORM   yLOG_note    (a_name);
   /*---(pre-check)----------------------*/
   --rce;  if (a_proc->e_link != NULL) {
      if (strcmp (a_proc->e_link->base, a_name) == 0) {
         DEBUG_NORM   yLOG_note    ("already attached correctly");
         DEBUG_NORM   yLOG_exit    (__FUNCTION__);
         return 2;
      } else {
         DEBUG_NORM   yLOG_note    ("already attached, unhook before repeating");
         DEBUG_NORM   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(walk)---------------------------*/
   e_temp = e_head;
   while (e_temp != NULL) {
      if (strcmp (e_temp->base, a_name) == 0) {
         e_curr = e_temp;
         break;
      }
      e_temp = e_temp->m_next;
   }
   /*---(add if necessary)---------------*/
   --rce;  if (e_temp == NULL) {
      DEBUG_NORM   yLOG_note    ("must add");
      rc = EXEC_new (&e_temp);
      DEBUG_NORM   yLOG_value   ("rc", rc);
      if (rc < 0) {
         DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      e_curr = e_temp;
      strlcpy (e_temp->base, a_name, LEN_TITLE);
      rc = 1;
   } else {
      DEBUG_NORM   yLOG_note    ("existing");
      rc = 0;
   }
   DEBUG_NORM   yLOG_point   ("e_curr", e_curr);
   /*---(initialize)---------------------*/
   a_proc->p_prev  = a_proc->p_next = NULL;
   /*---(link exec to proc)--------------*/
   if (e_curr->p_head == NULL) {
      DEBUG_DATA   yLOG_note   ("first on exec");
      e_curr->p_head  = e_curr->p_tail = a_proc;
   } else {
      DEBUG_DATA   yLOG_note    ("append to exec");
      a_proc->p_prev          = e_curr->p_tail;
      e_curr->p_tail->p_next  = a_proc;
      e_curr->p_tail          = a_proc;
   }
   /*---(tie proc back to exec)----------*/
   a_proc->e_link = e_curr;
   /*---(update count)-------------------*/
   ++e_curr->p_count;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
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
   DEBUG_NORM   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_point   ("a_proc", a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_exec = a_proc->e_link;
   DEBUG_NORM   yLOG_point   ("x_exec", x_exec);
   if (x_exec == NULL) {
      DEBUG_NORM    yLOG_exit    (__FUNCTION__);
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
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
EXEC_rehook             (tPROC *a_proc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_point   ("a_proc"    , a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook)-------------------------*/
   rc = EXEC_unhook (a_proc);
   DEBUG_NORM   yLOG_value   ("unhook"    , rc);
   /*---(re-hook)------------------------*/
   rc = EXEC_hook   (a_proc, a_name);
   DEBUG_NORM   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_NORM    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char EXEC_by_cursor  (tEXEC **r_curr, char a_move)  { return SHARE_by_cursor (TYPE_EXEC, r_curr, a_move); }
char EXEC_by_index   (tEXEC **r_curr, int a_index)  { return SHARE_by_index  (TYPE_EXEC, r_curr, a_index); }
char EXEC_by_inode   (tEXEC **r_curr, int a_inode)  { return SHARE_by_inode  (TYPE_EXEC, r_curr, a_inode); }
char EXEC_by_name    (tEXEC **r_curr, char *a_name) { return SHARE_by_name   (TYPE_EXEC, r_curr, a_name); }
char EXEC_by_hint    (tEXEC **r_curr, char *a_hint) { return SHARE_by_hint   (TYPE_EXEC, r_curr, a_hint); }



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
      EXEC_by_index (&x_exec, n);
      if (x_exec != NULL) {
         sprintf  (t, "%2dе%.10sж", strlen (x_exec->base), x_exec->base);
         sprintf  (s, "%2dе%.16sж", strlen (x_exec->full), x_exec->full);
         sprintf  (u, "еж");
         sprintf  (w, "еж");
         if (x_exec->p_head != NULL) {
            sprintf  (u, "е%dж", x_exec->p_head->rpid);
            sprintf  (w, "е%dж", x_exec->p_tail->rpid);
         }
         snprintf (unit_answer, LEN_RECD, "EXEC entry  (%2d) : %-14.14s %-9d %-20.20s   %2d %-8.8s %s",
               n, t, x_exec->inode, s, x_exec->p_count, u, w);
      } else {
         snprintf (unit_answer, LEN_RECD, "EXEC entry  (%2d) :  -еж           -          -еж                    - еж       еж", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



