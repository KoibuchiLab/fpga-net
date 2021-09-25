/* Copyright (c) 2010-2021. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

/* This source code simply loads the platform. This is only useful to play
 * with the tracing module. See the tesh file to see how to generate the
 * traces.
 */

#include "simgrid/instr.h"
#include "simgrid/s4u.hpp"
#include <memory>

namespace sg4 = simgrid::s4u;

/* The guy we will move from host to host. It move alone and then is moved by policeman back  */
static void emigrant()
{
  auto mailbox = sg4::Mailbox::by_name("master_mailbox");

  sg4::this_actor::sleep_for(2);

  while (true) { // I am an eternal emigrant
    auto destination = mailbox->get_unique<std::string>();
    if (destination->empty())
      break; // there is no destination, die
    sg4::this_actor::set_host(sg4::Host::by_name(*destination));
    sg4::this_actor::sleep_for(2); // I am tired, have to sleep for 2 seconds
  }
}

static void policeman()
{
  // I am the master of emigrant actor,
  // I tell it where it must emigrate to.
  auto destinations = {"Tremblay", "Jupiter", "Fafard", "Ginette", "Bourassa", "Fafard", "Tremblay", "Ginette", ""};
  auto mailbox      = sg4::Mailbox::by_name("master_mailbox");

  for (auto const& destination : destinations) {
    mailbox->put_init(new std::string(destination), 0)->set_tracing_category("migration_order")->wait();
  }
}

int main(int argc, char* argv[])
{
  sg4::Engine e(&argc, argv);
  xbt_assert(argc > 1, "Usage: %s platform_file\n \tExample: %s small_platform.xml\n", argv[0], argv[0]);

  e.load_platform(argv[1]);

  TRACE_category("migration_order");

  sg4::Actor::create("emigrant", sg4::Host::by_name("Fafard"), emigrant);
  sg4::Actor::create("policeman", sg4::Host::by_name("Tremblay"), policeman);

  e.run();
  return 0;
}
