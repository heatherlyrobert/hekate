/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



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
   tTIES      *x_tie       = NULL;
   tLIBS      *x_lib       = NULL;
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
   --rce;  switch (a_type) {
   case 'E' :
      x_size = sizeof (tEXEC);
      break;
   case 'P' :
      x_size = sizeof (tPROC);
      break;
   case 'T' :
      x_size = sizeof (tTIES);
      break;
   case 'L' :
      x_size = sizeof (tLIBS);
      break;
   default  :
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
   --rce;  switch (a_type) {
   case 'E' :
      x_exec = (tEXEC *) x_new;
      EXEC_wipe (x_exec, '*');
      if    (e_tail == NULL) { e_head = x_exec; }
      else { x_exec->m_prev  = e_tail; e_tail->m_next = x_exec; }
      e_tail = x_exec;
      ++e_count;
      break;
   case 'P' :
      x_proc = (tPROC *) x_new;
      PROC_wipe (x_proc, '*');
      if    (p_tail == NULL) { p_head = x_proc; }
      else { x_proc->m_prev  = p_tail; p_tail->m_next = x_proc; }
      p_tail = x_proc;
      ++p_count;
      break;
   case 'T' :
      x_tie  = (tTIES *) x_new;
      TIES_wipe (x_tie, '*');
      if    (t_tail == NULL) { t_head = x_tie; }
      else { x_tie->m_prev  = t_tail; t_tail->m_next = x_tie; }
      t_tail = x_tie;
      ++t_count;
      break;
   case 'L' :
      x_lib  = (tLIBS *) x_new;
      LIBS_wipe (x_lib, '*');
      if    (l_tail == NULL) { l_head = x_lib; }
      else { x_lib->m_prev  = l_tail; l_tail->m_next = x_lib; }
      l_tail = x_lib;
      ++l_count;
      break;
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
   tTIES      *x_tie       = NULL;
   tLIBS      *x_lib       = NULL;
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
   --rce;  switch (a_type) {
   case 'E' :
      x_exec = (tEXEC *) *a_old;
      if (x_exec->m_next != NULL) x_exec->m_next->m_prev   = x_exec->m_prev;
      else                        e_tail                   = x_exec->m_prev;
      if (x_exec->m_prev != NULL) x_exec->m_prev->m_next   = x_exec->m_next;
      else                        e_head                   = x_exec->m_next;
      --e_count;
      break;
   case 'P' :
      x_proc = (tPROC *) *a_old;
      if (x_proc->m_next != NULL) x_proc->m_next->m_prev   = x_proc->m_prev;
      else                        p_tail                   = x_proc->m_prev;
      if (x_proc->m_prev != NULL) x_proc->m_prev->m_next   = x_proc->m_next;
      else                        p_head                   = x_proc->m_next;
      --p_count;
      break;
   case 'T' :
      x_tie  = (tTIES *) *a_old;
      if (x_tie->m_next != NULL)  x_tie->m_next->m_prev    = x_tie->m_prev;
      else                        t_tail                   = x_tie->m_prev;
      if (x_tie->m_prev != NULL)  x_tie->m_prev->m_next    = x_tie->m_next;
      else                        t_head                   = x_tie->m_next;
      --t_count;
      break;
   case 'L' :
      x_lib  = (tLIBS *) *a_old;
      if (x_lib->m_next != NULL)  x_lib->m_next->m_prev    = x_lib->m_prev;
      else                        l_tail                   = x_lib->m_prev;
      if (x_lib->m_prev != NULL)  x_lib->m_prev->m_next    = x_lib->m_next;
      else                        l_head                   = x_lib->m_next;
      --l_count;
      break;
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
   --rce;  switch (a_type) {
   case 'E' :
      x_curr = e_head;
      break;
   case 'P' : case 'A' :
      x_curr = p_head;
      break;
   case 'T' :
      x_curr = t_head;
      break;
   case 'L' :
      x_curr = l_head;
      break;
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   while (x_curr != NULL) {
      switch (a_type) {
      case 'E' :
         EXEC_free (&x_curr);
         x_curr = e_head;
         break;
      case 'P' :
         PROC_free (&x_curr);
         x_curr = p_head;
         break;
      case 'A' :
         PROC_unhook (&x_curr);
         x_curr = p_head;
         break;
      case 'T' :
         TIES_free (&x_curr);
         x_curr = t_head;
         break;
      case 'L' :
         LIBS_free (&x_curr);
         x_curr = l_head;
         break;
      }
   }
   /*---(ground everything)--------------*/
   switch (a_type) {
   case 'E' :
      e_head   = e_tail   = e_curr   = NULL;
      e_count  = 0;
      break;
   case 'P' :
      p_head   = p_tail   = p_curr   = NULL;
      p_count  = 0;
      break;
   case 'T' :
      t_head   = t_tail   = t_curr   = NULL;
      t_count  = 0;
      break;
   case 'L' :
      l_head   = l_tail   = l_curr   = NULL;
      l_count  = 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       mass data changes                      ----===*/
/*====================------------------------------------====================*/
static void  o___FIND____________o () { return; }

#define  IF_EXEC   if      (a_type == 'E')
#define  EL_PROC   else if (a_type == 'P')
#define  EL_TIE    else if (a_type == 'T')
#define  EL_LIB    else                   

char
SHARE_by_cursor         (char a_type, char a_move, void **a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *e_temp      = NULL;
   tPROC      *p_temp      = NULL;
   tTIES      *t_temp      = NULL;
   tLIBS      *l_temp      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_curr != NULL)  *a_curr = NULL;
   IF_EXEC  x_curr = e_temp = e_curr;
   EL_PROC  x_curr = p_temp = p_curr;
   EL_TIE   x_curr = t_temp = t_curr;
   EL_LIB   x_curr = l_temp = l_curr;
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DREL, a_move) != NULL) {
         IF_EXEC  e_curr = e_temp;
         EL_PROC  p_curr = p_temp;
         EL_TIE   t_curr = t_temp;
         EL_LIB   l_curr = l_temp;
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(bounce types)----------------*/
      IF_EXEC  x_curr = e_temp = e_head;
      EL_PROC  x_curr = p_temp = p_head;
      EL_TIE   x_curr = t_temp = t_head;
      EL_LIB   x_curr = l_temp = l_head;
      DEBUG_YEXEC   yLOG_spoint  (x_curr);
      if (x_curr == NULL) {
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(switch)-------------------------*/
   DEBUG_YEXEC  yLOG_schar   (a_move);
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :
      IF_EXEC  x_curr = e_temp = e_head;
      EL_PROC  x_curr = p_temp = p_head;
      EL_TIE   x_curr = t_temp = t_head;
      EL_LIB   x_curr = l_temp = l_head;
      break;
   case YDLST_PREV : case YDLST_DPREV :
      IF_EXEC  x_curr = e_temp = e_temp->m_prev;
      EL_PROC  x_curr = p_temp = p_temp->m_prev;
      EL_TIE   x_curr = t_temp = t_temp->m_prev;
      EL_LIB   x_curr = l_temp = l_temp->m_prev;
      break;
   case YDLST_CURR : case YDLST_DCURR :
      break;
   case YDLST_NEXT : case YDLST_DNEXT :
      IF_EXEC  x_curr = e_temp = e_temp->m_next;
      EL_PROC  x_curr = p_temp = p_temp->m_next;
      EL_TIE   x_curr = t_temp = t_temp->m_next;
      EL_LIB   x_curr = l_temp = l_temp->m_next;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :
      IF_EXEC  x_curr = e_temp = e_tail;
      EL_PROC  x_curr = p_temp = p_tail;
      EL_TIE   x_curr = t_temp = t_tail;
      EL_LIB   x_curr = l_temp = l_tail;
      break;
   default         :
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (x_curr);
   /*---(check end)----------------------*/
   --rce;  if (x_curr == NULL) {
      /*---(bounce off ends)-------------*/
      if (a_move == YDLST_PREV) {
         IF_EXEC  x_curr = e_temp = e_head;
         EL_PROC  x_curr = p_temp = p_head;
         EL_TIE   x_curr = t_temp = t_head;
         EL_LIB   x_curr = l_temp = l_head;
      }
      if (a_move == YDLST_NEXT) {
         IF_EXEC  x_curr = e_temp = e_tail;
         EL_PROC  x_curr = p_temp = p_tail;
         EL_TIE   x_curr = t_temp = t_tail;
         EL_LIB   x_curr = l_temp = l_tail;
      }
      /*---(no bounce)-------------------*/
      if (x_curr == NULL) {
         IF_EXEC  e_curr = e_temp;
         EL_PROC  p_curr = p_temp;
         EL_TIE   t_curr = t_temp;
         EL_LIB   l_curr = l_temp;
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(mark trouble)----------------*/
      DEBUG_YEXEC   yLOG_snote   ("BOUNCE");
      rc = rce;
      /*---(done)------------------------*/
   }
   /*---(normal result)------------------*/
   IF_EXEC  x_curr = e_curr = e_temp;
   EL_PROC  x_curr = p_curr = p_temp;
   EL_TIE   x_curr = t_curr = t_temp;
   EL_LIB   x_curr = l_curr = l_temp;
   /*---(save back)----------------------*/
   if (a_curr != NULL)  *a_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}



