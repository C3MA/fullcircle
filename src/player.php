<!DOCTYPE html>
<html>
  <head>
    <meta charset=utf-8 />
    <title>FullCircle-Web-Player</title>
    <?php /*
    <link rel="stylesheet" type="text/css" media="screen" href="css/master.css" />
    <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"></script>*/
    ?>
    <!--[if IE]>
    <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
  </head>
  <body>

<?php

if($_SERVER["REQUEST_METHOD"]=="POST")
{
  //copy uploaded file to processing folder

  //generate link to player
  
}


if(isset($_GET["sequence"]) && $_GET["sequence"]!="")
{
        $videofile="http://fullcircle.comsound.de/data/".basename($_GET["sequence"]);
}
else
{
        $videofile="http://video-js.zencoder.com/oceans-clip";
}
?>
    <video id="fcwp1" class="video-js vjs-default-skin"  
      controls preload="auto" width="640" height="480"  
      poster="<?php echo $videofile; ?>.png"  
      data-setup='{"preload":"auto"}'>  
     <source src="<?php echo $videofile; ?>.mp4" type='video/mp4' />  
     <source src="<?php echo $videofile; ?>.webm" type='video/webm' />  
     <source src="<?php echo $videofile; ?>.ogv" type='video/ogg' />  
    </video>
<!-- include video-js -->
    <link href="http://fullcircle.comsound.de/video-js/video-js.css" rel="stylesheet">
    <script src="http://fullcircle.comsound.de/video-js/video.js"></script>
    <script>
      _V_.options.flash.swf = "http://fullcircle.comsound.de/video-js/video-js.swf"
    </script>

    </body>
</html>

