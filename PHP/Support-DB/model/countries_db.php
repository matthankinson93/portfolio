<?php

function get_countries() {
    global $db;
    $query = 'SELECT * FROM countries
              ORDER BY countryName';
    $statement = $db->prepare($query);
    $statement->execute();
    $countries = $statement->fetchAll();
    $statement->closeCursor();
    return $countries;
}

function get_country_from_code($code){
    global $db;
    $query = 'SELECT countries.countryName 
            FROM countries
            WHERE countryCode = :code';
    $statement = $db->prepare($query);
    $statement->bindValue(':code', $code);
    $statement->execute();
    $code = $statement->fetchAll();
    $statement->closeCursor();
    return $code;
}

function get_code_from_country($country){
    global $db;
    $query = 'SELECT countries.countryCode 
            FROM countries
            WHERE countryName = :country';
    $statement = $db->prepare($query);
    $statement->bindValue(':country', $country);
    $statement->execute();
    $country = $statement->fetchAll();
    $statement->closeCursor();
    return $country;
}
?>