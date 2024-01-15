<?php include '../view/header.php'; ?>
<main>
<form action="." method="post" id="aligned">
    <input type="hidden" name="action" value="update_customer_details">
    
    <label>Customer ID:</label>
    <input type="text" name="customer_ID" value="<?php echo $cust_ID; ?>" readonly style="border-width:0px"><br>

    <label>First Name:</label>
    <input type="text" name="fName" value="<?php echo $customers[0]['firstName']; ?>"><br>

    <label>Last Name:</label>
    <input type="text" name="lName" value="<?php echo $customers[0]['lastName']; ?>"><br>
    
    <label>Address:</label>
    <input type="text" name="address"value="<?php echo $customers[0]['address']; ?>"><br>

    <label>City:</label>
    <input type="text" name="city" value="<?php echo $customers[0]['city']; ?>"><br>

    <label>State:</label>
    <input type="text" name="state" value="<?php echo $customers[0]['state']; ?>"><br>

    <label>Postal Code:</label>
    <input type="text" name="pCode" value="<?php echo $customers[0]['postalCode']; ?>"><br>

    <label>Country</label>
    <select id="country_code" name ="country" style="width: 13.35em;">
    <option value="<?php echo $customer_country[0]['countryName']; ?>" selected hidden><?php echo $customer_country[0]['countryName']; ?></option>
        <?php foreach ($country as $country) : ?>
            <option value="<?php echo ($country['countryName']); ?>"><?php echo ($country['countryName']); ?></option>
            <?php endforeach; ?>
        </select><br>

    <label>Phone:</label>
    <input type="text" name="phone" value="<?php echo $customers[0]['phone']; ?>"><br>

    <label>Email:</label>
    <input type="text" name="email" value="<?php echo $customers[0]['email']; ?>"><br>

    <label>Password:</label>
    <input type="text" name="password" value="<?php echo $customers[0]['password']; ?>"><br>

    <input type="submit" value="Update Customer">
</form>
<nav>
        <ul>
            <li><a href="javascript:history.back()">Customers</a></li>
        </ul>
    </nav>
</main>
<?php include '../view/footer.php'; ?>