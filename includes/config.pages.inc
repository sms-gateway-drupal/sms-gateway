<?php

/**
 * @file
 * Page callbacks for manage config.
 */

/**
 * Menu callback; displays sms gateway config form.
 */
function sms_gateway_config_page() {
  $config = sms_gateway_config();

  if (!$config) {
    $config = sms_gateway_config_create();
  }

  $devices = array();

  if (empty($config->is_new)) {
    list($devices, $status) = sms_gateway_devices('list', array('config' => $config));
    $devices = !empty($devices) ? array_column($devices, 'name', 'id') : array();

    if ($status === 401) {
      drupal_set_message(t('Provided credentials no longer work. Please update your configuration for SMS gateway.'), 'warning');
    }
  }

  return drupal_get_form('sms_gateway_config_form', $config, $devices);
}

/**
 * Form constructor for sms gateway config form.
 */
function sms_gateway_config_form($form, &$form_state, $config, $devices) {
  $form_state['config'] = $config;
  $devices = !empty($form_state['devices']) ? $form_state['devices'] : $devices;

  $form['#prefix'] = '<div id="' . drupal_html_id('form-wrapper') . '">';
  $form['#suffix'] = '</div>';

  $form['credentials']['#tree'] = TRUE;

  $form['credentials']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Your smsgateway.me username.'),
    '#required' => TRUE,
    '#default_value' => $form_state['config']->username,
    '#disabled' => !empty($devices),
  );

  $form['credentials']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Your smsgateway.me password.'),
    '#required' => TRUE,
    '#disabled' => !empty($devices),
  );

  $form['credentials']['saved_password'] = array(
    '#type' => 'value',
    '#value' => $form_state['config']->password,
  );

  $form['device'] = array(
    '#type' => 'select',
    '#title' => t('Default device'),
    '#options' => array('' => t('None')) + $devices,
    '#description' => t('Set your default sending device.'),
    '#required' => TRUE,
    '#default_value' => $form_state['config']->device,
    '#access' => !empty($devices),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['check'] = array(
    '#type' => 'submit',
    '#value' => t('Check credentials'),
    '#access' => empty($devices),
    '#ajax' => array(
      'callback' => 'sms_gateway_config_check_credentials',
      'wrapper' => 'form-wrapper',
    ),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#access' => !empty($devices),
  );

  return $form;
}

/**
 * Form validation handler for sms gateway config form.
 */
function sms_gateway_config_form_validate($form, &$form_state) {
  $form_state['config']->username = $form_state['values']['credentials']['username'];

  $password = $form_state['values']['credentials']['password'];
  $saved_password = $form_state['values']['credentials']['saved_password'];

  $form_state['config']->password = !empty($password) ? $password : $saved_password;

  if (empty($form_state['devices'])) {
    list($devices,) = sms_gateway_devices('list', array('config' => $form_state['config']));

    if (empty($devices)) {
      form_set_error('credentials', t('Incorrect credentials.'));
    }

    $form_state['devices'] = array_column($devices, 'name', 'id');
  }
}

/**
 * Checks if the submitted credentials are correct.
 */
function sms_gateway_config_check_credentials($form, &$form_state) {
  return $form;
}

/**
 * Form submission handler for sms gateway config form.
 */
function sms_gateway_config_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] === t('Check credentials')) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  // If we were able to get through this stage, then it means that the
  // validation has passed and we already have a stored password.
  $form_state['config']->username = $form_state['values']['credentials']['username'];
  $form_state['config']->password = $form_state['values']['credentials']['saved_password'];
  $form_state['config']->device   = $form_state['values']['device'];

  sms_gateway_config_save($form_state['config']);
}
