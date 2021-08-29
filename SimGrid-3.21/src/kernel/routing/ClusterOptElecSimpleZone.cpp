/* Copyright (c) 2014-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "simgrid/kernel/routing/ClusterOptElecSimpleZone.hpp"
#include "simgrid/kernel/routing/NetPoint.hpp"
#include "src/surf/network_interface.hpp"
#include "src/surf/xml/platf_private.hpp"
#include "src/surf/xml/platf.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string>
#include <math.h>

XBT_LOG_NEW_DEFAULT_SUBCATEGORY(surf_route_cluster_optelecsimple, surf_route_cluster, "Optical-Electrical Hibrid switch system Routing part of surf");

namespace simgrid {
namespace kernel {
namespace routing {

ClusterOptElecSimpleZone::ClusterOptElecSimpleZone(NetZoneImpl* father, std::string name, resource::NetworkModel* netmodel)
    : ClusterZone(father, name, netmodel)
{
}

ClusterOptElecSimpleZone::~ClusterOptElecSimpleZone()
{	
  if (this->routers_ != nullptr) {
    for (unsigned int i = 0; i < this->num_router_; i++)
      delete routers_[i];
    delete[] routers_;
  }
  
  if (this->opticalRouter_ != nullptr){
	  delete this->opticalRouter_;
  }
  
  if (this->electricalRouter_ != nullptr){
	  delete this->electricalRouter_;
  }
}

void ClusterOptElecSimpleZone::rankId_to_coords(int rankId, unsigned int coords[2])
{
  // coords : routers, node
  coords[0] = rankId / node_per_router_;
  coords[1] = rankId % node_per_router_;
}

void ClusterOptElecSimpleZone::parse_specific_arguments(ClusterCreationArgs* cluster)
{
  std::vector<std::string> parameters;
  std::vector<std::string> tmp;
  boost::split(parameters, cluster->topo_parameters, boost::is_any_of("|"));
//Parameter format: Node per TOR|TOR|ELEC|TOR-OPT
  //TOR: TOR switch latency, {node-TOR link latency},{node-TOR link banwidth}
  //ELEC: TOR-ELEC link latency, TOR-ELEC link bandwidth, ELEC switch latency
  //OPT: TOR-OPT link latency, TOR-OPT link bandwidth, OPT switchi latency, OPT init latency (circuit switching))
  if (parameters.size() < 1 || parameters.empty()) {
    surf_parse_error(
        "ClusterOptElecSimple are defined by the number of node per routers");
  } 
  try {
    this->node_per_router_ = std::stoi(parameters[0]);
  } catch (std::invalid_argument& ia) {
    throw std::invalid_argument(std::string("First parameter is not the number of node per rack:") + parameters[0]);
  }
  XBT_ERROR("NTT - Parse argument:node_per_router_ %d", this->node_per_router_);
	
	//1. intra TOR configuration	
	this->swt_lat_ = 0;
	this->bw_  = cluster->bw;
	this->lat_ = cluster->lat;
	if (parameters.size() >= 2){
		boost::split(tmp, parameters[1], boost::is_any_of(","));
		if (tmp.size() < 1) {
			surf_parse_error("ClusterOptElecSimple are defined with TOR switch latency. Please set it in topo_parameters");
		}
		else{
			this->swt_lat_ = surf_parse_get_time(tmp[0].c_str(), "swt_lat of TOR", cluster->id);
			XBT_ERROR("NTT - Parse argument: swt_lat_ %s -> %.9f",tmp[0].c_str(), this->swt_lat_);
		}
		if (tmp.size() >= 2) {
			this->lat_ = surf_parse_get_time(tmp[1].c_str(), "link lat_", cluster->id);
			XBT_ERROR("NTT - Parse argument: lat_ %s -> %.9f",tmp[1].c_str(), this->lat_);
		}
		if (tmp.size() >= 3) {
			this->bw_ = surf_parse_get_bandwidth(tmp[2].c_str(), "link bw_", cluster->id);
			XBT_ERROR("NTT - Parse argument: bw_ %s -> %.9f",tmp[2].c_str(), this->bw_);
		}
	}
	
	//2. TOR-ELEC configuration (default = TOR configuartion)
	this->elec_bw_  = this->bw_ ;
	this->elec_lat_ = this->lat_;
	this->elec_swt_lat_ = this->swt_lat_;
	if (parameters.size() >= 3){
		boost::split(tmp, parameters[2], boost::is_any_of(","));
		if (tmp.size() != 3) {
			surf_parse_error("ClusterOptElecSimple are defined with ELEC switch. Please set them in topo_parameters");
		}
		else{
			this->elec_lat_ = surf_parse_get_time(tmp[0].c_str(), "link lat of ELEC", cluster->id);
			XBT_ERROR("NTT - Parse argument:elec_lat_ %s -> %.9f",tmp[0].c_str(), this->elec_lat_);
			this->elec_bw_ = surf_parse_get_bandwidth(tmp[1].c_str(), "link bw of ELEC", cluster->id);
			XBT_ERROR("NTT - Parse argument:elec_bw_ %s -> %.9f",tmp[1].c_str(), this->elec_bw_);
			this->elec_swt_lat_ = surf_parse_get_time(tmp[2].c_str(), "switch lat of ELEC", cluster->id);
			XBT_ERROR("NTT - Parse argument:elec_swt_lat_ %s -> %.9f",tmp[2].c_str(), this->elec_swt_lat_);
		}
	}
	
	//3. TOR-OPT configuration (default = TOR configuartion)
	this->opt_bw_  = this->bw_ ;
	this->opt_lat_ = this->lat_;
	this->opt_swt_lat_ = this->swt_lat_;
	this->init_lat_ = 0;
	if (parameters.size() >= 4){
		boost::split(tmp, parameters[3], boost::is_any_of(","));
		if (tmp.size() != 4) {
			surf_parse_error("ClusterOptElecSimple are defined with OPT switch. Please set them in topo_parameters");
		}
		else{
			this->opt_lat_ = surf_parse_get_time(tmp[0].c_str(), "link lat of OPT", cluster->id);
			XBT_ERROR("NTT - Parse argument:opt_lat_ %s -> %.9f",tmp[0].c_str(), this->opt_lat_);
			this->opt_bw_ = surf_parse_get_bandwidth(tmp[1].c_str(), "link bw of OPT", cluster->id);
			XBT_ERROR("NTT - Parse argument:opt_bw_ %s -> %.9f",tmp[1].c_str(), this->opt_bw_);
			this->opt_swt_lat_ = surf_parse_get_time(tmp[2].c_str(), "switch lat of OPT", cluster->id);
			XBT_ERROR("NTT - Parse argument:opt_swt_lat_ %s -> %.9f",tmp[2].c_str(), this->opt_swt_lat_);
			this->init_lat_ = surf_parse_get_time(tmp[3].c_str(), "init_lat of OPT", cluster->id);
			XBT_ERROR("NTT - Parse argument:init_lat_ %s -> %.9f",tmp[3].c_str(), this->init_lat_);
		}
	}
	
/*   if (parameters.size() >= 3){
	 this->opt_bw_ = surf_parse_get_bandwidth(parameters[2].c_str(), "opt_bw of cluster", "optelecsimplezone");
  }
  else{
	  this->opt_bw_ = cluster->bw;
  }
  XBT_ERROR("NTT - Parse argument:opt_bw_ %s -> %f", parameters[2].c_str(), this->opt_bw_);  
  if (parameters.size() >= 4){
	 this->opt_lat_ = surf_parse_get_time(parameters[3].c_str(), "opt_lat of cluster", "optelecsimplezone");
  } else{
	  this->opt_lat_ = cluster->lat;
  }
  XBT_ERROR("NTT - Parse argument:opt_lat_ %s -> %f",parameters[3].c_str(), this->opt_lat_);
  if (parameters.size() >= 5){
	 this->init_lat_ = surf_parse_get_time(parameters[4].c_str(), "init_lat of cluster", "optelecsimplezone");
  } else{
	  this->init_lat_ = 0;
  } 
  XBT_ERROR("NTT - Parse argument:init_lat_ %s -> %f",parameters[4].c_str(), this->init_lat_);
  if (parameters.size() >= 6){
	 this->opt_swt_lat_ = surf_parse_get_time(parameters[5].c_str(), "opt_swt_lat of cluster", "optelecsimplezone");
  } else{
	  this->opt_swt_lat_ = 0;
  } 
  XBT_ERROR("NTT - Parse argument:opt_swt_lat_ %s -> %f",parameters[5].c_str(), this->opt_swt_lat_);
    */
  this->num_node_ = cluster->radicals->size();
  XBT_ERROR("NTT - Parse argument: num_node_ %d",this->num_node_);
  this->num_router_ = ceil(this->num_node_ / this->node_per_router_); //TODO: check the function.
  XBT_ERROR("NTT - Parse argument: num_router_ %d",this->num_router_);
  this->num_links_per_node_ = 2;
  double routing_threshold = simgrid::config::get_value<double>("network/optelec_routing_threshold");
  XBT_ERROR("NTT - routing_threshold:%.9f",routing_threshold);
  this->sharing_policy_ = cluster->sharing_policy;
