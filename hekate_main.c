

char
main     (int a_argc, char *a_argv [])
{
   yURG_logger (a_argc, a_argv);
   yURG_urgs   (a_argc, a_argv);
   PROG_review ();
   DATA_treeify ();
   NCURSE_proc_list ();
   return 0;
}

