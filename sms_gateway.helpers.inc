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
function _sms_gateway_devices($action = "status") {
  $api_client = new SmsGatewayConnector("", "");
  $devices = $api_client->getDevices();

  if ($action === "status") {
    if ($devices['result']['total'] > 0) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  if ($action === "total") {
    return $devices['result']['total'];
  }

  if ($action === "list") {
    $devices = $devices['result']['data'];
    $devices = _sms_gateway_array($devices,
      array('id', 'name' => 'ngalan', 'provider'));
    return $devices;
  }
}
