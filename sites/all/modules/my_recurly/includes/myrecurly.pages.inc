<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */

/**
 * Page callback: My Recurly settings
 *
 * @see myrecurly_menu()
 */
function myrecurly_form($form, &$form_state) {
	$form['myrecurly_api_key'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Recurly API Key'),
	  '#default_value' => variable_get('recurly_api_key', ''),
	  '#size' => 50,
	  '#maxlength' => 50,
	  '#description' => t('The Recurly API Key'),
	  '#required' => TRUE,
	 );

	/*$form['list_subscriptions'] = array(
	  '#type' => 'link',
	  '#title' => t('List of Subscriptions'),
	  '#href' => 'admin/config/services/myrecurly/subscriptions',
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
	);

	$form['list_accounts'] = array(
	  '#type' => 'link',
	  '#title' => t('List of Accounts'),
	  '#href' => 'admin/config/services/myrecurly/accounts',
	  '#prefix' => '<h3>',
      '#suffix' => '</h3>',
	);*/

	return system_settings_form($form);
	//return $form;
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////// ACCOUNTS SECTION /////////////////////////////
///////////////////////////////////////////////////////////////////////////
/**
 * Table of available hooks and the modules implementing them, if any.
 */
function myrecurly_list_accounts_page() {
  $build = array();

  $header = array(
    array('data' => t('Account Code'), 'field' => 'account_code',  'sort' => 'ASC'),
    array('data' => t('First Name')),
    array('data' => t('Last Name')),
    array('data' => t('Status')),
    array('data' => t('Created @')),
    array('data' => t('Action'), 'colspan' => 3, 'style' => 'text-align:center;'),
  );

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  //$accounts = Recurly_AccountList::getActive(); // TODO
  $accounts = Recurly_AccountList::get();
  
  $rows = array();

  $path = drupal_get_path('module', 'myrecurly');
  drupal_add_js($path . '/js/myrecurly_helper.js');

  //drupal_add_js('alert("checking!")', 'inline');
  
  foreach ($accounts as $account) {
  	
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
  		$action1_url = 'admin/config/services/myrecurly/close_account/' . $account->account_code;
  		$action2 = 'Edit';
  		$action2_url = 'admin/config/services/myrecurly/edit_account/' . $account->account_code;
  		//$action3 = 'Postpone';
  		//$action3_url = 'admin/config/services/myrecurly/postpone_subscription/' . $account->account_code;
  	} else if ($state == 'closed') {
  		$action1 = 'Re-open';
  		$action1_url = 'admin/config/services/myrecurly/reopen_account/' . $account->account_code;
  	}

    $rows[] = array(
    	//array('data' => $account->account_code),
    	array('data' => l($account->account_code, 'admin/config/services/myrecurly/manage_subscription/' .
    	  $account->account_code)),
    	array('data' => $account->first_name, 'style' => 'color:red;'),
    	array('data' => $account->last_name),
    	array('data' => $account->state),
    	array('data' => $date_created_at),
    	array('data' => l($action1, $action1_url, array('attributes' => array('class' => 'js_link')))),
    	array('data' => l($action2, $action2_url, array('attributes' => array('class' => 'js_link')))),
    	array('data' => l($action3, $action3_url, array('attributes' => array('class' => 'js_link')))),
    	//array('data' => l('edit', 'www.google.com')),
    );
  }

  $attributes = array(
    'border'	  => 10,
    'cellspacing' => 0,
    'cellpadding' => 5,
  );

  $caption = '<div style="font-size:20px;text-align:left;">List of Accounts</div>';
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
 * Closes account's subscription
 */
function myrecurly_close_account_page() {
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

  drupal_goto('admin/config/services/myrecurly/accounts/');
}

/**
 *
 */
function myrecurly_acctinfo_form($form, &$form_state, $data) {
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
  

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    //'#submit' => TRUE,
  );

  return $form;
}

/**
 * user defined hook_form_validate
 */
function myrecurly_acctinfo_form_validate($form, &$form_state) {
  // Something here
}

/**
 * user defined hook_form_submit
 */
function myrecurly_acctinfo_form_submit($form, &$form_state) {
  
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
}

/**
 * Edit/Update Account information
 */
function myrecurly_edit_account_page() {
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

  return drupal_get_form('myrecurly_acctinfo_form', $account_info);

}

