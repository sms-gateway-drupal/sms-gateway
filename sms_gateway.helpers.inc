<?php

/**
 * @file
 * Helper functions for sms gateway.
 */

/**
 * Sanitize array with desired keys.
 *
 * @param array $arrays
 *   Array to be sanitized.
 * @param array $keys
 *   Only these defined keys shall be returned. To change key name, change value
 *   from being a string to an associative array.
 *
 * @return array
 *   Sanized array.
 */
function _sms_gateway_array(array $arrays = array(), array $keys = array()) {
  $return = array();

  foreach ($arrays as $array) {
    $array_new = array();

    foreach ($keys as $key) {
      if (is_array($key)) {
        $to_add = array(current($key) => $array[key($key)]);
      }
      else {
        $to_add = array($key => $array[$key]);
      }

      $array_new = array_merge($array_new, $to_add);
    }

    array_push($return, $array_new);
  }

  return $return;
}

/**
 * Retrieves the devices depending on the action.
 *
 * @param string $action
 *   The action with either of the values:
 *   - status: Retrieves the HTTP code status.
 *   - total: Retrieves the number of devices.
 *   - list: Retrieves the devices.
 * @param array $options
 *   The options to set when retrieving the devices:
 *   - (optional) properties: The properties to filter by.
 *   - (optional) config: The sms_gateway_config entity.
 *
 * @return mixed
 *   Output depending on the action.
 */
function sms_gateway_devices($action = "status", array $options = array()) {
  global $user;

  $options += array(
    'properties' => array('id', 'name'),
    'config' => sms_gateway_config_load_by_user($user),
  );

  $api_client = sms_gateway_client($options['config']);
  list($devices, $info) = $api_client->getDevices();

  if ($action === 'status') {
    return $info['http_code'];
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

    return array($devices, $info['http_code']);
  }
}

/**
 * Checks smsgateway.me status.
 *
 * @return bool
 *   The status of smsgateway.me.
 */
function sms_gateway_status() {
  $client = sms_gateway_client();

  if (!$client) {
    return FALSE;
  }

  return (bool) $client->isAvailable();
}

/**
 * Retrieves the client.
 *
 * @param object $config
 *   The sms_gateway_config entity.
 *
 * @return SmsGatewayConnector|false
 *   The connector client; othewise FALSE.
 */
function sms_gateway_client($config) {
  return new SmsGatewayConnector($config->username, $config->password);
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
