<?php
function get_customers() {
    global $db;
    $query = 'SELECT * FROM customers
              ORDER BY customerID';
    $statement = $db->prepare($query);
    $statement->execute();
    $customers = $statement->fetchAll();
    $statement->closeCursor();
    return $customers;
}

function get_customers_by_last_name($lName) {
    global $db;
    $query = 'SELECT customers.firstName, customers.lastName, customers.email, customers.city, customers.customerID
        FROM customers
        WHERE customers.lastName = :lName';
    $statement = $db->prepare($query);
    $statement->bindValue(':lName', $lName);
    $statement->execute();
    $customers = $statement->fetchAll();
    $statement->closeCursor();
    return $customers;
}

function get_customer_by_id($customer_id) {
    global $db;
    $query = 'SELECT *
        FROM customers
        WHERE customers.customerID = :customer_id';
    $statement = $db->prepare($query);
    $statement->bindValue(':customer_id', $customer_id);
    $statement->execute();
    $customers = $statement->fetchAll();
    $statement->closeCursor();
    return $customers;
}

function get_customer_by_email($email) {
    global $db;
    $query = 'SELECT customers.customerID, customers.firstName, customers.lastName, customers.email 
              FROM customers
              WHERE customers.email = :email';
    $statement = $db->prepare($query);
    $statement->bindValue(':email', $email);
    $statement->execute();
    $customers = $statement->fetchAll();
    $statement->closeCursor();
    return $customers;
}

function add_customer($first_name, $last_name, 
        $address, $city, $state, $postal_code, $country_code,
        $phone, $email, $password) {
}

function update_customer($customer_ID, $fName, $lName,
        $address, $city, $state, $pCode, $cCode,
        $phone, $email, $password) {
   global $db;
   $query = 'UPDATE customers
             SET customerID = :customer_ID, 
                firstName = :fName,
                lastName = :lName,
                address = :address,
                city = :city,
                state = :state,
                postalCode = :pCode,
                countryCode = :cCode,
                phone = :phone,
                email = :email,
                password = :password
                WHERE customerID = :customer_ID';
    $statement = $db->prepare($query);
    $statement->bindValue(':customer_ID', $customer_ID);
    $statement->bindValue(':fName', $fName);
    $statement->bindValue(':lName', $lName);
    $statement->bindValue(':address', $address);
    $statement->bindValue(':city', $city);
    $statement->bindValue(':state', $state);
    $statement->bindValue(':pCode', $pCode);
    $statement->bindValue(':cCode', $cCode);
    $statement->bindValue(':phone', $phone);
    $statement->bindValue(':email', $email);
    $statement->bindValue(':password', $password);
    $statement->execute();
    $statement->closeCursor();
}
?>