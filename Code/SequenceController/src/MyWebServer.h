typedef void (*ProgramUpdated)(const char* pProgram);
typedef const char* (*CurrentProgramFetcher)();
typedef bool (*CurrentExecutionStateFetcher)();

class MyWebServer
{
    static MyWebServer* pMyWebServerInstance;

    WebServer* _pWebServer;

    char* _pProgramBuffer;
    char* _pPageBuffer;

    ProgramUpdated _onProgramUpdated;
    CurrentProgramFetcher _currentProgramFetcher;
    CurrentExecutionStateFetcher _currentExecutionStateFetcher;

    static void handleRoot()
    {
      pMyWebServerInstance->handleRootInstance();
    }

    public:

      MyWebServer(ProgramUpdated onProgramUpdated, CurrentProgramFetcher currentProgramFetcher, CurrentExecutionStateFetcher currentExecutionStateFetcher)
      {
        _pWebServer = new WebServer(80);

        _pProgramBuffer = new char[16636];
        _pPageBuffer = new char[16636];

        _onProgramUpdated = onProgramUpdated;
        _currentProgramFetcher = currentProgramFetcher;
        _currentExecutionStateFetcher = currentExecutionStateFetcher;

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
    _onProgramUpdated(Program.c_str());

    _pWebServer->sendHeader("Location", String("/"), true);
   _pWebServer->send ( 302, "text/plain", "");
    Serial.println("Save done - redirecting"); Serial.flush();
  }

  CommandFormatter::PrettyFormat(_currentProgramFetcher(), _pProgramBuffer, 16636);

  const char* pRunning = _currentExecutionStateFetcher() ? "Program executing" : "<b>Program execution pending</b>";

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
    <FORM action=\"/\" method=\"post\">\
    <textarea rows = \"15\" cols = \"80\" name=\"Program\">%s</textarea><br>\
    <INPUT type=\"submit\" value=\"Send\">\
    </FORM>\
  </body>\
</html>", pRunning, _pProgramBuffer	);

	_pWebServer->send ( 200, "text/html", _pPageBuffer );
}

  void HandleClient()
  {
    _pWebServer->handleClient();
  }

};

MyWebServer* MyWebServer::pMyWebServerInstance;