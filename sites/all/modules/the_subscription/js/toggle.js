/**
 * When the "My billing address is different" checkbox is clicked, hide and
 * disable all fields in that fieldset. (except the checkbox itself).
 */
Drupal.behaviors.checkoutAddress = {
  attach: function(context, settings) {
    var $checkbox = jQuery('#use-billing', context);
    //var $siblings = $checkbox.parent().siblings();
    //var $inputs = jQuery('.customer_profile_billing input, .customer_profile_billing select', context);
    //var $name = jQuery('#edit-first', context);

    $checkbox.change(function() {

      if (this.checked) {
        var billing_address1 = jQuery('#edit-billing-address1').val();
        var billing_address2 = jQuery('edit-billing-address2').val();
        var billing_city = jQuery('#edit-billing-city').val();
        var billing_state = jQuery('#edit-billing-state').val();
        var billing_zipcode = jQuery('#edit-billing-zipcode').val();
        var billing_country_code = jQuery('#edit-billing-country-code').val();

        jQuery('#edit-shipping-address1').val(billing_address1);
        jQuery('#edit-shipping-address2').val(billing_address2);
        jQuery('#edit-shipping-city').val(billing_city);
        jQuery('#edit-shipping-state').val(billing_state);
        jQuery('#edit-shipping-zipcode').val(billing_zipcode);
        jQuery('#edit-shipping-country-code').val(billing_country_code);
        /*$siblings.slideDown();
        $inputs.removeAttr('disabled');*/
      }
      else {
        jQuery('#edit-shipping-address1').val('');
        jQuery('#edit-shipping-address2').val('');
        jQuery('#edit-shipping-city').val('');
        jQuery('#edit-shipping-state').val('');
        jQuery('#edit-shipping-zipcode').val('');
        jQuery('#edit-shipping-country-code').val('US');
        /*$siblings.slideUp();
        $inputs.attr('disabled', 'disabled');
        $checkbox.removeAttr('disabled');*/
      }
    });

    //$checkbox.change();
  }
};