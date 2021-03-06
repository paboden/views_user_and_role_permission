<?php
/**
 * @file
 * Definition of views_user_and_role_permission_plugin_access_user_and_role.
 */

/**
 * Access plugin that provides role-based access control.
 *
 * @ingroup views_access_plugins
 */
class views_user_and_role_permission_plugin_access_user_and_role extends views_plugin_access {

  function access($account) {
    $options = array();
    $options['rids'] = array_filter($this->options['role']);
    $options['contextual_match'] = $this->options['contextual_match'];

    return views_user_and_role_permission_check_roles($options, $account);
  }

  function get_access_callback() {
    $options = array();
    $options['rids'] = array_filter($this->options['role']);
    $options['contextual_match'] = $this->options['contextual_match'];

    return array('views_user_and_role_permission_check_roles', array($options));
  }

  function summary_title() {
    $current_user = $this->options['contextual_match'];
    $current_user_only_allowed = 'No restrictions';
    $current_user_allowed = '';
    if ($current_user == TRUE) {
      $current_user_only_allowed = t('Only matching logged in user');
      $current_user_allowed = t(' and matching logged in user');
    }
    $count = count($this->options['role']);
    if ($count < 1) {
      return $current_user_only_allowed;
    }
    elseif ($count > 1) {
      return t('Multiple roles') . $current_user_allowed;
    }
    else {
      $rids = views_ui_get_roles();
      $rid = reset($this->options['role']);
      return check_plain($rids[$rid]) . $current_user_allowed;
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['role'] = array('default' => array());
    $options['contextual_match'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['contextual_match'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow current user from contextual filter'),
      '#default_value' => $this->options['contextual_match'],
      '#description' => t('If the current users id matches the id in the url argument, this view will be accessible. (example, if UserID 5 tries to go to page/5/tab, they will see it. If they try to go to page/3/tab, they will be denied. This will only work if you have a contextual filter with the "logged in user id from url" value set.'),
    );
    $form['role'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => $this->options['role'],
      '#options' => array_map('check_plain', views_ui_get_roles()),
      '#description' => t('Along with the matching current user, The checked roles will be able to access this display. Note that users with "access all views" can see any view, regardless of role.'),
    );
  }

  function options_submit(&$form, &$form_state) {
    // I hate checkboxes.
    $form_state['values']['access_options']['role'] = array_filter($form_state['values']['access_options']['role']);
    $form_state['values']['access_options']['contextual_match'] = $form_state['values']['access_options']['contextual_match'];
  }
}