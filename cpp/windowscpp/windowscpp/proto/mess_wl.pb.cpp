// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mess_wl.proto

#include "mess_wl.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace downloadmessage {
class Mess_WLDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Mess_WL> _instance;
} _Mess_WL_default_instance_;
class Download_ResponseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Download_Response> _instance;
} _Download_Response_default_instance_;
}  // namespace downloadmessage
static void InitDefaultsscc_info_Download_Response_mess_5fwl_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::downloadmessage::_Download_Response_default_instance_;
    new (ptr) ::downloadmessage::Download_Response();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::downloadmessage::Download_Response::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Download_Response_mess_5fwl_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Download_Response_mess_5fwl_2eproto}, {}};

static void InitDefaultsscc_info_Mess_WL_mess_5fwl_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::downloadmessage::_Mess_WL_default_instance_;
    new (ptr) ::downloadmessage::Mess_WL();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::downloadmessage::Mess_WL::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Mess_WL_mess_5fwl_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Mess_WL_mess_5fwl_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_mess_5fwl_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_mess_5fwl_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_mess_5fwl_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_mess_5fwl_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, len_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, id_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, command_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Mess_WL, url_),
  1,
  2,
  3,
  0,
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Download_Response, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Download_Response, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Download_Response, id_),
  PROTOBUF_FIELD_OFFSET(::downloadmessage::Download_Response, percent_),
  0,
  1,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 9, sizeof(::downloadmessage::Mess_WL)},
  { 13, 20, sizeof(::downloadmessage::Download_Response)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::downloadmessage::_Mess_WL_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::downloadmessage::_Download_Response_default_instance_),
};

const char descriptor_table_protodef_mess_5fwl_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rmess_wl.proto\022\017downloadmessage\"\256\001\n\007Mes"
  "s_WL\022\013\n\003len\030\001 \002(\005\022\n\n\002id\030\002 \002(\005\0229\n\007command"
  "\030\003 \002(\0162(.downloadmessage.Mess_WL.Downloa"
  "dCommand\022\013\n\003url\030\004 \002(\t\"B\n\017DownloadCommand"
  "\022\014\n\010DOWNLOAD\020\000\022\t\n\005PAUSE\020\001\022\n\n\006RESUME\020\002\022\n\n"
  "\006REMOVE\020\003\"c\n\021Download_Response\022\n\n\002id\030\001 \002"
  "(\005\022\017\n\007percent\030\002 \002(\002\"1\n\005State\022\017\n\013DOWNLOAD"
  "ING\020\000\022\n\n\006PAUSED\020\001\022\013\n\007REMOVED\020\002"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_mess_5fwl_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_mess_5fwl_2eproto_sccs[2] = {
  &scc_info_Download_Response_mess_5fwl_2eproto.base,
  &scc_info_Mess_WL_mess_5fwl_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_mess_5fwl_2eproto_once;
static bool descriptor_table_mess_5fwl_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_mess_5fwl_2eproto = {
  &descriptor_table_mess_5fwl_2eproto_initialized, descriptor_table_protodef_mess_5fwl_2eproto, "mess_wl.proto", 310,
  &descriptor_table_mess_5fwl_2eproto_once, descriptor_table_mess_5fwl_2eproto_sccs, descriptor_table_mess_5fwl_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_mess_5fwl_2eproto::offsets,
  file_level_metadata_mess_5fwl_2eproto, 2, file_level_enum_descriptors_mess_5fwl_2eproto, file_level_service_descriptors_mess_5fwl_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_mess_5fwl_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_mess_5fwl_2eproto), true);
