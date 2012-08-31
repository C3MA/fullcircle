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
    <link href="http://heckotec.crux.uberspace.de/fullcircle/website/theme/Simplico/css/reset.css" rel="stylesheet">
    <!--<link href="http://heckotec.crux.uberspace.de/fullcircle/website/theme/Simplico/css/style.css?v=3.1.2" rel="stylesheet">-->
    <link type="text/css" rel="stylesheet" href="http://fonts.googleapis.com/css?family=Wire+One">
	<style type="text/css">
		body {
			color: #343434;
		       font-size: 12px;
		       font-family: Tahoma, Helvetica, sans-serif;
		}
	</style>
  </head>
  <body>

<?php
if($_GET["form"] !== "false")
{?>
	<p>
		<form action="player.php">
			Interaktionskennzeichen: <br />
			<input type="text" name="sequence" value="<?php echo $_GET["sequence"] ?>" />
			<input type="submit" value="Anzeigen" />
		</form>
	</p>

<?php
}

if($_SERVER["REQUEST_METHOD"]=="POST")
{
  //copy uploaded file to processing folder

  //generate link to player
  
}


if(isset($_GET["sequence"]) && $_GET["sequence"]!="")
{
	$id = $_GET["sequence"];
	if ( is_numeric($id) || $id === "perlin" )
	{
		if ( file_exists("data/".$id.".png") )
		{
			$videofile="http://fullcircle-data.ccc-mannheim.de/data/".basename($_GET["sequence"]);
?>
    <video id="fcwp1" class="video-js vjs-default-skin"  
      controls preload="auto" width="640" height="480"  
      poster="<?php echo $videofile; ?>.png"  
      data-setup='{"preload":"auto"}'>  
     <source src="<?php echo $videofile; ?>.webm" type='video/webm' />  
     <source src="<?php echo $videofile; ?>.ogv" type='video/ogg' />  
     <source src="<?php echo $videofile; ?>.mp4" type='video/mp4' />  
    </video>
<!-- include video-js -->
    <link href="http://fullcircle-data.ccc-mannheim.de/video-js/video-js.css" rel="stylesheet">
    <script src="http://fullcircle-data.ccc-mannheim.de/video-js/video.js"></script>
    <script>
      _V_.options.flash.swf = "http://fullcircle-data.ccc-mannheim.de/video-js/video-js.swf"
    </script>
		<?}
		else
		{?>
			<p>
				Die angebene ID ist ungültig. Bitte überprüfen sie ihre Eingabe und probieren es später nochmal.
			</p>
		<?}
	}
	else
	{?>
		<p>
				Die angebene ID entspricht nicht dem geforderten Format. Bitte geben sie nur die Zahl, nicht die Farbe ein.
		</p>
	<?}
}?>
    </body>
</html>

