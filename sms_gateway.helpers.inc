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

    foreach ($keys as $key) {
      $key_value = $array[$key];
      $to_add = array($key => $key_value);
      $array_new = array_merge($array_new, $to_add);
    }

    array_push($return, $array_new);
  }

  return $return;
}
