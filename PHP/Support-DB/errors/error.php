<?php include '../view/header.php'; ?>
<main>
    <h1>Error</h1>
    <p><?php echo htmlspecialchars($error); ?></p>
    <nav>
        <ul>
            <li><a href="javascript:history.back()">Back</a></li>
        </ul>
    </nav>
</main>
<?php include '../view/footer.php'; ?>