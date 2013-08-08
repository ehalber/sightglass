<?php


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// FROM RECURLY //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
function myrecurly_user_subscription_page() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $build = array();

  $header = array(
    array('data' => t('Plan Code')), 
    array('data' => t('Plan Name')),
    array('data' => t('Quantity')),
    array('data' => t('Unit Price')),
    array('data' => t('Activated @')),
    array('data' => t('Current Period Started @')),
    array('data' => t('Current Period Ends @')),
    //array('data' => t('Subscription ID')),
    array('data' => t('Status')),
    array('data' => t('Action'), 'colspan' => 3, 'style' => 'text-align:center;'),
  );

  $user_email = $user->mail;
  $account_code = $user_email;

  $per_page = 50;
  $subscription_list = Recurly_SubscriptionList::getForAccount($account_code, array('per_page' => $per_page));

  $page_subscriptions = myrecurly_pager_results($subscription_list, $per_page);

  $subscriptions['subscriptions']['#attached']['css'] = array(
    drupal_get_path('module', 'recurly') . '/css/recurly.css',
  );

  $total_displayed = 0;
  foreach ($page_subscriptions as $subscription) {
    // Do not show subscriptions that are not active if only showing active.
    // TODO: Figure out if Recurly_SubscriptionList can only return active
    // subscriptions to begin with, rather than hiding them like this.
    if ($subscription->state === 'expired' && $subscription_type === 'live') {
      continue;
    }
    $total_displayed++;

    // Gets account subscription's activated_at date
    foreach ($subscription->activated_at as $key => $activated_at) {
      if ($key == 'date') {
        $date_activated = $activated_at;
      }
    }

    // Gets account subscription's current_period_started_at date
    foreach ($subscription->current_period_started_at as $key => $current_period_started_at) {
      if ($key == 'date') {
        $current_date_started = $current_period_started_at;
      }
    }

    // Gets account subscription's current_period_ends_at date
    foreach ($subscription->current_period_ends_at as $key => $current_period_ends_at) {
      if ($key == 'date') {
        $current_date_ends = $current_period_ends_at;
      }
    }

    // Determine the state of this subscription.
    //$states = myrecurly_subscription_get_states($subscription, $account);
    $states = $subscription->state;

    $state = $subscription->state;
    $action1 = $action2 = $action3 = '';
    $action1_url = $action2_url = $action3_url = '';
    if ($state == 'active') {
      $action1 = 'Cancel';
      $action1_url = "user/{$user->uid}/cancel_mysubscription/{$subscription->uuid}";
      $action2 = 'Terminate';
      $action2_url = "user/{$user->uid}/terminate_mysubscription/{$subscription->uuid}";
      $action3 = 'Postpone';
      $action3_url = "user/{$user->uid}/postpone_mysubscription/{$subscription->uuid}";
      //$action3_url = 'admin/config/services/myrecurly/postpone_subscription/' . $subscription->uuid;
    } else if ($state == 'canceled') {
      $action1 = 'Re-Activate';
      $action1_url = "user/{$user->uid}/reactivate_mysubscription/{$subscription->uuid}";
    }

    $rows[] = array(
      //array('data' => l($subscription->plan->plan_code, 'admin/config/services/myrecurly/edit_subscription/' .
      array('data' => l($subscription->plan->plan_code, "user/{$user->uid}/edit_mysubscription/{$subscription->uuid}")),
      array('data' => $subscription->plan->name, 'width' => '120'),
      array('data' => $subscription->quantity, 'style' => 'width:100px;color:red;'),
      array('data' => $subscription->currency . ' ' . ($subscription->unit_amount_in_cents/100.00)),
      array('data' => $date_activated),
      array('data' => $current_date_started),
      array('data' => $current_date_ends),
      //array('data' => $subscription->uuid),
      array('data' => $subscription->state),
      array('data' => l($action1, $action1_url, array('attributes' => array('class' => 'js_link')))),
      array('data' => l($action2, $action2_url, array('attributes' => array('class' => 'js_link')))),
      array('data' => l($action3, $action3_url, array('attributes' => array('class' => 'js_link')))),
      /*array('data' => l('cancel', 'admin/config/services/myrecurly/cancel_subscription/' . 
        $subscription->plan->plan_code, array('attributes' => array('class' => 'js_link'))  )),*/
    );

  }

  $caption = '<div style="font-size:20px;text-align:left;">Manage Subscriptions</div>';
  $build['hook_table'] = array(
    '#theme' => 'table__myrecurly__hooks',
    '#header' => $header,
    '#rows' => $rows,
    //'#attributes' => $attributes,
    '#attributes' => array('id' => 'myrecurly-manage-subscription'),
    '#empty' => t('No Subscriptions is available.'),
    '#caption' => $caption, //'Manage Subscriptions',
    '#sticky' => TRUE,
  );

  return $build;
}
function myrecurly_user_warn_pagexxx() {
  global $user;

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $user_email = $user->mail;
  $account_code = $user_email;

  $per_page = 50;
  $subscription_list = Recurly_SubscriptionList::getForAccount($account_code, array('per_page' => $per_page));

  $page_subscriptions = myrecurly_pager_results($subscription_list, $per_page);

  $subscriptions['subscriptions']['#attached']['css'] = array(
    drupal_get_path('module', 'recurly') . '/css/recurly.css',
  );

  $total_displayed = 0;
  foreach ($page_subscriptions as $subscription) {
    // Do not show subscriptions that are not active if only showing active.
    // TODO: Figure out if Recurly_SubscriptionList can only return active
    // subscriptions to begin with, rather than hiding them like this.
    if ($subscription->state === 'expired' && $subscription_type === 'live') {
      continue;
    }
    $total_displayed++;

    // Determine the state of this subscription.
    //$states = myrecurly_subscription_get_states($subscription, $account);
    $states = $subscription->state;

    if (variable_get('recurly_subscription_max', '1') === '1') {
      $links = FALSE;
    }
    else {
      $links = myrecurly_subscription_links($entity_type, $entity, $subscription, $account, $states);
    }

    $plan = $subscription->plan;
    $add_ons = array();
    $total = 0;
    foreach ($subscription->subscription_add_ons as $add_on) {
      // Fully load the add on to get the name attribute.
      $full_add_on = Recurly_Addon::get($plan->plan_code, $add_on->add_on_code);
      $add_ons[$add_on->add_on_code] = array(
        'add_on_code' => $add_on->add_on_code,
        'name' => check_plain($full_add_on->name),
        'quantity' => check_plain($add_on->quantity),
        'cost' => recurly_format_currency($add_on->unit_amount_in_cents, $subscription->currency),
      );
      $total += $add_on->unit_amount_in_cents * $add_on->quantity;
    }
    $total += $subscription->unit_amount_in_cents * $subscription->quantity;

    $message = '';
    foreach ($states as $state) {
      $message = myrecurly_subscription_state_message($state, array('account' => $account, 'subscription' => $subscription));
      break;
    }

    $subscriptions['subscriptions'][$subscription->uuid] = array(
      '#theme' => 'recurly_subscription_summary',
      '#plan_code' => $plan->plan_code,
      '#plan_name' => check_plain($plan->name),
      '#state_array' => $states,
      '#cost' => myrecurly_format_currency($subscription->unit_amount_in_cents, $subscription->currency),
      '#quantity' => $subscription->quantity,
      '#add_ons' => $add_ons,
      '#start_date' => recurly_format_date($subscription->activated_at),
      '#end_date' => isset($subscription->expires_at) ? recurly_format_date($subscription->expires_at) : NULL,
      '#current_period_start' => recurly_format_date($subscription->current_period_started_at),
      '#current_period_ends_at' => recurly_format_date($subscription->current_period_ends_at),
      '#total' => myrecurly_format_currency($total, $subscription->currency),
      '#subscription_links' => $links ? theme('links', array('links' => $links, 'attributes' => array('class' => array('inline', 'links')))) : NULL,
      '#message' => $message,
    );
  }

  $subscriptions['pager'] = array(
    '#markup' => theme('pager'),
    '#access' => $subscription_list->count() > $per_page,
  );

  drupal_alter('myrecurly_subscription_list_page', $subscriptions);

  // If the user doesn't have any active subscriptions, redirect to the signup.
  if (count(element_children($subscriptions['subscriptions'])) === 0) {
    drupal_goto($entity_type . '/' . $id . '/subscription/signup');
  }

  return $subscriptions;
}