/*   if (cluster->sharing_policy == s4u::Link::SharingPolicy::SPLITDUPLEX)
    this->num_links_per_link_ = 2;
 */  
}

/* Generate the cluster once every node is created */
void ClusterOptElecSimpleZone::seal()
{
	XBT_DEBUG("NTT - seal");
   if (this->node_per_router_ == 0) {
    return;
  }

  this->generate_routers();
  this->generate_links();
}

OptElecRouter::OptElecRouter(int routerId, int isOpticMode)
{
	this->routerId = routerId;
	this->isOpticMode_ = isOpticMode;
}

OptElecRouter::~OptElecRouter()
{
  delete[] up_links_;
  delete[] down_links_;
}

bool OptElecRouter::is_configuration_required(int soureId, int destId){
	if(this->isOpticMode_ == true){
		if ((soureId == currentSourceId)&&(destId == currentDestId)){
			return false;
		}
		else if ((soureId == currentDestId)&&(destId == currentSourceId)){ //in revert order.
			return false;
		}
	}
	return true;
}

double OptElecRouter::config_new_path(int soureId, int destId){
	if (is_configuration_required(soureId,destId)){
		currentSourceId = soureId;
		currentDestId = destId;
		return this->init_lat;
	}
	else{
		return this->swt_lat;
	}
}

