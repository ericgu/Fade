
class MyWebServer
{
    static MyWebServer* pMyWebServerInstance;

    WebServer* _pWebServer;

    char* _pProgramBuffer;
    char* _pPageBuffer;

    Supervisor* _pSupervisor;

    static void handleRoot()
    {
      pMyWebServerInstance->handleRootInstance();
    }

    public:

      MyWebServer(Supervisor* pSupervisor)
      {
        _pWebServer = new WebServer(80);

        _pProgramBuffer = new char[16636];
        _pPageBuffer = new char[16636];

        _pSupervisor = pSupervisor;

        _pWebServer->on ( "/", handleRoot );
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


void handleRootInstance() 
{
     //DumpArgs();

  String Program;
  Program = _pWebServer->arg("Program");
  //Serial.println("1"); Serial.flush();
  if (Program.length() != 0)
  {
    //Serial.println("Received program: "); 
    //Serial.println(Program.c_str());
    const char* pProgram = Program.c_str();

    while (*pProgram != '\0')
    {
     // Serial.print((int) *pProgram); Serial.print(" "); Serial.println(*pProgram);
      pProgram++;
    }

    //Serial.println("Saving"); Serial.flush();
    _pSupervisor->UpdateProgram(Program.c_str());

    _pWebServer->sendHeader("Location", String("/"), true);
   _pWebServer->send ( 302, "text/plain", "");
    Serial.println("Save done - redirecting"); Serial.flush();
  }

  CommandFormatter::PrettyFormat(_pSupervisor->GetCurrentProgram(), _pProgramBuffer, 16636);

  const char* pRunning = _pSupervisor->GetExecutingProgramState() ? "Program executing" : "<b>Program execution pending</b>";

	snprintf ( _pPageBuffer, 16636,

"<html>\
  <head>\
    <title>EagleDecorations Sequence Controller</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>EagleDecorations Sequence Controller</h1>\
    <p>%s</p>\
    <table>\
    <tr>\
    <td style=\"vertical-align: top;\">\
    Code:\
    <FORM action=\"/\" method=\"post\">\
    <textarea rows = \"50\" cols = \"50\" name=\"Program\">\
%s\
    </textarea><br>\
    <INPUT type=\"submit\" value=\"Send\">\
    </FORM>\
    </td>\
    <td style=\"vertical-align: top;\">\
    Errors:\
    <FORM action=\"/\" method=\"post\">\
    <textarea rows = \"50\" cols = \"50\" name=\"Errors\">\
    %s\
    </textarea><br>\
    </FORM>\
    </td>\
    </tr>\
    </table>\
  </body>\
</html>", pRunning, _pProgramBuffer, _pSupervisor->GetCurrentErrors()	);

	_pWebServer->send ( 200, "text/html", _pPageBuffer );
}

  void HandleClient()
  {
    _pWebServer->handleClient();
  }

};

MyWebServer* MyWebServer::pMyWebServerInstance;