/**
 * Reactivates subscription in user leve;
 */
function myrecurly_user_reactivate_subscription() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $uuid = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscription = Recurly_Subscription::get($uuid);
  $subscription->reactivate();

  drupal_set_message(t('Your subscription was reactivated successfully.'));

  drupal_goto("user/{$user->uid}/mysubscription/");
}

/**
 * Cancels user's subscription
 */
function myrecurly_user_cancel_subscription() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $uuid = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscription = Recurly_Subscription::get($uuid);
  $subscription->cancel();

  drupal_set_message(t('Your subscription was canceled successfully.'));

  drupal_goto("user/{$user->uid}/mysubscription/");
}

/**
 * Terminates user's subscription
 */
function myrecurly_user_terminate_subscription() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $uuid = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscription = Recurly_Subscription::get($uuid);
  $subscription->terminateWithoutRefund();

  drupal_set_message(t('Your subscription was terminated successfully.'));

  drupal_goto("user/{$user->uid}/mysubscription/");
}

/**
 * Postpone user's subscription
 */
function myrecurly_user_postpone_subscription() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $uuid = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscription = Recurly_Subscription::get($uuid);

  $plan_list = Recurly_PlanList::get();

  $plans = array();
  foreach ($plan_list as $key => $value) {
    $plans[$value->plan_code] = $value->name;
  }

  // current_period_ends_at
  // Gets account subscription's current_period_ends_at date
  foreach ($subscription->current_period_ends_at as $key => $current_period_ends_at) {
    if ($key == 'date') {
      $current_date_ends = $current_period_ends_at;
    }
  }

  asort($plans);

  $subscription_info = array(
    'uuid' => $uuid,
    'plan_name' => $subscription->plan->name,
    'plan_code' => $subscription->plan->plan_code,
    'quantity' => $subscription->quantity,
    'plans' => $plans,
    'current_date_ends' => $current_date_ends,
  );

  return drupal_get_form('myrecurly_user_postpone_form', $subscription_info);

}

/**
 * Postpones user's subscription
 */
function myrecurly_user_postpone_form($form, &$form_state, $data) {
  $form['#attached']['css'][drupal_get_path('module', 'myrecurly') . '/sub_form.css'] = array();
  // Account Info section
  $form['subscription_information'] = array(
    '#type' => 'item',
    '#title' => t('Subscription Information'),
    '#prefix' => '<div style="font-size:20px;">',
    '#suffix' => '</div>',
  );
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    // Make the fieldset collapsible.
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  );
  $form['name'] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
  );
    
  // Make these fields required.
  $form['name']['plan_code'] = array(
    '#type' => 'select',
    '#title' => t('Plan name'),
    '#options' => $data['plans'],
    '#value' => $data['plan_code'],
    '#prefix' => '<div class="col1 name first">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    '#attributes' => array('disabled' => 'disabled'),
  );

  $form['name']['uuid'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription ID'),
    '#size' => 35,
    '#maxlength' => 35,
    '#value' => $data['uuid'],
    '#prefix' => '<div class="col1 name">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    '#attributes' => array('readonly' => 'readonly'),
  );

  $form['quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#size' => 3,
    '#maxlength' => 3,
    '#value' => $data['quantity'],
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#attributes' => array('readonly' => 'readonly'),
  );

  $form['current_date_ends'] = array(
    '#type' => 'date_popup',
    '#title' => t('Current Subscription Date Ends at'),
    '#date_format' => 'd-m-y',
    '#default_value' => $data['current_date_ends'], //date('Y-m-d'),
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => "F j, Y", // 'm/d/Y',
    '#size'=>20,
    '#attributes' => array('disabled' => 'disabled'),
  );

  $form['postpone_date'] = array(
    //'#type' => 'date',
    '#type' => 'date_popup',
    '#title' => t('Postpone Date'), 
    //'#default_value' => variable_get('pickup_date_'.$id,''),
    '#date_format' => 'd-m-y',
    '#default_value' => $data['current_date_ends'], //date('Y-m-d'),
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => "F j, Y", // 'm/d/Y',
    '#size'=>20,
    '#required' => true,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<input class="event-node-cancel" type="button" value="Cancel" onClick="history.go(-1)"/>',
  );

  return $form;

}

