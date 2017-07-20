const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<script src="https://code.jquery.com/jquery-3.1.0.js"></script>
<link href="https://fonts.googleapis.com/css?family=Ubuntu" rel="stylesheet"> 
<html>
  <head>
    <title>Schrankbeleuchtung</title>
  </head>
  <body link=black>
    <Center>
      <div>
           <button id="button" type="button">
            <img id="bulb"  style="width:150px;height:155px"> 
           </button>
      </div>
      <div> Lichtfarbe </div>
      <div>
        <input id="hue" type="range" min="-255" max="255" value="0" step="1" onchange="showHue(this.value)" />
      </div>
      <div> Helligkeit </div>
      <div>
        <input id="brightness" type="range" min="0" max="255" value="125" step="1" onchange="showBrightness(this.value)" />
      </div>
      <script type="text/javascript">
          var url = "http://simpleicon.com/wp-content/uploads/light-bulb-6.svg";
          var on = false;
          var sliderBrightness = 0;
          var sliderHue = 0;
          var hue = 127;
          var brightness = 0;
          var ip = getQueryVariable("ip");

          function getQueryVariable(variable) {
            var query = window.location.search.substring(1);
            var vars = query.split("&");
            for (var i=0;i<vars.length;i++) {
              var pair = vars[i].split("=");
              if (pair[0] == variable) {
                return pair[1];
              }
            }  
          }
        
        $("#ip").html(ip);
        $("#bulb").attr("src",url)
        $("#button").on("click", function(){
          if(on){
            url = "http://simpleicon.com/wp-content/uploads/light-bulb-6.svg";
            on = false;
            post();
          }
          else{
            url = "http://simpleicon.com/wp-content/uploads/light-bulb-7.svg";
            on = true;
            post();
          }
          $("#bulb").attr("src",url);
          })
          function showBrightness(newValue)
          {
            sliderBrightness = newValue;
            brightness = newValue;
            //console.log("showBrightness"+newValue);
            post();
          }          
          
          function showHue(newValue)
          {
            sliderHue = newValue;
            hue = (215+(sliderHue*(40/125)));           
            post();
          }
          function post () {
          jQuery.post( "http://"+ip+"/power" , {value : on, hue : sliderHue, brightness : brightness}, function(data, text){
            console.log("result: ", text)
          });
          console.log ("showURL" + "http://"+ip+"/power" , {value : on, hue : sliderHue, brightness : brightness})
          }
        </script>
        <style >
            body{
            font-size:27pt;
            color: black;
            text-shadow: 2px 1px rgb(180,180,180);
            font-family: Ubuntu;
            background-color: rgb(200,230,250)}
            #button {background-color: rgb(255,255,100);
                     border:2px solid #000}
        </style>
     </Center>
  </body>
</html>
)=====";
