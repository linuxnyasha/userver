#pragma once

/// @file storages/postgres/io/composite_types.hpp
/// @brief Composite types I/O support

#include <boost/pfr/precise.hpp>
#include <utility>

#include <storages/postgres/exceptions.hpp>
#include <storages/postgres/io/buffer_io_base.hpp>
#include <storages/postgres/io/field_buffer.hpp>
#include <storages/postgres/io/row_types.hpp>
#include <storages/postgres/io/traits.hpp>
#include <storages/postgres/io/type_mapping.hpp>
#include <storages/postgres/io/type_traits.hpp>
#include <storages/postgres/io/user_types.hpp>

#include <logging/log.hpp>

#include <utils/variadic_logic.hpp>

namespace storages::postgres::io {

// clang-format wraps snippet lines
// clang-format off
/// @page pg_composite_types µPg: Composite user types
///
/// The driver supports user-defined PostgreSQL composite types. The C++
/// counterpart type must satisfy the same requirements as for the row types,
/// (@ref pg_user_row_types) and must provide a specialisation of CppToUserPg
/// template (@ref pg_user_types).
///
/// Parsing a composite structure from PostgreSQL buffer will throw an error if
/// the number of fields in the postgres data is different from the number of
/// data members in target C++ type. This is the only sanity control for the
/// composite types. The driver doesn't check the data type oids, it's user's
/// responsibility to provide structures with compatible data members.
///
/// @par Examples from tests
///
/// @snippet storages/postgres/tests/composite_types_pg_test.cpp User type declaration
///
/// @warning The type mapping specialisation **must** be accessible at the
/// points where parsing/formatting of the C++ type is instantiated. The
/// header where the C++ type is declared is an appropriate place to do it.
///
/// @snippet storages/postgres/tests/composite_types_pg_test.cpp User type mapping
// clang-format on

namespace detail {

template <typename T>
struct CompositeBinaryParser : BufferParserBase<T> {
  using BaseType = BufferParserBase<T>;
  using RowType = io::RowType<T>;
  using IndexSequence = typename RowType::IndexSequence;

  using BaseType::BaseType;

  void operator()(FieldBuffer buffer, const TypeBufferCategory& categories) {
    Integer field_count{0};
    buffer.Read(field_count, BufferCategory::kPlainBuffer);

    if (field_count != RowType::size) {
      throw CompositeSizeMismatch(field_count, RowType::size);
    }

    ReadTuple(buffer, categories, RowType::GetTuple(this->value),
              IndexSequence{});
  }

 private:
  static constexpr std::size_t int_size = sizeof(Integer);

  template <typename U>
  void ReadField(FieldBuffer& buffer, const TypeBufferCategory& categories,
                 U& val) const {
    Integer field_type;
    buffer.Read(field_type, BufferCategory::kPlainBuffer);
    auto elem_category = GetTypeBufferCategory(categories, field_type);
    if (elem_category == BufferCategory::kNoParser) {
      throw LogicError{"Buffer category for oid " + std::to_string(field_type) +
                       " is unknown"};
    }
    buffer.ReadRaw(val, categories, elem_category);
  }
  template <typename Tuple, std::size_t... Indexes>
  void ReadTuple(FieldBuffer& buffer, const TypeBufferCategory& categories,
                 Tuple&& tuple, std::index_sequence<Indexes...>) const {
    (ReadField(buffer, categories,
               std::get<Indexes>(std::forward<Tuple>(tuple))),
     ...);
  }
};

template <typename T>
struct CompositeBinaryFormatter : BufferFormatterBase<T> {
  using BaseType = BufferFormatterBase<T>;
  using PgMapping = CppToPg<T>;
  using RowType = io::RowType<T>;
  using IndexSequence = typename RowType::IndexSequence;
  static constexpr std::size_t size = RowType::size;

  using BaseType::BaseType;

