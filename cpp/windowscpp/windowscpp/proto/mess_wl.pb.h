// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mess_wl.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_mess_5fwl_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_mess_5fwl_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_mess_5fwl_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_mess_5fwl_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_mess_5fwl_2eproto;
namespace downloadmessage {
class Download_Response;
class Download_ResponseDefaultTypeInternal;
extern Download_ResponseDefaultTypeInternal _Download_Response_default_instance_;
class Mess_WL;
class Mess_WLDefaultTypeInternal;
extern Mess_WLDefaultTypeInternal _Mess_WL_default_instance_;
}  // namespace downloadmessage
PROTOBUF_NAMESPACE_OPEN
template<> ::downloadmessage::Download_Response* Arena::CreateMaybeMessage<::downloadmessage::Download_Response>(Arena*);
template<> ::downloadmessage::Mess_WL* Arena::CreateMaybeMessage<::downloadmessage::Mess_WL>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace downloadmessage {

enum Mess_WL_DownloadCommand : int {
  Mess_WL_DownloadCommand_DOWNLOAD = 0,
  Mess_WL_DownloadCommand_PAUSE = 1,
  Mess_WL_DownloadCommand_RESUME = 2,
  Mess_WL_DownloadCommand_REMOVE = 3
};
bool Mess_WL_DownloadCommand_IsValid(int value);
constexpr Mess_WL_DownloadCommand Mess_WL_DownloadCommand_DownloadCommand_MIN = Mess_WL_DownloadCommand_DOWNLOAD;
constexpr Mess_WL_DownloadCommand Mess_WL_DownloadCommand_DownloadCommand_MAX = Mess_WL_DownloadCommand_REMOVE;
constexpr int Mess_WL_DownloadCommand_DownloadCommand_ARRAYSIZE = Mess_WL_DownloadCommand_DownloadCommand_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Mess_WL_DownloadCommand_descriptor();
template<typename T>
inline const std::string& Mess_WL_DownloadCommand_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Mess_WL_DownloadCommand>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Mess_WL_DownloadCommand_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Mess_WL_DownloadCommand_descriptor(), enum_t_value);
}
inline bool Mess_WL_DownloadCommand_Parse(
    const std::string& name, Mess_WL_DownloadCommand* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Mess_WL_DownloadCommand>(
    Mess_WL_DownloadCommand_descriptor(), name, value);
}
enum Download_Response_State : int {
  Download_Response_State_DOWNLOADING = 0,
  Download_Response_State_PAUSED = 1,
  Download_Response_State_REMOVED = 2,
  Download_Response_State_FAILED = 3,
  Download_Response_State_FINISHED = 4
};
bool Download_Response_State_IsValid(int value);
constexpr Download_Response_State Download_Response_State_State_MIN = Download_Response_State_DOWNLOADING;
constexpr Download_Response_State Download_Response_State_State_MAX = Download_Response_State_FINISHED;
constexpr int Download_Response_State_State_ARRAYSIZE = Download_Response_State_State_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Download_Response_State_descriptor();
template<typename T>
inline const std::string& Download_Response_State_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Download_Response_State>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Download_Response_State_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Download_Response_State_descriptor(), enum_t_value);
}
inline bool Download_Response_State_Parse(
    const std::string& name, Download_Response_State* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Download_Response_State>(
    Download_Response_State_descriptor(), name, value);
}
// ===================================================================

class Mess_WL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:downloadmessage.Mess_WL) */ {
 public:
  Mess_WL();
  virtual ~Mess_WL();

  Mess_WL(const Mess_WL& from);
  Mess_WL(Mess_WL&& from) noexcept
    : Mess_WL() {
    *this = ::std::move(from);
  }

