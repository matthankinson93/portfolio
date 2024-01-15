<?php
require('../model/database.php');
require('../model/technician_db.php');

$action = filter_input(INPUT_POST, 'action');
if ($action === NULL) {
    $action = filter_input(INPUT_GET, 'action');
    if ($action === NULL) {
        $action = 'list_technicians';
    }
}

if ($action == 'list_technicians') {
    $technicians = get_technicians();
    include ('technician_list.php');
} else if ($action == 'delete_technician') {
    $technician_ID = filter_input(INPUT_POST, 'technician_ID');
    delete_technician($technician_ID);
    header("Location: .");
} else if ($action == 'show_add_form') {
    include ('technician_add.php');
} else if ($action == 'add_technician') {
    $techID = filter_input(INPUT_POST, 'techID', FILTER_VALIDATE_FLOAT);
    $fName = filter_input(INPUT_POST, 'fName');
    $lName = filter_input(INPUT_POST, 'lName');
    $email = filter_input(INPUT_POST, 'email');
    $phone = filter_input(INPUT_POST, 'phone', FILTER_VALIDATE_FLOAT);
    $password = filter_input(INPUT_POST, 'password');
    
    // Validate the inputs
    if ($techID === NULL || $fName === NULL || $lName === NULL || $email === NULL || $phone === FALSE || $password === NULL) {
        $error = "Invalid techician data. Check fields and try again.";
        include('../errors/error.php');
    } else {
    add_technician($techID, $fName, $lName, $email, $phone, $password);
    header("Location: .");
    }
}
?>