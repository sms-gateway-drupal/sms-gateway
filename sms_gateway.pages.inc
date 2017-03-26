<?php

/**
 * @file
 * Page callbacks for managing entities.
 */

/**
 * Menu callback; displays Sms gateway config form.
 */
function sms_gateway_config_page($account) {
  $config = sms_gateway_config_load_by_user($account);

  if (!$config) {
    $config = sms_gateway_config_create($account);
  }

  return drupal_get_form('sms_gateway_config_form', $config);
}

/**
 * Form constructor for Sms gateway config form.
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
    $devices = array_combine(array_column($devices, 'id'), array_column($devices, 'name'));

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
 * Form submission handler for Sms gateway config form.
 */
function sms_gateway_config_form_submit($form, &$form_state) {
  $form_state['config']->username = $form_state['values']['username'];
  $form_state['config']->password = $form_state['values']['password'];

  sms_gateway_config_save($form_state['config']);
}
