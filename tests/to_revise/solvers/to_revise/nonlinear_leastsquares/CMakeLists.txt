TRIBITS_ADD_EXECUTABLE_AND_TEST(
  nonlinear_leastsquares_iterative1
  SOURCES
    main.cc
  NUM_MPI_PROCS 1
  # STANDARD_PASS_OUTPUT
  )