  template <typename Buffer>
  void operator()(const UserTypes& types, Buffer& buffer) const {
    const auto& type_desc =
        types.GetCompositeDescription(PgMapping::GetOid(types));
    if (type_desc.Size() != size) {
      throw CompositeSizeMismatch{type_desc.Size(), size};
    }
    // Number of fields
    WriteBinary(types, buffer, static_cast<Integer>(size));
    WriteTuple(types, type_desc, buffer, RowType::GetTuple(this->value),
               IndexSequence{});
  }

 private:
  template <typename Buffer, typename U>
  void WriteField(const UserTypes& types,
                  const CompositeTypeDescription& type_desc, std::size_t index,
                  Buffer& buffer, const U& val) const {
    Integer field_type = CppToPg<U>::GetOid(types);
    const auto& field_desc = type_desc[index];
    if (field_type != field_desc.type) {
      if (io::MappedToSameType(static_cast<PredefinedOids>(field_type),
                               static_cast<PredefinedOids>(field_desc.type))) {
        field_type = field_desc.type;
      } else {
        LOG_WARNING() << "Type mismatch for " << PgMapping::postgres_name.schema
                      << "." << PgMapping::postgres_name.name << " field "
                      << field_desc.name << ". In database the type oid is "
                      << field_desc.type << ", user supplied type oid is "
                      << field_type;
      }
    }
    WriteBinary(types, buffer, field_type);
    WriteRawBinary(types, buffer, val, field_type);
  }
  template <typename Buffer, typename Tuple, std::size_t... Indexes>
  void WriteTuple(const UserTypes& types,
                  const CompositeTypeDescription& type_desc, Buffer& buffer,
                  Tuple&& tuple, std::index_sequence<Indexes...>) const {
    (WriteField(types, type_desc, Indexes, buffer,
                std::get<Indexes>(std::forward<Tuple>(tuple))),
     ...);
  }
};

}  // namespace detail

namespace traits {

namespace detail {

template <typename T, DataFormat F>
constexpr bool DetectCompositeParsers() {
  if constexpr (kIsRowType<T>) {
    return TupleHasParsers<typename io::RowType<T>::TupleType, F>::value;
  }
  return false;
}

template <typename T, DataFormat F>
constexpr bool DetectCompositeFormatters() {
  if constexpr (kIsRowType<T>) {
    return TupleHasFormatters<typename io::RowType<T>::TupleType, F>::value;
  }
  return false;
}

template <typename T, DataFormat F>
struct CompositeHasParsers
    : std::integral_constant<bool, DetectCompositeParsers<T, F>()> {};
template <typename T, DataFormat F>
constexpr bool kCompositeHasParsers = CompositeHasParsers<T, F>::value;

template <typename T, DataFormat F>
struct CompositeHasFormatters
    : std::integral_constant<bool, DetectCompositeFormatters<T, F>()> {};
template <typename T, DataFormat F>
constexpr bool kCompositeHasFormatters = CompositeHasFormatters<T, F>::value;

}  // namespace detail

template <typename T>
struct Input<T, DataFormat::kBinaryDataFormat,
             std::enable_if_t<!detail::kCustomBinaryParserDefined<T> &&
                              kIsMappedToUserType<T> && kIsRowType<T>>> {
  static_assert(
      (detail::kCompositeHasParsers<T, DataFormat::kBinaryDataFormat> == true),
      "Not all composite type members have binary parsers");
  using type = io::detail::CompositeBinaryParser<T>;
};

template <typename T>
struct Output<T, DataFormat::kBinaryDataFormat,
              std::enable_if_t<!detail::kCustomBinaryFormatterDefined<T> &&
                               kIsMappedToUserType<T> && kIsRowType<T>>> {
  static_assert(
      (detail::kCompositeHasFormatters<T, DataFormat::kBinaryDataFormat> ==
       true),
      "Not all composite type members have binary formatters");
  using type = io::detail::CompositeBinaryFormatter<T>;
};

template <typename T>
struct ParserBufferCategory<io::detail::CompositeBinaryParser<T>>
    : std::integral_constant<BufferCategory, BufferCategory::kCompositeBuffer> {
};

}  // namespace traits
}  // namespace storages::postgres::io