/**
 * Validates user's postpone form
 */
function myrecurly_user_postpone_form_validate($form, &$form_state) {
  $postpone_date = $form_state['input']['postpone_date']['date'];
  $current_date_ends = $form_state['values']['current_date_ends'];

  if (time() > strtotime($postpone_date) && strtotime($current_date_ends) > strtotime($postpone_date)) {
    form_error($form, t('%name has to be in the future.', array('%name' => $form['postpone_date']['#title'])));
  }
}

/**
 * user defined hook_form_submit
 */
function myrecurly_user_postpone_form_submit($form, &$form_state) {
  global $user;

  $postpone_date = $form_state['input']['postpone_date']['date'];
  
  try {
    if (!myrecurly_client_initialize()) {
      return t('Could not initialize the Recurly client.');
    }
    
    $uuid = $form_state['input']['uuid'];

    $subscription = Recurly_Subscription::get($uuid);
    $subscription->postpone(date('c', strtotime($postpone_date)));

  } catch (Exception $e) {
    switch(get_class($e)){
      case 'Recurly_NotFoundError':
        drupal_set_message('Record could not be found', 'error');
      case 'Recurly_ValidationError':
        //if there are multiple errors, they are comma delimited
        /*$messages = explode(',',$e->getMessage()); 
        foreach($messages as $message){
          print $message . "\n";
        }*/
        drupal_set_message($e->getMessage(), 'error');
        break;
      case 'Recurly_ServerError':
        drupal_set_message('Problem communicating with Recurly', 'error');
      default:
        drupal_set_message(get_class($e) . ': ' . $e->getMessage(), 'error');
    }
    $form_state['redirect'] = FALSE;
  }

  $message = 'Subscription information was updated successfully.';
  drupal_set_message(t($message));

  drupal_goto("user/{$user->uid}/mysubscription/");
}

/**
 * Updates subscription information
 */
function myrecurly_user_edit_subscription() {
  global $user;
  
  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $uuid = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscription = Recurly_Subscription::get($uuid);

  $plan_list = Recurly_PlanList::get();

  $plans = array();
  foreach ($plan_list as $key => $value) {
  	$plans[$value->plan_code] = $value->name;
  }

  asort($plans);

  $subscription_info = array(
  	'uuid' => $uuid,
  	'plan_name' => $subscription->plan->name,
  	'plan_code' => $subscription->plan->plan_code,
  	'quantity' => $subscription->quantity,
  	'plans' => $plans,
  );

  return drupal_get_form('myrecurly_mysubinfo_form', $subscription_info);

}

/**
 * Subscription information form
 */
function myrecurly_mysubinfo_form($form, &$form_state, $data) {
  /////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////// STARTS THE FORM //////////////////////////////////////
  $form['#attached']['css'][drupal_get_path('module', 'myrecurly') . '/sub_form.css'] = array();
  // Account Info section
  $form['subscription_information'] = array(
    '#type' => 'item',
    '#title' => t('Subscription Information'),
    '#prefix' => '<div style="font-size:20px;">',
    '#suffix' => '</div>',
  );
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    // Make the fieldset collapsible.
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  );
  $form['name'] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
  );
    
  // Make these fields required.
  $form['name']['plan_code'] = array(
    '#type' => 'select',
    '#title' => t('Plan name'),
    '#options' => $data['plans'],
    '#value' => $data['plan_code'],
    '#prefix' => '<div class="col1 name first">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    //'#attributes' => array('readonly' => 'readonly'),
  );

  $form['name']['uuid'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription ID'),
    '#size' => 35,
    '#maxlength' => 35,
    '#value' => $data['uuid'],
    '#prefix' => '<div class="col1 name">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    '#attributes' => array('readonly' => 'readonly'),
  );

  $form['quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#size' => 3,
    '#maxlength' => 3,
    '#value' => $data['quantity'],
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<input class="event-node-cancel" type="button" value="Cancel" onClick="history.go(-1)"/>',
  );

  return $form;
}

/**
 * user defined hook_form_validate
 */
function myrecurly_mysubinfo_form_validate($form, &$form_state) {
  $quantity = $form_state['input']['quantity'];
  if ($quantity !== '' && (!is_numeric($quantity) || intval($quantity) != $quantity || $quantity <= 0)) {
  	form_error($form, t('%name must be a postive integer.', array('%name' => $form['quantity']['#title'])));
  }
}

/**
 * user defined hook_form_submit
 */
function myrecurly_mysubinfo_form_submit($form, &$form_state) {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }
  
  try {
  	if (!myrecurly_client_initialize()) {
      return t('Could not initialize the Recurly client.');
    }
    
    $uuid = $form_state['input']['uuid'];

    $subscription = Recurly_Subscription::get($uuid);
    $subscription->plan_code = $form_state['input']['plan_code'];
    $subscription->quantity= $form_state['input']['quantity'];

    $subscription->updateImmediately();

  } catch (Exception $e) {
  	switch(get_class($e)){
      case 'Recurly_NotFoundError':
        drupal_set_message('Record could not be found', 'error');
      case 'Recurly_ValidationError':
        //if there are multiple errors, they are comma delimited
        /*$messages = explode(',',$e->getMessage()); 
        foreach($messages as $message){
          print $message . "\n";
        }*/
        drupal_set_message($e->getMessage(), 'error');
        break;
      case 'Recurly_ServerError':
        drupal_set_message('Problem communicating with Recurly', 'error');
      default:
        drupal_set_message(get_class($e) . ': ' . $e->getMessage(), 'error');
    }
    $form_state['redirect'] = FALSE;
  }

  $message = 'Subscription information was updated successfully.';
  drupal_set_message(t($message));

  drupal_goto("user/{$user->uid}/mysubscription/");
}

