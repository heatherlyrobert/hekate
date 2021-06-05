/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



#define  IF_EXEC   if      (a_type == TYPE_EXEC)
#define  EL_PROC   else if (a_type == TYPE_PROC)
#define  EL_TIES   else if (a_type == TYPE_TIES)
#define  EL_LIBS   else if (a_type == TYPE_LIBS)
#define  EL_DATA   else if (a_type == TYPE_DATA)
#define  EL_ALLS   else if (a_type == TYPE_ALLS)
#define  ELSE      else 



/*====================------------------------------------====================*/
/*===----                       allocation/memory                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
SHARE_new               (char a_type, void **a_new, char a_force)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_new       = NULL;
   int         x_tries     =    0;
   int         x_size      =    0;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   tDATA      *x_data      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*a_new);
   --rce;  if (a_force != 'y' && *a_new != NULL) {
      DEBUG_DATA   yLOG_snote   ("already set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(check size)---------------------*/
   --rce;
   IF_EXEC   x_size = sizeof (tEXEC);
   EL_PROC   x_size = sizeof (tPROC);
   EL_TIES   x_size = sizeof (tTIES);
   EL_LIBS   x_size = sizeof (tLIBS);
   EL_DATA   x_size = sizeof (tDATA);
   ELSE {
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_sint    (x_size);
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (x_size);
      if (x_tries > 3)   break;
   }
   DEBUG_DATA   yLOG_sint    (x_tries);
   DEBUG_DATA   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe, attach, and increment)----*/
   --rce;
   IF_EXEC {
      x_exec = (tEXEC *) x_new;
      EXEC_wipe (x_exec, '*');
      if    (e_tail == NULL) { e_head = x_exec; }
      else { x_exec->m_prev  = e_tail; e_tail->m_next = x_exec; }
      e_tail = x_exec;
      strlhint (e_count, "ll", x_exec->hint);
      ++e_count;
   } EL_PROC {
      x_proc = (tPROC *) x_new;
      PROC_wipe (x_proc, '*');
      if    (p_tail == NULL) { p_head = x_proc; }
      else { x_proc->m_prev  = p_tail; p_tail->m_next = x_proc; }
      p_tail = x_proc;
      strlhint (p_count + (4 * 26), "ll", x_proc->hint);
      ++p_count;
   } EL_TIES {
      x_ties = (tTIES *) x_new;
      TIES_wipe (x_ties, '*');
      if    (t_tail == NULL) { t_head = x_ties; }
      else { x_ties->m_prev  = t_tail; t_tail->m_next = x_ties; }
      t_tail = x_ties;
      ++t_count;
   } EL_LIBS {
      x_libs = (tLIBS *) x_new;
      LIBS_wipe (x_libs, '*');
      if    (l_tail == NULL) { l_head = x_libs; }
      else { x_libs->m_prev  = l_tail; l_tail->m_next = x_libs; }
      l_tail = x_libs;
      strlhint (l_count + (11 * 26), "ll", x_libs->hint);
      ++l_count;
   } EL_DATA {
      x_data = (tDATA *) x_new;
      x_data->rpid = 0;
      x_data->m_prev = NULL;
      x_data->m_next = NULL;
      if    (d_tail == NULL) { d_head = x_data; }
      else { x_data->m_prev  = d_tail; d_tail->m_next = x_data; }
      d_tail = x_data;
      ++d_count;
   }
   /*---(save return)--------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
SHARE_free              (char a_type, void **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   tDATA      *x_data      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_DATA   yLOG_snote   ("never set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(detach and decrement)-----------*/
   --rce;
   IF_EXEC {
      x_exec = (tEXEC *) *a_old;
      if (x_exec->m_next != NULL) x_exec->m_next->m_prev   = x_exec->m_prev;
      else                        e_tail                   = x_exec->m_prev;
      if (x_exec->m_prev != NULL) x_exec->m_prev->m_next   = x_exec->m_next;
      else                        e_head                   = x_exec->m_next;
      --e_count;
   } EL_PROC {
      x_proc = (tPROC *) *a_old;
      if (x_proc->m_next != NULL) x_proc->m_next->m_prev   = x_proc->m_prev;
      else                        p_tail                   = x_proc->m_prev;
      if (x_proc->m_prev != NULL) x_proc->m_prev->m_next   = x_proc->m_next;
      else                        p_head                   = x_proc->m_next;
      --p_count;
   } EL_TIES {
      x_ties = (tTIES *) *a_old;
      if (x_ties->m_next != NULL)  x_ties->m_next->m_prev  = x_ties->m_prev;
      else                         t_tail                  = x_ties->m_prev;
      if (x_ties->m_prev != NULL)  x_ties->m_prev->m_next  = x_ties->m_next;
      else                         t_head                  = x_ties->m_next;
      --t_count;
   } EL_LIBS {
      x_libs  = (tLIBS *) *a_old;
      if (x_libs->m_next != NULL)  x_libs->m_next->m_prev  = x_libs->m_prev;
      else                         l_tail                  = x_libs->m_prev;
      if (x_libs->m_prev != NULL)  x_libs->m_prev->m_next  = x_libs->m_next;
      else                         l_head                  = x_libs->m_next;
      --l_count;
   } EL_DATA {
      x_data = (tDATA *) *a_old;
      if (x_data->m_next != NULL)  x_data->m_next->m_prev  = x_data->m_prev;
      else                         d_tail                  = x_data->m_prev;
      if (x_data->m_prev != NULL)  x_data->m_prev->m_next  = x_data->m_next;
      else                         d_head                  = x_data->m_next;
      --d_count;
   }
   /*---(clear and return)---------------*/
   free (*a_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
SHARE_purge             (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(prepare)------------------------*/
   --rce;
   IF_EXEC   x_curr = e_head;
   EL_PROC   x_curr = p_head;
   EL_TIES   x_curr = t_head;
   EL_LIBS   x_curr = l_head;
   EL_DATA   x_curr = d_head;
   EL_ALLS   x_curr = p_head;
   ELSE {
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   while (x_curr != NULL) {
      IF_EXEC {
         EXEC_free (&x_curr);
         x_curr = e_head;
      } EL_PROC {
         PROC_free (&x_curr);
         x_curr = p_head;
      } EL_TIES {
         TIES_free (&x_curr);
         x_curr = t_head;
      } EL_LIBS {
         LIBS_free (&x_curr);
         x_curr = l_head;
      } EL_DATA {
         DATA_free (&x_curr);
         x_curr = d_head;
      } EL_ALLS {
         PROC_unhook (&x_curr);
         x_curr = p_head;
      }
   }
   /*---(ground everything)--------------*/
   IF_EXEC {
      e_head   = e_tail   = e_curr   = NULL;
      e_count  = 0;
   } EL_PROC {
      p_head   = p_tail   = p_curr   = NULL;
      p_count  = 0;
   } EL_TIES {
      t_head   = t_tail   = t_curr   = NULL;
      t_count  = 0;
   } EL_LIBS {
      l_head   = l_tail   = l_curr   = NULL;
      l_count  = 0;
   } EL_DATA {
      d_head   = d_tail   = d_curr   = NULL;
      d_count  = 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       mass data changes                      ----===*/
/*====================------------------------------------====================*/
static void  o___FIND____________o () { return; }

char
SHARE_by_cursor         (char a_type, void **r_curr, char a_move)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   tDATA      *x_data      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   DEBUG_NORM   yLOG_schar   (a_move);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(starting point)-----------------*/
   --rce;
   IF_EXEC  x_curr = x_exec = e_curr;
   EL_PROC  x_curr = x_proc = p_curr;
   EL_TIES  x_curr = x_ties = t_curr;
   EL_LIBS  x_curr = x_libs = l_curr;
   EL_DATA  x_curr = x_data = d_curr;
   ELSE {
      DEBUG_NORM    yLOG_snote   ("unknown type");
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DREL, a_move) != NULL) {
         IF_EXEC  e_curr = x_exec;
         EL_PROC  p_curr = x_proc;
         EL_TIES  t_curr = x_ties;
         EL_LIBS  l_curr = x_libs;
         EL_DATA  d_curr = x_data;
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(bounce types)----------------*/
      IF_EXEC  x_curr = x_exec = e_head;
      EL_PROC  x_curr = x_proc = p_head;
      EL_TIES  x_curr = x_ties = t_head;
      EL_LIBS  x_curr = x_libs = l_head;
      EL_DATA  x_curr = x_data = d_head;
      DEBUG_NORM    yLOG_spoint  (x_curr);
      if (x_curr == NULL) {
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(switch)-------------------------*/
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :
      IF_EXEC  x_curr = x_exec = e_head;
      EL_PROC  x_curr = x_proc = p_head;
      EL_TIES  x_curr = x_ties = t_head;
      EL_LIBS  x_curr = x_libs = l_head;
      EL_DATA  x_curr = x_data = d_head;
      break;
   case YDLST_PREV : case YDLST_DPREV :
      IF_EXEC  x_curr = x_exec = x_exec->m_prev;
      EL_PROC  x_curr = x_proc = x_proc->m_prev;
      EL_TIES  x_curr = x_ties = x_ties->m_prev;
      EL_LIBS  x_curr = x_libs = x_libs->m_prev;
      EL_DATA  x_curr = x_data = x_data->m_prev;
      break;
   case YDLST_CURR : case YDLST_DCURR :
      break;
   case YDLST_NEXT : case YDLST_DNEXT :
      IF_EXEC  x_curr = x_exec = x_exec->m_next;
      EL_PROC  x_curr = x_proc = x_proc->m_next;
      EL_TIES  x_curr = x_ties = x_ties->m_next;
      EL_LIBS  x_curr = x_libs = x_libs->m_next;
      EL_DATA  x_curr = x_data = x_data->m_next;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :
      IF_EXEC  x_curr = x_exec = e_tail;
      EL_PROC  x_curr = x_proc = p_tail;
      EL_TIES  x_curr = x_ties = t_tail;
      EL_LIBS  x_curr = x_libs = l_tail;
      EL_DATA  x_curr = x_data = d_tail;
      break;
   default         :
      DEBUG_NORM   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_NORM   yLOG_spoint  (x_curr);
   /*---(check end)----------------------*/
   --rce;  if (x_curr == NULL) {
      /*---(bounce off ends)-------------*/
      if (a_move == YDLST_PREV) {
         IF_EXEC  x_curr = x_exec = e_head;
         EL_PROC  x_curr = x_proc = p_head;
         EL_TIES  x_curr = x_ties = t_head;
         EL_LIBS  x_curr = x_libs = l_head;
         EL_DATA  x_curr = x_data = d_head;
      }
      if (a_move == YDLST_NEXT) {
         IF_EXEC  x_curr = x_exec = e_tail;
         EL_PROC  x_curr = x_proc = p_tail;
         EL_TIES  x_curr = x_ties = t_tail;
         EL_LIBS  x_curr = x_libs = l_tail;
         EL_DATA  x_curr = x_data = d_tail;
      }
      /*---(no bounce)-------------------*/
      if (x_curr == NULL) {
         IF_EXEC  e_curr = x_exec;
         EL_PROC  p_curr = x_proc;
         EL_TIES  t_curr = x_ties;
         EL_LIBS  l_curr = x_libs;
         EL_DATA  l_curr = x_data;
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(mark trouble)----------------*/
      DEBUG_NORM    yLOG_snote   ("BOUNCE");
      rc = rce;
      /*---(done)------------------------*/
   }
   /*---(normal result)------------------*/
   IF_EXEC  x_curr = e_curr = x_exec;
   EL_PROC  x_curr = p_curr = x_proc;
   EL_TIES  x_curr = t_curr = x_ties;
   EL_LIBS  x_curr = l_curr = x_libs;
   EL_DATA  x_curr = d_curr = x_data;
   /*---(save back)----------------------*/
   if (r_curr != NULL)  *r_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
SHARE_cursor_by_owner  (char a_type, void **r_curr, void *a_owner, char a_move)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   DEBUG_NORM   yLOG_schar   (a_move);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_spoint  (a_owner);
   --rce;  if (a_owner == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(starting point)-----------------*/
   --rce;
   IF_EXEC {
      x_exec = (tEXEC *) a_owner;
      if   (p_curr == NULL)               x_curr = x_proc = x_exec->p_head;
      else if (p_curr->e_link != x_exec)  x_curr = x_proc = x_exec->p_head;
      else                                x_curr = x_proc = p_curr;
   } EL_PROC {
      x_proc = (tPROC *) a_owner;
      if   (t_curr == NULL)               x_curr = x_ties = x_proc->t_head;
      else if (t_curr->p_link != x_proc)  x_curr = x_ties = x_proc->t_head;
      else                                x_curr = x_ties = t_curr;
   } EL_LIBS {
      x_libs = (tLIBS *) a_owner;
      if   (t_curr == NULL)               x_curr = x_ties = x_libs->t_head;
      else if (t_curr->l_link != x_libs)  x_curr = x_ties = x_libs->t_head;
      else                                x_curr = x_ties = t_curr;
   } ELSE {
      DEBUG_NORM    yLOG_snote   ("unknown type");
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DREL, a_move) != NULL) {
         IF_EXEC  p_curr = x_curr;
         EL_PROC  t_curr = x_curr;
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(bounce types)----------------*/
      IF_EXEC   p_curr = x_exec->p_head;
      EL_PROC   t_curr = x_proc->t_head;
      EL_LIBS   t_curr = x_libs->t_head;
      DEBUG_NORM    yLOG_spoint  (x_curr);
      if (x_curr == NULL) {
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(switch)-------------------------*/
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :
      IF_EXEC  x_curr = x_proc = x_exec->p_head;
      EL_PROC  x_curr = x_ties = x_proc->t_head;
      EL_LIBS  x_curr = x_ties = x_libs->t_head;
      break;
   case YDLST_PREV : case YDLST_DPREV :
      IF_EXEC  x_curr = x_proc = x_proc->p_prev;
      EL_PROC  x_curr = x_ties = x_ties->p_prev;
      EL_LIBS  x_curr = x_ties = x_ties->l_prev;
      break;
   case YDLST_CURR : case YDLST_DCURR :
      break;
   case YDLST_NEXT : case YDLST_DNEXT :
      IF_EXEC  x_curr = x_proc = x_proc->p_next;
      EL_PROC  x_curr = x_ties = x_ties->p_next;
      EL_LIBS  x_curr = x_ties = x_ties->l_next;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :
      IF_EXEC  x_curr = x_proc = x_exec->p_tail;
      EL_PROC  x_curr = x_ties = x_proc->t_tail;
      EL_LIBS  x_curr = x_ties = x_libs->t_tail;
      break;
   default         :
      DEBUG_NORM   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_NORM   yLOG_spoint  (x_curr);
   /*---(check end)----------------------*/
   --rce;  if (x_curr == NULL) {
      /*---(bounce off ends)-------------*/
      if (a_move == YDLST_PREV) {
         IF_EXEC  x_curr = x_proc = x_exec->p_head;
         EL_PROC  x_curr = x_ties = x_proc->t_head;
         EL_LIBS  x_curr = x_ties = x_libs->t_head;
      }
      if (a_move == YDLST_NEXT) {
         IF_EXEC  x_curr = x_proc = x_exec->p_tail;
         EL_PROC  x_curr = x_ties = x_proc->t_tail;
         EL_LIBS  x_curr = x_ties = x_libs->t_tail;
      }
      /*---(no bounce)-------------------*/
      if (x_curr == NULL) {
         IF_EXEC  p_curr = x_curr;
         EL_PROC  t_curr = x_curr;
         EL_LIBS  t_curr = x_curr;
         DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(mark trouble)----------------*/
      DEBUG_NORM    yLOG_snote   ("BOUNCE");
      rc = rce;
      /*---(done)------------------------*/
   }
   /*---(normal result)------------------*/
   IF_EXEC  p_curr = x_curr;
   EL_PROC  t_curr = x_curr;
   EL_LIBS  t_curr = x_curr;
   /*---(save back)----------------------*/
   if (r_curr != NULL)  *r_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
SHARE_by_index          (char a_type, void **r_curr, int a_index)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIES      *x_ties      = NULL;
   tLIBS      *x_libs      = NULL;
   tDATA      *x_data      = NULL;
   void       *x_curr      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   DEBUG_DATA   yLOG_sint    (a_index);
   /*---(default)------------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(short-cut)----------------------*/
   --rce;  if (a_index == -1) {
      DEBUG_DATA   yLOG_snote   ("requested current");
      IF_EXEC  x_curr = e_curr;
      EL_PROC  x_curr = p_curr;
      EL_TIES  x_curr = t_curr;
      EL_LIBS  x_curr = l_curr;
      EL_DATA  x_curr = d_curr;
      if (r_curr != NULL)  *r_curr = x_curr;
      DEBUG_DATA   yLOG_spoint  (x_curr);
      --rce;  if (x_curr == NULL) {
         DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   --rce;
   IF_EXEC   x_curr = x_exec = e_head;
   EL_PROC   x_curr = x_proc = p_head;
   EL_TIES   x_curr = x_ties = t_head;
   EL_LIBS   x_curr = x_libs = l_head;
   EL_DATA   x_curr = x_data = d_head;
   ELSE {
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_sint    (a_index);
   --rce;  if (a_index < -1) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk data)----------------------*/
   while (x_curr != NULL) {
      if (c == a_index)  break;
      ++c;
      IF_EXEC     x_curr = x_exec = x_exec->m_next;
      EL_PROC     x_curr = x_proc = x_proc->m_next;
      EL_TIES     x_curr = x_ties = x_ties->m_next;
      EL_LIBS     x_curr = x_libs = x_libs->m_next;
      EL_DATA     x_curr = x_data = x_data->m_next;
   }
   /*---(check result)-------------------*/
   DEBUG_DATA   yLOG_sint    (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save current)-------------------*/
   IF_EXEC     e_curr = x_exec;
   EL_PROC     p_curr = x_proc;
   EL_TIES     t_curr = x_ties;
   EL_LIBS     l_curr = x_libs;
   EL_DATA     d_curr = x_data;
   if (r_curr != NULL)  *r_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
SHARE_by_inode          (char a_type, void **r_curr, int a_inode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_inode     =    0;
   tEXEC      *x_exec      = NULL;
   tLIBS      *x_libs      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   DEBUG_DATA   yLOG_sint    (a_inode);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(prepare)------------------------*/
   --rce;
   IF_EXEC   x_curr = x_exec = e_head;
   EL_LIBS   x_curr = x_libs = l_head;
   ELSE {
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   while (x_curr != NULL) {
      rc = 0;
      IF_EXEC   x_inode = x_exec->inode;
      EL_LIBS   x_inode = x_libs->inode;
      if (x_inode == a_inode) {
         IF_EXEC  e_curr = x_exec;
         EL_LIBS  l_curr = x_libs;
         break;
      }
      IF_EXEC   x_curr = x_exec = x_exec->m_next;
      EL_LIBS   x_curr = x_libs = x_libs->m_next;
   }
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (r_curr != NULL)  *r_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
SHARE_by_name           (char a_type, void **r_curr, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *x_name      = NULL;
   tEXEC      *x_exec      = NULL;
   tLIBS      *x_libs      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_NORM   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   DEBUG_DATA   yLOG_snote   (a_name);
   /*---(defaults)-----------------------*/
   if (r_curr != NULL)  *r_curr = NULL;
   /*---(prepare)------------------------*/
   --rce;
   IF_EXEC   x_curr = x_exec = e_head;
   EL_LIBS   x_curr = x_libs = l_head;
   ELSE {
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   while (x_curr != NULL) {
      rc = 0;
      IF_EXEC   x_name  = x_exec->base;
      EL_LIBS   x_name  = x_libs->terse;
      if (strcmp (x_name, a_name) == 0) {
         IF_EXEC  e_curr = x_exec;
         EL_LIBS  l_curr = x_libs;
         break;
      }
      IF_EXEC   x_curr = x_exec = x_exec->m_next;
      EL_LIBS   x_curr = x_libs = x_libs->m_next;
   }
   /*---(defense)------------------------*/
   DEBUG_NORM   yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      DEBUG_NORM    yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (r_curr != NULL)  *r_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_NORM   yLOG_sexit   (__FUNCTION__);
   return 0;
}



