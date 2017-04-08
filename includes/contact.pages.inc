<?php

/**
 * @file
 * Page callbacks for manage contact.
 */

/**
 * Menu callback; displays sms gateway contact list page.
 */
function sms_gateway_contact_list_page() {
  return 'Contacts list page.';
}

/**
 * Menu callback; displays sms gateway contact detail page.
 */
function sms_gateway_contact_detail_page() {
  return 'Contact detail page.';
}

/**
 * Menu callback; displays sms gateway contact edit page.
 */
function sms_gateway_contact_edit_page() {
  return 'Contact edit page.';
}

/**
 * Menu callback; displays sms gateway contact delete page.
 */
function sms_gateway_contact_delete_page() {
  return 'Contact delete page.';
}

/**
 * Menu callback; displays sms gateway contact add page.
 */
function sms_gateway_contact_add_page() {
  $contact = sms_gateway_contact_create();
  return drupal_get_form('sms_gateway_contact_add_form', $contact);
}

/**
 * Form constructor for sms gateway contact add form.
 */
function sms_gateway_contact_add_form($form, &$form_state, $contact) {
  $form_state['contact'] = $contact;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $form_state['contact']->name,
  );

  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number'),
    '#default_value' => $form_state['contact']->number,
  );

  $form['contact_id'] = array(
    '#type' => 'value',
    '#value' => $form_state['contact']->contact_id,
  );

  $langcode = entity_language('sms_gateway_contact', $form_state['contact']);

  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('sms_gateway_contact', $form_state['contact'], $form, $form_state, $langcode);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for sms gateway contact add form.
 */
function sms_gateway_contact_add_form_submit($form, &$form_state) {
  $form_state['contact']->name       = $form_state['values']['name'];
  $form_state['contact']->number     = $form_state['values']['number'];
  $form_state['contact']->contact_id = $form_state['values']['contact_id'];

  field_attach_submit('sms_gateway_contact', $form_state['contact'], $form, $form_state);

  sms_gateway_contact_save($form_state['contact']);

  drupal_set_message(t('Contact added.'));
}
