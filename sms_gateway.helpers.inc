<?php

/**
 * @file
 * Helper functions for sms gateway.
 */

/**
 * Sanitize array with desired keys.
 */
function _sms_gateway_array($arrays = array(), $keys = array()) {
  $return = array();

  foreach ($arrays as $array) {
    $array_new = array();

    foreach ($keys as $key => $key_value) {
      if (is_int($key)) {
        $to_add = array($key_value => $array[$key_value]);
      }
      else {
        $to_add = array($key_value => $array[$key]);
      }

      $array_new = array_merge($array_new, $to_add);
    }

    array_push($return, $array_new);
  }

  return $return;
}

/**
 * Helper devices.
 */
function sms_gateway_devices($action = "status", array $options = array()) {
  $api_client = _sms_gateway_client();
  $devices = $api_client->getDevices();

  if ($action === "status") {
    return $devices['result']['total'] > 0;
  }

  if ($action === "total") {
    return $devices['result']['total'];
  }

  if ($action === "list") {
    $devices = $devices['result']['data'];

    if (empty($options['properties'])) {
      $options['properties'] = array('id', 'name');
    }

    $devices = _sms_gateway_array($devices, $options['properties']);

    return $devices;
  }
}

/**
 * Retrieves the client for the current user.
 *
 * @param object $account
 *   (optional The account to get the sms_gateway_config entity.
 *
 * @return SmsGatewayConnector|false
 *   The connector client; othewise FALSE.
 */
function _sms_gateway_client($account = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }

  if ($config = sms_gateway_config_load_by_user($account)) {
    return new SMSGatewayConnector($config->username, $config->password);
  }

  return FALSE;
}
