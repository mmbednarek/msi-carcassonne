// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Carcassonne.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Carcassonne_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Carcassonne_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019001 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Carcassonne_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Carcassonne_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Carcassonne_2eproto;
namespace carcassonne {
namespace proto {
class Move;
struct MoveDefaultTypeInternal;
extern MoveDefaultTypeInternal _Move_default_instance_;
class Rollout;
struct RolloutDefaultTypeInternal;
extern RolloutDefaultTypeInternal _Rollout_default_instance_;
}  // namespace proto
}  // namespace carcassonne
PROTOBUF_NAMESPACE_OPEN
template<> ::carcassonne::proto::Move* Arena::CreateMaybeMessage<::carcassonne::proto::Move>(Arena*);
template<> ::carcassonne::proto::Rollout* Arena::CreateMaybeMessage<::carcassonne::proto::Rollout>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace carcassonne {
namespace proto {

enum Direction : int {
  North = 0,
  East = 1,
  South = 2,
  West = 3,
  Middle = 4,
  NorthEast = 5,
  EastNorth = 6,
  EastSouth = 7,
  SouthEast = 8,
  SouthWest = 9,
  WestSouth = 10,
  WestNorth = 11,
  NorthWest = 12,
  Direction_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  Direction_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool Direction_IsValid(int value);
constexpr Direction Direction_MIN = North;
constexpr Direction Direction_MAX = NorthWest;
constexpr int Direction_ARRAYSIZE = Direction_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Direction_descriptor();
template<typename T>
inline const std::string& Direction_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Direction>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Direction_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Direction_descriptor(), enum_t_value);
}
inline bool Direction_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, Direction* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Direction>(
    Direction_descriptor(), name, value);
}
// ===================================================================

