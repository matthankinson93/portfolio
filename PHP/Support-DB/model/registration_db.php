<?php
function add_registration($customer_id, $product_code, $date) {
    global $db;
    $query = 'INSERT INTO registrations
                 (customerID, productCode, registrationDate)
              VALUES
                 (:customer_id, :product_code, :date)';
    $statement = $db->prepare($query);
    $statement->bindValue(':customer_id', $customer_id);
    $statement->bindValue(':product_code', $product_code);
    $statement->bindValue(':date', $date);
    $statement->execute();
    $statement->closeCursor();
}
?>