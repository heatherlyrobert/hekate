/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



tTIES      *t_head      = NULL;
tTIES      *t_tail      = NULL;
tTIES      *t_curr      = NULL;
int         t_count     = 0;



static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*
TIES__memory            (tTIES *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "�__.__.___.___�", LEN_RECD);
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_heap      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->p_link      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->l_link      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->l_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->l_next      != NULL)        s_print [n] = 'X';
   return s_print;
}

char
TIES_wipe               (tTIES *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(memory--------------------------*/
   a_new->m_data   = 0;
   a_new->m_heap   = 0;
   /*---(links)--------------------------*/
   if (a_type == '*') {
      a_new->m_prev   = NULL;
      a_new->m_next   = NULL;
      a_new->p_link   = NULL;
      a_new->p_prev   = NULL;
      a_new->p_next   = NULL;
      a_new->l_link   = NULL;
      a_new->l_prev   = NULL;
      a_new->l_next   = NULL;
   }
   /*---(done)---------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      memory allocation                       ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char TIES_new     (void **a_new) { return SHARE_new   (TYPE_TIES, a_new, '-'); }
char TIES_force   (void **a_new) { return SHARE_new   (TYPE_TIES, a_new, 'y'); }
char TIES_free    (void **a_old) { return SHARE_free  (TYPE_TIES, a_old); }
char TIES_purge   (void)         { return SHARE_purge (TYPE_TIES); }



/*====================------------------------------------====================*/
/*===----                     searching and finding                    ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

char TIES_by_cursor      (tTIES **r_curr, char a_move)                  { return SHARE_by_cursor       (TYPE_TIES, r_curr, a_move); }
char TIES_by_proc_cursor (tTIES **r_curr, tPROC *a_owner, char a_move)  { return SHARE_cursor_by_owner (TYPE_PROC, r_curr, a_owner, a_move); }
char TIES_by_libs_cursor (tTIES **r_curr, tLIBS *a_owner, char a_move)  { return SHARE_cursor_by_owner (TYPE_LIBS, r_curr, a_owner, a_move); }
char TIES_by_index       (tTIES **r_curr, int a_index)                  { return SHARE_by_index        (TYPE_TIES, r_curr, a_index); }

char
TIES_by_exec_cursor     (tTIES **r_curr, tEXEC *a_owner, char a_move)
{
   char        rce         =  -10;
   --rce;  if (a_owner == NULL)  return rce;
   return SHARE_cursor_by_owner (TYPE_PROC, r_curr, a_owner->p_head, a_move);
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
TIES__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tTIES      *x_tie       = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        w           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "TIES             : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "count"    )      == 0) {
      x_tie = t_head; while (x_tie != NULL) { ++x_fore; x_tie = x_tie->m_next; }
      x_tie = t_tail; while (x_tie != NULL) { ++x_back; x_tie = x_tie->m_prev; }
      snprintf (unit_answer, LEN_RECD, "TIES count       : num=%4d, fore=%4d, back=%4d", t_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "list"        )   == 0) {
      snprintf (unit_answer, LEN_RECD, "TIES list        : num=%4d, head=%-10p, tail=%p", t_count, t_head, t_tail);
   }
   else if (strcmp (a_question, "entry"    )      == 0) {
      TIES_by_index (&x_tie, n);
      if (x_tie != NULL) {
         strcpy (t, " -��");
         strcpy (u, " -��");
         if (x_tie->p_link != NULL && x_tie->p_link->e_link)  sprintf  (t, "%2d�%.10s�", strlen (x_tie->p_link->e_link->base), x_tie->p_link->e_link->base);
         if (x_tie->l_link != NULL)  sprintf  (u, "%2d�%.10s�", strlen (x_tie->l_link->terse), x_tie->l_link->terse);
         snprintf (unit_answer, LEN_RECD, "TIES entry  (%2d) : %-5d  %-14.14s %s",
               n, x_tie->p_link->rpid, t, u);
      } else {
         snprintf (unit_answer, LEN_RECD, "TIES entry  (%2d) : -       -��            -��", n);
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


