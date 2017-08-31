//Smit Shah sss160030@utdallas.edu CS3376.001
#include "Prog5.h"
#include <unistd.h> /* needed for fork() */
#include "tclap/CmdLine.h" //tclap
#include <rude/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/inotify.h>
#include <time.h>
using namespace std;
bool EXIT;
int EXITcode;
map<int,string> options;
ofstream logfile;
int main(int argc, char* argv[])
{//int EXITcode = 0; //EXITcode--;//EXIT = false;
  rude::Config config;//enum e1{LOWER,UPPER,OUTPUT,INPUT};
  enum e1{D,INPUT,V,L,P,N,W};//map<int,string> options;
  try{
    TCLAP::CmdLine cmd("cs3376dirmond Directory Monitor Daemon",' ', "1.0");
    TCLAP::SwitchArg dArg("d","deaemon","Run in daemon mode (forks to run as a daemon).",cmd,false);
    TCLAP::UnlabeledValueArg<string> inputArg("infile","The name of the configuration file. Defaults to cs3376dirmond.conf.",false,"cs3376dirmond.conf","config filename",false);
    cmd.add(inputArg);
    cmd.parse(argc,argv);
    if(dArg.getValue())
      options[D] = "true";
    else
      options[D] = "false";
    options[INPUT] = inputArg.getValue();
  }
  catch(TCLAP::ArgException &e)
  {
      std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
  }
	
	
	char commentchar = '#';
	config.setCommentCharacter(commentchar);
	string Verbose = "Verbose"; string LogFile = "LogFile"; string Password = "Password"; string NumVersions = "NumVersions"; string WatchDir = "WatchDir";
	string file = options[INPUT];
	if(config.load(file.c_str()))
	{	
		config.setSection("Parameters",true);
		cout << "Reading the Config file. Data members in Parameters section: " << config.getNumDataMembers() << endl;
		cout << "The logging will continue into the logging file." << endl;
		if(config.getNumDataMembers() == 0)
		{
			cout << "Error: Missing Parameters section or missing all 5 values in it" << endl; return 1;
		}
		if(config.exists(Verbose.c_str()))
		{
			options[V] = config.getStringValue(Verbose.c_str());
		}
		else
		{
			cout << "Invalid Verbose" << endl; return 1;
		}
		if(config.exists(LogFile.c_str()))
		{
			options[L] = config.getStringValue(LogFile.c_str());
		}
		else
		{
			cout << "Invalid LogFile" << endl;return 1;
		}
		if(config.exists(Password.c_str()))
		{
			options[P] = config.getStringValue(Password.c_str());
		}
		else
		{
			cout << "Invalid Password" << endl;return 1;
		}
		if(config.exists(NumVersions.c_str()))
		{
			options[N] = config.getStringValue(NumVersions.c_str());
		}
		else
		{
			cout << "Invalid NumVersions" << endl;return 1;
		}
		if(config.exists(WatchDir.c_str()))
		{
			options[W] = config.getStringValue(WatchDir.c_str());
		}
		else
		{
			cout << "Invalid WatchDir" << endl;return 1;
		}
	}
	else
	{
		std::cout << "Error opening file: " << config.getError() << "\n";return 1;
	}
	ofstream logfile(options[L].c_str(),std::ofstream::out | ofstream::app);
	logfile << "D flag: " << options[D] << endl;
	logfile << "config file: " << options[INPUT] << endl;
	if(options[V] == "true")
	{
		logfile << "CONFIG FILE SETTINGS AS READ:" << endl;
		logfile << options[V] << endl;
		logfile << options[L] << endl;
		logfile << options[P] << endl;
		logfile << options[N] << endl;
		logfile << options[W] << endl;
	}
	
	
	
		
		string mkdir = "mkdir -p "; string mkdir2 = mkdir + options[W] + "/.versions";
		system(mkdir2.c_str());
		signal(SIGINT,SIGINThandler);
		signal(SIGTERM,SIGINThandler);
		signal(SIGHUP,SIGHUPhandler);
		pid_t forkval;
	if(options[D] == "true")
	{
		forkval = fork();//cout << "forkval: " << forkval << endl;
		if(forkval == 0)
		{//cout << "forkval(child): " << forkval << endl;
			ifstream infile("cs3376dirmond.pid");
			logfile <<"Running as daemon.Testing if a pid file is open or not. Result: " << infile.good() << endl;
			if(infile.good())
			{
				logfile << "Multiple daemons running: pid file already exists" << endl;
				return 1;
			}
			ofstream pidfile;
			pidfile.open("cs3376dirmond.pid",std::ofstream::out);
			pidfile << getpid() << endl;
			pidfile.close();
		}
		else
		{//cout << forkval << " exiting" << endl;
			return 0;
		}
	}
	#define EVENT_SIZE  ( sizeof (struct inotify_event) )
	#define BUF_LEN     ( 1 * ( EVENT_SIZE + 16 ) )
	char buffer[BUF_LEN];
	int length = 0;
	int i = 0; length++; length--;
	int fd;
	fd = inotify_init();
	int wd;
	wd = inotify_add_watch(fd,options[W].c_str(),IN_MODIFY);// | IN_CREATE);
	logfile << "Watch added to " << options[W].c_str() << endl;//ofstream logfile(options[L].c_str(),std::ofstream::out | ofstream::app);
	string lastcomm = "";//cout << "lastcomm before loop:" << lastcomm << endl;
			while(true)
			{//cout << "lastcomm start loop:" << lastcomm << endl;
				logfile << "Waiting the Watchdir to get modified" << endl;
				length = read( fd, buffer, BUF_LEN );//while ( i < length )//{
					if(options[V] == "true")
					logfile << "A file was modified" << endl;
					logfile.close();
					ofstream logfile(options[L].c_str(),std::ofstream::out | ofstream::app);
					struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
					if ( event->len ) 
					{
						if ( event->mask & IN_MODIFY ) 
						{//printf( "The file %s was modified.%s\n", event->name ,currentDateTime().c_str());
							if(options[V] == "true")
								logfile << "The time is: " << currentDateTime().c_str() << endl;
							logfile << "The file " << (event->name) << " was modified and copy command is being sent" << endl;
							string mkdir3 = options[W] + "/.versions" + "/" + (event->name) + "." + currentDateTime();
							string cp = "cp "; string cp2 = cp + options[W] + "/" + (event->name) + " " + mkdir3;//cout << "lastcomm at if: " << lastcomm << endl;//cout << "cp2 at if: " << cp2 << endl;
							if(lastcomm != cp2)
							{
								logfile << "Copy Command being executed:" << cp2.c_str() << endl;
								string cp3 = cp2 + " 2>/dev/null";
								system(cp3.c_str());
								lastcomm = cp2;
							}
							else
							{
								cout << "repeat" << endl;
							}
							sleep(1);
						}
					}
					event->len = 0;
					//i += EVENT_SIZE + event->len;
				//}
				//logfile << "every 15 second" << endl;
				//sleep(3);    //Sleep for 60 seconds
				//logfile << "value of EXIT:" << EXIT << "value of EXITcode:" << EXITcode << endl;
				/*if(EXIT)
				{
					EXIT = false;
					logfile << "exited" << endl;
					logfile.close();
					exit(EXITcode);
					return 0;
				}*///cout << "lastcomm end loop:" << lastcomm << endl;
			}
	inotify_rm_watch( fd, wd );
	return 0;
}
void SIGHUPhandler(int signum)
{
	enum e1{D,INPUT,V,L,P,N,W};rude::Config config;//cout << "options[V]" << options[V] <<endl;
	string Verbose = "Verbose"; string LogFile = "LogFile"; string NumVersions = "NumVersions"; string WatchDir = "WatchDir";
	string file = options[INPUT];
	if(config.load(file.c_str()))
	{	
		string temp = "Parameters";
		config.setSection("Parameters",true);
		if(options[V] == "true")
		{cout << "dirmond: SIGHUP: In Process of updating CONF settings. New settings: " << config.getNumDataMembers() << endl;}
		if(config.getNumDataMembers() == 0)
		{
			cout << "Error: Missing Parameters section or missing all 5 values in it" << endl;
		}
		if(config.exists(Verbose.c_str()))
		{
			options[V] = config.getStringValue(Verbose.c_str());
		}
		else
		{
			cout << "Invalid Verbose" << endl;
		}
		if(config.exists(LogFile.c_str()))
		{
			options[L] = config.getStringValue(LogFile.c_str());//logfile.close();//ofstream logfile(options[L].c_str(),std::ofstream::out | ofstream::app);
		}
		else
		{
			cout << "Invalid LogFile" << endl;
		}
		if(config.exists(NumVersions.c_str()))
		{
			options[N] = config.getStringValue(NumVersions.c_str());
		}
		else
		{
			cout << "Invalid NumVersions" << endl;
		}
	}
	else
	{
		std::cout << "Error opening file: " << config.getError() << "\n";
	}
	if(options[V] == "true")
	{cout << options[V] << endl;
	cout << options[L] << endl;
	cout << options[N] << endl;}
}
void SIGINThandler(int signum)
{
   printf("dirmond: Caught signal %d, closing log file, deleting pid file if necessary, exiting.\n",signum);//ofstream logfile(options[3].c_str());//logfile << "THRU SIGHANDLER" << endl; 
   logfile.close();/*EXIT = true;EXITcode = 0;EXITcode--;if(EXIT)*/
   system("rm -f cs3376dirmond.pid");
   exit(signum);//EXITcode = signum;//logfile << "value of EXIT:" << EXIT << "value of EXITcode" << EXITcode << endl;
}
