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
    array('data' => t('Account Code'), 'field' => 'account_code',  'sort' => 'ASC', 'style' => 'text-align:center;'), 
    array('data' => t('First Name')),
    array('data' => t('Last Name')),
    array('data' => t('Status')),
    array('data' => t('Created @')),
    array('data' => t('Action'), 'colspan' => 3, 'style' => 'text-align:center;'),
  );

  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $accounts = Recurly_AccountList::getActive();
  
  $rows = array();
  
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
  	}

    $rows[] = array(
    	//array('data' => $account->account_code),
    	array('data' => l($account->account_code, 'admin/config/services/myrecurly/manage_subscription/' .
    	  $account->account_code)),
    	array('data' => $account->first_name, 'style' => 'width:100px;color:red;'),
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
  /*if ($_POST) {
  	die('# code...');
  }*/
  /*echo '<pre>data: ';
  print_r($data);exit;*/
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

  //echo '<pre>$form: ';print_r($form_state);exit;
  //dsm(print_r($form));
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

///////////////////////////////////////////////////////////////////////////
///////////////////////// END OF ACCOUNTs SECTION /////////////////////////
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
////////////////////////// SUBSCRIPTIONS SECTION //////////////////////////
///////////////////////////////////////////////////////////////////////////

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
    	array('data' => $acct_subscription->plan->plan_code, 'width' => '120'),
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
////////////////////////////////////////// OLD CODE BELOW //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 */
function myrecurly_list_accounts() {
	// Initialize the Recurly client with the site-wide settings.
  if (!myrecurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  $per_page = 20;
  //$invoice_list = Recurly_InvoiceList::getForAccount($account->account_code, array('per_page' => $per_page));
  $subscriptions = Recurly_SubscriptionList::getActive();
  //$invoices = recurly_pager_results($subscriptions, $per_page);

  return theme('myrecurly_list_accounts',
  	array(
  	  'subscriptions' => $subscriptions,
  	)
  );
  /*return theme('myrecurly_list_accounts', 
  	array(
  		'plan_code' => '$subscriptions',
  		'quantity' => '$subscriptions',
  		'unit_price' => '$subscriptions',
  		'per_page' => $per_page,
  		'total' => $subscriptions->count()
  	)
  );*/
}
function myrecurly_list_accounts_WORKS() {
  $page_array['list_accounts'] = array(
  	//Title serves as page subtitle
    '#title' => t('All posts from the last week'),
    '#markup' => t('No posts available.'),
  );

  myrecurly_client_initialize();

  $accounts = array();

  $subscriptions = Recurly_SubscriptionList::getActive();

  foreach ($subscriptions as $key => $subscription) {
  	
  	// Gets subscription's activated_at date
  	foreach ($subscription->activated_at as $key1 => $activated_at) {
  		if ($key1 == 'date') {
  			$date_activated = $activated_at;
  		}
  	}

  	// Gets subscription's current_period_started_at date
  	foreach ($subscription->current_period_started_at as $key2 => $current_period_started_at) {
  		if ($key2 == 'date') {
  			$current_date_started = $current_period_started_at;
  		}
  	}

  	// Gets subscription's current_period_ends_at date
  	foreach ($subscription->current_period_ends_at as $key3 => $current_period_ends_at) {
  		if ($key3 == 'date') {
  			$current_date_ends = $current_period_ends_at;
  		}
  	}

  	$accounts['list_accounts'][$subscription->uuid] = array(
  	  '#theme' => 'myrecurly_list_accounts',
  	  //'#theme' => 'recurly_credit_card_information',
  	  //'#theme' => 'recurly_subscription_summary',
  	  '#plan_code' => $subscription->plan->plan_code,
  	  '#quantity' => $subscription->quantity,
  	  '#unit_price' => $subscription->currency . ' ' . ($subscription->unit_amount_in_cents/100.00),
  	  '#activated_at' => $date_activated,
  	  '#current_period_started_at' => $current_date_started,
  	  '#current_period_ends_at' => $current_date_ends,
  	  '#subscription_id' => $subscription->uuid,
  	);
  }

  $per_page = 10;
  $accounts['pager'] = array(
    '#markup' => theme('pager'),
    //'#markup' => t('No posts available.'),
    //'#markup' => print_r($accounts['list_accounts'], true),
    //'#access' => $subscriptions->count() > $per_page,
  );

  drupal_alter('myrecurly_list_accounts', $accounts);

  //echo '<pre>accounts: ';print_r($accounts);exit;

  return $accounts;
  //return $page_array;
}
function myrecurly_list_accounts_xxx() {

  myrecurly_client_initialize();

  $accounts['subject'] = t('Current active subscriptions');
  
  $subscriptions = Recurly_SubscriptionList::getActive();
	  
  $items = array();

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
	
    $items[] = array(
    	array('data' => $subscription->plan->plan_code, 'width' => '120'),
    	array('data' => $subscription->quantity, 'style' => 'width:100px;color:red;'),
    	array('data' => $subscription->currency . ' ' . ($subscription->unit_amount_in_cents/100.00)),
    	array('data' => $date_activated),
    	array('data' => $current_date_started),
    	array('data' => $current_date_ends),
    	array('data' => $subscription->uuid),
    );
  }

  if (empty($subscriptions)) {
  	$accounts['content'] = t('There is no active subscription available.');
  } else {
	$header = array(
	  array('data' => t('Plan Code')), 
	  array('data' => t('Quantity')),
	  array('data' => t('Unit Price')),
	  array('data' => t('Activated @')),
	  array('data' => t('Current Period Started @')),
	  array('data' => t('Current Period Ends @')),
	  array('data' => t('Subscription ID')),
	);

		// creating first row
    $rows[] = array(
      // output of first cell in 1 row
      array('data' => t('Row 1 – Cell 1')),
      // second cell
      array('data' => t('Row 1 – Cell 2')),
      // third cell
      array('data' => t('Row 1 – Cell 3')) 
    );
	//second row output
	$rows[] = array(
	  array('data' => t('Row 2 – Cell 1')),
	  array('data' => t('Row 2 – Cell 2')),
	  array('data' => t('Row 2 – Cell 3'))
	);

	$attributes = array(
	  'border'		=> 10,
	  'cellspacing' => 0,
	  'cellpadding' => 5,
	  //'class'       => 'example'
	);
	/*echo '<pre>$rows: ';
	print_r($items);exit;*/
	$accounts['content'] = theme('table', array('header' => $header, 'rows' => $items, 'attributes' => $attributes));
	//$block['content'] = theme('table', $header, $rows);

  }

  /*$accounts['theme'] = array(
  	'#theme' => 
  );*/

  echo '<pre>accounts: ';
  print_r($accounts);exit;
  return $accounts;


  //$accounts = array('1' => 'one');

  //return $accounts;
}

/**
 * Preprocess variables for the myrecurly-list-accounts.tpl.php file.
 */
//function template_preprocess_myrecurly_list_accounts(&$variables) {
function template_preprocess_myrecurly_list_accounts($variables) {
//function template_preprocess_myrecurly_list_accounts() {
  /*$variables['classes_array'][] = 'main';

  dpm($variables);

}

function xxx() {*/
  //myrecurly_client_initialize();

  $header = array(
	  array('data' => t('Plan Code')),
	  array('data' => t('Quantity')),
	  array('data' => t('Unit Price')),
	  array('data' => t('Activated @')),
	  array('data' => t('Current Period Started @')),
	  array('data' => t('Current Period Ends @')),
	  array('data' => t('Subscription ID')),
  );
  $rows = array();

  //$subscriptions = Recurly_SubscriptionList::getActive();
  $subscriptions = $variables['subscriptions'];
  //echo '<pre>$subscriptions: ';print_r($subscriptions);exit;

  foreach ($subscriptions as $key => $subscription) {
  	
  	// Gets subscription's activated_at date
  	foreach ($subscription->activated_at as $key1 => $activated_at) {
  		if ($key1 == 'date') {
  			$date_activated = $activated_at;
  		}
  	}

  	// Gets subscription's current_period_started_at date
  	foreach ($subscription->current_period_started_at as $key2 => $current_period_started_at) {
  		if ($key2 == 'date') {
  			$current_date_started = $current_period_started_at;
  		}
  	}

  	// Gets subscription's current_period_ends_at date
  	foreach ($subscription->current_period_ends_at as $key3 => $current_period_ends_at) {
  		if ($key3 == 'date') {
  			$current_date_ends = $current_period_ends_at;
  		}
  	}

  	$row = array();
    $row[] = $subscription->plan->plan_code;
    $row[] = $subscription->quantity;
    $row[] = recurly_format_currency($subscription->unit_amount_in_cents/100.00, $subscription->currency);
    $row[] = recurly_format_date($activated_at);
    $row[] = recurly_format_date($current_period_started_at);
    $row[] = recurly_format_date($current_period_ends_at);
    $row[] = $subscription->uuid;
    $rows[] = array(
      'data' => $row,
      //'class' => array(check_plain($invoice->state)),
    );
    //echo '<pre>$rows: ';print_r($rows);

  	/*$accounts['list_accounts'][$subscription->uuid] = array(
  	  '#theme' => 'myrecurly_list_accounts',
  	  //'#theme' => 'recurly_credit_card_information',
  	  //'#theme' => 'recurly_subscription_summary',
  	  '#plan_code' => $subscription->plan->plan_code,
  	  '#quantity' => $subscription->quantity,
  	  '#unit_price' => $subscription->currency . ' ' . ($subscription->unit_amount_in_cents/100.00),
  	  '#activated_at' => $date_activated,
  	  '#current_period_started_at' => $current_date_started,
  	  '#current_period_ends_at' => $current_date_ends,
  	  '#subscription_id' => $subscription->uuid,
  	);*/
  }
  $attributes = array(
	  'border'		=> 1,
	  'cellspacing' => 0,
	  'cellpadding' => 5,
	  //'class'       => 'example'
	);
  //$attributes = array('attributes' => array('id' => 'permissionsxxxx'));
  //$accounts['content'] = theme('table', array('header' => $header, 'rows' => $items, 'attributes' => $attributes));

  /*$variables['table'] = array(
  //$accounts['table'] = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => $attributes,
    //'attributes' => array('id' => 'ralphs-node-table'),
    //'attributes' => array('class' => array('invoice-list')),
    //'sticky' => FALSE,
  );*/

  $accounts['content'] = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));


  //echo '<pre>$variables: ';print_r($variables);exit;
  //echo '<pre>$accounts: ';print_r($accounts);exit;
  return $accounts;
}