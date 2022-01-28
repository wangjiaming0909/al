#pragma once
#include <boost/functional/hash/hash.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

#define SKIP_SPACE(s)                                                          \
  while (isspace(*s))                                                          \
  s++

namespace gtid {
using Gno = long long int;
using Sidno = int;
Gno parse_gno(const char **s);

struct Uuid {
  using StringT = std::string;
  static const unsigned int TEXT_LENGTH = 36;
  static const unsigned int SECTION_NUM = 5;
  static const int bytes_per_section[SECTION_NUM];
  static const unsigned int UUID_LENGTH = 32;
  unsigned char uuid[UUID_LENGTH];
  int parse(const char *text, unsigned int len);
  int read_section(unsigned char **start, unsigned int section_len,
                   const char **text);
  StringT to_string() const;
  static bool is_valid(const char *text, size_t len);
};
inline bool operator==(const Uuid &left, const Uuid &right) {
  return std::memcmp(left.uuid, right.uuid, Uuid::UUID_LENGTH) == 0;
}

struct UuidHash {
  size_t operator()(const Uuid &uuid) const {
    return boost::hash_value(uuid.uuid);
  }
};
using Sid = Uuid;

struct Gtid {
  Gno gno;
  Sidno sidno;
};

class SidMap {
public:
  struct Node {
    Sidno sidno;
    Sid sid;
  };
  using NodePtr = boost::shared_ptr<Node>;
  using SidSidnoMap = boost::unordered_map<Sid, NodePtr, UuidHash>;
  using SidnoSidMap = std::vector<NodePtr>;
  Sidno add_sid(const Sid &sid);
  Sidno sid_to_sidno(const Sid &sid) const;
  const Sid &sidno_to_sid(Sidno sidno) const;
  Sidno get_max_sidno() const { return _sid_sidno_map.size(); }
  void clear() {
    _sid_sidno_map.clear();
    _sidno_sid_map.clear();
  }

private:
  int add_node(Sidno sidno, const Sid &sid);
  SidSidnoMap _sid_sidno_map;
  SidnoSidMap _sidno_sid_map;
};

class GtidSet {
public:
  GtidSet(boost::shared_ptr<SidMap> sid_map) : _sid_map(sid_map) {}
  struct Interval {
    Gno begin;
    Gno end;
  };
  using IntervalPtr = boost::shared_ptr<Interval>;
  using IntervalList = std::list<IntervalPtr>;
  GtidSet(SidMap *sid_map);
  int add_gtid_text(const char *text);
  void add_gno_interval(Gno begin, Gno end, Sidno sidno);
  std::string to_string() const;
  const IntervalList &get_intervals(Sidno sidno) const {
    return _intervals.at(sidno - 1);
  }
  IntervalList &get_intervals(Sidno sidno) {
    return _intervals.at(sidno - 1);
  }
  void remove_gtid_set(const GtidSet &gtid_set);
  void remove_gno_intervals(Sidno this_sidno, const GtidSet &other,
                            Sidno other_sidno);

  void remove_gno_interval(IntervalList &intvls, Gno begin, Gno end);
  static bool is_valid(const char *text);
  size_t get_sid_count() const;
  Gno get_gno_count(const Sid &sid) const;
  Gno get_gno_count(Sidno sidno) const;
  void clear() {
    _sid_map->clear();
    _intervals.clear();
  }

private:
  bool contains_sidno(Sidno sidno) const;
  boost::shared_ptr<SidMap> _sid_map;
  std::vector<IntervalList> _intervals;
};
} // namespace gtid
