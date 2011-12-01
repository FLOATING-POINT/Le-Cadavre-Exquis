#include "FTPSaver.h"

bool FTPSaver::uploadToFTP(string folder, string fileName)
{

	try
	{
	    // Connection to ftp

	    cout << "FTP : Connecting...";

        FTPClientSession session(FTP_HOST, 21);
        session.login(FTP_LOGIN, FTP_PASS);

        cout << " OK" << endl;

        // Directory management

        cout << "FTP : Setting/Creating " + folder + " directory...";

        session.setWorkingDirectory("common/photos/");
        if (!directoryExist(&session, folder)) session.createDirectory(folder);
        session.setWorkingDirectory(folder);

        cout << " OK" << endl;

        // File upload

        cout << "FTP : Uploading file " + fileName + "...";

        FileInputStream fileStr( ofToDataPath("output_web/" + folder + "/"+ fileName) );
        ostream& ostr = session.beginUpload(fileName);
        StreamCopier::copyStream(fileStr, ostr);
        session.endUpload();

        cout << " DONE" << endl;

        // XML upload

        cout << "FTP : Uploading XML ...";

        string cdUpCommand = "CDUP"; // Manual "cd .."
        string cdUpResponse = "250 CDUP command successful";
        session.sendCommand(cdUpCommand, cdUpResponse);

        FileInputStream xmlStr( ofToDataPath("output/data.xml"));
        ostream& xmlOStream = session.beginUpload("data.xml");
        StreamCopier::copyStream(xmlStr, xmlOStream);
        session.endUpload();

        session.close();

        cout << " DONE" << endl;

        return true;
	}

	catch (Poco::Exception& exc)
	{
		cerr << exc.displayText() << endl;
		return false;
	}

}

bool FTPSaver::directoryExist(FTPClientSession * session, string folder)
{
    // Get the directory listing as a string stream
    istream& istr = session->beginList();
    ostringstream dataStr;
    StreamCopier::copyStream(istr, dataStr);
    session->endList();

    // Split the string into a vector
    vector<string> dirList = ofSplitString(dataStr.str(), "\n");

    // Look if the folder already exist
    for (int i=0; i< dirList.size(); i++)
    {
        // remove last character ('\' or 'n')
        /// TODO : find a better way to do the split
        dirList[i].resize( dirList[i].size()-1 );

        if(dirList[i] == folder) return true;
    }

    return false;
}

bool FTPSaver::updateConfig(bool connected, string tcpServerIP, int tcpServerPort)
{
    try
	{
        FTPClientSession session(FTP_HOST, 21);
        session.login(FTP_LOGIN, FTP_PASS);

        session.setWorkingDirectory("common/xml/");

        ostream& ostr = session.beginUpload("server_settings.xml");
        ostr << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
        ostr << "<data><statut>" << (connected ? "online" : "offline") << "</statut>";
        ostr << "<ip>" + tcpServerIP +"</ip>";
        ostr << "<port>"+ ofToString(tcpServerPort) +"</port></data>";
        session.endUpload();

        session.close();

        cout << "FTP Saver : SERVER SETTINGS UPDATED" << endl;
	}
	catch (Poco::Exception& exc)
	{
		cerr << exc.displayText() << endl;
		return false;
	}
}
