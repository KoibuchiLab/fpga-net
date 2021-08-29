/*******************************/
/* GENERATED FILE, DO NOT EDIT */
/*******************************/

#include <stdio.h>
#include "xbt.h"
/*******************************/
/* GENERATED FILE, DO NOT EDIT */
/*******************************/

#line 720 "xbt/config.cpp" 

#include <string>

#include "simgrid/Exception.hpp"
#include "xbt.h"
#include "xbt/ex.h"

#include <xbt/config.hpp>

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(xbt_cfg);


XBT_PUBLIC_DATA simgrid::config::Config* simgrid_config;

static void make_set()
{
  simgrid_config = nullptr;
  xbt_log_threshold_set(&_XBT_LOGV(xbt_cfg), xbt_log_priority_critical);
  simgrid::config::declare_flag<int>("speed", "description", 0);
  simgrid::config::declare_flag<std::string>("peername", "description", "");
  simgrid::config::declare_flag<std::string>("user", "description", "");
}                               /* end_of_make_set */

XBT_TEST_UNIT("memuse", test_config_memuse, "Alloc and free a config set")
{
  auto temp = simgrid_config;
  make_set();
  xbt_test_add("Alloc and free a config set");
  simgrid::config::set_parse("peername:veloce user:bidule");
  simgrid::config::finalize();
  simgrid_config = temp;
}

XBT_TEST_UNIT("use", test_config_use, "Data retrieving tests")
{
  auto temp = simgrid_config;
  make_set();
  xbt_test_add("Get a single value");
  {
    /* get_single_value */
    simgrid::config::set_parse("peername:toto:42 speed:42");
    int ival = simgrid::config::get_value<int>("speed");
    if (ival != 42)
      xbt_test_fail("Speed value = %d, I expected 42", ival);
  }

  xbt_test_add("Access to a non-existant entry");
  {
    try {
      simgrid::config::set_parse("color:blue");
    } catch(xbt_ex& e) {
      if (e.category != not_found_error)
        xbt_test_exception(e);
    }
  }
  simgrid::config::finalize();
  simgrid_config = temp;
}

XBT_TEST_UNIT("c++flags", test_config_cxx_flags, "C++ flags")
{
  auto temp = simgrid_config;
  make_set();
  xbt_test_add("C++ declaration of flags");

  simgrid::config::Flag<int> int_flag("int", "", 0);
  simgrid::config::Flag<std::string> string_flag("string", "", "foo");
  simgrid::config::Flag<double> double_flag("double", "", 0.32);
  simgrid::config::Flag<bool> bool_flag1("bool1", "", false);
  simgrid::config::Flag<bool> bool_flag2("bool2", "", true);

  xbt_test_add("Parse values");
  simgrid::config::set_parse("int:42 string:bar double:8.0 bool1:true bool2:false");
  xbt_test_assert(int_flag == 42, "Check int flag");
  xbt_test_assert(string_flag == "bar", "Check string flag");
  xbt_test_assert(double_flag == 8.0, "Check double flag");
  xbt_test_assert(bool_flag1, "Check bool1 flag");
  xbt_test_assert(not bool_flag2, "Check bool2 flag");

  simgrid::config::finalize();
  simgrid_config = temp;
}

/*******************************/
/* GENERATED FILE, DO NOT EDIT */
/*******************************/

