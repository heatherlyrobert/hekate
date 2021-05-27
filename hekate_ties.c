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
   strlcpy (s_print, "å__.__.___.___æ", LEN_RECD);
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

char TIES_new     (void **a_new) { return SHARE_new   ('T', a_new, '-'); }
char TIES_force   (void **a_new) { return SHARE_new   ('T', a_new, 'y'); }
char TIES_free    (void **a_old) { return SHARE_free  ('T', a_old); }
char TIES_purge   (void)         { return SHARE_purge ('T'); }


