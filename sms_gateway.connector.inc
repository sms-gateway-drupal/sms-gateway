<?php

/**
 * @file
 * Define connector file.
 */

/**
 * Class for connecting to SMS Gateway.
 */
class SMSGatewayConnector {

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
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Sends a POST request to smsgateway.me.
   *
   * @param string $resource
   *   The path to send the request.
   * @param array $data
   *   Data to be sent to smsgateway.me.
   *
   * @return mixed
   *   The JSON data returned from smsgateway.me converted to an array.
   *
   * @todo Would be nice to use drupal_http_request() although `verify_peer`
   * needs to be disabled. Unfortunately, we can't get it to work (using
   * context). We use cURL here for now.
   */
  protected function sendResource($resource, array $data = array()) {
    $url = $this->endpointUrl . $resource;

    $data += array(
      'email' => $this->username,
      'password' => $this->password,
    );

    $request = curl_init();

    curl_setopt($request, CURLOPT_URL, $url);
    curl_setopt($request, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($request, CURLOPT_POST, TRUE);
    curl_setopt($request, CURLOPT_POSTFIELDS, http_build_query($data));
    curl_setopt($request, CURLOPT_SSL_VERIFYPEER, FALSE);

    $response = curl_exec($request);

    if ($error = curl_error($request)) {
      watchdog('sms_gateway', 'cURL error: @error', array('@error' => $error), WATCHDOG_ERROR);
      return FALSE;
    }

    curl_close($request);

    $json_response = drupal_json_decode($response);
    if (!empty($json_response['success'])) {
      return $json_response;
    }

    return FALSE;
  }

  /**
   * Retrieves the list of devices.
   */
  public function getDevices() {
    return $this->sendResource('/api/v3/devices');
  }

}
