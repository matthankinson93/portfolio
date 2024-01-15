<?php
require('../model/incidents_db.php');
require('../model/database.php');
require('../model/customer_db.php');
require('../model/product_db.php');


$action = filter_input(INPUT_POST, 'action');
if ($action === NULL) {
    $action = filter_input(INPUT_GET, 'action');
    if ($action === NULL) {
        $action = 'customer_login';
    }
}

if ($action == 'customer_login') {
    include('validate_customer.php');
} else if ($action == 'validate_customer') {
     $customer = get_customer_by_email(filter_input(INPUT_POST, 'email'));
    if (count($customer) === 0 || filter_input(INPUT_POST, 'email') === " " || filter_input(INPUT_POST, 'email') === "") {
        $error = "Invalid login. Please ensure user is a valid customer";
        include('../errors/error.php');
    } else {
        $product = get_products_by_customer(filter_input(INPUT_POST, 'email'));
        if (count($product) > 0) {
            $customer_name = $customer[0]['firstName']." ".$customer[0]['lastName'];
            include('create_incident.php');
        } else {
            $error = "User has no products registered! Please register a product before proceeding.";
            include('../errors/error.php');
        }
    }
} else if ($action === 'create_incident') {
    $product = get_product_by_name(filter_input(INPUT_POST, 'incident_product'));
    $title = filter_input(INPUT_POST, 'title');
    $description = filter_input(INPUT_POST, 'description');
    $customer = get_customers_by_last_name(explode(" ",filter_input(INPUT_POST, 'customer_name'))[1]);
    $date = date("Y/m/d");
    if ($product === "" || $title === "" || $description === "") {
        $error = "Invalid entry. Please check values and try again";
            include('../errors/error.php');
    } else {
        create_incident($customer[0]['customerID'], $product['productCode'], $date, $title, $description);
        include('successful.php');
    }
}

?>