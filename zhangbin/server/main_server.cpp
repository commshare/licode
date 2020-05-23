#include <iostream>

#include "stdafx.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"
#include "tclap/CmdLine.h"
#include "MemorySettingsRepository.h"

#include <exception>
#include <csignal>
#include <stdlib.h>
#include <server/OSSupport/NetworkSingleton.h>


/** If something has told the server to stop; checked periodically in cRoot */
bool cRoot::m_TerminateEventRaised = false;

/** Set to true when the server terminates, so our CTRL handler can then tell the OS to close the console. */
static bool g_ServerTerminated = false;

/** If set to true, binary will attempt to run as a service on Windows */
bool cRoot::m_RunAsService = false;




std::unique_ptr<cMemorySettingsRepository> parseArguments(int argc, char **argv)
{
  try
  {
    // Parse the comand line args:
    TCLAP::CmdLine cmd("MCServer");
    TCLAP::ValueArg<int> slotsArg    ("s", "max-players",         "Maximum number of slots for the server to use, overrides setting in setting.ini", false, -1, "number", cmd);
    TCLAP::MultiArg<int> portsArg    ("p", "port",                "The port number the server should listen to", false, "port", cmd);
    TCLAP::SwitchArg commLogArg      ("",  "log-comm",            "Log server client communications to file", cmd);
    TCLAP::SwitchArg commLogInArg    ("",  "log-comm-in",         "Log inbound server client communications to file", cmd);
    TCLAP::SwitchArg commLogOutArg   ("",  "log-comm-out",        "Log outbound server client communications to file", cmd);
    TCLAP::SwitchArg crashDumpFull   ("",  "crash-dump-full",     "Crashdumps created by the server will contain full server memory", cmd);
    TCLAP::SwitchArg crashDumpGlobals("",  "crash-dump-globals",  "Crashdumps created by the server will contain the global variables' values", cmd);
    TCLAP::SwitchArg noBufArg        ("",  "no-output-buffering", "Disable output buffering", cmd);
    TCLAP::SwitchArg runAsServiceArg ("d", "service",             "Run as a service on Windows, or daemon on UNIX like systems", cmd);
    cmd.parse(argc, argv);

    // Copy the parsed args' values into a settings repository:
    auto repo = cpp14::make_unique<cMemorySettingsRepository>();
    if (slotsArg.isSet())
    {
      int slots = slotsArg.getValue();
      repo->AddValue("Server", "MaxPlayers", static_cast<Int64>(slots));
    }
    if (portsArg.isSet())
    {
      for (auto port: portsArg.getValue())
      {
        repo->AddValue("Server", "Port", static_cast<Int64>(port));
      }
    }

    if (noBufArg.getValue())
    {
      setvbuf(stdout, nullptr, _IONBF, 0);
    }
    repo->SetReadOnly();

    // Set the service flag directly to cRoot:
    if (runAsServiceArg.getValue())
    {
      cRoot::m_RunAsService = true;
    }

    // Apply the CrashDump flags for platforms that support them:
#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)  // 32-bit Windows app compiled in MSVC
    if (crashDumpGlobals.getValue())
			{
				g_DumpFlags = static_cast<MINIDUMP_TYPE>(g_DumpFlags | MiniDumpWithDataSegs);
			}
			if (crashDumpFull.getValue())
			{
				g_DumpFlags = static_cast<MINIDUMP_TYPE>(g_DumpFlags | MiniDumpWithFullMemory);
			}
#endif  // 32-bit Windows app compiled in MSVC

    return repo;
  }
  catch (const TCLAP::ArgException & exc)
  {
    printf("Error reading command line %s for arg %s", exc.error().c_str(), exc.argId().c_str());
    return cpp14::make_unique<cMemorySettingsRepository>();
  }
}
////////////////////////////////////////////////////////////////////////////////
// universalMain - Main startup logic for both standard running and as a service

void universalMain(std::unique_ptr<cSettingsRepositoryInterface> overridesRepo)
{
#ifdef _WIN32
  if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
	{
		LOGERROR("Could not install the Windows CTRL handler!");
	}
#endif

  // Initialize LibEvent:
  cNetworkSingleton::Get();

#if !defined(ANDROID_NDK)
  try
#endif
  { //上面是初始化libevent库，然后第二步就是加载服务器的配置，文件io可能会有异常
    //这里好像加载配置一样的
    cRoot Root;
    Root.Start(std::move(overridesRepo));
  }
#if !defined(ANDROID_NDK)
  catch (std::exception & e)
  {
    LOGERROR("Standard exception: %s", e.what());
  }
  catch (...)
  {
    LOGERROR("Unknown exception!");
  }
#endif

  g_ServerTerminated = true;

  // Shutdown all of LibEvent:
  cNetworkSingleton::Get().Terminate();
}



int main(int argc, char **argv)
{
  auto argsRepo = parseArguments(argc, argv);

  universalMain(std::move(argsRepo));

  return 0;
}