void ClusterOptElecSimpleZone::generate_routers()
{
	this->routers_ = new OptElecRouter*[this->num_router_];
	int i;
	for (i = 0; i < this->num_router_; i++){ 
		OptElecRouter* router = new OptElecRouter(i,false);
		router->swt_lat = this->swt_lat_;
		this->routers_[i] = router;
		XBT_ERROR("NTT - Generate TOR router %d, %.9f", i, router->swt_lat);
	}
	
	this->electricalRouter_ = new OptElecRouter(this->num_router_,false);
	this->electricalRouter_->swt_lat = this->elec_swt_lat_;
	
	this->opticalRouter_ = new OptElecRouter(this->num_router_ + 1,true);
	this->opticalRouter_->init_lat = this->init_lat_;
	this->opticalRouter_->swt_lat = this->opt_swt_lat_;
	
}

void ClusterOptElecSimpleZone::create_optical_link(const std::string& id, int numlinks, resource::LinkImpl** linkup,
                                resource::LinkImpl** linkdown)
{
  *linkup   = nullptr;
  *linkdown = nullptr;
  LinkCreationArgs linkTemplate;
  linkTemplate.bandwidth = this->opt_bw_ * numlinks;
  linkTemplate.latency   = this->opt_lat_;
  linkTemplate.policy    = s4u::Link::SharingPolicy::FATPIPE;
  linkTemplate.id        = id;
  sg_platf_new_link(&linkTemplate);
  XBT_DEBUG("NTT - Generating optical-switch link %s", id.c_str());
  resource::LinkImpl* link;	
	link      = s4u::Link::by_name(linkTemplate.id)->get_impl();
	*linkup   = link;
	*linkdown = link;
}