/**
 *
 */
function myrecurly_reopen_account_page() {
  // Gets get parameter
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
  $account->reopen();

  drupal_set_message(t('Your account was reopened successfully.'));

  drupal_goto('admin/config/services/myrecurly/accounts/');
}

///////////////////////////////////////////////////////////////////////////
///////////////////////// END OF ACCOUNTs SECTION /////////////////////////
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
////////////////////////// SUBSCRIPTIONS SECTION //////////////////////////
///////////////////////////////////////////////////////////////////////////

function myrecurly_subinfo_form($form, &$form_state, $data) {
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
  /* OLD STUFF
  $form['name']['plan_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Plan name'),
    '#value' => $data['plan_name'],
    '#size' => 30,
    '#maxlength' => 30,
    '#prefix' => '<div class="col1 name first">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    '#attributes' => array('readonly' => 'readonly'),
  );

  $form['name']['plan_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Plan code'),
    '#size' => 30,
    '#maxlength' => 30,
    '#value' => $data['plan_code'],
    '#prefix' => '<div class="col1 name">',
    '#suffix' => '</div>',
    '#required' => TRUE, // Added
    '#attributes' => array('readonly' => 'readonly'),
  );
  */

  $form['quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#size' => 3,
    '#maxlength' => 3,
    '#value' => $data['quantity'],
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  /*$form['subscriptions'] = array(
    //'#type' => 'textfield',
    '#type' => 'select',
    '#options' => $data['subscriptions'],
    //'#default_value' => $data['plan_code'],
    '#value' => $data['plan_code'],
    '#title' => t('Subscriptions xCode'),
    '#required' => TRUE,
  );*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    //'#submit' => TRUE,
  );

  return $form;
}

/**
 * user defined hook_form_validate
 */
function myrecurly_subinfo_form_validate($form, &$form_state) {
  $quantity = $form_state['input']['quantity'];
  if ($quantity !== '' && (!is_numeric($quantity) || intval($quantity) != $quantity || $quantity <= 0)) {
  	form_error($form, t('%name must be a postive integer.', array('%name' => $form['quantity']['#title'])));
  }
}

/**
 * user defined hook_form_submit
 */
function myrecurly_subinfo_form_submit($form, &$form_state) {
  
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
}

/**
 * Table of available hooks and the modules implementing them, if any.
 */
function myrecurly_manage_subscription_page() {
  // Gets get parameter
  $get_param = $_GET;
  $param = trim($_GET['q']);

  // Gets account_code from $param
  $tmp = explode('/', $param);
  // Gets the last element of an array
  $account_code = end($tmp);

  $build = array();

  $header = array(
    array('data' => t('Plan Code'), 'field' => '', 'style' => 'align:center;'), 
    array('data' => t('Plan Name')),
    array('data' => t('Quantity')),
    array('data' => t('Unit Price')),
    array('data' => t('Activated @')),
    array('data' => t('Current Period Started @')),
    array('data' => t('Current Period Ends @')),
    //array('data' => t('Subscription ID')),
    array('data' => t('State')),
    array('data' => t('Action'), 'colspan' => 3, 'style' => 'text-align:center;'),
  );

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Gets subscription information for this account
  $acct_subscriptions = Recurly_SubscriptionList::getForAccount($account_code);

  $rows = array();
  
  foreach ($acct_subscriptions as $acct_subscription) {
  	
  	// Gets account subscription's activated_at date
  	foreach ($acct_subscription->activated_at as $key => $activated_at) {
  	  if ($key == 'date') {
  	  	$date_activated = $activated_at;
  	  }
  	}

  	// Gets account subscription's current_period_started_at date
  	foreach ($acct_subscription->current_period_started_at as $key => $current_period_started_at) {
  	  if ($key == 'date') {
  	  	$current_date_started = $current_period_started_at;
  	  }
  	}

  	// Gets account subscription's current_period_ends_at date
  	foreach ($acct_subscription->current_period_ends_at as $key => $current_period_ends_at) {
  	  if ($key == 'date') {
  	  	$current_date_ends = $current_period_ends_at;
  	  }
  	}

  	$state = $acct_subscription->state;
  	$action1 = $action2 = $action3 = '';
  	$action1_url = $action2_url = $action3_url = '';
  	if ($state == 'active') {
  		$action1 = 'Cancel';
  		$action1_url = 'admin/config/services/myrecurly/cancel_subscription/' . $acct_subscription->uuid;
  		$action2 = 'Terminate';
  		$action2_url = 'admin/config/services/myrecurly/terminate_subscription/' . $acct_subscription->uuid;
  		$action3 = 'Postpone';
  		$action3_url = 'admin/config/services/myrecurly/postpone_subscription/' . $acct_subscription->uuid;
  	} else if ($state == 'canceled') {
  		$action1 = 'Re-Activate';
  		$action1_url = 'admin/config/services/myrecurly/reactivate_subscription/' . $acct_subscription->uuid;
  	}

    $rows[] = array(
    	//array('data' => $acct_subscription->plan->plan_code, 'width' => '120'),
    	array('data' => l($acct_subscription->plan->plan_code, 'admin/config/services/myrecurly/edit_subscription/' .
    	  $acct_subscription->uuid)),
    	array('data' => $acct_subscription->plan->name, 'width' => '120'),
    	array('data' => $acct_subscription->quantity, 'style' => 'width:100px;color:red;'),
    	array('data' => $acct_subscription->currency . ' ' . ($acct_subscription->unit_amount_in_cents/100.00)),
    	array('data' => $date_activated),
    	array('data' => $current_date_started),
    	array('data' => $current_date_ends),
    	//array('data' => $acct_subscription->uuid),
    	array('data' => $acct_subscription->state),
    	array('data' => l($action1, $action1_url, array('attributes' => array('class' => 'js_link')))),
    	array('data' => l($action2, $action2_url, array('attributes' => array('class' => 'js_link')))),
    	array('data' => l($action3, $action3_url, array('attributes' => array('class' => 'js_link')))),
    	/*array('data' => l('cancel', 'admin/config/services/myrecurly/cancel_subscription/' . 
    		$acct_subscription->plan->plan_code, array('attributes' => array('class' => 'js_link'))  )),*/
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

/**
 * Table of available hooks and the modules implementing them, if any.
 */
function myrecurly_list_subscriptions_page() {
  $build = array();

  $header = array(
    array('data' => t('Plan Code'), 'field' => '', 'style' => 'align:center;'), 
    array('data' => t('Quantity')),
    array('data' => t('Unit Price')),
    array('data' => t('Activated @')),
    array('data' => t('Current Period Started @')),
    array('data' => t('Current Period Ends @')),
    array('data' => t('Subscription ID')),
    array('data' => t('Edit')),
    array('data' => t('Cancel')),
  );

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $subscriptions = Recurly_SubscriptionList::getActive();
  	  
  $rows = array();
  
  foreach ($subscriptions as $subscription) {
  	
  	// Gets subscription's activated_at date
  	foreach ($subscription->activated_at as $key => $activated_at) {
  	  if ($key == 'date') {
  	  	$date_activated = $activated_at;
  	  }
  	}

  	// Gets subscription's current_period_started_at date
  	foreach ($subscription->current_period_started_at as $key => $current_period_started_at) {
  	  if ($key == 'date') {
  	  	$current_date_started = $current_period_started_at;
  	  }
  	}

  	// Gets subscription's current_period_ends_at date
  	foreach ($subscription->current_period_ends_at as $key => $current_period_ends_at) {
  	  if ($key == 'date') {
  	  	$current_date_ends = $current_period_ends_at;
  	  }
  	}
	
    $rows[] = array(
    	array('data' => $subscription->plan->plan_code, 'width' => '120'),
    	array('data' => $subscription->quantity, 'style' => 'width:100px;color:red;'),
    	array('data' => $subscription->currency . ' ' . ($subscription->unit_amount_in_cents/100.00)),
    	array('data' => $date_activated),
    	array('data' => $current_date_started),
    	array('data' => $current_date_ends),
    	array('data' => $subscription->uuid),
    	array('data' => l('edit', 'www.google.com')),
    	array('data' => l('cancel', 'admin/config/services/myrecurly/cancel_subscription/' . 
    		$subscription->plan->plan_code, array('attributes' => array('class' => 'js_link'))  )),
    );
  }

  $attributes = array(
    'border'	  => 10,
    'cellspacing' => 0,
    'cellpadding' => 5,
  );

  $caption = '<div style="font-size:20px;text-align:left;">List of Subscriptions</div>';
  $build['hook_table'] = array(
  	'#theme' => 'table__myrecurly__hooks',
  	'#header' => $header,
  	'#rows' => $rows,
  	//'#attributes' => $attributes,
  	'#attributes' => array('id' => 'myrecurly-list-subscription'),
  	'#empty' => t('No Subscriptions is available.'),
  	'#caption' => $caption,
  );

  drupal_add_tabledrag('myrecurly-list-subscription', 'order', 'sibling', 'mytable-order-weight');
  
  return $build;
}

/**
 * Cancels account's subscription
 */
function myrecurly_cancel_subscription_page() {
  
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

  drupal_goto('admin/config/services/myrecurly/accounts/');
}

/**
 * Reactivates a canceled subscription
 */
function myrecurly_reactivate_subscription_page() {
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

  drupal_goto('admin/config/services/myrecurly/accounts/');
}

/**
 * Terminates an account's subscription
 */
function myrecurly_terminate_subscription_page() {
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

  drupal_goto('admin/config/services/myrecurly/accounts/');
}

/**
 * Updates subscription information
 */
function myrecurly_edit_subscription_page() {
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

  return drupal_get_form('myrecurly_subinfo_form', $subscription_info);

}

///////////////////////////////////////////////////////////////////////////
/////////////////////// END OF SUBSCRIPTIONS SECTION //////////////////////
///////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// FROM RECURLY //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Menu callback; Show a list of available plans to which a user may subscribe.
 *
 * This menu callback is used both for new subscriptions and for updating
 * existing subscriptions.
 *
 * @param $entity_type
 *   The type of the entity whose subscription is being changed.
 * @param $entity
 *   The entity whose subscription is being changed.
 * @param $currency
 *   If this is a new subscription, the currency to be used.
 * @param $subscription_id
 *   The UUID of the current subscription if changing the plan on an existing
 *   subscription.
 */
function myrecurly_subscription_plan_select($entity_type, $entity, $currency = NULL, $subscription_id = NULL) {
  die('xxx');
  // Initialize the Recurly client with the site-wide settings.
  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // If loading an existing subscription.
  if ($subscription_id) {
    if ($subscription_id === 'latest') {
      list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
      $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
      $subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');
      $subscription = reset($subscriptions);
      $subscription_id = $subscription->uuid;
      $currency = $subscription->plan->currency;
      $mode = 'change';
    }
    else {
      try {
        $subscription = Recurly_Subscription::get($subscription_id);
        $subscriptions[$subscription->uuid] = $subscription;
        $currency = $subscription->plan->currency;
        $mode = 'change';
      }
      catch (Recurly_NotFoundError $e) {
        drupal_set_message(t('Subscription not found'));
        return MENU_NOT_FOUND;
      }
    }
  }
  // If signing up to a new subscription, ensure the user doesn't have a plan.
  else {
    $subscriptions = array();
    $currency = isset($currency) ? $currency : variable_get('recurly_default_currency', 'USD');
    $mode = 'signup';
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));
    if ($account) {
      $subscriptions = recurly_account_get_subscriptions($account->account_code, 'active');
    }
  }

  // Make the list of subscriptions based on plan keys, rather than uuid.
  $plan_subscriptions = array();
  foreach ($subscriptions as $subscription) {
    $plan_subscriptions[$subscription->plan->plan_code] = $subscription;
  }

  $all_plans = recurly_subscription_plans();
  $enabled_plan_keys = variable_get('recurly_subscription_plans', array());
  $enabled_plans = array();
  foreach ($enabled_plan_keys as $plan_code => $enabled) {
    foreach ($all_plans as $plan) {
      if ($enabled && $plan_code == $plan->plan_code) {
        $enabled_plans[$plan_code] = $plan;
      }
    }
  }
  return theme(array('recurly_subscription_plan_select__' . $mode, 'recurly_subscription_plan_select'), array('plans' => $enabled_plans, 'entity_type' => $entity_type, 'entity' => $entity, 'currency' => $currency, 'mode' => $mode, 'subscriptions' => $plan_subscriptions, 'subscription_id' => $subscription_id));
}
