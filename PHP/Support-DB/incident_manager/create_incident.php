<?php include '../view/header.php'; ?>
<main>
<form action="." method="post" id="aligned">
    <input type="hidden" name="action" value="create_incident">

    <label>Customer:</label>
    <input type="text" name="customer_name" value="<?php echo $customer_name; ?>" readonly style="border-width:0px"><br>
    
    <label>Product:</label>
    <select id="product_list" name ="incident_product">
        <?php foreach ($product as $product) : ?>
            <option value="<?php echo htmlspecialchars($product['name']); ?>"><?php echo htmlspecialchars($product['name']); ?></option>
            <?php endforeach; ?>
        </select><br>
        
    <label>Title:</label>
    <input type="text" name="title"><br>

    <label>Description:</label>
    <input type="text" name="description" style="height: 100px;"><br>

    <input type="submit" value="Update">
</form>
</main>
<?php include '../view/footer.php'; ?>