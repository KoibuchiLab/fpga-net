/* Copyright (c) 2014-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SURF_ROUTING_CLUSTER_OPT_ELEC_SIMPLE_HPP_
#define SURF_ROUTING_CLUSTER_OPT_ELEC_SIMPLE_HPP_

#include <simgrid/kernel/routing/ClusterZone.hpp>
#include <simgrid/s4u/Link.hpp>
#include "xbt/config.h"
#include <xbt/config.hpp>

namespace simgrid {
namespace kernel {
namespace routing {

class OptElecRouter {
public:
  int routerId;
  double init_lat = 0;
  double swt_lat = 0;
  resource::LinkImpl** up_links_  = nullptr;
  resource::LinkImpl** down_links_ = nullptr;
  OptElecRouter(int routerId, int isOpticMode);
  /**
  Optical switch use circuit switching. Everytime change the path, it need configuring again.
  Time for configure is init_lat;
  **/
  bool is_configuration_required(int soureId, int destId);
  double config_new_path(int soureId, int destId); // return the configuartion latency;
  ~OptElecRouter();
private:
  int currentSourceId = 0;
  int currentDestId = 0;  
  bool isOpticMode_;
};

/** @ingroup ROUTING_API
 * @brief NetZone using a Optical-Electrical Hibrid switch system (a simple tree topology)
 *
 * Generate topology with n compute node per router (ToR switch).
 * All the ToR switches connect to the 2 top-level switches. 
 * One electrical switch and one optical switch.
 * 
 * Routing is selected based on the message size. 
 * For huge flows: using optical interconnect.
 * For small flows: using electrical interconnect.
 *
 * An additional latency is needed for setup the optical links.
 * 
 * LIMITATIONS (for now):
 *  - Routing for is only static and uses minimal routes.
 *  - When n links are used between two routers/groups, we consider only one link with n times the bandwidth (needs to be validated on a real system)
 *  - All links have the same characteristics for now
 * */
class XBT_PUBLIC ClusterOptElecSimpleZone : public ClusterZone {
public:
  explicit ClusterOptElecSimpleZone(NetZoneImpl* father, std::string name, resource::NetworkModel* netmodel);
  ~ClusterOptElecSimpleZone() override;
  //      void create_links_for_node(sg_platf_cluster_cbarg_t cluster, int id, int rank, int position) override;
  void get_local_route(NetPoint* src, NetPoint* dst,double size, RouteCreationArgs* into, double* latency) override;
  void parse_specific_arguments(ClusterCreationArgs* cluster) override;
  void seal() override;

  void rankId_to_coords(int rank_id, unsigned int coords[2]);

private:
  void generate_routers();
  void generate_links();
  void create_electrical_link(const std::string& id, int numlinks, resource::LinkImpl** linkup, resource::LinkImpl** linkdown);
  void create_optical_link(const std::string& id, int numlinks, resource::LinkImpl** linkup, resource::LinkImpl** linkdown);
  void create_link(const std::string& id, int numlinks, resource::LinkImpl** linkup, resource::LinkImpl** linkdown);

  simgrid::s4u::Link::SharingPolicy sharing_policy_;
  double bw_  = 0;
  double lat_ = 0;
  double swt_lat_ = 0;
  
  double elec_bw_  = 0;
  double elec_lat_ = 0;
  double elec_swt_lat_ = 0;
  
  double opt_bw_  = 0;
  double opt_lat_ = 0;
  double init_lat_ = 0;
  double opt_swt_lat_ = 0;
  
  OptElecRouter* opticalRouter_  = nullptr;
  OptElecRouter* electricalRouter_  = nullptr;
  OptElecRouter** routers_        = nullptr;
  
	unsigned int num_router_             = 0;
	unsigned int node_per_router_         = 0;
	unsigned int num_node_         = 0;
};
} // namespace routing
} // namespace kernel
} // namespace simgrid
#endif