/**
 * Shows account information
 */
function myrecurly_user_account() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  $build = array();

  $header = array(
    array('data' => t('Account Code')),
    array('data' => t('First Name')),
    array('data' => t('Last Name')),
    array('data' => t('Status')),
    array('data' => t('Created @')),
    array('data' => t('Action'), 'colspan' => 3, 'style' => 'text-align:center;'),
  );

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $account = Recurly_Account::get($user->mail);
  
  $rows = array();

  $path = drupal_get_path('module', 'myrecurly');
  drupal_add_js($path . '/js/myrecurly_helper.js');

  //drupal_add_js('alert("checking!")', 'inline');
  
  	
  // Gets account's created_at date
  foreach ($account->created_at as $key => $created_at) {
    if ($key == 'date') {
   	  $date_created_at = $created_at;
  	}
  }

  $state = $account->state;
  $action1 = $action2 = $action3 = '';
  $action1_url = $action2_url = $action3_url = '';
  if ($state == 'active') {
  	$action1 = 'Close';
  	$action1_url = "user/{$user->uid}/close_myaccount/{$account->account_code}";
  	$action2 = 'Edit';
  	$action2_url = "user/{$user->uid}/edit_myaccount/{$account->account_code}";
  	//$action3 = 'Postpone';
  	//$action3_url = 'admin/config/services/myrecurly/postpone_subscription/' . $account->account_code;
  } else if ($state == 'closed') {
  	$action1 = 'Re-open';
  	$action1_url = "user/{$user->uid}/reopen_myaccount/{$account->account_code}";
  }

  $rows[] = array(
    /*array('data' => l($account->account_code, 'admin/config/services/myrecurly/manage_subscription/' .
      $account->account_code)),*/
    array('data' => $account->account_code),
    array('data' => $account->first_name, 'style' => 'color:red;'),
    array('data' => $account->last_name),
    array('data' => $account->state),
    array('data' => $date_created_at),
    array('data' => l($action1, $action1_url, array('attributes' => array('class' => 'js_link')))),
    array('data' => l($action2, $action2_url, array('attributes' => array('class' => 'js_link')))),
    array('data' => l($action3, $action3_url, array('attributes' => array('class' => 'js_link')))),
    //array('data' => l('edit', 'www.google.com')),
  );

  $attributes = array(
    'border'	  => 10,
    'cellspacing' => 0,
    'cellpadding' => 5,
  );

  $caption = '<div style="font-size:20px;text-align:left;">Account Information</div>';
  $build['hook_table'] = array(
  	'#theme' => 'table__myrecurly__hooks',
  	'#header' => $header,
  	'#rows' => $rows,
  	//'#attributes' => $attributes,
  	'#attributes' => array('id' => 'myrecurly-list-accounts'),
  	'#empty' => t('No accounts subscribed.'),
  	'#caption' => $caption,
  );

  //drupal_add_tabledrag('myrecurly-list-subscription', 'order', 'sibling', 'mytable-order-weight');
  
  return $build;
}

/**
 * Edits/Updates account's information
 */
function myrecurly_user_edit_account() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $account_code = end($tmp);

  if ($account_code == 'edit_account') {
  	$url = substr($param, 0, strpos($param, $account_code));
  	form_set_error('invalid_account_code', 'Invalid Account Code selected. Cannot edit this Account.');
  	drupal_goto($url . 'accounts');
  	return;
  }

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $account = Recurly_Account::get($account_code);

  $billing_info = Recurly_BillingInfo::get($account_code);

  $account_info = array(
  	'first_name' => $account->first_name,
  	'last_name' => $account->last_name,
  	'email' => $account->email,
  	'billing_first_name' => $billing_info->first_name,
    'billing_last_name' => $billing_info->last_name,
    'billing_address1' => $billing_info->address1,
    'billing_city' => $billing_info->city,
    'billing_state' => $billing_info->state,
    'billing_zipcode' => $billing_info->zip,
    'billing_coutry' => $billing_info->country,
    'billing_card_type' => $billing_info->card_type,
    'billing_year' => $billing_info->year,
    'billing_month' => $billing_info->month,
    'cc_first_six' => $billing_info->first_six,
    'cc_last_four' => $billing_info->last_four,
    'billing_cc_number' => '***********' . $billing_info->last_four,
    'billing_phone_number' => $billing_info->phone,
  );

  return drupal_get_form('myrecurly_useracctinfo_form', $account_info);
}

/**
 * Account information form
 */
