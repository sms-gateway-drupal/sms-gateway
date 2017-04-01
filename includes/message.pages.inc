<?php

/**
 * @file
 * Page callbacks for manage message.
 */

/**
 * Menu callback; displays sms gateway message list page.
 */
function sms_gateway_message_list_page() {
  return 'Messages list page.';
}

/**
 * Menu callback; displays sms gateway message detail page.
 */
function sms_gateway_message_detail_page() {
  return 'Message detail page.';
}

/**
 * Menu callback; displays sms gateway message detail reply page.
 */
function sms_gateway_message_detail_reply_page() {
  return 'Message detail page with preloaded reply.';
}

/**
 * Menu callback; displays sms gateway message delete page.
 */
function sms_gateway_message_delete_page() {
  return 'Message delete page.';
}

/**
 * Menu callback; displays sms gateway message send page.
 */
function sms_gateway_message_send_page() {
  return 'Send message page.';
}

/**
 * Menu callback; redirects to message send page.
 */
function sms_gateway_send_page() {
  drupal_goto('admin/sms/message/send');
}

/**
 * Menu callback; redirects to message list page.
 */
function sms_gateway_messages_page() {
  drupal_goto('admin/sms');
}
