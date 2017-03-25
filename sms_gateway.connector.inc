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
   * Retrieves the list of devices.
   */
  public function getDevices() {
    return $this->sendResource('/api/v3/devices');
  }

}
