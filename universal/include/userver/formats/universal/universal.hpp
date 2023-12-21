#pragma once
#include <userver/utils/impl/type_list.hpp>
#include <userver/utils/constexpr_string.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/parse/try_parse.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/formats/common/meta.hpp>
#include <userver/formats/common/items.hpp>
#include <userver/formats/parse/try_parse.hpp>
#include <unordered_map>
#include <boost/pfr/core_name.hpp>
#include <boost/pfr/core.hpp>

USERVER_NAMESPACE_BEGIN
namespace formats::universal {

namespace impl {

struct Disabled {};

struct Additional;

template <auto>
struct Default;

} //namespace impl

template <auto... Params>
struct Configurator {
  using kParams = utils::impl::TypeList<decltype(Params)...>;
};

template <typename T>
inline static constexpr auto kSerialization = impl::Disabled{};

template <typename T>
inline static constexpr auto kDeserialization = kSerialization<T>;

namespace impl {

template <auto Needed, auto Value, typename T, typename F>
consteval auto TransformIfEqual(const T& obj, const F& f) {
  if constexpr(Needed == Value) {
    return f(obj);
  } else {
    return obj;
  };
};

template <typename T, auto I, typename... Params>
struct FieldParametries {
  static constexpr auto kIndex = I;
  using kType = T;
  using kFieldType = std::remove_cvref_t<decltype(boost::pfr::get<I>(std::declval<T>()))>;
};

template <typename Field>
constexpr inline auto Check(const Field&, Disabled) noexcept {
  return true;
};

struct NeverToBeUsed;
template <typename... Ts>
struct Error : public std::false_type {};

template <> // https://eel.is/c++draft/temp#res.general-6.1
struct Error<NeverToBeUsed, NeverToBeUsed> : public std::true_type {};

namespace exam {

template <typename Field, typename CheckT>
constexpr inline bool Check(const Field&, CheckT) {
  static_assert(Error<Field, CheckT>::value, "Check Not Found "
      "Probably you forgot to include the "
      "<userver/formats/universal/common_checks.hpp>");
  return false;
};

template <typename T, auto I, typename Field, typename CheckT>
constexpr inline const char* ErrorMessage(Field&&, CheckT) {
  static_assert(Error<Field, CheckT>::value, "Error Message Not Found "
      "Probably you forgot to include the "
      "<userver/formats/universal/common_checks.hpp>");
  return "";
};

template <typename T, auto I, typename Builder, typename Field, typename CheckT>
constexpr inline auto RunCheckFor(const Builder&, Field&&, CheckT) {
  static_assert(Error<Field, CheckT>::value, "Check Not Found "
      "Probably you forgot to include the "
      "<userver/formats/universal/common_checks.hpp>");
};

template <typename T, auto I, typename Value, typename Field, typename CheckT>
constexpr inline auto RunParseCheckFor(Value&&, const Field& field, CheckT check) {
  using exam::ErrorMessage;
  if(!Check(field, check)) {
    throw std::runtime_error(ErrorMessage<T, I>(field, check));
  };
};

template <typename T, auto I, typename... Params, typename Builder, typename Field>
constexpr inline auto RunWrite(Builder& builder, Field&& field) {
  builder[std::string(boost::pfr::get_name<I, T>())] = std::forward<Field>(field);
};
template <typename T, auto I, typename... Params, typename Builder, typename Field>
constexpr inline auto RunWrite(Builder& builder, const std::optional<Field>& field) {
  if(field) {
    return RunWrite<T, I, Params...>(builder, *field);
  };
};

template <typename T, auto I, typename... Params, typename Value, typename Field>
constexpr inline Field Read(Value&& value, parse::To<Field>) {
  return value[boost::pfr::get_name<I, T>()].template As<Field>();
};

template <typename T, auto I, typename... Params, typename From, typename Value>
constexpr inline
std::enable_if_t<utils::impl::AnyOf(utils::impl::IsSameCarried<Additional>(), utils::impl::TypeList<Params...>{}), std::unordered_map<std::string, Value>>
Read(const From& value, parse::To<std::unordered_map<std::string, Value>>) {
  constexpr auto names = boost::pfr::names_as_array<T>();
  std::unordered_map<std::string, Value> result;
  for(const auto& [name, value] : common::Items(value)) {
    if(std::find(names.begin(), names.end(), name) == names.end()) {
      result[name] = value.template As<Value>();
    };
  };
  return result;
};

template <typename T, auto I, typename... Params, typename From, typename Value>
constexpr inline
std::enable_if_t<utils::impl::AnyOf(utils::impl::IsSameCarried<Additional>(), utils::impl::TypeList<Params...>{}), std::optional<std::unordered_map<std::string, Value>>>
Read(const From& value, parse::To<std::optional<std::unordered_map<std::string, Value>>>) {
  return Read<T, I, Params...>(value, parse::To<std::unordered_map<std::string, Value>>{});
};

template <typename T>
struct IsDefault : public std::false_type {};

template <auto Value>
struct IsDefault<Default<Value>> : public std::true_type {};

template <typename T, auto I, typename... Params, typename Value, typename Field>
constexpr inline
std::enable_if_t<!utils::impl::anyOf<IsDefault>(utils::impl::TypeList<Params...>{}), std::optional<Field>>
Read(Value&& value, parse::To<std::optional<Field>>) {
  using parse::TryParse;
  static_assert(common::impl::kHasTryParse<Value, Field>, "Not Found Try Parse");
  return TryParse(value[boost::pfr::get_name<I, T>()], parse::To<Field>{});
};
template <typename T, auto I, typename... Params, typename Value, typename Field>
constexpr inline
std::enable_if_t<utils::impl::anyOf<IsDefault>(utils::impl::TypeList<Params...>{}), std::optional<Field>>
Read(Value&& value, parse::To<std::optional<Field>>) {
  using parse::TryParse;
  static_assert(common::impl::kHasTryParse<Value, Field>, "Not Found Try Parse");
  auto response = TryParse(value[boost::pfr::get_name<I, T>()], parse::To<Field>{});
  if(!response) {
    return []<typename... Ts, auto DefaultValue, typename... Tss>(Ts..., Default<DefaultValue>, Tss...){
      return DefaultValue;
    }(Params{}...);
  };
  return response;
};

template <typename T, auto I, typename... Params, typename Value, typename Field>
constexpr inline std::optional<Field> Read(Value&& value, parse::To<std::optional<std::optional<Field>>>) {
  return Read<T, I, Params...>(std::forward<Value>(value), parse::To<std::optional<Field>>{});
};


template <typename T, auto I, typename Builder, typename Field, auto Value>
constexpr inline auto RunCheckFor(Builder& builder, const std::optional<Field>& field, Default<Value>) {
  if(!field.has_value()) {
    builder[std::string(boost::pfr::get_name<I, T>())] = Value;
  };
};


template <typename T, auto I, typename... Params, typename Builder, typename Value>
constexpr inline std::enable_if_t<utils::impl::AnyOf(utils::impl::IsSameCarried<Additional>(), utils::impl::TypeList<Params...>{}), void>
RunWrite(Builder& builder, const std::unordered_map<std::string, Value>& field) {
  for(const auto& element : field) {
    builder[element.first] = element.second;
  };
};

template <typename T, auto I, typename Builder, typename Field>
constexpr inline auto RunCheckFor(Builder&, Field&&, Disabled) noexcept {};

template <typename T, auto I, typename Builder, typename Field>
constexpr inline auto RunCheckFor(Builder&, const std::optional<Field>&, Disabled) noexcept {};

template <typename T, auto I, typename Builder, typename Field, typename CheckT>
constexpr inline auto RunCheckFor(Builder&, Field&& field, CheckT check) {
  if(!Check(field, check)) {
    throw std::runtime_error(ErrorMessage<T, I>(std::forward<Field>(field), check));
  };
};

} // namespace exam
template <typename T, auto I, typename Builder, typename... Params>
constexpr inline auto UniversalSerializeField(
     FieldParametries<T, I, Params...>
    ,Builder& builder
    ,const T& obj) {
  const auto& value = boost::pfr::get<I>(obj);
  using exam::RunCheckFor;
  using exam::RunWrite;
  (RunCheckFor<T, I>(builder, value, Params{}), ...);
  RunWrite<T, I, Params...>(builder, value);
};

template <typename T, auto I, typename Format, typename... Params>
constexpr inline auto UniversalParseField(
     FieldParametries<T, I, Params...>
    ,Format&& from) {
  using exam::Read;
  using exam::RunParseCheckFor;
  using FieldType = std::remove_cvref_t<decltype(boost::pfr::get<I>(std::declval<T>()))>;
  auto value = Read<T, I, Params...>(from, userver::formats::parse::To<FieldType>{});
  (RunParseCheckFor<T, I>(from, value, Params{}), ...);
  return value;
};


template <typename T, auto I, typename Format, typename... Params>
constexpr inline std::optional<std::remove_cvref_t<decltype(boost::pfr::get<I>(std::declval<T>()))>>
UniversalTryParseField(
     FieldParametries<T, I, Params...>
    ,Format&& from) noexcept {
  using FieldType = std::remove_cvref_t<decltype(boost::pfr::get<I>(std::declval<T>()))>;
  using exam::Check;
  using exam::Read;


  auto val = Read<T, I, Params...>(from, userver::formats::parse::To<std::optional<FieldType>>{});
  if((Check(val, Params{}) && ...)) {
    return val;
  };
  return std::nullopt;
};

} // namespace impl

template <typename T, typename... Params>
class SerializationConfig {
  public:
    static consteval auto Create() {
      return []<auto... I>(std::index_sequence<I...>){
        return SerializationConfig<T, impl::FieldParametries<T, I>...>{};
      }(std::make_index_sequence<boost::pfr::tuple_size_v<T>>());
    };


