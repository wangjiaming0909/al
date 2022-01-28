#include "mysql_gtid.h"
#include <boost/exception/to_string.hpp>
#include <cassert>

namespace gtid
{
const int Uuid::bytes_per_section[Uuid::SECTION_NUM] = {8, 4, 4, 4,
                                                                  12};

Gno parse_gno(const char **s) {
  char *endp;
  Gno ret = strtoll(*s, &endp, 0);
  if (ret < 0 || ret == __LONG_LONG_MAX__) {
    return -1;
  }
  *s = endp;
  return ret;
}

int Uuid::parse(const char *text, unsigned int len) {
  switch (len) {
  case TEXT_LENGTH: {
    unsigned char *start = uuid;
    for (int i = 0; i < SECTION_NUM - 1; ++i) {
      read_section(&start, bytes_per_section[i], &text);
      if (*text == '-')
        text++;
      else
        return -1;
    }
    read_section(&start, bytes_per_section[SECTION_NUM - 1], &text);
  } break;
  default:
    return -1;
  }
  return 0;
}

int Uuid::read_section(unsigned char **start, unsigned int section_len,
                            const char **text) {
  for (int i = 0; i < section_len; ++i) {
    *(*start) = *(*text);
    ++*text;
    ++*start;
  }
  return 0;
}

bool Uuid::is_valid(const char* text, size_t len)
{
  Uuid uuid;
  return uuid.parse(text, len) == 0;
}

Uuid::StringT Uuid::to_string() const {
  StringT ret;
  const char *p = (const char *)uuid;
  for (int i = 0; i < SECTION_NUM - 1; ++i) {
    ret.append(p, bytes_per_section[i]);
    ret.append("-");
    p += bytes_per_section[i];
  }
  ret.append(p, bytes_per_section[SECTION_NUM - 1]);
  return ret;
}

Sidno SidMap::add_sid(const Sid &sid) {
  SidSidnoMap::iterator it = _sid_sidno_map.find(sid);
  Sidno sidno;
  if (it != _sid_sidno_map.end()) {
    return it->second->sidno;
  } else {
    sidno = get_max_sidno() + 1;
    add_node(sidno, sid);
  }
  return sidno;
}

int SidMap::add_node(Sidno sidno, const Sid &sid) {
  NodePtr node(new Node());
  node->sid = sid;
  node->sidno = sidno;
  _sidno_sid_map.push_back(node);
  _sid_sidno_map[sid] = node;
  return 0;
}

Sidno SidMap::sid_to_sidno(const Sid &sid) const {
  SidSidnoMap::const_iterator it = _sid_sidno_map.find(sid);
  if (it != _sid_sidno_map.end()) {
    return it->second->sidno;
  }
  return 0;
}
const Sid &SidMap::sidno_to_sid(Sidno sidno) const {
  assert(sidno >= 1 && sidno <= _sidno_sid_map.size());
  return _sidno_sid_map[sidno - 1]->sid;
}

int GtidSet::add_gtid_text(const char *text) {
  if (!text)
    return -1;
  SKIP_SPACE(text);
  if (*text == 0) {
    return 0;
  }
  int n_intervals = 0;
  const char *s = text;
  for (; *s; s++)
    if (*s == ':')
      n_intervals++;
  if (n_intervals > _intervals.size())
    _intervals.resize(n_intervals);
  s = text;

  while (1) {
    // skip comma
    while (*s == ',') {
      s++;
      SKIP_SPACE(s);
    }
    // empty gtid_set only contains comma
    if (*s == 0)
      return 0;

    Sid sid;
    if (sid.parse(s, Sid::TEXT_LENGTH) != 0) {
      return -1;
    }
    s += Sid::TEXT_LENGTH;
    Sidno sidno = _sid_map->add_sid(sid);
    if (sidno <= 0)
      return -1;
    SKIP_SPACE(s);
    // intervals
    while (*s == ':') {
      s++;
      // start interval
      Gno begin = parse_gno(&s);
      if (begin == 0)
        return -1;
      SKIP_SPACE(s);
      // end interval
      Gno end;
      if (*s == '-') {
        s++;
        end = parse_gno(&s);
        if (end < 0)
          return -1;
        end++;
        SKIP_SPACE(s);
      } else {
        end = begin + 1;
      }
      if (end > begin) {
        add_gno_interval(begin, end, sidno);
      }
    }
  }
  if (*s != ',' && *s != 0)
    return -1;
}

void GtidSet::add_gno_interval(Gno begin, Gno end,
                                    Sidno sidno) {
  IntervalList &intvls = _intervals[sidno - 1];
  IntervalList::iterator cur_intv = intvls.begin();
  // combine with the existing intervals
  while (cur_intv != intvls.end()) {
    IntervalPtr intv = *cur_intv;
    if (intv->end >= begin) {

      if (intv->begin > end) //(begin, end) is strictly before cur intv
        break;
      if (intv->begin < begin)
        begin = intv->begin;
      IntervalList::iterator next_intv = cur_intv;
      ++next_intv;
      //(begin, end) intersect with the following intvs
      while (next_intv != intvls.end() && (*next_intv)->begin <= end) {
        intvls.erase(cur_intv);
        cur_intv = next_intv;
        ++next_intv;
      }
      // store the interval
      intv = *cur_intv;
      intv->begin = begin;
      if (intv->end < end)
        intv->end = end;
      return;
    }
    ++cur_intv;
  }

  // the newly added gno interval can't be conbined into the existing intervals
  IntervalPtr new_intv(new Interval());
  new_intv->begin = begin;
  new_intv->end = end;
  intvls.insert(cur_intv, new_intv);
}

bool GtidSet::contains_sidno(Sidno sidno) const {
  if (sidno > _intervals.size())
    return false;
  return get_intervals(sidno).size() > 0;
}

std::string GtidSet::to_string() const {
  bool first_sid = true;
  std::string ret;
  for (int sid_i = 0; sid_i < _sid_map->get_max_sidno(); ++sid_i) {
    Sidno sidno = sid_i + 1;
    if (contains_sidno(sidno)) {
      const IntervalList &intvls = get_intervals(sidno);
      if (first_sid)
        first_sid = false;
      else
        ret.append(",");

      ret.append(_sid_map->sidno_to_sid(sidno).to_string());
      IntervalList::const_iterator intvl = intvls.begin();
      IntervalPtr intv;
      do {
        intv = *intvl;
        ret.append(":");
        ret.append(boost::to_string(intv->begin));
        if (intv->end > intv->begin + 1) {
          ret.append("-");
          ret.append(boost::to_string(intv->end - 1));
        }
        ++intvl;
      } while (intvl != intvls.end());
    }
  }
  return ret;
}

void GtidSet::remove_gtid_set(const GtidSet &other) {
  Sidno max_sidno = other._intervals.size();
  if (max_sidno == 0)
    return;
  if (other._sid_map.get() == _sid_map.get() || !other._sid_map || !_sid_map) {
    max_sidno = std::min(max_sidno, (Sidno)_intervals.size());
    for (Sidno sidno = 1; sidno <= max_sidno; ++sidno) {
      remove_gno_intervals(sidno, other, sidno);
    }
  } else {
    for (Sidno other_sidno = 1; other_sidno <= max_sidno; ++other_sidno) {
      if (other.contains_sidno(other_sidno) &&
          !other.get_intervals(other_sidno).empty()) {
        const Sid &sid = other._sid_map->sidno_to_sid(other_sidno);
        Sidno this_sidno = _sid_map->sid_to_sidno(sid);
        if (this_sidno != 0)
          remove_gno_intervals(this_sidno, other, other_sidno);
      }
    }
  }
}

void GtidSet::remove_gno_intervals(Sidno sidno,
                                        const GtidSet &other,
                                        Sidno other_sidno) {
  const auto& other_intvls = other.get_intervals(other_sidno);
  auto& intvls = get_intervals(sidno);
  IntervalList::const_iterator other_intv = other_intvls.begin();
  IntervalList::iterator intv = intvls.begin();
  while (other_intv != other_intvls.end()) {
    remove_gno_interval(intvls, (*other_intv)->begin, (*other_intv)->end);
    if (intvls.empty())
      break;
    ++other_intv;
  }
}

void GtidSet::remove_gno_interval(IntervalList &intvls, Gno begin,
                                       Gno end) {
  IntervalList::iterator intv_it = intvls.begin();
  IntervalPtr intv;
  // skip invervals strictly before begin
  while (1) {
    if (intv_it == intvls.end())
      return;
    intv = *intv_it;
    if (intv->end > begin)
      break;
    ++intv_it;
  }

  if (intv->begin < begin) {
    if (intv->end >
        end) { //(begin, end) is within this interval, split this
               // interval into two, (intv->begin, begin) and (end, intv->end)
      IntervalPtr new_intv(new Interval);
      new_intv->begin = end;
      new_intv->end = intv->end;
      intv->end = begin;
      ++intv_it;
      intvls.insert(intv_it, new_intv);
      return;
    }

    // intv->begin < begin < intv->end < end
    intv->end = begin;
    intv_it++;
    intv = *intv_it;
    if (intv_it == intvls.end())
      return;
  }

  // intv has already moved to the next one
  // begin < intv->begin < end
  while (intv->end <= end) {
    intv_it = intvls.erase(intv_it);
    if (intv_it == intvls.end())
      return;
  }

  // intv->begin <= end <= intv->end
  if (intv->begin < end)
    intv->begin = end;
}

bool GtidSet::is_valid(const char* text)
{
  const char* s = text;
  SKIP_SPACE(s);
  do {
    while (*s == ',') {
      s++;
      SKIP_SPACE(s);
    }
    if (*s == 0) return true;

    //parse sid
    if (!Uuid::is_valid(text, Sid::TEXT_LENGTH))
      return false;
    s += Sid::TEXT_LENGTH;
    SKIP_SPACE(s);

    //intervals
    while(*s ==':') {
      s++;

      //gno
      if (parse_gno(&s) <= 0) return false;
      SKIP_SPACE(s);
      if (*s == '-') {
        s++;
        if (parse_gno(&s) <= 0) return false;
        SKIP_SPACE(s);
      }
    }
  } while(*s == ',');
  if (*s != 0) return false;
  return true;
}

size_t GtidSet::get_sid_count() const {
  Sidno max_sidno = _sid_map->get_max_sidno();
  size_t ret = 0;
  for (Sidno sidno = 1; sidno <= max_sidno; sidno++) {
    if (contains_sidno(sidno))
      ret++;
  }
  return ret;
}

Gno GtidSet::get_gno_count(const Sid& sid) const {
  Sidno sidno = _sid_map->sid_to_sidno(sid);
  return get_gno_count(sidno);
}

Gno GtidSet::get_gno_count(Sidno sidno) const {
  Gno ret = 0;
  if (sidno >= 1 && contains_sidno(sidno)) {
    const IntervalList& intvls = get_intervals(sidno);
    IntervalList::const_iterator intvl_it = intvls.cbegin();
    IntervalPtr intv;
    for (; intvl_it != intvls.end(); ++intvl_it) {
      intv = *intvl_it;
      Gno tmp = intv->end - intv->begin;
      ret += tmp;
    }
  }
  return ret;
}
}
