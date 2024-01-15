<?php include '../view/header.php'; ?>
<main>

    <!-- display a table of customer information -->
    <h2>Customer Search</h2>
    <form action="." method="post">
        <input type="hidden" name="action" value="customer_display">
        <input type="text" name="lName">
        <label>Last Name:</label>
        <input type="submit" value="Search">
    </form>
    <form action="." method="post" id="aligned">
        <h2>Results</h2>
        <table>
            <tr>
                <th>Last Name</th>
                <th>First Name</th>
                <th>Email Address</th>
                <th>City</th>
                <th>&nbsp;</th>
            </tr>
            <?php foreach ($customers as $customer) : ?>
                <form action="." method ="post"> 
                <input type="hidden" name="action" value="update_customer">
                <tr>
                    <td><?php echo htmlspecialchars($customer['lastName']); ?></td>
                    <td><?php echo htmlspecialchars($customer['firstName']); ?></td>
                    <td><?php echo htmlspecialchars($customer['email']); ?></td>
                    <td><?php echo htmlspecialchars($customer['city']); ?></td>
                    <td>
                        <input type="hidden" name="customer_ID"
                            value="<?php echo htmlspecialchars($customer['customerID']); ?>">
                        <input type="submit" value="Select">
                    </td>
                    </form>
                </tr>
            <?php endforeach; ?>
        </table>
    </form>
    <p><a href="">Search Customers</a></p>
</main>
<?php include '../view/footer.php'; ?>