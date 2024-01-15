<?php include '../view/header.php'; ?>
<main>
    <h1>Add Technician</h1>
    <form action="." method="post" id="aligned">
    <input type="hidden" name="action" value="add_technician">

        <label>ID:</label>
        <input type="text" name="techID"><br>

        <label>First Name:</label>
        <input type="text" name="fName"><br>

        <label>Last Name:</label>
        <input type="text" name="lName"><br>

        <label>Email</label>
        <input type="text" name="email"><br>

        <label>Phone:</label>
        <input type="text" name="phone" /><br>

        <label>Password:</label>
        <input type="text" name="password" /><br>
       
        <label>&nbsp;</label>
        <input type="submit" value="Add Technician" />
    </form>
    <p><a href="?action=list_technicians">View Technician List</a></p>

</main>
<?php include '../view/footer.php'; ?>