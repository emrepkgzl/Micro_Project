 #include <WiFi.h>  
 #include <SPI.h>              //Library for using SPI Communication 
 #include <mcp2515.h>          //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
 #include <SoftwareSerial.h>
 #include <Adafruit_NeoPixel.h>
 #include <string>
 
 #define PIXELPIN  18
 #define NUMPIXELS 1
 
 SoftwareSerial mySerial(17, 18); // RX, TX
  
 bool flag = false;
 int get_data2 = 0;
 int get_data3 = 0;
 int room_temp = 0;
 bool combi_state = false;
 String check;
 String check2;
 uint8_t j = 0;
   
 // Create the objects for server and client  
 WiFiServer server(80);  
 WiFiClient client;  
   
 const char* ssid   = "Room temp manager";// This is the SSID that ESP32 will broadcast  
 const char* password = "stm32stm32";     // password should be atleast 8 characters to make it work  
   
 // Create the global variable  
 String http;
 bool send_data = false;
 bool get_data = true;

  Adafruit_NeoPixel pixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800); 
  void setup() {  
  pixel.setPixelColor(0, pixel.Color(25, 0, 0));
  pixel.show();
  
  Serial.begin(115200);             //Begins Serial Communication at 9600 baudrate
  //Serial.print("Connecting to ");  
  Serial.println(ssid);  
   
  // Create the ESP32 access point  
  WiFi.softAP(ssid, password);  
   
  //Serial.println( "" );  
  //Serial.println( "WiFi AP is now running" );  
  //Serial.println( "IP address: " );  
  //Serial.println( WiFi.softAPIP() );  
   
  
  server.begin();                    // Start our ESP32 server  

  delay(1000);


  //Serial.println("void setup done");
  pixel.setPixelColor(0, pixel.Color(0, 25, 25));
  pixel.show();
 }  
   
 void loop(){  
    
  if ( client = server.available() ) {  // Checks if a new client tries to connect to our server  
   //Serial.println("New Client.");  
   String clientData = "";  
   while ( client.connected() ) {    // Wait until the client finish sending HTTP request  
    if ( client.available() ) {     // If there is a data,  
     char c = client.read();      //  read one character  
     http += c;            //  then parse it  
     //Serial.write(c);
     if (c == '\n') {         // If the character is carriage return,   
      ///////////////////////
      char get_data[2];
      check = String(http.charAt(8)) +  String(http.charAt(9)) +  String(http.charAt(10)) +  String(http.charAt(11)) +  String(http.charAt(12)) +  String(http.charAt(13)) +  String(http.charAt(14)) +  String(http.charAt(15));
      check2 = check;
      if(check == "?input1=")
      {
          get_data[0] = http.charAt(16);
          get_data[1] = http.charAt(17);
          //Serial.print("DATA: ");
          get_data2 = get_data[0] - '0';
          get_data2 = get_data2 * 10;
          get_data2 += get_data[1] - '0';
          //Serial.println(get_data2);
      }
      get_data[0] = 0;
      get_data[1] = 0;
      if(check == "?input2=")
      {
          get_data[0] = http.charAt(16);
          get_data[1] = http.charAt(17);
          //Serial.print("DATA: ");
          get_data3 = get_data[0] - '0';
          get_data3 = get_data3 * 10;
          get_data3 += get_data[1] - '0';
          //Serial.println(get_data3);
      }

      while(Serial.available())
      {
        pixel.setPixelColor(0, pixel.Color(25, 0, 0));
          pixel.show();
        int incoming_data = Serial.read();
        if(incoming_data == 97)
        {
          combi_state = true;
        }
        else if(incoming_data == 98)
        {
          combi_state = false;
        }
      }
      check == "";
     ///////////////////////    
                      //  it means end of http request from client  
      if (clientData.length() == 0) { //  Now that the clientData is cleared,  
       sendResponse();        //    perform the necessary action  
       
      updateWebpage();    
      mySerial.write(0xAA);
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
   //Serial.println("Client disconnected.");  
   //Serial.println("");  
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
    
  

  client.print("<hr>");

  if(get_data == true)
  {
      // Web Page Heading  
      client.print("<body><h1>Room stats");  
      //client.print(999, HEX);
      client.println("</h1>");
  
      client.println("<meta http-equiv=refresh content=0"); //refresh


          if((get_data2 <= 27) && (get_data2 >= 20) && (get_data3 <= 28) && (get_data3 >= 21) && (get_data3 > get_data2))
          {
            client.print("<p>Lower limit: ");  
            client.print(get_data2);  
            client.print("C");
            client.print("<p>Upper limit: ");  
            client.print(get_data3);  
            client.print("C");
            client.print("<p>Combi state: "); 
            if(combi_state)
            {
               client.print("on");
            }
            else
            {
              client.print("off");
            }
            //client.print(combi_state);  
            
      
            //client.print(cbyte.byte);
            client.println("</p>");
          
           
            client.print("<hr>");  
          }    
          else
          {
            client.print("<p>Please set the limits</p>");
          }
      
    
  }

    
  if(send_data == true)
  {
    if(check2 == "?input1=")
      {
        
          check2 = "";
          
            client.print("<form action=/get>Type your upper limit (21 - 28):  ");   
                    
          client.println("<input type=number name=input2><input type=submit value=Apply></form><br>");

        
      }
      else if(check2 == "?input2=")
      {
        send_data = false;
        //client.println("<p><a href=\"/GET_DATA/on\"><button>Show stats</button></a></p>");

        //mySerial.write(get_data2 + 100); //UARTTAN GONDER
        Serial.print(get_data2 + 100);
        delay(200);
        //mySerial.write(get_data3 + 200);
        Serial.print(get_data3 + 200);
      }
      
      else
      {
         // Web Page Heading  
        client.print("<body><h1>Configure Settings</h1>");  
        
        //client.println("<label for=Name>Name:</label>");
        //client.print("<input type=text id=Name name=Name placeholder=Number of digits>");
        client.println("<form action=/get>Type your lower limit (20 - 27): <input type=number name=input1><input type=submit value=Apply></form><br>");
        client.println("<p><a href=\"/SEND_DATA/off\"><button>Cancel</button></a></p>");
      }
  }

  //DISPLAYING BUTTONS

  if(send_data == false)
  {
    client.println("<p><a href=\"/SEND_DATA/on\"><button>Configure settings</button></a></p>");

    if(get_data == true)
    {
      //client.println("<p><a href=\"/GET_DATA/off\"><button>Do not get data</button></a></p>");
    }
    else
    {
      client.println("<p><a href=\"/GET_DATA/on\"><button>Show stats</button></a></p>");
    }
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
