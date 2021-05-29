/*============================[[ beg-of-code ]]===============================*/



/*===[[ ONE_LINERS ]]=========================================================*/
/*-------   --12345678  "123456789-123456789-123456789-123456789-123456789-123456789-"  */

#define     P_FOCUS     "OS (kernel/operrating system)"
#define     P_NICHE     "pr (process anaylsis)"
#define     P_SUBJECT   "process and memory monitoring"
#define     P_PURPOSE   "deep dive on the process and memory execution environment"

#define     P_NAMESAKE  "hekate-triodia (queen of the crossroads)"
#define     P_PRONOUNCE "heck-uh-tea"
#define     P_HERITAGE  "greek titaness of boundaries, crossroads, witchcraft, and ghosts"
#define     P_IMAGERY   "triple goddess with three bodies facing three directions"
#define     P_REASON    "as the goddess of witchcraft, boundaries and the unseen"

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  "hekate"
#define     P_FULLPATH  "/usr/local/sbin/hekate"
#define     P_SUFFIX    "n/a"
#define     P_CONTENT   "n/a"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "ySTR"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2021-05"

#define     P_VERMAJOR  "0.--, pre-production"
#define     P_VERMINOR  "0.5-, bring development from yEXEC into a program"
#define     P_VERNUM    "0.5f"
#define     P_VERTXT    "unit tested funcy cursoring by parents (exec, libs, proc)"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*-------   --12345678  "123456789-123456789-123456789-123456789-123456789-123456789-"  */
/*===[[ END ONE_LINERS ]]=====================================================*/



/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include    <stdio.h>        /* CLIBC   standard input/output                 */
#include    <stdlib.h>       /* CLIBC   standard general purpose              */
#include    <string.h>       /* CLIBC   standard string handling              */
#include    <unistd.h>       /* CLIBC   standard unix system handling         */
#include    <math.h>         /* CLIBC   standard expanded mathematics         */
#include    <dirent.h>
#include    <sys/stat.h>


/*---(heatherly made)--------------------*/
#include    <yURG.h>         /* CUSTOM  heatherly urgent processing           */
#include    <yLOG.h>         /* CUSTOM  heatherly program logging             */
#include    <ySTR.h>         /* CUSTOM  heatherly string handling             */
#include    <yEXEC.h>        /* CUSTOM  heatherly process execution           */
#include    <yDLST_solo.h>   /* CUSTOM : heatherly list constants             */



typedef struct  cEXEC  tEXEC;
typedef struct  cPROC  tPROC;
typedef struct  cLIBS  tLIBS;
typedef struct  cTIES  tTIES;



static struct cEXEC {
   /*---(master)------------*/
   char        name        [LEN_TITLE];
   long        inode;
   /*---(memory)------------*/
   int         m_text;
   /*---(size)--------------*/
   int         s_total;
   int         s_text;
   int         s_data;
   int         s_bss;
   int         s_disk;
   /*---(master list)-------*/
   tEXEC      *m_prev;
   tEXEC      *m_next;
   /*---(processes)---------*/
   tPROC      *p_head;
   tPROC      *p_tail;
   int         p_count;
   /*---(done)--------------*/
};
extern tEXEC      *e_head;
extern tEXEC      *e_tail;
extern tEXEC      *e_curr;
extern int         e_count;



static struct cPROC {
   /*---(master)------------*/
   int         rpid;
   int         ppid;
   char        land;
   /*---(cpu)---------------*/
   char        c_state;
   long        c_utime;
   long        c_stime;
   char        c_snice;
   char        c_flag;
   /*---(memory)------------*/
   long        m_max;
   long        m_base;
   long        m_min;
   long        m_text;
   long        m_data;
   long        m_heap;
   long        m_stack;
   long        m_kern;
   long        m_libs;
   long        m_other;
   char        m_flag;
   /*---(disk)--------------*/
   long        d_read;
   long        d_write;
   char        d_flag;
   /*---(net)---------------*/
   long        n_read;
   long        n_write;
   char        n_flag;
   /*---(executable)--------*/
   tEXEC      *e_link;
   tPROC      *p_prev;
   tPROC      *p_next;
   /*---(master list)-------*/
   tPROC      *m_prev;
   tPROC      *m_next;
   /*---(libs)--------------*/
   tTIES      *t_head;
   tTIES      *t_tail;
   int         t_count;
   /*---(done)--------------*/
};
extern tPROC      *p_head;
extern tPROC      *p_tail;
extern tPROC      *p_curr;
extern int         p_count;



static struct cTIES {
   /*---(memory)------------*/
   long        m_data;
   long        m_heap;
   /*---(ties)--------------*/
   tTIES      *m_prev;
   tTIES      *m_next;
   /*---(proc link)---------*/
   tPROC      *p_link;
   tTIES      *p_prev;
   tTIES      *p_next;
   /*---(lib link)----------*/
   tLIBS      *l_link;
   tTIES      *l_prev;
   tTIES      *l_next;
   /*---(done)--------------*/
};
extern tTIES      *t_head;
extern tTIES      *t_tail;
extern tTIES      *t_curr;
extern int         t_count;



