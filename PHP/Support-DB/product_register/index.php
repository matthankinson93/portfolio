<?php
require('../model/database.php');
require('../model/customer_db.php');
require('../model/product_db.php');
require('../model/registration_db.php');

//instantiate variable(s)
$email = '';
$product_name = '';
$product_code = '';

$action = filter_input(INPUT_POST, 'action');

if ($action == NULL) {
    $action = filter_input(INPUT_GET, 'action');
    if ($action === NULL) {
        $action = 'login_customer';
    }
}


if ($action == 'login_customer'){
    include('customer_login.php');
} else if ($action == 'get_customer') {
    $customer = get_customer_by_email(filter_input(INPUT_POST, 'email'));
    $customer_name = $customer[0]['firstName']." ".$customer[0]['lastName'];
    $customer_ID = $customer[0]['customerID'];
    if (count($customer) === 0 || filter_input(INPUT_POST, 'email') === "" || filter_input(INPUT_POST, 'email') === " ") {
        $error = "Invalid login. Check all fields and try again.";
        include('../errors/error.php');
    } else {
        $products = get_products();
        include('product_register.php');
    }
} else if ($action == 'register_product') {
    $product_code = get_product_by_name(filter_input(INPUT_POST, 'name'));
    $customer_ID = filter_input(INPUT_POST, 'customer_ID');
    $date = date("Y-m-d H:i:s");
    add_registration($customer_ID, $product_code['productCode'], $date);
    include('successful.php');
}
?>