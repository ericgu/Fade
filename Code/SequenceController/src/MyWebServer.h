
class MyWebServer
{
  static MyWebServer *pMyWebServerInstance;

  WebServer *_pWebServer;

  char *_pProgramBuffer;
  char *_pPageBuffer;

  IPAddress _myIPAddress;

  Supervisor *_pSupervisor;

  static void handleRoot()
  {
    pMyWebServerInstance->handleRootInstance();
  }

  static void handleNodeName()
  {
    pMyWebServerInstance->handleNodeNameInstance();
  }

  static void handleButton()
  {
    pMyWebServerInstance->handleButtonInstance();
  }

public:
  MyWebServer(Supervisor *pSupervisor, IPAddress myIPAddress)
  {
    _pWebServer = new WebServer(80);

    _pProgramBuffer = new char[16636];
    _pPageBuffer = new char[16636];

    _pSupervisor = pSupervisor;
    _myIPAddress = myIPAddress;

    _pWebServer->on("/", handleRoot);
    _pWebServer->on("/SetNodeName", handleNodeName);
    _pWebServer->on("/Button", handleButton);
    _pWebServer->begin();

    pMyWebServerInstance = this;
  }

  void DumpArgs()
  {
    Serial.print("Args: ");
    Serial.println(_pWebServer->args());

    for (int i = 0; i < _pWebServer->args(); i++)
    {
      Serial.print(_pWebServer->argName(i));
      Serial.print(" ");
      Serial.println(_pWebServer->arg(i));
    }
  }

  void handleButtonInstance()
  {
    //DumpArgs();

    String ButtonNumber;
    ButtonNumber = _pWebServer->arg("ButtonNumber");
    if (ButtonNumber.length() != 0)
    {
      int buttonNumber = atoi(ButtonNumber.c_str());

      Serial.print("Button: ");
      Serial.println(buttonNumber);
      snprintf(_pPageBuffer, 16636, "<h1>%d</h1>", buttonNumber);
      _pWebServer->send(200, "text/html", _pPageBuffer);
    }
  }

  void handleRootInstance()
  {
    //DumpArgs();

    String Program;
    Program = _pWebServer->arg("Program");
    if (Program.length() != 0)
    {
      snprintf(_pPageBuffer, 16636,
               "<meta http-equiv=\"refresh\" content=\"10;url=http://%d.%d.%d.%d/\">\
        <html><H1>Updating program</H1></html>",
               (int)_myIPAddress[0], (int)_myIPAddress[1], (int)_myIPAddress[2], (int)_myIPAddress[3]);

      Serial.println("Updating");
      _pWebServer->send(200, "text/html", _pPageBuffer);
      delay(1000); // wait for the response to be sent...

      Serial.println("Response sent");

      _pSupervisor->UpdateProgram(Program.c_str()); // this resets the ESP...
    }

    CommandFormatter::PrettyFormat(_pSupervisor->GetCurrentProgram(), _pProgramBuffer, 16636);

    const char *pRunning = _pSupervisor->GetExecutingProgramState() ? "Program executing" : "<b>Program execution pending</b>";
    const char *pNodeName = _pSupervisor->GetNodeName();

    snprintf(_pPageBuffer, 16636,

             "<html>\
  <head>\
    <title>%s: EagleDecorations Sequence Controller</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1><a href=\"SetNodeName\">%s</a>: EagleDecorations Sequence Controller</h1>\
    <p>%s (up for %d seconds)</p>\
    <table>\
    <tr>\
    <td style=\"vertical-align: top;\">\
    Code:\
    <FORM action=\"/\" method=\"post\">\
    <textarea rows = \"50\" cols = \"80\" name=\"Program\">\
%s\
    </textarea><br>\
    <INPUT type=\"submit\" spellcheck=\"off\" value=\"Send\">\
    </FORM>\
    </td>\
    <td style=\"vertical-align: top;\">\
    Errors:\
    <FORM action=\"/\" method=\"post\">\
    <textarea rows = \"50\" cols = \"50\" spellcheck=\"off\" name=\"Errors\">\
    %s\
    </textarea><br>\
    </FORM>\
    </td>\
    </tr>\
    </table>\
  </body>\
</html>",
             pNodeName,
             pNodeName,
             pRunning,
             _pSupervisor->GetExecutionCount() / 100,
             _pProgramBuffer,
             _pSupervisor->GetCurrentErrors());

    _pWebServer->send(200, "text/html", _pPageBuffer);
  }

  void handleNodeNameInstance()
  {
    // DumpArgs();

    String nodeName;
    nodeName = _pWebServer->arg("nodename");
    if (nodeName.length() != 0)
    {
      _pSupervisor->UpdateNodeName(nodeName.c_str());

      _pWebServer->sendHeader("Location", String("/"), true);
      _pWebServer->send(302, "text/plain", "");
      Serial.println("Node name update done - redirecting");
      Serial.flush();
    }

    snprintf(_pPageBuffer, 16636,

             "<html>\
  <head>\
    <title>Node name update</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Node name update</h1>\
    <FORM action=\"/SetNodeName\" method=\"post\">\
    Node name: \
    <input type=\"text\" name=\"nodename\" value=\"%s\">\
    <INPUT type=\"submit\" value=\"Update\">\
    < / FORM >\
                 < / body >\
                 </ html> ",
             _pSupervisor->GetNodeName());

    _pWebServer->send(200, "text/html", _pPageBuffer);
  }

  void HandleClient()
  {
    _pWebServer->handleClient();
  }
};

MyWebServer *MyWebServer::pMyWebServerInstance;