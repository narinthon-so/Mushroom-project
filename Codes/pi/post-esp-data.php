<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "mushroom";
// REPLACE with Database user
$username = "admin";
// REPLACE with Database user password
$password = "keybale321";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key = $temp = $humi = $temp_limit_min = $temp_limit_max = $humi_limit_min = $humi_limit_max = $ctrl_mode = $pump_state = $fan_state = $lux = $set_lux = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $temp = test_input($_POST["temp"]);
        $humi = test_input($_POST["humi"]);
        $lux = test_input($_POST["lux"]);
        $temp_limit_min = test_input($_POST["temp_limit_min"]);
        $temp_limit_max = test_input($_POST["temp_limit_max"]);
        $humi_limit_min = test_input($_POST["humi_limit_min"]);
        $humi_limit_max = test_input($_POST["humi_limit_max"]);
        $set_lux = test_input($_POST["set_lux"]);
        $ctrl_mode = test_input($_POST["ctrl_mode"]);
        $pump_state = test_input($_POST["pump_state"]);
        $fan_state = test_input($_POST["fan_state"]);

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (temp, humi, lux, temp_limit_min, temp_limit_max, humi_limit_min, humi_limit_max, set_lux, ctrl_mode, pump_state, fan_state)
        VALUES ('" . $temp . "', '" . $humi . "', '" . $lux . "', '" . $temp_limit_min . "', '" . $temp_limit_max . "', '" . $humi_limit_min . "', '" . $humi_limit_max . "', '" . $set_lux . "', '" . $ctrl_mode . "', '" . $pump_state . "', '" . $fan_state . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
