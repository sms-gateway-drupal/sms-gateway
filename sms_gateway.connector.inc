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
  public function __construct($username, $password) {
    $this->endpointUrl = $endpointUrl;
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Gets endpoint url.
   *
   * @return string
   *   Smsgateway.me endpoint url.
   */
  public function getEndpointUrl() {
    return $this->endpointUrl;
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
   */
  protected function sendResource($resource, array $data = array()) {
    $url = $this->getEndpointUrl() . $resource;
  }

  /**
   * Retrieves the list of devices.
   */
  public function getDevices() {
    return $this->sendResource('/api/v3/devices');
  }

}