function myrecurly_useracctinfo_form($form, &$form_state, $data) {
  /////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////// STARTS THE FORM //////////////////////////////////////
  $form['#attached']['css'][drupal_get_path('module', 'myrecurly') . '/sub_form.css'] = array();
  // Account Info section
  $form['contact_information'] = array(
    '#type' => 'item',
    '#title' => t('Contact Information'),
    '#prefix' => '<div style="font-size:20px;">',
    '#suffix' => '</div>',
  );
  $form['name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    // Make the fieldset collapsible.
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  );
  $form['name'] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
  );
    
  // Make these fields required.
  $form['name']['first']['default_value'] = '';
  $form['name']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#value' => $data['first_name'],
    '#size' => 30,
    '#maxlength' => 30,
    '#prefix' => '<div class="col1 name first">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
  );

  $form['name']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#size' => 30,
    '#maxlength' => 30,
    '#value' => $data['last_name'],
    '#prefix' => '<div class="col1 name">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
  );

  $form['e_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#size' => 35,
    '#maxlength' => 35,
    '#value' => $data['email'],
    '#required' => TRUE,
  );
  
  $form['billing_information'] = array(
    '#type' => 'item',
    '#title' => t('Billing Information'),
    '#prefix' => '<div style="font-size:20px;">',
    '#suffix' => '</div>',
  );

  $form['billing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Billing'),
    // Make the fieldset collapsible.
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  );

  $form['billing'] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
  );

  $form['billing']['cc_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit Card Number'),
    '#size' => 20,
    '#maxlength' => 20,
    '#value' => $data['billing_cc_number'],
    '#attributes' => array('autocomplete' =>'off'),
    '#prefix' => '<div class="col1">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );

  $form['billing']['cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV'),
    '#size' => 4,
    '#attributes' => array('autocomplete' =>'off'),
    '#prefix' => '<div class="col1">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
  );

  $cc_exp = $data['billing_year'] . '-0' . $data['billing_month'] . '-01';

  $form['cc_exp'] = array(
    '#type' => 'date_select',
    '#title' => t('Credit Card Expiration'),
    //'#title_display' => 'invisible',
    '#date_format' => 'm-Y',
    '#default_value' => $cc_exp, //$cc_default,
    '#date_year_range' => '0:+15',
    '#size' => 40,
    //'#attributes' => array('title' => t('Enter your CC expiration')),
    '#date_label_position' => 'within',
    '#required' => TRUE,
  );

  $form['billing_address1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 1'),
    '#size' => 30,
    '#maxlength' => 30,
    '#value' => $data['billing_address1'],
    '#required' => TRUE,
  );

  $form['billing_address2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => FALSE,
  );

  $form['billing_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#size' => 20,
    '#maxlength' => 20,
    '#value' => $data['billing_city'],
    '#required' => TRUE,
  );

  $form['bil_state_zip'] = array(
    '#type' => 'fieldset',
    '#title' => t('Billing'),
    // Make the fieldset collapsible.
    '#collapsible' => TRUE, // Added
    '#collapsed' => FALSE,  // Added
  );

  $form['bil_state_zip'] = array(
    '#prefix' => '<div class="two-col">',
    '#suffix' => '</div>'
  );

  $form['bil_state_zip']['billing_state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#size' => 20,
    '#maxlength' => 20,
    '#value' => $data['billing_state'],
    '#prefix' => '<div class="col1">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );

  $form['bil_state_zip']['billing_zipcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip Code'),
    '#size' => 12,
    '#maxlength' => 12,
    '#value' => $data['billing_zipcode'],
    '#prefix' => '<div class="col1">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  $form['billing_country_code'] = array(
    //'#type' => 'textfield',
    '#type' => 'select',
    '#options' => myrecurly_list_countries(),
    '#default_value' => 'US',
    '#value' => 'US',
    '#title' => t('Country Code'),
    '#required' => TRUE,
  );

  $form['billing_phone_number'] = array(
    //'#type' => 'textfield',
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#size' => 20,
    '#maxlength' => 20,
    '#value' => $data['billing_phone_number'],
    '#required' => FALSE,
  );

  $form['reopen_account'] = array(
  	'#type' => 'hidden',
  	'#value' => isset($data['reopen_account']) ? 'yes' : 'no',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<input class="event-node-cancel" type="button" value="Cancel" onClick="history.go(-1)"/>',
  );

  return $form;
}

/**
 * user defined hook_form_validate
 */
function myrecurly_useracctinfo_form_validate($form, &$form_state) {
  // Something here
}

/**
 * user defined hook_form_submit
 */
function myrecurly_useracctinfo_form_submit($form, &$form_state) {
  global $user;
  
  try {
  	if (!myrecurly_client_initialize()) {
      return t('Could not initialize the Recurly client.');
    }
    $account = new Recurly_Account();
    $account->account_code = $form_state['input']['e_mail'];;
    $account->email = $form_state['input']['e_mail'];
    $account->first_name = $form_state['input']['first'];
    $account->last_name = $form_state['input']['last'];

    $billing_info = new Recurly_BillingInfo();
    $billing_info->first_name = $form_state['input']['first'];
    $billing_info->last_name = $form_state['input']['last'];
    $billing_info->address1 = $form_state['input']['billing_address1'];
    $billing_info->address2 = $form_state['input']['billing_address2'];
    $billing_info->city = $form_state['input']['billing_city'];
    $billing_info->state = $form_state['input']['billing_state'];
    $billing_info->zip = $form_state['input']['billing_zipcode'];
    $billing_info->country = $form_state['input']['billing_country_code'];
    $billing_info->number = $form_state['input']['cc_number'];
    $billing_info->month = $form_state['input']['cc_exp']['month'];
    $billing_info->year = $form_state['input']['cc_exp']['year'];
    $billing_info->phone = $form_state['input']['billing_phone_number'];
    $billing_info->ip_address = $_SERVER['REMOTE_ADDR'];

    $account->billing_info = $billing_info;

    $account->update();

    // If there is a flag reopen_account with value yes
    if (isset($form_state['values']['reopen_account']) && $form_state['values']['reopen_account'] == 'yes') {
      $account = Recurly_Account::get($account->account_code);
      $account->reopen();
      drupal_set_message(t('Your account was reopened successfully.'));
    }

  } catch (Exception $e) {
  	switch(get_class($e)){
      case 'Recurly_NotFoundError':
        drupal_set_message('Record could not be found', 'error');
      case 'Recurly_ValidationError':
        //if there are multiple errors, they are comma delimited
        /*$messages = explode(',',$e->getMessage()); 
        foreach($messages as $message){
          print $message . "\n";
        }*/
        drupal_set_message($e->getMessage(), 'error');
        break;
      case 'Recurly_ServerError':
        drupal_set_message('Problem communicating with Recurly', 'error');
      default:
        drupal_set_message(get_class($e) . ': ' . $e->getMessage(), 'error');
    }
    $form_state['redirect'] = FALSE;
  }

  $message = 'Account information was updated successfully.';
  drupal_set_message(t($message));

  drupal_goto("user/{$user->uid}/myaccount/");
  
}