namespace downloadmessage {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Mess_WL_DownloadCommand_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_mess_5fwl_2eproto);
  return file_level_enum_descriptors_mess_5fwl_2eproto[0];
}
bool Mess_WL_DownloadCommand_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Mess_WL_DownloadCommand Mess_WL::DOWNLOAD;
constexpr Mess_WL_DownloadCommand Mess_WL::PAUSE;
constexpr Mess_WL_DownloadCommand Mess_WL::RESUME;
constexpr Mess_WL_DownloadCommand Mess_WL::REMOVE;
constexpr Mess_WL_DownloadCommand Mess_WL::DownloadCommand_MIN;
constexpr Mess_WL_DownloadCommand Mess_WL::DownloadCommand_MAX;
constexpr int Mess_WL::DownloadCommand_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Download_Response_State_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_mess_5fwl_2eproto);
  return file_level_enum_descriptors_mess_5fwl_2eproto[1];
}
bool Download_Response_State_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Download_Response_State Download_Response::DOWNLOADING;
constexpr Download_Response_State Download_Response::PAUSED;
constexpr Download_Response_State Download_Response::REMOVED;
constexpr Download_Response_State Download_Response::State_MIN;
constexpr Download_Response_State Download_Response::State_MAX;
constexpr int Download_Response::State_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void Mess_WL::InitAsDefaultInstance() {
}
class Mess_WL::_Internal {
 public:
  using HasBits = decltype(std::declval<Mess_WL>()._has_bits_);
  static void set_has_len(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_id(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_command(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_url(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

Mess_WL::Mess_WL()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:downloadmessage.Mess_WL)
}
Mess_WL::Mess_WL(const Mess_WL& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  url_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_url()) {
    url_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.url_);
  }
  ::memcpy(&len_, &from.len_,
    static_cast<size_t>(reinterpret_cast<char*>(&command_) -
    reinterpret_cast<char*>(&len_)) + sizeof(command_));
  // @@protoc_insertion_point(copy_constructor:downloadmessage.Mess_WL)
}

void Mess_WL::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Mess_WL_mess_5fwl_2eproto.base);
  url_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&len_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&command_) -
      reinterpret_cast<char*>(&len_)) + sizeof(command_));
}

Mess_WL::~Mess_WL() {
  // @@protoc_insertion_point(destructor:downloadmessage.Mess_WL)
  SharedDtor();
}

void Mess_WL::SharedDtor() {
  url_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void Mess_WL::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Mess_WL& Mess_WL::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Mess_WL_mess_5fwl_2eproto.base);
  return *internal_default_instance();
}


void Mess_WL::Clear() {
// @@protoc_insertion_point(message_clear_start:downloadmessage.Mess_WL)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    url_.ClearNonDefaultToEmptyNoArena();
  }
  if (cached_has_bits & 0x0000000eu) {
    ::memset(&len_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&command_) -
        reinterpret_cast<char*>(&len_)) + sizeof(command_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* Mess_WL::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required int32 len = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_len(&has_bits);
          len_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required int32 id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_id(&has_bits);
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required .downloadmessage.Mess_WL.DownloadCommand command = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::downloadmessage::Mess_WL_DownloadCommand_IsValid(val))) {
            _internal_set_command(static_cast<::downloadmessage::Mess_WL_DownloadCommand>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(3, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // required string url = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParserUTF8Verify(_internal_mutable_url(), ptr, ctx, "downloadmessage.Mess_WL.url");
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Mess_WL::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:downloadmessage.Mess_WL)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 len = 1;
  if (cached_has_bits & 0x00000002u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_len(), target);
  }

  // required int32 id = 2;
  if (cached_has_bits & 0x00000004u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_id(), target);
  }

  // required .downloadmessage.Mess_WL.DownloadCommand command = 3;
  if (cached_has_bits & 0x00000008u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      3, this->_internal_command(), target);
  }

  // required string url = 4;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_url().data(), static_cast<int>(this->_internal_url().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "downloadmessage.Mess_WL.url");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_url(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:downloadmessage.Mess_WL)
  return target;
}

size_t Mess_WL::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:downloadmessage.Mess_WL)
  size_t total_size = 0;

  if (has_url()) {
    // required string url = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_url());
  }

  if (has_len()) {
    // required int32 len = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_len());
  }

  if (has_id()) {
    // required int32 id = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_id());
  }

  if (has_command()) {
    // required .downloadmessage.Mess_WL.DownloadCommand command = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_command());
  }

  return total_size;
}
size_t Mess_WL::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:downloadmessage.Mess_WL)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000000f) ^ 0x0000000f) == 0) {  // All required fields are present.
    // required string url = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_url());

    // required int32 len = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_len());

    // required int32 id = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_id());

    // required .downloadmessage.Mess_WL.DownloadCommand command = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_command());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Mess_WL::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:downloadmessage.Mess_WL)
  GOOGLE_DCHECK_NE(&from, this);
  const Mess_WL* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Mess_WL>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:downloadmessage.Mess_WL)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:downloadmessage.Mess_WL)
    MergeFrom(*source);
  }
}

