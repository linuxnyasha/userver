
#pragma once
#include <userver/formats/universal/universal.hpp>
#include <userver/utils/regex.hpp>
#include <fmt/format.h>
#include <map>

USERVER_NAMESPACE_BEGIN
namespace formats::universal {

template <auto Value>
struct Min {
  static constexpr auto kValue = Value;
};

template <auto Value>
struct Max {
  static constexpr auto kValue = Value;
};

template <auto Value>
struct Default {
  static constexpr auto kValue = Value;
};

template <utils::ConstexprString Regex>
struct Pattern {
  static constexpr auto kValue = Regex;
};

struct Additional {};

template <typename Field, auto Value>
constexpr inline auto Check(const Field& field, Max<Value>) noexcept {
  return Value >= field;
};

template <typename Field, auto Value>
constexpr inline auto Check(const Field& field, Min<Value>) noexcept {
  return field >= Value;
};

template <typename Field, auto Value>
constexpr inline auto Check(const Field&, Default<Value>) noexcept {
  return true;
};

template <utils::ConstexprString Regex>
static const userver::utils::regex kRegex(Regex);

template <utils::ConstexprString Regex>
constexpr inline auto Check(const std::string& field, Pattern<Regex>) noexcept {
  return utils::regex_match(field, kRegex<Regex>);
};

template <typename Field, auto Value>
constexpr inline auto Check(const std::optional<Field>&, Default<Value>) noexcept {
  return true;
};

template <typename Field>
constexpr inline auto Check(const Field&, Additional) noexcept {
  return true;
};

template <typename Key, auto Value>
constexpr inline auto Check(const std::vector<Key>& field, Max<Value>) noexcept {
  return Value >= field.size();
};

template <typename Key, typename Tp, auto Value>
constexpr inline auto Check(const std::map<Key, Tp>& field, Max<Value>) noexcept {
  return Value >= field.size();
};

template <typename Key, typename Tp, auto Value>
constexpr inline auto Check(const std::unordered_map<Key, Tp>& field, Max<Value>) noexcept {
  return Value >= field.size();
};

template <typename Field, typename CheckT>
constexpr inline bool Check(const std::optional<Field>& field, CheckT check) noexcept {
  if(field.has_value()) {
    return Check(*field, check);
  };
  return true;
};

template <typename T, auto I, typename Key, typename Value, auto Maximum>
constexpr inline auto ErrorMessage(const std::unordered_map<Key, Value>& field, Max<Maximum>) {
  return fmt::format("Error with field {0} Map size: {1} Maximum Size: {2}", boost::pfr::get_name<I, T>(), field.size(), Maximum);
};

template <typename T, auto I, typename Field, template <auto> typename Check, auto Value>
constexpr inline auto ErrorMessage(const Field& field, Check<Value>) {
  return fmt::format("Error with field {0} Field value: {1} Check Value: {2}", boost::pfr::get_name<I, T>(), field, Value);
};


template <typename T, auto I, typename Value, typename Check>
constexpr inline auto ErrorMessage(const std::unordered_map<std::string, Value>&, Additional) {
  return "Error";
};

template <typename T, auto I, typename Field, typename Check>
constexpr inline auto ErrorMessage(const Field&, Check) {
  return "Error";
};

template <typename T, auto I, typename Field, template <auto> typename Check, auto Value>
constexpr inline auto ErrorMessage(const std::optional<Field>& field, Check<Value> check) {
  return ErrorMessage<T, I>(*field, check);
};

} // namespace formats::universal

USERVER_NAMESPACE_END

