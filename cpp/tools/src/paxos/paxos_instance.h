#pragma once
#include <string>
#include <vector>
#include <memory>

namespace paxos {
using NodeId = uint64_t;
static const NodeId null_node = 0;

class NodeInfo {
public:
  NodeInfo();
  NodeInfo(NodeId node_id);
  NodeInfo(const std::string &ip, int port);
  virtual ~NodeInfo() {}

  NodeId GetNodeId() const;
  const std::string &GetIp() const;
  int GetPort() const;
  void SetIpPort(const std::string &ip, int port);
  void SetNodeId(NodeId node_id);

private:
  NodeId node_id_;
  std::string ip_;
  int port_;
};

using NodeInfoList = std::vector<NodeInfo>;
class Transport;
struct Options {
  Options();
  NodeInfo my_node;
  NodeInfoList nodes_;
  Transport* transport;
};

class Node;

class Transport {
public:
  Transport();
  virtual ~Transport() {}
  virtual void Start() = 0;
  virtual void Stop() = 0;

private:
  friend class Node;
  Node *node_;
};

class DefaultTrasport : public Transport {
public:
  DefaultTrasport();
  virtual ~DefaultTrasport();

  int Init(const std::string &listen_ip, int listen_port);

  void Start() override;
  void Stop() override;
};

class Node {
public:
  Node() = default;
  virtual ~Node() = default;

  static int RunNode(const Options &opts, Node *&node);

  virtual int Propose() = 0;

protected:
  std::unique_ptr<Transport> transport_;
};

class PNode : public Node {
public:
  PNode();
  ~PNode();

  int Init(const Options &opts);

public:
  int Propose() override;

private:
  int InitTransport(const Options &opts);
};
} // namespace paxos
