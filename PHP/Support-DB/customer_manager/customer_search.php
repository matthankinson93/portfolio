<?php include '../view/header.php'; ?>
<main>
    <h2>Customer Search</h2>
    <form action="." method="post">
    <input type="hidden" name="action" value="customer_display">
    <label>Last Name:</label>
    <input type="text" name="lName">
    <input type="submit" value="Search">
</form>
</main>
<?php include '../view/footer.php'; ?>