    template <std::size_t I, typename... ConfigElements>
    consteval auto With(ConfigElements...) const {
      return SerializationConfig<T, Params...>::AddParamsTo<I, ConfigElements...>();
    };

    template <utils::ConstexprString field, typename... ConfigElements>
    consteval auto With(ConfigElements... config) const {
      constexpr auto names = boost::pfr::names_as_array<T>();
      constexpr std::size_t index = std::find(names.begin(), names.end(), field) - names.begin();
      return With<index>(config...);
    };

    template <std::size_t I, auto... ConfigElements>
    consteval auto With(Configurator<ConfigElements...>) const {
      return With<I>(ConfigElements...);
    };

    template <typename From, std::size_t I>
    consteval auto FromStruct() {
      if constexpr(I == boost::pfr::tuple_size_v<From>) {
        return SerializationConfig<T, Params...>();
      } else {
        constexpr auto fieldName = boost::pfr::get_name<I, From>();
        constexpr auto names = boost::pfr::names_as_array<T>();
        constexpr std::size_t Index = std::find(names.begin(), names.end(), fieldName) - names.begin();
        return SerializationConfig<T, Params...>().With<Index>(boost::pfr::get<I>(From{})).template FromStruct<From, I + 1>();
      };
    };

    template <typename From>
    consteval auto FromStruct() {
      return FromStruct<From, 0>();
    };