class Move final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:carcassonne.proto.Move) */ {
 public:
  inline Move() : Move(nullptr) {}
  ~Move() override;
  explicit constexpr Move(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Move(const Move& from);
  Move(Move&& from) noexcept
    : Move() {
    *this = ::std::move(from);
  }

  inline Move& operator=(const Move& from) {
    CopyFrom(from);
    return *this;
  }
  inline Move& operator=(Move&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Move& default_instance() {
    return *internal_default_instance();
  }
  static inline const Move* internal_default_instance() {
    return reinterpret_cast<const Move*>(
               &_Move_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Move& a, Move& b) {
    a.Swap(&b);
  }
  inline void Swap(Move* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Move* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Move* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Move>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Move& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Move& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Move* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "carcassonne.proto.Move";
  }
  protected:
  explicit Move(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kXFieldNumber = 1,
    kYFieldNumber = 2,
    kRotationFieldNumber = 3,
    kSkipFigureFieldNumber = 4,
    kDirectionFieldNumber = 5,
  };
  // int32 x = 1;
  void clear_x();
  int32_t x() const;
  void set_x(int32_t value);
  private:
  int32_t _internal_x() const;
  void _internal_set_x(int32_t value);
  public:

  // int32 y = 2;
  void clear_y();
  int32_t y() const;
  void set_y(int32_t value);
  private:
  int32_t _internal_y() const;
  void _internal_set_y(int32_t value);
  public:

  // int32 rotation = 3;
  void clear_rotation();
  int32_t rotation() const;
  void set_rotation(int32_t value);
  private:
  int32_t _internal_rotation() const;
  void _internal_set_rotation(int32_t value);
  public:

  // bool skip_figure = 4;
  void clear_skip_figure();
  bool skip_figure() const;
  void set_skip_figure(bool value);
  private:
  bool _internal_skip_figure() const;
  void _internal_set_skip_figure(bool value);
  public:

  // .carcassonne.proto.Direction direction = 5;
  void clear_direction();
  ::carcassonne::proto::Direction direction() const;
  void set_direction(::carcassonne::proto::Direction value);
  private:
  ::carcassonne::proto::Direction _internal_direction() const;
  void _internal_set_direction(::carcassonne::proto::Direction value);
  public:

  // @@protoc_insertion_point(class_scope:carcassonne.proto.Move)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t x_;
  int32_t y_;
  int32_t rotation_;
  bool skip_figure_;
  int direction_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Carcassonne_2eproto;
};
// -------------------------------------------------------------------

class Rollout final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:carcassonne.proto.Rollout) */ {
 public:
  inline Rollout() : Rollout(nullptr) {}
  ~Rollout() override;
  explicit constexpr Rollout(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Rollout(const Rollout& from);
  Rollout(Rollout&& from) noexcept
    : Rollout() {
    *this = ::std::move(from);
  }

  inline Rollout& operator=(const Rollout& from) {
    CopyFrom(from);
    return *this;
  }
  inline Rollout& operator=(Rollout&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Rollout& default_instance() {
    return *internal_default_instance();
  }
  static inline const Rollout* internal_default_instance() {
    return reinterpret_cast<const Rollout*>(
               &_Rollout_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Rollout& a, Rollout& b) {
    a.Swap(&b);
  }
  inline void Swap(Rollout* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Rollout* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Rollout* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Rollout>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Rollout& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Rollout& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Rollout* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "carcassonne.proto.Rollout";
  }
  protected:
  explicit Rollout(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMovesFieldNumber = 3,
    kSeedFieldNumber = 2,
    kPlayerCountFieldNumber = 1,
  };
  // repeated .carcassonne.proto.Move moves = 3;
  int moves_size() const;
  private:
  int _internal_moves_size() const;
  public:
  void clear_moves();
  ::carcassonne::proto::Move* mutable_moves(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::carcassonne::proto::Move >*
      mutable_moves();
  private:
  const ::carcassonne::proto::Move& _internal_moves(int index) const;
  ::carcassonne::proto::Move* _internal_add_moves();
  public:
  const ::carcassonne::proto::Move& moves(int index) const;
  ::carcassonne::proto::Move* add_moves();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::carcassonne::proto::Move >&
      moves() const;

  // uint64 seed = 2;
  void clear_seed();
  uint64_t seed() const;
  void set_seed(uint64_t value);
  private:
  uint64_t _internal_seed() const;
  void _internal_set_seed(uint64_t value);
  public:

  // int32 player_count = 1;
  void clear_player_count();
  int32_t player_count() const;
  void set_player_count(int32_t value);
  private:
  int32_t _internal_player_count() const;
  void _internal_set_player_count(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:carcassonne.proto.Rollout)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::carcassonne::proto::Move > moves_;
  uint64_t seed_;
  int32_t player_count_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Carcassonne_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Move

// int32 x = 1;
inline void Move::clear_x() {
  x_ = 0;
}
inline int32_t Move::_internal_x() const {
  return x_;
}
inline int32_t Move::x() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Move.x)
  return _internal_x();
}
inline void Move::_internal_set_x(int32_t value) {
  
  x_ = value;
}
inline void Move::set_x(int32_t value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Move.x)
}

// int32 y = 2;
inline void Move::clear_y() {
  y_ = 0;
}
inline int32_t Move::_internal_y() const {
  return y_;
}
inline int32_t Move::y() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Move.y)
  return _internal_y();
}
inline void Move::_internal_set_y(int32_t value) {
  
  y_ = value;
}
inline void Move::set_y(int32_t value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Move.y)
}

// int32 rotation = 3;
inline void Move::clear_rotation() {
  rotation_ = 0;
}
inline int32_t Move::_internal_rotation() const {
  return rotation_;
}
inline int32_t Move::rotation() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Move.rotation)
  return _internal_rotation();
}
inline void Move::_internal_set_rotation(int32_t value) {
  
  rotation_ = value;
}
inline void Move::set_rotation(int32_t value) {
  _internal_set_rotation(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Move.rotation)
}