void ClusterOptElecSimpleZone::create_electrical_link(const std::string& id, int numlinks, resource::LinkImpl** linkup,
                                resource::LinkImpl** linkdown)
{
  *linkup   = nullptr;
  *linkdown = nullptr;
  LinkCreationArgs linkTemplate;
  linkTemplate.bandwidth = this->elec_bw_ * numlinks;
  linkTemplate.latency   = this->elec_lat_;
  linkTemplate.policy    = this->sharing_policy_;
  linkTemplate.id        = id;
  sg_platf_new_link(&linkTemplate);
  XBT_DEBUG("NTT - Generating ELEC link %s", id.c_str());
  resource::LinkImpl* link;
  if (this->sharing_policy_ == s4u::Link::SharingPolicy::SPLITDUPLEX) {
    *linkup   = s4u::Link::by_name(linkTemplate.id + "_UP")->get_impl();   // check link?
    *linkdown = s4u::Link::by_name(linkTemplate.id + "_DOWN")->get_impl(); // check link ?
  } else {
    link      = s4u::Link::by_name(linkTemplate.id)->get_impl();
    *linkup   = link;
    *linkdown = link;
  }
}	
void ClusterOptElecSimpleZone::create_link(const std::string& id, int numlinks, resource::LinkImpl** linkup,
                                resource::LinkImpl** linkdown)
{	
  *linkup   = nullptr;
  *linkdown = nullptr;
  LinkCreationArgs linkTemplate;
  linkTemplate.bandwidth = this->bw_ * numlinks;
  linkTemplate.latency   = this->lat_;
  linkTemplate.policy    = this->sharing_policy_;
  linkTemplate.id        = id;
  sg_platf_new_link(&linkTemplate);
  XBT_DEBUG("NTT - Generating link %s", id.c_str());
  resource::LinkImpl* link;
  if (this->sharing_policy_ == s4u::Link::SharingPolicy::SPLITDUPLEX) {
    *linkup   = s4u::Link::by_name(linkTemplate.id + "_UP")->get_impl();   // check link?
    *linkdown = s4u::Link::by_name(linkTemplate.id + "_DOWN")->get_impl(); // check link ?
  } else {
    link      = s4u::Link::by_name(linkTemplate.id)->get_impl();
    *linkup   = link;
    *linkdown = link;
  }
}

void ClusterOptElecSimpleZone::generate_links()
{
  static int uniqueId = 0;
  resource::LinkImpl* linkup;
  resource::LinkImpl* linkdown;

  // Link from routers to their local nodes.
  std::string id = "";
  for (unsigned int i = 0; i < this->num_router_; i++) {
	  this->routers_[i]->down_links_ = new resource::LinkImpl*[this->node_per_router_];
	  for (unsigned int j = 0; j < this->node_per_router_; j++) {
		id = "l_r" + std::to_string(i) + "_n" +
                       std::to_string(j) + "_" + std::to_string(uniqueId);
		this->create_link(id, 1, &linkup, &linkdown);
		this->routers_[i]->down_links_[j] = linkup;	
		//TODO: Store the linkdown?
		if (this->sharing_policy_ == s4u::Link::SharingPolicy::SPLITDUPLEX){
			  //Do nothing now
		}
	    uniqueId++;
	  }
  }	

  //Link from ToR router to the top-level switch.
  this->electricalRouter_->down_links_ = new resource::LinkImpl*[this->num_router_];
  this->opticalRouter_->down_links_ = new resource::LinkImpl*[this->num_router_];
  for (unsigned int i = 0; i < this->num_router_; i++) {
		this->routers_[i]->up_links_ = new resource::LinkImpl*[2];
		id = "l_r" + std::to_string(i) + "_elec" +
                       std::to_string(this->electricalRouter_->routerId) + "_" + std::to_string(uniqueId);
	  	this->create_electrical_link(id, 1, &linkup, &linkdown);
		this->routers_[i]->up_links_[0] = linkdown;
		this->electricalRouter_->down_links_[i] = linkup;
		
		id = "l_r" + std::to_string(i) + "_opt" +
                       std::to_string(this->opticalRouter_->routerId) + "_" + std::to_string(uniqueId);
	  	this->create_optical_link(id, 1, &linkup, &linkdown);
		this->routers_[i]->up_links_[1] = linkdown;
		this->opticalRouter_->down_links_[i] = linkup;
		uniqueId++;
  }
  
  //TODO: up_link of top-level router to outside the cluster
}