/**
 * Closes subscription account
 */
function myrecurly_user_close_account() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $account_code = end($tmp);

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $account = Recurly_Account::get($account_code);
  $account->close();

  drupal_set_message(t('Your account was closed successfully.'));

  drupal_goto("user/{$user->uid}/myaccount/");
}

/**
 * Reopen closed subscription account
 */
function myrecurly_user_reopen_account() {
  global $user;

  if (!property_exists($user, 'mail')) {
  	return drupal_access_denied();
  }

  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $account_code = end($tmp);

  if ($account_code == 'reopen_account') {
  	$url = substr($param, 0, strpos($param, $account_code));
  	form_set_error('invalid_account_code', 'Invalid Account Code selected. Cannot Re-Open this Account.');
  	drupal_goto($url . 'accounts');
  	return;
  }

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $account = Recurly_Account::get($account_code);

  $account_info = array(
  	'first_name' => $account->first_name,
  	'last_name' => $account->last_name,
  	'email' => $account->email,
  	'billing_first_name' => '',
    'billing_last_name' => '',
    'billing_address1' => '',
    'billing_city' => '',
    'billing_state' => '',
    'billing_zipcode' => '',
    'billing_coutry' => '',
    'billing_card_type' => '',
    'billing_year' => '',
    'billing_month' => '',
    'cc_first_six' => '',
    'cc_last_four' => '',
    'billing_cc_number' => '',
    'billing_phone_number' => '',
    'reopen_account' => 'yes',
  );

  return drupal_get_form('myrecurly_useracctinfo_form', $account_info);

  // Gets get parameter
  /*$get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $account_code = end($tmp);

  if ($account_code == 'reopen_account') {
  	$url = substr($param, 0, strpos($param, $account_code));
  	form_set_error('invalid_account_code', 'Invalid Account Code selected. Cannot Re-Open this Account.');
  	drupal_goto($url . 'accounts');
  	return;
  }

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $account = Recurly_Account::get($account_code);
  $account->reopen();

  drupal_set_message(t('Your account was reopened successfully.'));

  drupal_goto("user/{$user->uid}/myaccount/");*/
}

/**
 * Utility function to retrieve a specific page of results from a Recurly_Pager.
 *
 * @param $pager_object
 *   Any object that extends a Recurly_Pager object, such as a
 *   Recurly_InvoiceList, Recurly_SubscriptionList, or Recurly_TransactionList.
 * @param $element
 *   A unique identifier for this pager.
 * @param $per_page
 *   The number of items to display per page.
 * @param $page_num
 *   The desired page number to display. Usually automatically determined from
 *   the URL.
 */
function myrecurly_pager_results($pager_object, $per_page, $page_num = NULL) {
  if (!isset($page_num)) {
    $page_num = isset($_GET['page']) ? (int) $_GET['page'] : 0;
  }

  // Fast forward the list to the current page.
  $start = $page_num * $per_page;
  for ($n = 0; $n < $start; $n++) {
    $pager_object->next();
  }

  // Populate $page_results with the current page.
  $total = $pager_object->count();
  $page_end = min($start + $per_page, $total);
  $page_results = array();
  for ($n = $start; $n < $page_end; $n++) {
    $item = $pager_object->current();
    $page_results[$item->uuid] = $item;
    $pager_object->next();
  }

  pager_default_initialize($total, $per_page);

  return $page_results;
}

/**
 * Build a list of links to manage a subscription.
 */
function myrecurly_subscription_links($entity_type, $entity, $subscription, $account, $states) {
  // Generate the list of links for this subscription.
  $url_context = array(
    'entity_type' => $entity_type,
    'entity' => $entity,
    'subscription' => $subscription,
    'account' => $account,
  );

  $links = array();
  if ($subscription->state === 'active') {
    $links['change'] = array(
      'href' => recurly_url('change_plan', $url_context),
      'external' => TRUE,
      'title' => t('Change plan'),
    );
    $links['cancel'] = array(
      'href' => recurly_url('cancel', $url_context),
      'external' => TRUE,
      'title' => t('Cancel'),
      // Pass in the past_due flag to accurately calculate refunds.
      'query' => in_array('past_due', $states) ? array('past_due' => '1') : NULL,
    );
  }
  elseif ($subscription->state === 'canceled') {
    $links['reactivate'] = array(
      'href' => recurly_url('reactivate', $url_context),
      'external' => TRUE,
      'title' => t('Reactivate'),
    );
  }
  // Allow other modules to provide links, perhaps "suspend" for example.
  drupal_alter('myrecurly_subscription_links', $links);

  return $links;
}

/**
 * Returns a message for a subscription if the subscription state is not active.
 */
function myrecurly_subscription_state_message($state, $context) {
  switch ($state) {
    case 'active':
      return '';
    case 'closed':
      return t('This account is closed.');
    case 'in_trial':
      return t('Currently in trial period.');
    case 'past_due':
      $url = recurly_url('update_billing', $context);
      if ($url) {
        return t('This account is past due. Please <a href="!url">update your billing information</a>.', array('!url' => $url));
      }
      else {
        return t('This account is past due. Please contact an administrator to update your billing information.');
      }
    case 'canceled':
      $url = recurly_url('reactivate', $context);
      if ($url) {
        return t('This plan is canceled and will not renew.') . ' ' . t('You may  <a href="!url">reactivate the plan</a> to resume billing.', array('!url' => $url));
      }
      else {
        return t('This plan is canceled and will not renew.');
      }
    case 'expired':
      $url = recurly_url('select_plan', $context);
      if ($url) {
        return t('This plan has expired.') . ' ' . t('Please <a href="!url">purchase a new subscription</a>.', array('!url' => $url));
      }
      else {
        return t('This plan has expired.');
      }
    case 'pending_subscription':
      return t('This plan with be changed to @plan on @date.', array('@plan' => $context['subscription']->pending_subscription->plan->name, '@date' => recurly_format_date($context['subscription']->current_period_ends_at)));
    case 'future':
      return t('This plan has not started yet. Please contact support if you have any questions.');
    default:
      return '';
  }
}

