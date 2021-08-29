/* log_large_test -- log a very large string to test the dynamic variants   */

/* Copyright (c) 2007-2018. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "xbt.h"
#include <simgrid/msg.h>

XBT_LOG_NEW_DEFAULT_CATEGORY(test, "Logs of this example");

int main(int argc, char *argv[])
{
  MSG_init(&argc, argv);
  char *tmp = bprintf("\n%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s",
    1, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    2, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    3, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    4, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    5, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    6, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    7, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    8, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    9, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n",
    0, ".........1.........2.........3.........4.........5.........6.........7.........8.........9.........0\n");

  XBT_INFO("This is a very large message:\n0%s1%s2%s3%s4%s5%s6%s7%s8%s9%s",
         tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp, tmp);
  XBT_INFO("Done (strlen>%d)", (int) (10 * strlen(tmp)));
  free(tmp);

  return 0;
}
