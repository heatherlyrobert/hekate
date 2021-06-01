/*============================[[    beg-code    ]]============================*/
#include   "hekate.h"



char
DATA_exename            (char *a_file, char *a_base, char *a_full)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   char       *p           = NULL;
   char       *q           = NULL;
   char       *r           = NULL;
   int        i            =    0;
   int        l            =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_base != NULL)  strlcpy (a_base, "", LEN_TITLE);
   if (a_full != NULL)  strlcpy (a_full, "", LEN_RECD);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get first line)-----------------*/
   fgets (x_recd, LEN_RECD, f);
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   /*---(open proc)----------------------*/
   p = strtok_r (x_recd, " \t", &r);
   --rce;  for (i = 0; i < 5; ++i) {
      p = strtok_r (NULL, " \t", &r);
      if (p == NULL) {
         DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save back)----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_full"    , p);
   if (a_full != NULL)  strlcpy (a_full, p, LEN_RECD);
   /*---(parse name)---------------------*/
   q = strrchr (p, '/');
   if (q != NULL) ++q;
   else           q = p;
   DEBUG_ENVI   yLOG_info    ("a_base"    , q);
   if (a_base != NULL)  strlcpy (a_base, q, LEN_TITLE);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA_pubname            (char *a_file, char *a_public)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         l           =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_public != NULL)  strlcpy (a_public, "", LEN_TITLE);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(saveback)----------------------*/
   if (a_public != NULL)  strlcpy (a_public, x_recd, LEN_TITLE);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
DATA_cmdline            (char *a_file, char *a_cmdline)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   /*---(header)------------------------*/
   DEBUG_ENVI   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (a_cmdline != NULL)  strlcpy (a_cmdline, "", LEN_RECD);
   /*---(defense)-----------------------*/
   DEBUG_ENVI   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ENVI   yLOG_info    ("a_file"    , a_file);
   /*---(open proc)----------------------*/
   f = fopen (a_file, "rt");
   DEBUG_ENVI   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   while (1) {
      if (x_recd [i] == '\0' && x_recd [i + 1] == '\0')  break;
      if (x_recd [i] == '\0')  x_recd [i] = ' ';
      ++i;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_ENVI   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(saveback)----------------------*/
   if (a_cmdline != NULL)  strlcpy (a_cmdline, x_recd, LEN_RECD);
   /*---(complete)----------------------*/
   DEBUG_ENVI   yLOG_exit    (__FUNCTION__);
   return 0;
}


