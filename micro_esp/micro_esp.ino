 #include <WiFi.h>  
 #include <SPI.h>              //Library for using SPI Communication 
 #include <Adafruit_NeoPixel.h>
 #include <string>
 #define PIXELPIN  18
 #define NUMPIXELS 1
   
   
 uint8_t byte_counter = 0;
 uint8_t bit_counter = 0;
 uint8_t bit_counter_2 = 0;
 uint8_t last_digits = 0;
 bool flag = false;
 int get_data2;
 String check;
 String check2;
 uint8_t j = 0;

 // Create the objects for server and client  
 WiFiServer server(80);  
 WiFiClient client;  
   
 const char* ssid   = "ESP32-CanBus-WebServer";// This is the SSID that ESP32 will broadcast  
 const char* password = "microver";     // password should be atleast 8 characters to make it work  
   
 // Create the global variable  
 String http;
 bool send_data = false;
 bool get_data = true;

  Adafruit_NeoPixel pixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800); 
  void setup() {  
  pixel.setPixelColor(0, pixel.Color(25, 0, 0));
  pixel.show();
  
  Serial.begin(115200);             //Begins Serial Communication at 9600 baudrate
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
   
  // Create the ESP32 access point  
  WiFi.softAP(ssid, password);  
   
  Serial.println( "" );  
  Serial.println( "WiFi AP is now running" );  
  Serial.println( "IP address: " );  
  Serial.println( WiFi.softAPIP() );  
   
  
  server.begin();                    // Start our ESP32 server  
           
  SPI.begin();                       //Begins SPI communication
  
  delay(1000);

  Serial.println("void setup done");
  pixel.setPixelColor(0, pixel.Color(0, 25, 25));
  pixel.show();
 }  
   
 void loop(){  
    
  if ( client = server.available() ) {  // Checks if a new client tries to connect to our server  
   Serial.println("New Client.");  
   String clientData = "";  
   while ( client.connected() ) {    // Wait until the client finish sending HTTP request  
    if ( client.available() ) {     // If there is a data,  
     char c = client.read();      //  read one character  
     http += c;            //  then parse it  
     Serial.write(c);
     if (c == '\n') {         // If the character is carriage return,   
      ///////////////////////
      char get_data[2];
      check = String(http.charAt(8)) +  String(http.charAt(9)) +  String(http.charAt(10)) +  String(http.charAt(11)) +  String(http.charAt(12)) +  String(http.charAt(13)) +  String(http.charAt(14)) +  String(http.charAt(15));
      check2 = check;
      if(check == "?input1=")
      {
          get_data[0] = http.charAt(16);
          get_data[1] = http.charAt(17);
          Serial.print("DATA: ");
          get_data2 = get_data[0] - '0';
          get_data2 = get_data2 * 10;
          get_data2 += get_data[1] - '0';
          Serial.println(get_data2);
      }
      check == "";
     ///////////////////////    
                      //  it means end of http request from client  
      if (clientData.length() == 0) { //  Now that the clientData is cleared,  
       sendResponse();        //    perform the necessary action  
       
      updateWebpage();    
      delay(300); //stands for refresh
  
       break;  
      } else {  
       clientData = "";       //  First, clear the clientData  
      }  
     } else if (c != '\r') {      // Or if the character is NOT new line  
      clientData += c;        //  store the character to the clientData variable  
     }  
    }  
   }   
   http = "";  
   client.stop();            // Disconnect the client.  
   Serial.println("Client disconnected.");  
   Serial.println("");  
  }  
 }  
   
 void sendResponse() {  
  // Send the HTTP response headers  
  client.println("HTTP/1.1 200 OK");  
  client.println("Content-type:text/html");  
  client.println("Connection: close");  
  client.println();   
 }  
   
 void updateWebpage() {  
    
  // Send the whole HTML  
  client.println("<!DOCTYPE html><html>");  
  client.println("<head>");  
  client.println("<title>ESP32-MCP2515-CANBUS</title>");  
  client.println("</head>");  
    
  

  //client.print("<hr>");

  if(get_data == true)
  {

      // Web Page Heading  
      client.print("<body><h1>Room Thermostat Manager");  
      //client.print(999, HEX);
      client.println("</h1>");
  
      //client.println("<meta http-equiv=refresh content=0"); //refresh
      /*for(int i = 1; i < 10; i++)
      {

    
        switch (i) //DISPLAYING DATA
        {
          case 1: client.print("<p>");  
                  client.print(i);  
                  client.print("st data: "); break;
          case 2: client.print("<p>");  
                  client.print(i);  
                  client.print("nd data: "); break;
          case 3: client.print("<p>");  
                  client.print(i);  
                  client.print("rd data: "); break;
          default:client.print("<p>");  
                  client.print(i);  
                  client.print("th data: "); break;
        }
    
        //client.print(cbyte.byte);
        client.println("</p>");
        
         
        client.print("<hr>");      
      }*/
      
      client.print("<hr>");
      client.print("<p>Room temp: ");
      client.print(5);
      client.print("<hr>");
      client.print("<p>Offset: ");
      client.print(5);
      client.print("<hr>");
      client.print("<p>Upper limit: ");
      client.print(5);
      client.print("<hr>");
      client.print("<p>Lower limit: ");
      client.print(5);
      client.print("<hr>");
      client.print("<p>Combi state: ");
      client.print(5);
      client.print("<hr>");
      client.println("</p>");
    
  }

    
  if(send_data == true)
  {
    if(check2 == "?input1=" || check2 == "?input2=")
      {
        if(j < 1)//get_data2)
        {
          // Web Page Heading  
          client.print("<body><h1>Configure your room settings</h1>");
          j++;
          check2 = "";
          
            client.print("<form action=/get>Lower temp limit: ");  
            //client.print(j);   
          client.println("<input type=number name=input2><input type=submit value=Apply></form><br>");
        }
        else
        {
          if(check2 == "?input2=")
          {
            client.println("<p>OK.<p>");
            send_data = false;
          }
        }
      }
      else
      {
         // Web Page Heading  
        client.print("<body><h1>Configure your room settings</h1>");  
        
        //client.println("<label for=Name>Name:</label>");
        //client.print("<input type=text id=Name name=Name placeholder=Number of digits>");
        client.println("<p>Please set your upper temp limit. If your room temp pass tis value your combi will be turned off.<p>"); 
        client.println("<form action=/get>Upper limit: <input type=number name=input1><input type=submit value=Apply></form><br>");
        client.println("<p><a href=\"/SEND_DATA/<%Eval(Name)%>\"><button>Next page</button></a></p>");
        client.println("<p><a href=\"/SEND_DATA/off\"><button>Cancel</button></a></p>");
      }
  }

  //DISPLAYING BUTTONS

  if(send_data == false)
  {
    client.println("<p><a href=\"/SEND_DATA/on\"><button>Go to settings</button></a></p>");

    /*if(get_data == true)
    {
      client.println("<p><a href=\"/GET_DATA/off\"><button>Do not get data</button></a></p>");
    }
    else
    {
      client.println("<p><a href=\"/GET_DATA/on\"><button>Get data</button></a></p>");
    }*/
  }
  
  //BUTTON ACTS
  if(http.indexOf("GET /GET_DATA/on") >= 0)
  {
    get_data = true;
  }
  if(http.indexOf("GET /GET_DATA/off") >= 0)
  {
    get_data = false;
  }
  if(http.indexOf("GET /SEND_DATA/on") >= 0)
  {
    send_data = true;
    get_data = false;
  }
  if(http.indexOf("GET /SEND_DATA/off") >= 0)
  {
    send_data = false;
  }

  /*client.print("<p>");
  client.print("<p>");
  client.print("<p>");
  client.print("send data: ");
  client.println(send_data);
  client.print("get data: ");
  client.println(get_data);*/
  
    
  client.println("</body></html>");  
  client.println();  
 }  
