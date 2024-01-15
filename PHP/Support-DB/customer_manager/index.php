<?php
require('../model/countries_db.php');
require('../model/database.php');
require('../model/customer_db.php');

$action = filter_input(INPUT_POST, 'action');
if ($action === NULL) {
    $action = filter_input(INPUT_GET, 'action');
    if ($action === NULL) {
        $action = 'search_customers';
    }
}

if ($action == 'search_customers') {   
    include ('customer_search.php');
} else if ($action == 'customer_display') {
    $customers = get_customers_by_last_name(filter_input(INPUT_POST, 'lName'));
    if (count($customers) === 0 || filter_input(INPUT_POST, 'lName' === "")) {
        $error = "Invalid customer last name. Please ensure entry is a valid customer.";
        include('../errors/error.php');
    } else {
    include('customer_display.php');
    }
}  else if ($action == 'update_customer') {
    $cust_ID = filter_input(INPUT_POST, 'customer_ID');
    $country = get_countries();
    $customers = get_customer_by_id($cust_ID);
    $customer_country = get_country_from_code($customers[0]['countryCode']);
    include('customer_update.php');
} else if ($action == 'update_customer_details'){
    $customer_ID = filter_input(INPUT_POST, 'customer_ID');
    $fName =  filter_input(INPUT_POST, 'fName');
    $lName =  filter_input(INPUT_POST, 'lName');
    $address =  filter_input(INPUT_POST, 'address');
    $city =  filter_input(INPUT_POST, 'city');
    $state =  filter_input(INPUT_POST, 'state');
    $pCode =  filter_input(INPUT_POST, 'pCode', FILTER_VALIDATE_FLOAT);
    $cCode =  get_code_from_country(filter_input(INPUT_POST, 'country'));
    $phone =  filter_input(INPUT_POST, 'phone', FILTER_VALIDATE_FLOAT);
    $email =  filter_input(INPUT_POST, 'email');
    $password =  filter_input(INPUT_POST, 'password');

     // validate inputs 
     if ($customer_ID === "" || $fName === "" || $lName === "" || $address === "" || $city === "" || $state === ""|| $pCode === false || $cCode[0]['countryCode'] === "" || $phone === false || $email === "" || $password === "") {
        $error = "Invalid customer data. Check fields and try again.";
        include('../errors/error.php');
    } else {
        update_customer($customer_ID, $fName, $lName, $address, $city, $state, $pCode, $cCode[0]['countryCode'], $phone, $email, $password);
        include('successful.php');
    }
}
?>