#include "paxos_instance.h"

namespace paxos {

NodeInfo::NodeInfo() : node_id_(null_node), ip_(), port_(-1) {}

NodeInfo::NodeInfo(NodeId node_id) : node_id_(node_id), ip_(), port_(-1) {}

Options::Options() : my_node(null_node), nodes_() {}

Transport::Transport() : node_(nullptr) {}

int Node::RunNode(const Options &opts, Node *&node) {
  auto *new_node = new PNode();
  int ret = new_node->Init(opts);
  if (ret != 0) {
    delete new_node;
    return ret;
  }
  new_node->transport_->node_ = new_node;
  new_node->transport_->Start();
  node = new_node;
  return 0;
}

int PNode::Init(const Options &opts) {
  int ret = InitTransport(opts);
  if (ret != 0) return ret;
}

int PNode::InitTransport(const Options& opts) {
  if (opts.transport) {
    transport_.reset(opts.transport);
    return 0;
  }

  auto* trans = new DefaultTrasport();
  if (!trans) {
    return -1;
  }

  int ret = trans->Init(opts.my_node.GetIp(), opts.my_node.GetPort());
  if (ret != 0) {
    return ret;
  }
  transport_.reset(trans);
  return 0;
}
} // namespace paxos