  inline Mess_WL& operator=(const Mess_WL& from) {
    CopyFrom(from);
    return *this;
  }
  inline Mess_WL& operator=(Mess_WL&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Mess_WL& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Mess_WL* internal_default_instance() {
    return reinterpret_cast<const Mess_WL*>(
               &_Mess_WL_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Mess_WL& a, Mess_WL& b) {
    a.Swap(&b);
  }
  inline void Swap(Mess_WL* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Mess_WL* New() const final {
    return CreateMaybeMessage<Mess_WL>(nullptr);
  }

  Mess_WL* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Mess_WL>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Mess_WL& from);
  void MergeFrom(const Mess_WL& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Mess_WL* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "downloadmessage.Mess_WL";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mess_5fwl_2eproto);
    return ::descriptor_table_mess_5fwl_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef Mess_WL_DownloadCommand DownloadCommand;
  static constexpr DownloadCommand DOWNLOAD =
    Mess_WL_DownloadCommand_DOWNLOAD;
  static constexpr DownloadCommand PAUSE =
    Mess_WL_DownloadCommand_PAUSE;
  static constexpr DownloadCommand RESUME =
    Mess_WL_DownloadCommand_RESUME;
  static constexpr DownloadCommand REMOVE =
    Mess_WL_DownloadCommand_REMOVE;
  static inline bool DownloadCommand_IsValid(int value) {
    return Mess_WL_DownloadCommand_IsValid(value);
  }
  static constexpr DownloadCommand DownloadCommand_MIN =
    Mess_WL_DownloadCommand_DownloadCommand_MIN;
  static constexpr DownloadCommand DownloadCommand_MAX =
    Mess_WL_DownloadCommand_DownloadCommand_MAX;
  static constexpr int DownloadCommand_ARRAYSIZE =
    Mess_WL_DownloadCommand_DownloadCommand_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  DownloadCommand_descriptor() {
    return Mess_WL_DownloadCommand_descriptor();
  }
  template<typename T>
  static inline const std::string& DownloadCommand_Name(T enum_t_value) {
    static_assert(::std::is_same<T, DownloadCommand>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function DownloadCommand_Name.");
    return Mess_WL_DownloadCommand_Name(enum_t_value);
  }
  static inline bool DownloadCommand_Parse(const std::string& name,
      DownloadCommand* value) {
    return Mess_WL_DownloadCommand_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kUrlFieldNumber = 4,
    kLenFieldNumber = 1,
    kIdFieldNumber = 2,
    kCommandFieldNumber = 3,
  };
  // required string url = 4;
  bool has_url() const;
  private:
  bool _internal_has_url() const;
  public:
  void clear_url();
  const std::string& url() const;
  void set_url(const std::string& value);
  void set_url(std::string&& value);
  void set_url(const char* value);
  void set_url(const char* value, size_t size);
  std::string* mutable_url();
  std::string* release_url();
  void set_allocated_url(std::string* url);
  private:
  const std::string& _internal_url() const;
  void _internal_set_url(const std::string& value);
  std::string* _internal_mutable_url();
  public:

  // required int32 len = 1;
  bool has_len() const;
  private:
  bool _internal_has_len() const;
  public:
  void clear_len();
  ::PROTOBUF_NAMESPACE_ID::int32 len() const;
  void set_len(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_len() const;
  void _internal_set_len(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required int32 id = 2;
  bool has_id() const;
  private:
  bool _internal_has_id() const;
  public:
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required .downloadmessage.Mess_WL.DownloadCommand command = 3;
  bool has_command() const;
  private:
  bool _internal_has_command() const;
  public:
  void clear_command();
  ::downloadmessage::Mess_WL_DownloadCommand command() const;
  void set_command(::downloadmessage::Mess_WL_DownloadCommand value);
  private:
  ::downloadmessage::Mess_WL_DownloadCommand _internal_command() const;
  void _internal_set_command(::downloadmessage::Mess_WL_DownloadCommand value);
  public:

  // @@protoc_insertion_point(class_scope:downloadmessage.Mess_WL)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr url_;
  ::PROTOBUF_NAMESPACE_ID::int32 len_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  int command_;
  friend struct ::TableStruct_mess_5fwl_2eproto;
};
// -------------------------------------------------------------------

class Download_Response :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:downloadmessage.Download_Response) */ {
 public:
  Download_Response();
  virtual ~Download_Response();

  Download_Response(const Download_Response& from);
  Download_Response(Download_Response&& from) noexcept
    : Download_Response() {
    *this = ::std::move(from);
  }

  inline Download_Response& operator=(const Download_Response& from) {
    CopyFrom(from);
    return *this;
  }
  inline Download_Response& operator=(Download_Response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Download_Response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Download_Response* internal_default_instance() {
    return reinterpret_cast<const Download_Response*>(
               &_Download_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Download_Response& a, Download_Response& b) {
    a.Swap(&b);
  }
  inline void Swap(Download_Response* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Download_Response* New() const final {
    return CreateMaybeMessage<Download_Response>(nullptr);
  }

  Download_Response* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Download_Response>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Download_Response& from);
  void MergeFrom(const Download_Response& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Download_Response* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "downloadmessage.Download_Response";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_mess_5fwl_2eproto);
    return ::descriptor_table_mess_5fwl_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef Download_Response_State State;
  static constexpr State DOWNLOADING =
    Download_Response_State_DOWNLOADING;
  static constexpr State PAUSED =
    Download_Response_State_PAUSED;
  static constexpr State REMOVED =
    Download_Response_State_REMOVED;
  static constexpr State FAILED =
    Download_Response_State_FAILED;
  static constexpr State FINISHED =
    Download_Response_State_FINISHED;
  static inline bool State_IsValid(int value) {
    return Download_Response_State_IsValid(value);
  }
  static constexpr State State_MIN =
    Download_Response_State_State_MIN;
  static constexpr State State_MAX =
    Download_Response_State_State_MAX;
  static constexpr int State_ARRAYSIZE =
    Download_Response_State_State_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  State_descriptor() {
    return Download_Response_State_descriptor();
  }
  template<typename T>
  static inline const std::string& State_Name(T enum_t_value) {
    static_assert(::std::is_same<T, State>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function State_Name.");
    return Download_Response_State_Name(enum_t_value);
  }
  static inline bool State_Parse(const std::string& name,
      State* value) {
    return Download_Response_State_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kPercentFieldNumber = 2,
    kStateFieldNumber = 3,
  };
  // required int32 id = 1;
  bool has_id() const;
  private:
  bool _internal_has_id() const;
  public:
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // required float percent = 2;
  bool has_percent() const;
  private:
  bool _internal_has_percent() const;
  public:
  void clear_percent();
  float percent() const;
  void set_percent(float value);
  private:
  float _internal_percent() const;
  void _internal_set_percent(float value);
  public:

  // required .downloadmessage.Download_Response.State state = 3;
  bool has_state() const;
  private:
  bool _internal_has_state() const;
  public:
  void clear_state();
  ::downloadmessage::Download_Response_State state() const;
  void set_state(::downloadmessage::Download_Response_State value);
  private:
  ::downloadmessage::Download_Response_State _internal_state() const;
  void _internal_set_state(::downloadmessage::Download_Response_State value);
  public:

  // @@protoc_insertion_point(class_scope:downloadmessage.Download_Response)
 private:
  class _Internal;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  float percent_;
  int state_;
  friend struct ::TableStruct_mess_5fwl_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Mess_WL

// required int32 len = 1;
inline bool Mess_WL::_internal_has_len() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool Mess_WL::has_len() const {
  return _internal_has_len();
}
inline void Mess_WL::clear_len() {
  len_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Mess_WL::_internal_len() const {
  return len_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Mess_WL::len() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Mess_WL.len)
  return _internal_len();
}
inline void Mess_WL::_internal_set_len(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  len_ = value;
}
inline void Mess_WL::set_len(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_len(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Mess_WL.len)
}

// required int32 id = 2;
inline bool Mess_WL::_internal_has_id() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool Mess_WL::has_id() const {
  return _internal_has_id();
}
inline void Mess_WL::clear_id() {
  id_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Mess_WL::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Mess_WL::id() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Mess_WL.id)
  return _internal_id();
}
inline void Mess_WL::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000004u;
  id_ = value;
}
inline void Mess_WL::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Mess_WL.id)
}

// required .downloadmessage.Mess_WL.DownloadCommand command = 3;
inline bool Mess_WL::_internal_has_command() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool Mess_WL::has_command() const {
  return _internal_has_command();
}
inline void Mess_WL::clear_command() {
  command_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::downloadmessage::Mess_WL_DownloadCommand Mess_WL::_internal_command() const {
  return static_cast< ::downloadmessage::Mess_WL_DownloadCommand >(command_);
}
inline ::downloadmessage::Mess_WL_DownloadCommand Mess_WL::command() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Mess_WL.command)
  return _internal_command();
}
inline void Mess_WL::_internal_set_command(::downloadmessage::Mess_WL_DownloadCommand value) {
  assert(::downloadmessage::Mess_WL_DownloadCommand_IsValid(value));
  _has_bits_[0] |= 0x00000008u;
  command_ = value;
}
inline void Mess_WL::set_command(::downloadmessage::Mess_WL_DownloadCommand value) {
  _internal_set_command(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Mess_WL.command)
}

// required string url = 4;
inline bool Mess_WL::_internal_has_url() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool Mess_WL::has_url() const {
  return _internal_has_url();
}
inline void Mess_WL::clear_url() {
  url_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& Mess_WL::url() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Mess_WL.url)
  return _internal_url();
}
inline void Mess_WL::set_url(const std::string& value) {
  _internal_set_url(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Mess_WL.url)
}
inline std::string* Mess_WL::mutable_url() {
  // @@protoc_insertion_point(field_mutable:downloadmessage.Mess_WL.url)
  return _internal_mutable_url();
}
inline const std::string& Mess_WL::_internal_url() const {
  return url_.GetNoArena();
}
inline void Mess_WL::_internal_set_url(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  url_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void Mess_WL::set_url(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  url_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:downloadmessage.Mess_WL.url)
}
inline void Mess_WL::set_url(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  url_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:downloadmessage.Mess_WL.url)
}
inline void Mess_WL::set_url(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  url_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:downloadmessage.Mess_WL.url)
}
inline std::string* Mess_WL::_internal_mutable_url() {
  _has_bits_[0] |= 0x00000001u;
  return url_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* Mess_WL::release_url() {
  // @@protoc_insertion_point(field_release:downloadmessage.Mess_WL.url)
  if (!has_url()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return url_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void Mess_WL::set_allocated_url(std::string* url) {
  if (url != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  url_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), url);
  // @@protoc_insertion_point(field_set_allocated:downloadmessage.Mess_WL.url)
}

// -------------------------------------------------------------------

// Download_Response

// required int32 id = 1;
inline bool Download_Response::_internal_has_id() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool Download_Response::has_id() const {
  return _internal_has_id();
}
inline void Download_Response::clear_id() {
  id_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Download_Response::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Download_Response::id() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Download_Response.id)
  return _internal_id();
}
inline void Download_Response::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000001u;
  id_ = value;
}
inline void Download_Response::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Download_Response.id)
}

