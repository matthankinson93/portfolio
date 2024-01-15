<?php include '../view/header.php'; ?>
<main>

    <h2>Register Product</h2>
        <form action="." method="post" id="aligned">
        <input type="hidden" name="action" value="register_product">
        <label>Customer:</label>
        <input type="text" name="customer_name" value="<?php echo $customer_name; ?>" readonly style="border-width:0px"><br>
        <input type="hidden" name="customer_ID" value="<?php echo $customer_ID; ?>">
        <label>Product:</label>
        <select id="product_list" name ="name">
        <?php foreach ($products as $product) : ?>
            <option value="<?php echo htmlspecialchars($product['name']); ?>"><?php echo htmlspecialchars($product['name']); ?></option>
            <?php endforeach; ?>
        </select>
        <br>
        <input type="submit" value="Register Product">
        </form>
</main>
<?php include '../view/footer.php'; ?>