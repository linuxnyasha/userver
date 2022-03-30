#pragma once

/// @file userver/dynamic_config/client/component.hpp
/// @brief @copybrief components::DynamicConfigClient

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/client/client.hpp>

USERVER_NAMESPACE_BEGIN

namespace components {

// clang-format off

/// @ingroup userver_components
///
/// @brief Component that starts a clients::dynamic_config::Client client.
///
/// Returned references to clients::dynamic_config::Client live for a lifetime
/// of the component and are safe for concurrent use.
///
/// The component must be configured in service config.
///
/// ## Static options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// get-configs-overrides-for-service | send service-name field | true
/// service-name | name of the service to send if the get-configs-overrides-for-service is true | -
/// http-timeout | HTTP request timeout to the remote in utils::StringToDuration() suitable format | -
/// http-retries | HTTP retries before reporting the request failure | -
/// config-url | HTTP URL to request configs via POST request, ignored if use-uconfigs is true | -
/// use-uconfigs | set to true to read stage name from "/etc/yandex/settings.json" and send it in requests | false
/// uconfigs-url | HTTP URL to request configs via POST request if use-uconfigs is true | -
/// fallback-to-no-proxy | make additional attempts to retrieve configs by bypassing proxy that is set in USERVER_HTTP_PROXY runtime variable | true
///
/// ## Static configuration example:
///
/// @snippet components/common_component_list_test.cpp  Sample dynamic configs client component config

// clang-format on
class DynamicConfigClient : public LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "taxi-configs-client";

  DynamicConfigClient(const ComponentConfig&, const ComponentContext&);
  ~DynamicConfigClient() override = default;

  [[nodiscard]] dynamic_config::Client& GetClient() const;

  static yaml_config::Schema GetStaticConfigSchema();

 private:
  std::unique_ptr<dynamic_config::Client> config_client_;
};

using TaxiConfigClient = DynamicConfigClient;

template <>
inline constexpr bool kHasValidate<DynamicConfigClient> =
    false;  // TODO: replace to true TAXICOMMON-4601

}  // namespace components

USERVER_NAMESPACE_END
