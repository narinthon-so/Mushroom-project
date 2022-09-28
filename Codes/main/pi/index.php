
<!DOCTYPE html>
<html>

<head>
    <title>Index</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        a {
            padding-left: 10px;
            font-size: 20px;
        }
    </style>
</head>

<body>
    <H2>Welcome to Mushroom Database Server</H2>
    <a href="javascript:openPhpMyAdmin()">phpMyAdmin</a><br>
    <a href="javascript:openDataLogger()">Data Logger</a><br>
    <a href="javascript:openChart()">Chart</a>

    <script>
        openPhpMyAdmin = function() {
            location.href = document.URL + "phpmyadmin";
        }
        openDataLogger = function() {
            location.href = document.URL + "esp-data.php";
        }
        openChart = function() {
            location.href = document.URL + "esp-chart.php";
        }
    </script>
</body>

</html>