/*
  This code helps make the default pager a little
  more usable by cloning it and moving it into 
  different positions around the view
 */

(function ($) {
  
  $(document).ready(function() {

  	/*
  	  Change shopping cart to bag on every cart page
  	 */
    $('.page-cart h1').html('<span>Shopping Bag</span>');

    /*
      Change page title from username to account on edit/view account pages
     */
    $('.page-user h1').html('<span>Account</span>');

    /*
      Change username to account on edit/view account pages
     */
    $('.page-user-edit h1').html('<span>Account</span>');

    /*
      Change page title to previous orders on order history page
     */
    $('.page-user-orders h1').html('<span>Previous Orders</span>');

    /*
      Change page title from usernae to address book
     */
    $('.page-user-addressbook h1').html('<span>Address Book</span>');


  });
  
}(jQuery));