/* Copyright (c) 2014-2021. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_ROUTING_CLUSTER_FAT_TREE_HPP_
#define SIMGRID_ROUTING_CLUSTER_FAT_TREE_HPP_

#include <simgrid/kernel/routing/ClusterZone.hpp>

namespace simgrid {
namespace kernel {
namespace routing {

class XBT_PRIVATE FatTreeLink;

/** @brief A node in a fat tree (@ref FatTreeZone).
 * A FatTreeNode can either be a switch or a processing node. Switches are
 * identified by a negative ID. This class is closely related to fat
 */
class XBT_PRIVATE FatTreeNode {
public:
  /** Unique ID which identifies every node. */
  int id;
  /* Level into the tree, with 0 being the leafs.
   */
  unsigned int level;
  /* @brief Position into the level, starting from 0.
   */
  unsigned int position;
  /** In order to link nodes between them, each one must be assigned a label,
   * consisting of l integers, l being the levels number of the tree. Each label
   * is unique in the level, and the way it is generated allows the construction
   * of a fat tree which fits the desired topology.
   */
  std::vector<unsigned int> label;

  /** Links to the lower level, where the position in the vector corresponds to
   * a port number.
   */
  std::vector<std::shared_ptr<FatTreeLink>> children;
  /** Links to the upper level, where the position in the vector corresponds to
   * a port number.
   */
  std::vector<std::shared_ptr<FatTreeLink>> parents;

  /** Virtual link standing for the node global capacity.
   */
  resource::LinkImpl* limiter_link_;
  /** If present, communications from this node to this node will pass through it
   * instead of passing by an upper level switch.
   */
  resource::LinkImpl* loopback_;
  FatTreeNode(int id, int level, int position, resource::LinkImpl* limiter, resource::LinkImpl* loopback)
      : id(id), level(level), position(position), limiter_link_(limiter), loopback_(loopback)
  {
  }
};

/** @brief Link in a fat tree (@ref FatTreeZone).
 *
 * Represents a single, duplex link in a fat tree. This is necessary to have a tree.
 * It is equivalent to a physical link.
 */
class FatTreeLink {
public:
  FatTreeLink(FatTreeNode* src, FatTreeNode* dst, resource::LinkImpl* linkup, resource::LinkImpl* linkdown)
      : up_node_(dst), down_node_(src), up_link_(linkup), down_link_(linkdown)
  {
  }
  /** Upper end of the link */
  FatTreeNode* up_node_;
  /** Lower end of the link */
  FatTreeNode* down_node_;
  /** Link going up in the tree */
  resource::LinkImpl* up_link_;
  /** Link going down in the tree */
  resource::LinkImpl* down_link_;
};

/** @ingroup ROUTING_API
 * @brief NetZone using a Fat-Tree topology
 *
 * Generate fat trees according to the topology asked for, according to:
 * Eitan Zahavi, D-Mod-K Routing Providing Non-Blocking Traffic for Shift
 * Permutations on Real Life Fat Trees (2010).
 *
 * RLFT are PGFT with some restrictions to address real world constraints,
 * which are not currently enforced.
 *
 * The exact topology is described in the mandatory topo_parameters
 * field, and follow the "h ; m_1, ..., m_h ; w_1, ..., w_h ; p_1, ..., p_h" format.
 * h stands for the switches levels number, i.e. the fat tree is of height h,
 * without the processing nodes. m_i stands for the number of lower level nodes
 * connected to a node in level i. w_i stands for the number of upper levels
 * nodes connected to a node in level i-1. p_i stands for the number of
 * parallel links connecting two nodes between level i and i - 1. Level h is
 * the topmost switch level, level 1 is the lowest switch level, and level 0
 * represents the processing nodes. The number of provided nodes must be exactly
 * the number of processing nodes required to fit the topology, which is the
 * product of the m_i's.
 *
 * Routing is made using a destination-mod-k scheme.
 */
class XBT_PRIVATE FatTreeZone : public ClusterBase {
  /** @brief Generate the fat tree
   *
   * Once all processing nodes have been added, this will make sure the fat
   * tree is generated by calling generateLabels(), generateSwitches() and
   * then connection all nodes between them, using their label.
   */
  // description of a PGFT (TODO : better doc)
  unsigned long levels_ = 0;
  std::vector<unsigned int> num_children_per_node_; // number of children by node
  std::vector<unsigned int> num_parents_per_node_;  // number of parents by node
  std::vector<unsigned int> num_port_lower_level_;  // ports between each level l and l-1

  std::map<int, std::shared_ptr<FatTreeNode>> compute_nodes_;
  std::vector<std::shared_ptr<FatTreeNode>> nodes_;
  std::vector<std::shared_ptr<FatTreeLink>> links_;
  std::vector<unsigned int> nodes_by_level_;

  void add_link(FatTreeNode* parent, unsigned int parent_port, FatTreeNode* child, unsigned int child_port);
  int get_level_position(const unsigned int level);
  void generate_switches(const s4u::ClusterCallbacks& set_callbacks);
  void generate_labels();
  int connect_node_to_parents(FatTreeNode* node);
  bool are_related(FatTreeNode* parent, FatTreeNode* child) const;
  bool is_in_sub_tree(const FatTreeNode* root, const FatTreeNode* node) const;

  void do_seal() override;

public:
  explicit FatTreeZone(const std::string& name) : ClusterBase(name){};
  FatTreeZone(const FatTreeZone&) = delete;
  FatTreeZone& operator=(const FatTreeZone&) = delete;
  void get_local_route(const NetPoint* src, const NetPoint* dst, Route* into, double* latency) override;

  /**
   * @brief Parse the topology parameters from string format
   *
   * @param topo_parameters String with topology, e.g. "2;4,4;1,2;1,2"
   */
  static s4u::FatTreeParams parse_topo_parameters(const std::string& topo_parameters);
  /** @brief Checks topology parameters */
  static void check_topology(unsigned int n_levels, const std::vector<unsigned int>& down_links,
                             const std::vector<unsigned int>& up_links, const std::vector<unsigned int>& link_count);
  /** @brief Set FatTree topology */
  void set_topology(unsigned int n_levels, const std::vector<unsigned int>& down_links,
                    const std::vector<unsigned int>& up_links, const std::vector<unsigned int>& link_count);
  void add_processing_node(int id, resource::LinkImpl* limiter, resource::LinkImpl* loopback);
  /**
   * @brief Build upper levels (switches) in Fat-Tree
   *
   * Suppose that set_topology and add_processing_node have already been called
   */
  void build_upper_levels(const s4u::ClusterCallbacks& set_callbacks);
  void generate_dot_file(const std::string& filename = "fat_tree.dot") const;
};
} // namespace routing
} // namespace kernel
} // namespace simgrid

#endif