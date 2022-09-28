<?php
    $json_obj = file_get_contents("http://localhost:4040/api/tunnels");
    $php_obj = json_decode($json_obj, true);

    var_dump($php_obj);

?>