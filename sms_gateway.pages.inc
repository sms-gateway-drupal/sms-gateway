<?php

/**
 * @file
 * Page callbacks for managing entities.
 */

/**
 * Menu callback; displays sms gateway config form.
 */
function sms_gateway_config_page() {
  global $user;

  $config = sms_gateway_config_load_by_user($user);

  if (!$config) {
    $config = sms_gateway_config_create($user);
  }

  return drupal_get_form('sms_gateway_config_form', $config);
}

/**
 * Menu callback; displays sms gateway devices form.
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
 * Form constructor for sms gateway config form.
 */
function sms_gateway_config_form($form, &$form_state, $config) {
  $form_state['config'] = $config;

  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Your smsgateway.me username.'),
    '#required' => TRUE,
    '#default_value' => $form_state['config']->username,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Your smsgateway.me password.'),
    '#required' => TRUE,
  );

  if (empty($form_state['config']->is_new)) {
    $devices = sms_gateway_devices('list');
    $devices = array_column($devices, 'name', 'id');

    $form['device'] = array(
      '#type' => 'select',
      '#title' => t('Default device'),
      '#options' => array(0 => t('None')) + $devices,
      '#description' => t('Set your default sending device.'),
      '#required' => TRUE,
      '#default_value' => $form_state['config']->device,
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
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
        'connection_type',
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
      '#title' => $device['id'] . ': ' . $device['name'],
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
      'model' => array(
        '#type' => 'item',
        '#title' => t('Unit model'),
        '#markup' => $device['make'] . ', ' . $device['model'],
        '#prefix' => '<div class="sms-gateway-devices-info">',
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
        '#markup' => $device['connection_type'],
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
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Form submission handler for sms gateway config form.
 */
function sms_gateway_config_form_submit($form, &$form_state) {
  $form_state['config']->username = $form_state['values']['username'];
  $form_state['config']->password = $form_state['values']['password'];

  sms_gateway_config_save($form_state['config']);
}