/**
 * Get a list of all states in which a subscription exists currently.
 *
 * @param $subscription
 *   A Recurly subscription object.
 * @param $account
 *   A Recurly account object.
 */
function myrecurly_subscription_get_states($subscription, $account) {
  static $past_due = array();
  $states = array();

  // Determine if in a trial.
  if ($subscription->trial_started_at && $subscription->trial_ends_at) {
    $subscription->trial_started_at->setTimezone(new DateTimeZone('UTC'));
    $subscription->trial_ends_at->setTimezone(new DateTimeZone('UTC'));
    $start = $subscription->trial_started_at->format('U');
    $end = $subscription->trial_ends_at->format('U');
    if (REQUEST_TIME > $start && REQUEST_TIME < $end) {
      $states[] = 'in_trial';
    }
  }

  // Determine if non-renewing.
  if (!empty($subscription->total_billing_cycles)) {
    $states[] = 'non_renewing';
  }

  // Retrieve past due subscriptions.
  if (!isset($past_due[$account->account_code])) {
    $subscriptions = Recurly_SubscriptionList::getForAccount($account->account_code, array('state' => 'past_due'));
    $past_due[$account->account_code] = array();
    foreach ($subscriptions as $past_due_subscription) {
      $past_due[$account->account_code][] = $past_due_subscription->uuid;
    }
  }
  if (in_array($subscription->uuid, $past_due[$account->account_code])) {
    $states[] = 'past_due';
  }

  // Subscriptions that have pending changes.
  if (!empty($subscription->pending_subscription)) {
    $states[] = 'pending_subscription';
  }

  $states[] = $subscription->state;
  return $states;
}

/**
 * Lists of available countries
 */