    constexpr SerializationConfig() noexcept {
      static_assert(sizeof...(Params) == boost::pfr::tuple_size_v<T>, "Use Create");
    };
  private:
    template <auto I, typename... Parameters>
    static consteval auto AddParamsTo() {
      return []<auto... Is>(std::index_sequence<Is...>){
        return SerializationConfig<T, decltype(
          impl::TransformIfEqual<I, Is>(Params{},
              []<typename... FieldParams>(impl::FieldParametries<T, I, FieldParams...>){
            return impl::FieldParametries<T, I, FieldParams..., Parameters...>();
          })
        )...>();
      }(std::make_index_sequence<sizeof...(Params)>());
    };


};


} // namespace formats::universal
namespace formats::parse {

template <typename Format, typename T>
constexpr inline
std::enable_if_t<!std::is_same_v<decltype(universal::kDeserialization<std::remove_cvref_t<T>>), const universal::impl::Disabled>, T>
Parse(Format&& from,
    To<T>) {
  using Config = std::remove_const_t<decltype(universal::kDeserialization<std::remove_cvref_t<T>>)>;
  using Type = std::remove_cvref_t<T>;
  return [from = std::forward<Format>(from)]<typename... Params>(universal::SerializationConfig<Type, Params...>){
    return T{universal::impl::UniversalParseField(Params{}, std::forward<Format>(from))...};
  }(Config{});
};

template <typename Format, typename T>
constexpr inline
std::enable_if_t<!std::is_same_v<decltype(universal::kDeserialization<std::remove_cvref_t<T>>), const universal::impl::Disabled>, std::optional<T>>
TryParse(Format&& from,
    To<T>) {
  using Config = std::remove_const_t<decltype(universal::kDeserialization<std::remove_cvref_t<T>>)>;
  using Type = std::remove_cvref_t<T>;
  return [&]<typename... Params>(universal::SerializationConfig<Type, Params...>) -> std::optional<T> {
    auto fields = std::make_tuple(universal::impl::UniversalTryParseField(Params{}, from)...);
    constexpr auto fieldsCount = boost::pfr::tuple_size_v<T>;
    if([&]<auto... I>(std::index_sequence<I...>){
      return (std::get<I>(fields) && ...);
    }(std::make_index_sequence<fieldsCount>())) {
      return [&]<auto... I>(std::index_sequence<I...>){
        return T{*std::get<I>(fields)...};
      }(std::make_index_sequence<fieldsCount>());
    };
    return std::nullopt;
  }(Config{});
};

} // namespace formats::parse

namespace formats::serialize {

template <typename T, typename Value>
inline constexpr
std::enable_if_t<!std::is_same_v<decltype(universal::kSerialization<std::remove_cvref_t<T>>), const universal::impl::Disabled>, Value>
Serialize(T&& obj,
    serialize::To<Value>) {
  using Config = std::remove_const_t<decltype(universal::kSerialization<std::remove_cvref_t<T>>)>;
  using Type = std::remove_cvref_t<T>;
  return [&]<typename... Params>
      (universal::SerializationConfig<Type, Params...>){
    typename Value::Builder builder;
    (universal::impl::UniversalSerializeField(Params{}, builder, obj), ...);
    return builder.ExtractValue();
  }(Config{});
};

} // namespace formats::serialize

USERVER_NAMESPACE_END

