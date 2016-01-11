<?php

namespace CVWO\HelloWorld {

	require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

	/**
	 * Displays the form for adding/viewing/editing details of the user record
	 */
	function helloworld_form($form, &$form_state, $page_mode = 'add', $helloworld_id = null) {
		$form = array();

		if ($helloworld_id != null && $page_mode != 'add') {
			// we store the helloworld_id here for future reference (when updating/deleting later)
			$form['helloworld_id'] = array(
				'#type' => 'hidden',
				'#value' => $helloworld_id,
			);
			if (!load_user($form_state, $helloworld_id)) {
				drupal_set_message('Failed to load user details');
				drupal_goto(MODULE_NAME);
			}
		}
		
		if ($page_mode == 'view') {
			$disabled = TRUE;
		} else {
			$disabled = FALSE;
		}
		
		$form['data'] = array(
			'#type' => 'textarea',
			'#title' => t('Enter list of names'),
			'#disabled' => $disabled,
			//'#default_value' => $values['data'],
			'#rows' => 10,
			'#required' => TRUE,
		);
		
		$form['comment'] = array(
			'#type' => 'textarea',
			'#title' => t('Comment(optional):'),
			'#disabled' => $disabled,
			//'#default_value' => $values['data'],
			'#rows' => 1,
			'#required' => FALSE,
		);
		
		// We create the buttons based on the page mode
		$form['add'] = array(
			'#type' => 'submit',
			'#value' => t('Add'),
		);
		
		return $form;
	}

	/**
	 * Implements form_validate
	 */
/* 	function helloworld_form_validate($form, &$form_state){
		// valid_email_address is from drupal api
		// validation function checks whether the email is valid before inserting/updating/deleting
		if (!valid_email_address($form_state['values']['email'])){
			form_set_error('email', t('Please enter a valid email address.'));
		}
	} */

	/**
	 * Implements form_submit
	 */
	function helloworld_form_submit($form, &$form_state){
		$error = FALSE;
		
		$people = explode("\n", $form_state['values']['data']);
		$comment = $form_state['values']['comment'];
		
		for ($i = 0; $i < count($people); $i++) {
			$name = explode(",", $people[$i], 2);
			$value = array(
				'name' => trim($name[0]),
				'email' => trim($name[1]),
				'comment' => $comment,
			);
			
			//validate user email addresses
			if (!valid_email_address($value['email'])){
				form_set_error('data', t($value['email'].' is not a valid email address.'));
			} else {
				add_user($value);
			}
		}

/* 		if (add_user($form_state['values'])) {
			drupal_set_message(t('%name has been successfully added', array('%name' => $form_state['values']['name'])));
		} else {
			drupal_set_message(t('An error was encountered while adding %name', array('%name' => $form_state['values']['name'])));
			$error = TRUE;
		} */
		
		// if we encountered an error, we keep the form as it is
		// else we redirect back to home page
		if ($error) {
			$form_state['rebuild'] = TRUE;
		} else {
			$form_state['redirect'] = MODULE_NAME;
		}
	}

	/**
	 * Function implements the Helloworld admin settings form
	 */ 
	 function helloworld_admin() {
		$form[PAGELIMIT] = array(
			'#type' => 'textfield',
			'#title' => t('Maximum number of records in a page'),
			'#default_value' => variable_get(PAGELIMIT),
			'#size' => 2,
			'#maxlength' => 2,
			'#description' => t("The maximum number of records to display in the user records listing."),
			'#required' => TRUE,
		);
		
		$form[AUTOCOMPLETELIMIT] = array(
			'#type' => 'textfield',
			'#title' => t('Maximum number of records in autocomplete'),
			'#default_value' => variable_get(AUTOCOMPLETELIMIT),
			'#size' => 2,
			'#maxlength' => 2,
			'#description' => t("The maximum number of records to display under autocomplete function."),
			'#required' => TRUE,
		);
		
		return system_settings_form($form);
	 }
	 
	 /**
	 * Form validation handler for helloworld_admin().
	 *
	 * @see helloworld_admin()
	 */
	function helloworld_admin_validate($form, &$form_state) {
	  $helloworld_pagelimit = $form_state['values'][PAGELIMIT];
	  $helloworld_autocompletelimit = $form_state['values'][AUTOCOMPLETELIMIT];
	  
	  if (!is_numeric($helloworld_pagelimit)) {
	    form_set_error('helloworld_pagelimit', t('You must enter a number for the maximum number of records in a page.'));
	  } else if ($helloworld_pagelimit <= 0) {
	    form_set_error('helloworld_pagelimit', t('Maximum number of records in a page must be positive.'));
	  }
	 
	  if (!is_numeric($helloworld_autocompletelimit)) {
	    form_set_error('helloworld_autocompletelimit', t('You must enter a number for the maximum number of records displayed in autocomplete function.'));
	  } else if ($helloworld_autocompletelimit <= 0) {
	    form_set_error('helloworld_autocompletelimit', t('Maximum number of records in autocomplete function must be positive.'));
	  } 
	 
	}
}
