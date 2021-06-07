

char
main     (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(startup)------------------------*/
   rc = PROG_startup (a_argc, a_argv);
   if (rc <  0) {
      PROG_shutdown ();
      return -1;
   }
   /*---(run)----------------------------*/
   PROG_review ();
   YVIKEYS_update ();
   /*> NCURSE_proc_list ();                                                           <*/
   PROG_driver ();
   /*---(shutdown)-----------------------*/
   PROG_shutdown ();
   /*---(complete)-----------------------*/
   return 0;
}

