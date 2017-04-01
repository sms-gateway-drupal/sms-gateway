<?php

/**
 * @file
 * Page callbacks for manage device.
 */

/**
 * Menu callback; displays sms gateway devices page.
 */
function sms_gateway_devices_page() {
  global $user;

  $config = sms_gateway_config_load_by_user($user);

  if (!$config) {
    $config = sms_gateway_config_create($user);
  }

  return drupal_get_form('sms_gateway_devices_form', $config);
}

/**
 * Form constructor for sms gateway devices form.
 */
function sms_gateway_devices_form($form, &$form_state, $config) {
  $form_state['config'] = $config;

  $form['title'] = array(
    '#markup' => t('<h1>List of devices</h1>'),
  );

  $devices = sms_gateway_devices(
    'list',
    array(
      'properties' => array(
        'id',
        'name',
        'make',
        'model',
        'number',
        'provider',
        'country',
        array('connection_type' => 'connection'),
        'battery',
        'signal',
        'wifi',
        'last_seen',
      ),
    )
  );

  $form['devices'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'edit-',
  );

  foreach ($devices as $device) {
    $form['device_' . $device['id']] = array(
      '#type' => 'fieldset',
      '#title' => $device['name'],
      '#collapsible' => TRUE,
      '#group' => 'devices',
      '#attributes' => array(
        'class' => array(
          'sms-gateway-devices',
        ),
      ),
    );

    $form['device_' . $device['id']]['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('Device name set in smsgateway.me.'),
      '#required' => TRUE,
      '#default_value' => $device['name'],
    );

    $form['device_' . $device['id']] += array(
      'id' => array(
        '#type' => 'item',
        '#title' => t('Device ID'),
        '#markup' => $device['id'],
        '#prefix' => '<div class="sms-gateway-devices-info">',
      ),
      'model' => array(
        '#type' => 'item',
        '#title' => t('Unit model'),
        '#markup' => $device['make'] . ', ' . $device['model'],
      ),
      'number' => array(
        '#type' => 'item',
        '#title' => t('Mobile number'),
        '#markup' => $device['number'],
      ),
      'provider' => array(
        '#type' => 'item',
        '#title' => t('Network provider'),
        '#markup' => $device['provider'],
      ),
      'country' => array(
        '#type' => 'item',
        '#title' => t('Country'),
        '#markup' => $device['country'],
      ),
      'connection' => array(
        '#type' => 'item',
        '#title' => t('Connection type'),
        '#markup' => $device['connection'],
      ),
      'battery' => array(
        '#type' => 'item',
        '#title' => t('Battery status'),
        '#markup' => $device['battery'] . '%',
      ),
      'signal' => array(
        '#type' => 'item',
        '#title' => t('Signal status'),
        '#markup' => $device['signal'] . '%',
      ),
      'wifi' => array(
        '#type' => 'item',
        '#title' => t('Wifi status'),
        '#markup' => $device['wifi'] . '%',
      ),
      'last_seen' => array(
        '#type' => 'item',
        '#title' => t('Last seen'),
        '#markup' => _sms_gateway_time_elapsed_string('@' . $device['last_seen']),
        '#suffix' => '</div>',
      ),
    );

    // Per device actions.
    $form['device_' . $device['id']]['actions'] = array(
      'ping' => array(
        '#type' => 'button',
        '#value' => t('Ping device'),
        '#prefix' => '<div class="sms-gateway-devices-actions">',
      ),
      'delete' => array(
        '#type' => 'button',
        '#value' => t('Delete device'),
        '#suffix' => '</div>',
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}
