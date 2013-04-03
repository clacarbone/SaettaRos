// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "timetests.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace TimeTests {

namespace {

const ::google::protobuf::Descriptor* Machine_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Machine_reflection_ = NULL;
const ::google::protobuf::Descriptor* Traceroute_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Traceroute_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_timetests_2eproto() {
  protobuf_AddDesc_timetests_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "timetests.proto");
  GOOGLE_CHECK(file != NULL);
  Machine_descriptor_ = file->message_type(0);
  static const int Machine_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, machine_name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, ntp_delay_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, ntp_offset_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, ntp_jitter_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, timestamp_),
  };
  Machine_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Machine_descriptor_,
      Machine::default_instance_,
      Machine_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Machine, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Machine));
  Traceroute_descriptor_ = file->message_type(1);
  static const int Traceroute_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Traceroute, header_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Traceroute, machine_),
  };
  Traceroute_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Traceroute_descriptor_,
      Traceroute::default_instance_,
      Traceroute_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Traceroute, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Traceroute, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Traceroute));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_timetests_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Machine_descriptor_, &Machine::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Traceroute_descriptor_, &Traceroute::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_timetests_2eproto() {
  delete Machine::default_instance_;
  delete Machine_reflection_;
  delete Traceroute::default_instance_;
  delete Traceroute_reflection_;
}

void protobuf_AddDesc_timetests_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017timetests.proto\022\tTimeTests\"m\n\007Machine\022"
    "\024\n\014machine_name\030\001 \002(\t\022\021\n\tntp_delay\030\002 \002(\002"
    "\022\022\n\nntp_offset\030\003 \002(\002\022\022\n\nntp_jitter\030\004 \002(\002"
    "\022\021\n\ttimestamp\030\005 \002(\004\"A\n\nTraceroute\022\016\n\006hea"
    "der\030\001 \002(\t\022#\n\007machine\030\002 \003(\0132\022.TimeTests.M"
    "achine", 206);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "timetests.proto", &protobuf_RegisterTypes);
  Machine::default_instance_ = new Machine();
  Traceroute::default_instance_ = new Traceroute();
  Machine::default_instance_->InitAsDefaultInstance();
  Traceroute::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_timetests_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_timetests_2eproto {
  StaticDescriptorInitializer_timetests_2eproto() {
    protobuf_AddDesc_timetests_2eproto();
  }
} static_descriptor_initializer_timetests_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Machine::kMachineNameFieldNumber;
const int Machine::kNtpDelayFieldNumber;
const int Machine::kNtpOffsetFieldNumber;
const int Machine::kNtpJitterFieldNumber;
const int Machine::kTimestampFieldNumber;
#endif  // !_MSC_VER

Machine::Machine()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Machine::InitAsDefaultInstance() {
}

Machine::Machine(const Machine& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Machine::SharedCtor() {
  _cached_size_ = 0;
  machine_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ntp_delay_ = 0;
  ntp_offset_ = 0;
  ntp_jitter_ = 0;
  timestamp_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Machine::~Machine() {
  SharedDtor();
}

void Machine::SharedDtor() {
  if (machine_name_ != &::google::protobuf::internal::kEmptyString) {
    delete machine_name_;
  }
  if (this != default_instance_) {
  }
}

void Machine::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Machine::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Machine_descriptor_;
}

const Machine& Machine::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_timetests_2eproto();  return *default_instance_;
}

Machine* Machine::default_instance_ = NULL;

Machine* Machine::New() const {
  return new Machine;
}

