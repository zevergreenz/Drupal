/**
 * Form for configurable Drupal action to beep multiple times
 */
function beep_multiple_beep_action_form($context) {
	$form['beeps'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of beeps'),
		'#description' => t('Enter the number of times to beep when this action executes'),
		'#default_value' => isset($context['beeps']) ? $context['beeps'] : '1',
		'#required' => TRUE,
	);
	return $form;
}

function beep_multiple_beep_action_validate($form, $form_state) {
	$beeps = $form_state['values']['beeps'];
	if (!is_int($beeps)) {
		form_set_error('beeps', t('Please enter a whole number between 0 and 10.'));
	}
	else if ((int) $beeps > 10 ) {
		form_set_error('beeps', t('That would be too annoying. Please choose fewer than 10
		beeps.'));
	} else if ((int) $beeps < 0) {
		form_set_error('beeps', t('That would likely create a black hole! Beeps must be a
		positive integer.'));
	}
}

function beep_multiple_beep_action_submit($form, $form_state) {
	return array(
		'beeps' => (int)$form_state['values']['beeps']
	);
} 