// required float percent = 2;
inline bool Download_Response::_internal_has_percent() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool Download_Response::has_percent() const {
  return _internal_has_percent();
}
inline void Download_Response::clear_percent() {
  percent_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline float Download_Response::_internal_percent() const {
  return percent_;
}
inline float Download_Response::percent() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Download_Response.percent)
  return _internal_percent();
}
inline void Download_Response::_internal_set_percent(float value) {
  _has_bits_[0] |= 0x00000002u;
  percent_ = value;
}
inline void Download_Response::set_percent(float value) {
  _internal_set_percent(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Download_Response.percent)
}

// required .downloadmessage.Download_Response.State state = 3;
inline bool Download_Response::_internal_has_state() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool Download_Response::has_state() const {
  return _internal_has_state();
}
inline void Download_Response::clear_state() {
  state_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::downloadmessage::Download_Response_State Download_Response::_internal_state() const {
  return static_cast< ::downloadmessage::Download_Response_State >(state_);
}
inline ::downloadmessage::Download_Response_State Download_Response::state() const {
  // @@protoc_insertion_point(field_get:downloadmessage.Download_Response.state)
  return _internal_state();
}
inline void Download_Response::_internal_set_state(::downloadmessage::Download_Response_State value) {
  assert(::downloadmessage::Download_Response_State_IsValid(value));
  _has_bits_[0] |= 0x00000004u;
  state_ = value;
}
inline void Download_Response::set_state(::downloadmessage::Download_Response_State value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:downloadmessage.Download_Response.state)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace downloadmessage

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::downloadmessage::Mess_WL_DownloadCommand> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::downloadmessage::Mess_WL_DownloadCommand>() {
  return ::downloadmessage::Mess_WL_DownloadCommand_descriptor();
}
template <> struct is_proto_enum< ::downloadmessage::Download_Response_State> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::downloadmessage::Download_Response_State>() {
  return ::downloadmessage::Download_Response_State_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_mess_5fwl_2eproto
