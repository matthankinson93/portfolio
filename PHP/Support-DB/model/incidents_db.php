<?php

function create_incident($customer_id, $product_code, $date, $title, $description) {
    global $db;
    $query = 'INSERT INTO incidents
                 (customerID, productCode, dateOpened, title, description)
              VALUES
                 (:customer_id, :product_code, :date, :title, :description)';
    $statement = $db->prepare($query);
    $statement->bindValue(':customer_id', $customer_id);
    $statement->bindValue(':product_code', $product_code);
    $statement->bindValue(':date', $date);
    $statement->bindValue(':title', $title);
    $statement->bindValue(':description', $description);
    $statement->execute();
    $statement->closeCursor();
}

?>