void Mess_WL::MergeFrom(const Mess_WL& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:downloadmessage.Mess_WL)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      url_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.url_);
    }
    if (cached_has_bits & 0x00000002u) {
      len_ = from.len_;
    }
    if (cached_has_bits & 0x00000004u) {
      id_ = from.id_;
    }
    if (cached_has_bits & 0x00000008u) {
      command_ = from.command_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Mess_WL::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:downloadmessage.Mess_WL)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Mess_WL::CopyFrom(const Mess_WL& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:downloadmessage.Mess_WL)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Mess_WL::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000000f) != 0x0000000f) return false;
  return true;
}

void Mess_WL::InternalSwap(Mess_WL* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  url_.Swap(&other->url_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(len_, other->len_);
  swap(id_, other->id_);
  swap(command_, other->command_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Mess_WL::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void Download_Response::InitAsDefaultInstance() {
}
class Download_Response::_Internal {
 public:
  using HasBits = decltype(std::declval<Download_Response>()._has_bits_);
  static void set_has_id(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_percent(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

Download_Response::Download_Response()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:downloadmessage.Download_Response)
}
Download_Response::Download_Response(const Download_Response& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&percent_) -
    reinterpret_cast<char*>(&id_)) + sizeof(percent_));
  // @@protoc_insertion_point(copy_constructor:downloadmessage.Download_Response)
}

void Download_Response::SharedCtor() {
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&percent_) -
      reinterpret_cast<char*>(&id_)) + sizeof(percent_));
}

Download_Response::~Download_Response() {
  // @@protoc_insertion_point(destructor:downloadmessage.Download_Response)
  SharedDtor();
}

void Download_Response::SharedDtor() {
}

void Download_Response::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Download_Response& Download_Response::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Download_Response_mess_5fwl_2eproto.base);
  return *internal_default_instance();
}


void Download_Response::Clear() {
// @@protoc_insertion_point(message_clear_start:downloadmessage.Download_Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&id_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&percent_) -
        reinterpret_cast<char*>(&id_)) + sizeof(percent_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* Download_Response::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required int32 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_id(&has_bits);
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required float percent = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 21)) {
          _Internal::set_has_percent(&has_bits);
          percent_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Download_Response::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:downloadmessage.Download_Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 id = 1;
  if (cached_has_bits & 0x00000001u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_id(), target);
  }

  // required float percent = 2;
  if (cached_has_bits & 0x00000002u) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(2, this->_internal_percent(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:downloadmessage.Download_Response)
  return target;
}

size_t Download_Response::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:downloadmessage.Download_Response)
  size_t total_size = 0;

  if (has_id()) {
    // required int32 id = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_id());
  }

  if (has_percent()) {
    // required float percent = 2;
    total_size += 1 + 4;
  }

  return total_size;
}
size_t Download_Response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:downloadmessage.Download_Response)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000003) ^ 0x00000003) == 0) {  // All required fields are present.
    // required int32 id = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_id());

    // required float percent = 2;
    total_size += 1 + 4;

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Download_Response::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:downloadmessage.Download_Response)
  GOOGLE_DCHECK_NE(&from, this);
  const Download_Response* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Download_Response>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:downloadmessage.Download_Response)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:downloadmessage.Download_Response)
    MergeFrom(*source);
  }
}

void Download_Response::MergeFrom(const Download_Response& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:downloadmessage.Download_Response)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      id_ = from.id_;
    }
    if (cached_has_bits & 0x00000002u) {
      percent_ = from.percent_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Download_Response::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:downloadmessage.Download_Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Download_Response::CopyFrom(const Download_Response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:downloadmessage.Download_Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Download_Response::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  return true;
}

void Download_Response::InternalSwap(Download_Response* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  swap(id_, other->id_);
  swap(percent_, other->percent_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Download_Response::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace downloadmessage
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::downloadmessage::Mess_WL* Arena::CreateMaybeMessage< ::downloadmessage::Mess_WL >(Arena* arena) {
  return Arena::CreateInternal< ::downloadmessage::Mess_WL >(arena);
}
template<> PROTOBUF_NOINLINE ::downloadmessage::Download_Response* Arena::CreateMaybeMessage< ::downloadmessage::Download_Response >(Arena* arena) {
  return Arena::CreateInternal< ::downloadmessage::Download_Response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
