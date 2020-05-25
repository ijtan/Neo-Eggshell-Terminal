#include "external.h"

using namespace std;
vector<proc> StpProcs;

int Executer(vector<string>& argVector, vector<int> conf) {
  int waitOpt = 0;
  vector<pid_t> toWait;
  
  if (conf[4] == 1)
    waitOpt = WNOHANG;


  pid_t mainPID = getpid();
  if (conf[3] == 1) {
    auto feedback = initPipes(argVector, toWait);
    if (feedback[0] == "100")
      if (getpid() != mainPID)
        return -5;

  } else {
    int intern = internalChecker(argVector[0]);
    if (intern == 1) {
      internalHandlerNoCHild(argVector[0], argVector);
      return 0;
    }

    // main should be here if no piping ->  since yet we have no forks and no
    // internal commands
    pid_t pid = fork();
    if (pid == -1) {
      perror("redirection fork");
    } else if (pid == 0) {
      if (conf[0] == 1 || conf[1] == 1 || conf[2] == 1) {
        // making sure unset and cd are not used in redirects! this would cuase
        // the action to be done child side only!

        cout << "redirection initialization call:" << endl;
        int redir = InitialzeRedir(conf, argVector);
        if (redir != 0) {
          return -5;
        }
      }
      if (internalHandler(argVector[0], argVector) == 0) {
        // checks if the internalHandler matched; meaning that an internal
        // command was run and we do not need further execution
        return -5;
      }

      char* args[argVector.size()];
      int i = 0;
      for (auto arg : argVector) {
        args[i] = strdup(arg.c_str());
        i++;
      }
      args[i] = NULL;

      int code = execvp(args[0], args);
      if (code == -1) {
        perror("Execution");
        cout << "np its was: " << args[0] << endl;
        for (int j = 0; j < i; j++)
          free(args[j]);
        return -5;
      }
    } else {
      int status;
      waitpid(pid, &status, 0);
    }
  }
  return 0;
}

int statusChecker(int status, pid_t pid, string name) {
  if (WIFSIGNALED(status)) {
    cout << "Signal Code " << WTERMSIG(status) << ":"
         << sys_siglist[WTERMSIG(status)] << endl;
    if (WTERMSIG(status) == SIGTSTP)
      StpProcs.push_back(proc{pid, move(name)});

    return -1;
  }
  if (WIFEXITED(status)) {
    string str = to_string(WEXITSTATUS(status));
    better_set("EXITCODE", str);
    return 0;
  }
  return 0;
}

vector<proc2> getStpProcs() {
  vector<proc2> procs2;
  for (auto prc : StpProcs) {
    proc2 tmp{prc.pid, prc.name};
    procs2.push_back(tmp);
  }
  return procs2;
}
