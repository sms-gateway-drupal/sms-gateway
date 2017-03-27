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
  $options += array('properties' => array('id', 'name'));

  $api_client = _sms_gateway_client();
  $devices = $api_client->getDevices();

  if (!$devices) {
    return FALSE;
  }

  if ($action === "status") {
    return $devices['status'];
  }

  if ($action === "total") {
    return $devices['result']['total'];
  }

  if ($action === "list") {
    $devices = $devices['result']['data'];
    $devices = _sms_gateway_array($devices, $options['properties']);

    return $devices;
  }
}

/**
 * Checks smsgateway.me status.
 *
 * @return bool
 *   The status of smsgateway.me.
 */
function sms_gateway_status() {
  $client = _sms_gateway_client();

  if (!$client) {
    return FALSE;
  }

  return (bool) $client->isAvailable();
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

/**
 * Convert unixtimestamp to time ago.
 *
 * Source: http://stackoverflow.com/a/18602474/2931717
 */
function _sms_gateway_time_elapsed_string($datetime, $full = FALSE) {
  $now = new DateTime();
  $ago = new DateTime($datetime);
  $diff = $now->diff($ago);

  $diff->w = floor($diff->d / 7);
  $diff->d -= $diff->w * 7;

  $string = array(
    'y' => 'year',
    'm' => 'month',
    'w' => 'week',
    'd' => 'day',
    'h' => 'hour',
    'i' => 'minute',
    's' => 'second',
  );
  foreach ($string as $k => &$v) {
    if ($diff->$k) {
      $v = $diff->$k . ' ' . $v . ($diff->$k > 1 ? 's' : '');
    }
    else {
      unset($string[$k]);
    }
  }

  if (!$full) {
    $string = array_slice($string, 0, 1);
  }

  return $string ? implode(', ', $string) . ' ago' : 'just now';
}
