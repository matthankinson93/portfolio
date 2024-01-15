<?php
function get_technicians() {
    global $db;
    $query = 'SELECT * FROM technicians
              ORDER BY techID';
    $statement = $db->prepare($query);
    $statement->execute();  
    $technicians = $statement->fetchAll();
    $statement->closeCursor();
    return $technicians;
}

function get_tech_by_ID($technician_ID){
    global $db;
    $query = 'SELECT * FROM technicians
             WHERE techID = :technician_ID';
    $statement = $db->prepare($query);
    $statement->bindValue(':technician_ID', $technician_ID);
    $statement->execute();
    $technician = $statement->fetch();
    $statement->closeCursor();
    return $technician;
}

function delete_technician($technician_ID) {
    global $db;
    $query = 'DELETE FROM technicians
              WHERE techID = :technician_ID';
    $statement = $db->prepare($query);
    $statement->bindValue(':technician_ID', $technician_ID);
    $statement->execute();
    $statement->closeCursor();
}

function add_technician( $techID, $fName, $lName, $email, $phone, $password) {
    global $db;
    $query = 'INSERT INTO technicians
                 (techID, firstName, lastName, email, phone, password)
              VALUES
                 (:techID, :fName, :lName, :email, :phone, :password)';
    $statement = $db->prepare($query);
    $statement->bindValue(':techID', $techID);
    $statement->bindValue(':fName', $fName);
    $statement->bindValue(':lName', $lName);
    $statement->bindValue(':email', $email);
    $statement->bindValue(':phone', $phone);
    $statement->bindValue(':password', $password);
    $statement->execute();
    $statement->closeCursor();
}
?>