// bool skip_figure = 4;
inline void Move::clear_skip_figure() {
  skip_figure_ = false;
}
inline bool Move::_internal_skip_figure() const {
  return skip_figure_;
}
inline bool Move::skip_figure() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Move.skip_figure)
  return _internal_skip_figure();
}
inline void Move::_internal_set_skip_figure(bool value) {
  
  skip_figure_ = value;
}
inline void Move::set_skip_figure(bool value) {
  _internal_set_skip_figure(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Move.skip_figure)
}

// .carcassonne.proto.Direction direction = 5;
inline void Move::clear_direction() {
  direction_ = 0;
}
inline ::carcassonne::proto::Direction Move::_internal_direction() const {
  return static_cast< ::carcassonne::proto::Direction >(direction_);
}
inline ::carcassonne::proto::Direction Move::direction() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Move.direction)
  return _internal_direction();
}
inline void Move::_internal_set_direction(::carcassonne::proto::Direction value) {
  
  direction_ = value;
}
inline void Move::set_direction(::carcassonne::proto::Direction value) {
  _internal_set_direction(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Move.direction)
}

// -------------------------------------------------------------------

// Rollout

// int32 player_count = 1;
inline void Rollout::clear_player_count() {
  player_count_ = 0;
}
inline int32_t Rollout::_internal_player_count() const {
  return player_count_;
}
inline int32_t Rollout::player_count() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Rollout.player_count)
  return _internal_player_count();
}
inline void Rollout::_internal_set_player_count(int32_t value) {
  
  player_count_ = value;
}
inline void Rollout::set_player_count(int32_t value) {
  _internal_set_player_count(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Rollout.player_count)
}

// uint64 seed = 2;
inline void Rollout::clear_seed() {
  seed_ = uint64_t{0u};
}
inline uint64_t Rollout::_internal_seed() const {
  return seed_;
}
inline uint64_t Rollout::seed() const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Rollout.seed)
  return _internal_seed();
}
inline void Rollout::_internal_set_seed(uint64_t value) {
  
  seed_ = value;
}
inline void Rollout::set_seed(uint64_t value) {
  _internal_set_seed(value);
  // @@protoc_insertion_point(field_set:carcassonne.proto.Rollout.seed)
}

// repeated .carcassonne.proto.Move moves = 3;
inline int Rollout::_internal_moves_size() const {
  return moves_.size();
}
inline int Rollout::moves_size() const {
  return _internal_moves_size();
}
inline void Rollout::clear_moves() {
  moves_.Clear();
}
inline ::carcassonne::proto::Move* Rollout::mutable_moves(int index) {
  // @@protoc_insertion_point(field_mutable:carcassonne.proto.Rollout.moves)
  return moves_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::carcassonne::proto::Move >*
Rollout::mutable_moves() {
  // @@protoc_insertion_point(field_mutable_list:carcassonne.proto.Rollout.moves)
  return &moves_;
}
inline const ::carcassonne::proto::Move& Rollout::_internal_moves(int index) const {
  return moves_.Get(index);
}
inline const ::carcassonne::proto::Move& Rollout::moves(int index) const {
  // @@protoc_insertion_point(field_get:carcassonne.proto.Rollout.moves)
  return _internal_moves(index);
}
inline ::carcassonne::proto::Move* Rollout::_internal_add_moves() {
  return moves_.Add();
}
inline ::carcassonne::proto::Move* Rollout::add_moves() {
  ::carcassonne::proto::Move* _add = _internal_add_moves();
  // @@protoc_insertion_point(field_add:carcassonne.proto.Rollout.moves)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::carcassonne::proto::Move >&
Rollout::moves() const {
  // @@protoc_insertion_point(field_list:carcassonne.proto.Rollout.moves)
  return moves_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace carcassonne

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::carcassonne::proto::Direction> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::carcassonne::proto::Direction>() {
  return ::carcassonne::proto::Direction_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Carcassonne_2eproto