static struct cLIBS {
   /*---(master)------------*/
   char        terse       [LEN_TITLE];
   char        name        [LEN_TITLE];
   long        inode;
   /*---(memory)------------*/
   int         m_full;
   int         m_text;
   int         m_data;
   /*---(size)--------------*/
   int         s_total;
   int         s_text;
   int         s_data;
   int         s_bss;
   int         s_disk;
   /*---(procs)-------------*/
   tTIES      *t_head;
   tTIES      *t_tail;
   int         t_count;
   /*---(libs)--------------*/
   tLIBS      *m_prev;
   tLIBS      *m_next;
   /*---(done)--------------*/
};
extern tLIBS      *l_head;
extern tLIBS      *l_tail;
extern tLIBS      *l_curr;
extern int         l_count;




/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef     struct      dirent      tDIRENT;
typedef     struct      stat        tSTAT;




typedef  struct cMY  tMY;
struct cMY {
   /*---(runtime config)------*/
   char        version     [LEN_HUND];      /* version string                 */
   /*---(done)----------------*/
};
extern      tMY         my;
extern      char        unit_answer [LEN_RECD];


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(memory)---------------*/
char        PROG_purge              (void);
/*---(unittest)-------------*/
char        PROG__unit_quiet        (void);
char        PROG__unit_loud         (void);
char        PROG__unit_end          (void);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(memory)---------------*/
char        SHARE_new               (char a_type, void **a_new, char a_force);
char        SHARE_free              (char a_type, void **a_old);
char        SHARE_purge             (char a_type);
/*---(find)-----------------*/
char        SHARE_by_cursor         (char a_type, char a_move, void **a_curr);
char        SHARE_cursor_by_owner   (char a_type, void *a_owner, char a_move, void **a_curr);
char        SHARE_by_index          (char a_type, int a_index, void **a_curr);
char        SHARE_by_inode          (char a_type, int a_inode, void **a_curr);
char        SHARE_by_name           (char a_type, char *a_name, tEXEC **a_curr);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char*       EXEC__memory            (tEXEC *a_cur);
char        EXEC_wipe               (tEXEC *a_new, char a_type);
/*---(memory)---------------*/
char        EXEC_new                (void **a_new);
char        EXEC_force              (void **a_new);
char        EXEC_free               (void **a_old);
char        EXEC_purge              (void);
/*---(hooking)--------------*/
char        EXEC_hook               (tPROC *a_proc, char *a_name);
char        EXEC_unhook             (tPROC *a_proc);
char        EXEC_rehook             (tPROC *a_proc, char *a_name);
/*---(searching)------------*/
char        EXEC_by_cursor          (char a_move, tEXEC **a_curr);
char        EXEC_by_index           (int a_index, tEXEC **a_curr);
char        EXEC_by_inode           (int a_inode, tEXEC **a_curr);
char        EXEC_by_name            (char *a_name, tEXEC **a_curr);
/*---(unittest)-------------*/
char*       EXEC__unit              (char *a_question, int n);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char*       PROC__memory            (tPROC *a_cur);
char        PROC_wipe               (tPROC *a_new, char a_type);
/*---(memory)---------------*/
char        PROC_new                (void **a_new);
char        PROC_force              (void **a_new);
char        PROC_free               (void **a_old);
char        PROC_purge              (void);
/*---(hooking)--------------*/
char        PROC_hook               (tPROC **a_proc, int a_rpid);
char        PROC_unhook             (tPROC **a_proc);
/*---(searching)------------*/
char        PROC_by_cursor          (char a_move, tPROC **a_curr);
char        PROC_by_exec_cursor     (tEXEC *a_owner, char a_move, tPROC **a_curr);
char        PROC_by_index           (int a_index, tPROC **a_curr);
char        PROC_by_rpid            (int a_rpid, tPROC **a_curr);
/*---(unittest)-------------*/
char*       PROC__unit              (char *a_question, int n);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char*       TIES__memory            (tTIES *a_cur);
char        TIES_wipe               (tTIES *a_new, char a_type);
/*---(memory)---------------*/
char        TIES_new                (void **a_new);
char        TIES_force              (void **a_new);
char        TIES_free               (void **a_old);
char        TIES_purge              (void);
/*---(searching)------------*/
char        TIES_by_cursor          (char a_move, tTIES **a_curr);
char        TIES_by_exec_cursor     (tEXEC *a_owner, char a_move, tTIES **a_curr);
char        TIES_by_proc_cursor     (tPROC *a_owner, char a_move, tTIES **a_curr);
char        TIES_by_libs_cursor     (tLIBS *a_owner, char a_move, tTIES **a_curr);
char        TIES_by_index           (int a_index, tTIES **a_curr);
/*---(unittest)-------------*/
char*       TIES__unit              (char *a_question, int n);
/*---(done)-----------------*/



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char*       LIBS__memory            (tLIBS *a_cur);
char        LIBS_wipe               (tLIBS *a_new, char a_type);
/*---(memory)---------------*/
char        LIBS_new                (void **a_new);
char        LIBS_force              (void **a_new);
char        LIBS_free               (void **a_old);
char        LIBS_purge              (void);
/*---(hooking)--------------*/
char        LIBS_hook               (tPROC *a_proc, char *a_name);
char        LIBS_unhook             (tPROC *a_proc);
/*---(searching)------------*/
char        LIBS_by_cursor          (char a_move, tLIBS **a_curr);
char        LIBS_by_index           (int a_index, tLIBS **a_curr);
char        LIBS_by_inode           (int a_inode, tLIBS **a_curr);
char        LIBS_by_name            (char *a_name, tLIBS **a_curr);
/*---(unittest)-------------*/
char*       LIBS__unit              (char *a_question, int n);
/*---(done)-----------------*/