void Machine::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_machine_name()) {
      if (machine_name_ != &::google::protobuf::internal::kEmptyString) {
        machine_name_->clear();
      }
    }
    ntp_delay_ = 0;
    ntp_offset_ = 0;
    ntp_jitter_ = 0;
    timestamp_ = GOOGLE_ULONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Machine::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string machine_name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_machine_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->machine_name().data(), this->machine_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(21)) goto parse_ntp_delay;
        break;
      }
      
      // required float ntp_delay = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_ntp_delay:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &ntp_delay_)));
          set_has_ntp_delay();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(29)) goto parse_ntp_offset;
        break;
      }
      
      // required float ntp_offset = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_ntp_offset:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &ntp_offset_)));
          set_has_ntp_offset();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(37)) goto parse_ntp_jitter;
        break;
      }
      
      // required float ntp_jitter = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_ntp_jitter:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &ntp_jitter_)));
          set_has_ntp_jitter();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_timestamp;
        break;
      }
      
      // required uint64 timestamp = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_timestamp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &timestamp_)));
          set_has_timestamp();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Machine::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string machine_name = 1;
  if (has_machine_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->machine_name().data(), this->machine_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->machine_name(), output);
  }
  
  // required float ntp_delay = 2;
  if (has_ntp_delay()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(2, this->ntp_delay(), output);
  }
  
  // required float ntp_offset = 3;
  if (has_ntp_offset()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->ntp_offset(), output);
  }
  
  // required float ntp_jitter = 4;
  if (has_ntp_jitter()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(4, this->ntp_jitter(), output);
  }
  
  // required uint64 timestamp = 5;
  if (has_timestamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(5, this->timestamp(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Machine::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string machine_name = 1;
  if (has_machine_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->machine_name().data(), this->machine_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->machine_name(), target);
  }
  
  // required float ntp_delay = 2;
  if (has_ntp_delay()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(2, this->ntp_delay(), target);
  }
  
  // required float ntp_offset = 3;
  if (has_ntp_offset()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(3, this->ntp_offset(), target);
  }
  
  // required float ntp_jitter = 4;
  if (has_ntp_jitter()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(4, this->ntp_jitter(), target);
  }
  
  // required uint64 timestamp = 5;
  if (has_timestamp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(5, this->timestamp(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Machine::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string machine_name = 1;
    if (has_machine_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->machine_name());
    }
    
    // required float ntp_delay = 2;
    if (has_ntp_delay()) {
      total_size += 1 + 4;
    }
    
    // required float ntp_offset = 3;
    if (has_ntp_offset()) {
      total_size += 1 + 4;
    }
    
    // required float ntp_jitter = 4;
    if (has_ntp_jitter()) {
      total_size += 1 + 4;
    }
    
    // required uint64 timestamp = 5;
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->timestamp());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Machine::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Machine* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Machine*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Machine::MergeFrom(const Machine& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_machine_name()) {
      set_machine_name(from.machine_name());
    }
    if (from.has_ntp_delay()) {
      set_ntp_delay(from.ntp_delay());
    }
    if (from.has_ntp_offset()) {
      set_ntp_offset(from.ntp_offset());
    }
    if (from.has_ntp_jitter()) {
      set_ntp_jitter(from.ntp_jitter());
    }
    if (from.has_timestamp()) {
      set_timestamp(from.timestamp());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Machine::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Machine::CopyFrom(const Machine& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Machine::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;
  
  return true;
}

void Machine::Swap(Machine* other) {
  if (other != this) {
    std::swap(machine_name_, other->machine_name_);
    std::swap(ntp_delay_, other->ntp_delay_);
    std::swap(ntp_offset_, other->ntp_offset_);
    std::swap(ntp_jitter_, other->ntp_jitter_);
    std::swap(timestamp_, other->timestamp_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Machine::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Machine_descriptor_;
  metadata.reflection = Machine_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Traceroute::kHeaderFieldNumber;
const int Traceroute::kMachineFieldNumber;
#endif  // !_MSC_VER

Traceroute::Traceroute()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Traceroute::InitAsDefaultInstance() {
}

Traceroute::Traceroute(const Traceroute& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Traceroute::SharedCtor() {
  _cached_size_ = 0;
  header_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Traceroute::~Traceroute() {
  SharedDtor();
}

void Traceroute::SharedDtor() {
  if (header_ != &::google::protobuf::internal::kEmptyString) {
    delete header_;
  }
  if (this != default_instance_) {
  }
}

void Traceroute::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Traceroute::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Traceroute_descriptor_;
}

const Traceroute& Traceroute::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_timetests_2eproto();  return *default_instance_;
}

Traceroute* Traceroute::default_instance_ = NULL;

Traceroute* Traceroute::New() const {
  return new Traceroute;
}

void Traceroute::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_header()) {
      if (header_ != &::google::protobuf::internal::kEmptyString) {
        header_->clear();
      }
    }
  }
  machine_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Traceroute::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string header = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_header()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->header().data(), this->header().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_machine;
        break;
      }
      
      // repeated .TimeTests.Machine machine = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_machine:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_machine()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_machine;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Traceroute::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string header = 1;
  if (has_header()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->header().data(), this->header().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->header(), output);
  }
  
  // repeated .TimeTests.Machine machine = 2;
  for (int i = 0; i < this->machine_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->machine(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Traceroute::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string header = 1;
  if (has_header()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->header().data(), this->header().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->header(), target);
  }
  
  // repeated .TimeTests.Machine machine = 2;
  for (int i = 0; i < this->machine_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->machine(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Traceroute::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string header = 1;
    if (has_header()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->header());
    }
    
  }
  // repeated .TimeTests.Machine machine = 2;
  total_size += 1 * this->machine_size();
  for (int i = 0; i < this->machine_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->machine(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Traceroute::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Traceroute* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Traceroute*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Traceroute::MergeFrom(const Traceroute& from) {
  GOOGLE_CHECK_NE(&from, this);
  machine_.MergeFrom(from.machine_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_header()) {
      set_header(from.header());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Traceroute::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Traceroute::CopyFrom(const Traceroute& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Traceroute::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  for (int i = 0; i < machine_size(); i++) {
    if (!this->machine(i).IsInitialized()) return false;
  }
  return true;
}

void Traceroute::Swap(Traceroute* other) {
  if (other != this) {
    std::swap(header_, other->header_);
    machine_.Swap(&other->machine_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Traceroute::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Traceroute_descriptor_;
  metadata.reflection = Traceroute_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace TimeTests

// @@protoc_insertion_point(global_scope)
