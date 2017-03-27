<?php

/**
 * @file
 * Define connector file.
 */

/**
 * Class for connecting to SMS Gateway.
 */
class SmsGatewayConnector {

  private $endpointUrl;
  private $username;
  private $password;

  /**
   * Sets username and password.
   *
   * @param string $username
   *   The username in SMS Gateway.
   * @param string $password
   *   The password in SMS Gateway.
   */
  public function __construct($username, $password, $endpointUrl = 'https://smsgateway.me') {
    $this->endpointUrl = $endpointUrl;
    $this->username    = $username;
    $this->password    = $password;
  }

  /**
   * Retrieves the list of devices.
   */
  public function getDevices() {
    return $this->makeRequest('/api/v3/devices');
  }

  /**
   * Checks if the server is available.
   */
  public function isAvailable() {
    return $this->sendResource('', array(), 'GET');
  }

  /**
   * Sends a POST request to smsgateway.me.
   *
   * @param string $resource
   *   The path to send the request.
   * @param array $data
   *   Data to be sent to smsgateway.me.
   * @param string $method
   *   Whether to use GET or POST.
   *
   * @return bool
   *   Whether the request was made successfully or not.
   *
   * @todo Would be nice to use drupal_http_request() although `verify_peer`
   * needs to be disabled. Unfortunately, we can't get it to work (using
   * context). We use cURL here for now.
   */
  protected function sendResource($resource = '', array $data = array(), $method = 'POST') {
    $url = $this->endpointUrl . $resource;

    $data += array(
      'email' => $this->username,
      'password' => $this->password,
    );

    $request = curl_init();
    $query = http_build_query($data);

    curl_setopt($request, CURLOPT_URL, $url);
    curl_setopt($request, CURLOPT_RETURNTRANSFER, TRUE);

    if ($method === 'POST') {
      curl_setopt($request, CURLOPT_POST, TRUE);
      curl_setopt($request, CURLOPT_POSTFIELDS, $query);
    }
    elseif ($method === 'GET') {
      $url .= '?' . $query;
    }

    curl_setopt($request, CURLOPT_SSL_VERIFYPEER, FALSE);

    $response = curl_exec($request);

    if ($error = curl_error($request)) {
      watchdog('sms_gateway', 'cURL error: @error', array('@error' => $error), WATCHDOG_ERROR);
      return FALSE;
    }

    curl_close($request);

    return $response;
  }

  /**
   * Makes a request.
   *
   * @param string $resource
   *   The path to send the request.
   * @param array $data
   *   Data to be sent to smsgateway.me.
   * @param string $method
   *   Whether to use GET or POST.
   *
   * @return mixed
   *   The JSON data returned from smsgateway.me converted to an array.
   */
  protected function makeRequest($resource = '', array $data = array(), $method = 'POST') {
    $response = $this->sendResource($resource, $data, $method);

    if ($response) {
      $json_response = drupal_json_decode($response);
      if (!empty($json_response['success'])) {
        return $json_response;
      }
    }

    return FALSE;
  }

}
