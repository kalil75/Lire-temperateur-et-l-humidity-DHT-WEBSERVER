#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
//Insérez vos informations d'identification réseau dans les variables suivantes, afin que l'ESP32 puisse se connecter à votre wfi
const char *ssid = "xxxx";
const char *password = "xxxx";

WebServer server(80);   //Créé WebServerobjet sur le port 80.
DHT dht(26, DHT11);    //instancier unDHTobjet avec le type et la broche 26 et le type DHT11

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
  // la page Web affiche un titre et deux paragraphes. Il y a un paragraphe pour afficher la température et un autre pour afficher l'humidité. Il y a aussi deux icônes pour styliser notre page.


           "<html>\
  <head>\
  //La balise <meta> suivante rend votre page Web réactive dans n'importe quel navigateur.
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
   // La balise <link> est nécessaire pour charger les icônes du site fontawesome
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\


    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>ESP32 DHT Server!</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}

void setup(void) {
//initialiser le moniteur série à des fins de débogage.
  Serial.begin(115200);
  //Initialiser le capteur DHT
  dht.begin();
 // Connectez-vous à votre réseau local et imprimez l'adresse IP ESP32
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
}

//lire la température (readDHTTemperature()) et l'autre pour lire l'humidité (readDHTHumidity()).
float readDHTTemperature() {
  
  // lire la temperateur en celesius
  float t = dht.readTemperature();
  //Nous avons également une condition qui renvoie deux tirets (–) au cas où le capteur ne parviendrait pas à obtenir les lectures 
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}
//le méme cas pour la fonction readDHTHumidity

float readDHTHumidity() {
  
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
  
  
  //REMARQUE:si vous voulez d'ajouter l'option de synchronisation et fait mise a jour automatique
  //code JavaScript dans notre page Web qui met à jour automatiquement la température et l'humidité, toutes les 10 secondes
  //Les scripts dans le texte HTML doivent être placés entre les balises <script></script> .
//<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