void ClusterOptElecSimpleZone::get_local_route(NetPoint* src, NetPoint* dst,double size, RouteCreationArgs* route, double* latency)
{
  if (dst->is_router() || src->is_router())
    return;

  //XBT_VERB("ClusterOptElecSimple getLocalRoute from '%s'[%u] to '%s'[%u]", src->get_cname(), src->id(), dst->get_cname(),dst->id());
  XBT_ERROR("getLocalRoute from '%s'[%u] to '%s'[%u] for message[%f bytes]", src->get_cname(), src->id(), dst->get_cname(),dst->id(),size);
           
  if ((src->id() == dst->id()) && has_loopback_) {
    std::pair<resource::LinkImpl*, resource::LinkImpl*> info = private_links_.at(node_pos(src->id()));

    route->link_list.push_back(info.first);
    if (latency)
      *latency += info.first->get_latency();
    return;
  }
  
  unsigned int myCoords[2];
  rankId_to_coords(src->id(), myCoords);
  XBT_ERROR("src : %u router, %u node_id", myCoords[0], myCoords[1]);
  OptElecRouter* myRouter = this->routers_[myCoords[0]];
  unsigned int targetCoords[2];
  rankId_to_coords(dst->id(), targetCoords);
  XBT_ERROR("dst : %u router, %u node_id", targetCoords[0], targetCoords[1]);
  OptElecRouter* targetRouter = this->routers_[targetCoords[0]];
  double step_lat = 0;
  // node->router local link
  route->link_list.push_back(myRouter->down_links_[myCoords[1]]);
  if (latency){
	  step_lat = myRouter->down_links_[myCoords[1]]->get_latency();
	  step_lat+= myRouter->swt_lat; // TOR switch latency;
	  *latency += step_lat;
	  XBT_ERROR("Src node to ToR. Latency = %.9f (total: %.9f)", step_lat,latency[0]);
  }
  if (has_limiter_) { // limiter for sender
    std::pair<resource::LinkImpl*, resource::LinkImpl*> info = private_links_.at(node_pos_with_loopback(src->id()));
    route->link_list.push_back(info.first);
  }
  if (myCoords[0] != targetCoords[0]) {
	   //XBT_ERROR("targetRouter:%d, myRouter:%d",targetRouter->routerId, myRouter->routerId);
	  double routing_threshold = simgrid::config::get_value<double>("network/optelec_routing_threshold");
	  XBT_ERROR("routing_threshold:%.9f",routing_threshold);
	   
	  if (size < routing_threshold){
		  //Using electrical switch
		  XBT_ERROR("Electric switch");
		  route->link_list.push_back(myRouter->up_links_[0]);
		  if (latency){
				step_lat = myRouter->up_links_[0]->get_latency();
				step_lat += this->electricalRouter_->swt_lat; // electric switch latency;
				*latency += step_lat;
				XBT_ERROR("ToR to electricRouter. Latency = %.9f (total: %.9f)", step_lat,latency[0]);
		  }
		  route->link_list.push_back(this->electricalRouter_->down_links_[targetRouter->routerId]);
		  if (latency){
			  step_lat= this->electricalRouter_->down_links_[targetRouter->routerId]->get_latency();
			  step_lat += targetRouter->swt_lat; // TOR switch latency;
			  *latency += step_lat;
			  XBT_ERROR("electricRouter to ToR. Latency = %.9f (total: %.9f)", step_lat,latency[0]);
		  }
	  }
	  else{
		  //Using optical switch
		  XBT_ERROR("optical switch");
		  route->link_list.push_back(myRouter->up_links_[1]);
		  if (latency){
			  step_lat = myRouter->up_links_[1]->get_latency();
			  step_lat += this->opticalRouter_->config_new_path(myCoords[0],targetCoords[0]);
			  //*latency += this->opticalRouter_->swt_lat; // optical switch latency;
			  *latency += step_lat;
			  XBT_ERROR("ToR to opticRouter. Latency = %.9f (total: %.9f)", step_lat,latency[0]);
		  }

		  route->link_list.push_back(this->opticalRouter_->down_links_[targetRouter->routerId]);
		  if (latency){
			  step_lat = this->opticalRouter_->down_links_[targetRouter->routerId]->get_latency();
			  step_lat += targetRouter->swt_lat; // TOR switch latency;
			  *latency += step_lat;
			  XBT_ERROR("opticRouter to ToR. Latency = %.9f (total: %.9f)", step_lat,latency[0]);
		  }
	  }
  }

  if (has_limiter_) { // limiter for receiver
    std::pair<resource::LinkImpl*, resource::LinkImpl*> info = private_links_.at(node_pos_with_loopback(dst->id()));
    route->link_list.push_back(info.first);
  }
  
  // router->node local link
  route->link_list.push_back(targetRouter->down_links_[targetCoords[1]]);
  if (latency){
		*latency += targetRouter->down_links_[targetCoords[1]]->get_latency();
		XBT_ERROR("ToR to dst node. Latency = %.9f (total: %.9f)", targetRouter->down_links_[targetCoords[1]]->get_latency(),latency[0]);
  }
}

}
}
} // namespace
