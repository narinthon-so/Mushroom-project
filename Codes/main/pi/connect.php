<?php
$servername = "localhost";

// REPLACE with your Database name
$dbname = "mushroom";
// REPLACE with Database user
$username = "admin";
// REPLACE with Database user password
$password = "keybale321";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
?>