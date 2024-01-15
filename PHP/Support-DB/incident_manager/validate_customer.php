<?php include '../view/header.php'; ?>
<main>
    <p>You must enter valid customers email to select the customer.</p>
    <form action="." method="post">
    <input type="hidden" name="action" value="validate_customer">
    <label>Email:</label>
    <input type="text" name="email">
    <input type="submit" value="Search">
    </form>
</main>
<?php include '../view/footer.php'; ?>