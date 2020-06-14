<?php

include 'connect.php';

$sql = "SELECT id, temp, humi, save_time FROM SensorData ORDER BY save_time DESC LIMIT 24"; //ASC||DESC

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()) {
  $sensor_data[] = $data;
}

$save_time = array_column($sensor_data, 'save_time');

$temp = json_encode(array_reverse(array_column($sensor_data, 'temp')), JSON_NUMERIC_CHECK);
$humi = json_encode(array_reverse(array_column($sensor_data, 'humi')), JSON_NUMERIC_CHECK);
$save_time = json_encode(array_reverse($save_time), JSON_NUMERIC_CHECK);

/*echo $value1;
echo $value2;
echo $value3;
echo $reading_time;*/

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://code.highcharts.com/highcharts.js"></script>
<style>
  body {
    min-width: 310px;
    max-width: 1280px;
    height: 500px;
    margin: 0 auto;
  }

  h2 {
    font-family: Arial;
    font-size: 2.5rem;
    text-align: center;
  }
</style>

<body>
  <h2>Mushroom Chart</h2>
  <div id="chart-temperature" class="container"></div>
  <div id="chart-humidity" class="container"></div>
  <script>
    var temp = <?php echo $temp; ?>;
    var humi = <?php echo $humi; ?>;
    var save_time = <?php echo $save_time; ?>;

    var chartT = new Highcharts.Chart({
      chart: {
        renderTo: 'chart-temperature'
      },
      title: {
        text: 'Mushroom Temperature'
      },
      series: [{
        showInLegend: false,
        data: temp
      }],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: {
            enabled: true
          }
        },
        series: {
          color: '#059e8a'
        }
      },
      xAxis: {
        type: 'datetime',
        categories: save_time
      },
      yAxis: {
        title: {
          text: 'Temperature (Celsius)'
        }
        //title: { text: 'Temperature (Fahrenheit)' }
      },
      credits: {
        enabled: false
      }
    });

    var chartH = new Highcharts.Chart({
      chart: {
        renderTo: 'chart-humidity'
      },
      title: {
        text: 'Mushroom Humidity'
      },
      series: [{
        showInLegend: false,
        data: humi
      }],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: {
            enabled: true
          }
        }
      },
      xAxis: {
        type: 'datetime',
        //dateTimeLabelFormats: { second: '%H:%M:%S' },
        categories: save_time
      },
      yAxis: {
        title: {
          text: 'Humidity (%)'
        }
      },
      credits: {
        enabled: false
      }
    });
  </script>
</body>

</html>