function myrecurly_list_countries() {

  $array = array(
    "AF" => "Afghanistan",
    "AL" => "Albania",
    "DZ" => "Algeria",
    "AS" => "American Samoa",
    "AD" => "Andorra",
    "AO" => "Angola",
    "AI" => "Anguilla",
    "AQ" => "Antarctica",
    "AG" => "Antigua and Barbuda",
    "AR" => "Argentina",
    "AM" => "Armenia",
    "AW" => "Aruba",
    "AC" => "Ascension Island",
    "AU" => "Australia",
    "AT" => "Austria",
    "AZ" => "Azerbaijan",
    "BS" => "Bahamas",
    "BH" => "Bahrain",
    "BD" => "Bangladesh",
    "BB" => "Barbados",
    "BY" => "Belarus",
    "BE" => "Belgium",
    "BZ" => "Belize",
    "BJ" => "Benin",
    "BM" => "Bermuda",
    "BT" => "Bhutan",
    "BO" => "Bolivia",
    "BA" => "Bosnia and Herzegovina",
    "BW" => "Botswana",
    "BV" => "Bouvet Island",
    "BR" => "Brazil",
    "BQ" => "British Antarctic Territory",
    "IO" => "British Indian Ocean Territory",
    "VG" => "British Virgin Islands",
    "BN" => "Brunei",
    "BG" => "Bulgaria",
    "BF" => "Burkina Faso",
    "BI" => "Burundi",
    "KH" => "Cambodia",
    "CM" => "Cameroon",
    "CA" => "Canada",
    "IC" => "Canary Islands",
    "CT" => "Canton and Enderbury Islands",
    "CV" => "Cape Verde",
    "KY" => "Cayman Islands",
    "CF" => "Central African Republic",
    "EA" => "Ceuta and Melilla",
    "TD" => "Chad",
    "CL" => "Chile",
    "CN" => "China",
    "CX" => "Christmas Island",
    "CP" => "Clipperton Island",
    "CC" => "Cocos [Keeling] Islands",
    "CO" => "Colombia",
    "KM" => "Comoros",
    "CD" => "Congo [DRC]",
    "CG" => "Congo [Republic]",
    "CK" => "Cook Islands",
    "CR" => "Costa Rica",
    "HR" => "Croatia",
    "CU" => "Cuba",
    "CY" => "Cyprus",
    "CZ" => "Czech Republic",
    "DK" => "Denmark",
    "DG" => "Diego Garcia",
    "DJ" => "Djibouti",
    "DM" => "Dominica",
    "DO" => "Dominican Republic",
    "NQ" => "Dronning Maud Land",
    "TL" => "East Timor",
    "EC" => "Ecuador",
    "EG" => "Egypt",
    "SV" => "El Salvador",
    "GQ" => "Equatorial Guinea",
    "ER" => "Eritrea",
    "EE" => "Estonia",
    "ET" => "Ethiopia",
    "FK" => "Falkland Islands [Islas Malvinas]",
    "FO" => "Faroe Islands",
    "FJ" => "Fiji",
    "FI" => "Finland",
    "FR" => "France",
    "GF" => "French Guiana",
    "PF" => "French Polynesia",
    "TF" => "French Southern Territories",
    "FQ" => "French Southern and Antarctic Territories",
    "GA" => "Gabon",
    "GM" => "Gambia",
    "GE" => "Georgia",
    "DE" => "Germany",
    "GH" => "Ghana",
    "GI" => "Gibraltar",
    "GR" => "Greece",
    "GL" => "Greenland",
    "GD" => "Grenada",
    "GP" => "Guadeloupe",
    "GU" => "Guam",
    "GT" => "Guatemala",
    "GG" => "Guernsey",
    "GN" => "Guinea",
    "GW" => "Guinea-Bissau",
    "GY" => "Guyana",
    "HT" => "Haiti",
    "HM" => "Heard Island and McDonald Islands",
    "HN" => "Honduras",
    "HK" => "Hong Kong",
    "HU" => "Hungary",
    "IS" => "Iceland",
    "IN" => "India",
    "ID" => "Indonesia",
    "IR" => "Iran",
    "IQ" => "Iraq",
    "IE" => "Ireland",
    "IM" => "Isle of Man",
    "IL" => "Israel",
    "IT" => "Italy",
    "CI" => "Ivory Coast",
    "JM" => "Jamaica",
    "JP" => "Japan",
    "JE" => "Jersey",
    "JT" => "Johnston Island",
    "JO" => "Jordan",
    "KZ" => "Kazakhstan",
    "KE" => "Kenya",
    "KI" => "Kiribati",
    "KW" => "Kuwait",
    "KG" => "Kyrgyzstan",
    "LA" => "Laos",
    "LV" => "Latvia",
    "LB" => "Lebanon",
    "LS" => "Lesotho",
    "LR" => "Liberia",
    "LY" => "Libya",
    "LI" => "Liechtenstein",
    "LT" => "Lithuania",
    "LU" => "Luxembourg",
    "MO" => "Macau",
    "MK" => "Macedonia [FYROM]",
    "MG" => "Madagascar",
    "MW" => "Malawi",
    "MY" => "Malaysia",
    "MV" => "Maldives",
    "ML" => "Mali",
    "MT" => "Malta",
    "MH" => "Marshall Islands",
    "MQ" => "Martinique",
    "MR" => "Mauritania",
    "MU" => "Mauritius",
    "YT" => "Mayotte",
    "FX" => "Metropolitan France",
    "MX" => "Mexico",
    "FM" => "Micronesia",
    "MI" => "Midway Islands",
    "MD" => "Moldova",
    "MC" => "Monaco",
    "MN" => "Mongolia",
    "ME" => "Montenegro",
    "MS" => "Montserrat",
    "MA" => "Morocco",
    "MZ" => "Mozambique",
    "MM" => "Myanmar [Burma]",
    "NA" => "Namibia",
    "NR" => "Nauru",
    "NP" => "Nepal",
    "NL" => "Netherlands",
    "AN" => "Netherlands Antilles",
    "NT" => "Neutral Zone",
    "NC" => "New Caledonia",
    "NZ" => "New Zealand",
    "NI" => "Nicaragua",
    "NE" => "Niger",
    "NG" => "Nigeria",
    "NU" => "Niue",
    "NF" => "Norfolk Island",
    "KP" => "North Korea",
    "VD" => "North Vietnam",
    "MP" => "Northern Mariana Islands",
    "NO" => "Norway",
    "OM" => "Oman",
    "QO" => "Outlying Oceania",
    "PC" => "Pacific Islands Trust Territory",
    "PK" => "Pakistan",
    "PW" => "Palau",
    "PS" => "Palestinian Territories",
    "PA" => "Panama",
    "PZ" => "Panama Canal Zone",
    "PG" => "Papua New Guinea",
    "PY" => "Paraguay",
    "YD" => "People&#x27;s Democratic Republic of Yemen",
    "PE" => "Peru",
    "PH" => "Philippines",
    "PN" => "Pitcairn Islands",
    "PL" => "Poland",
    "PT" => "Portugal",
    "PR" => "Puerto Rico",
    "QA" => "Qatar",
    "RO" => "Romania",
    "RU" => "Russia",
    "RW" => "Rwanda",
    "RE" => "Réunion",
    "BL" => "Saint Barthélemy",
    "SH" => "Saint Helena",
    "KN" => "Saint Kitts and Nevis",
    "LC" => "Saint Lucia",
    "MF" => "Saint Martin",
    "PM" => "Saint Pierre and Miquelon",
    "VC" => "Saint Vincent and the Grenadines",
    "WS" => "Samoa",
    "SM" => "San Marino",
    "SA" => "Saudi Arabia",
    "SN" => "Senegal",
    "RS" => "Serbia",
    "CS" => "Serbia and Montenegro",
    "SC" => "Seychelles",
    "SL" => "Sierra Leone",
    "SG" => "Singapore",
    "SK" => "Slovakia",
    "SI" => "Slovenia",
    "SB" => "Solomon Islands",
    "SO" => "Somalia",
    "ZA" => "South Africa",
    "GS" => "South Georgia and the South Sandwich Islands",
    "KR" => "South Korea",
    "ES" => "Spain",
    "LK" => "Sri Lanka",
    "SD" => "Sudan",
    "SR" => "Suriname",
    "SJ" => "Svalbard and Jan Mayen",
    "SZ" => "Swaziland",
    "SE" => "Sweden",
    "CH" => "Switzerland",
    "SY" => "Syria",
    "ST" => "São Tomé and Príncipe",
    "TW" => "Taiwan",
    "TJ" => "Tajikistan",
    "TZ" => "Tanzania",
    "TH" => "Thailand",
    "TG" => "Togo",
    "TK" => "Tokelau",
    "TO" => "Tonga",
    "TT" => "Trinidad and Tobago",
    "TA" => "Tristan da Cunha",
    "TN" => "Tunisia",
    "TR" => "Turkey",
    "TM" => "Turkmenistan",
    "TC" => "Turks and Caicos Islands",
    "TV" => "Tuvalu",
    "UM" => "U.S. Minor Outlying Islands",
    "PU" => "U.S. Miscellaneous Pacific Islands",
    "VI" => "U.S. Virgin Islands",
    "UG" => "Uganda",
    "UA" => "Ukraine",
    "AE" => "United Arab Emirates",
    "GB" => "United Kingdom",
    "US" => "United States",
    "UY" => "Uruguay",
    "UZ" => "Uzbekistan",
    "VU" => "Vanuatu",
    "VA" => "Vatican City",
    "VE" => "Venezuela",
    "VN" => "Vietnam",
    "WK" => "Wake Island",
    "WF" => "Wallis and Futuna",
    "EH" => "Western Sahara",
    "YE" => "Yemen",
    "ZM" => "Zambia",
    "ZW" => "Zimbabwe",
    "AX" => "Åland Islands",
  );

  return $array;
}