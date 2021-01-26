#ifndef BWE_EXECUTOR_H
#define BWE_EXECUTOR_H
#include "Task.hxx"
#include "Action.hxx"
#include "ActionQueue.hxx"
#include "Dependency.hxx"

#include <list>
#include <set>
#include <queue>

namespace bwe
{
class Task;
class DependencySupervisor;
class TaskSupervisor;
class RegisterTaskAction;

class Executor
{
public:
  ~Executor();

  void start();
  void stop(); // wait for the end of registered tasks
  
  Dependency* registerTask(Task* t, const std::list<Dependency*>& inDeps);
  void registerDependencyReady(Dependency* dep);
  void registerFinishedTask(Dependency* dep);

  void newTask(Task* t, const std::set<Dependency*>& inDeps, Dependency* outDep);
  void launchTask(Task* t);
  void finishTask(Dependency* dep);

private:
  class TaskMonitor : public DependencyObserver, public TaskObserver
  {
  public:
    TaskMonitor(Executor& exec,
                Task* t,
                std::set<Dependency*> inDeps,
                Dependency* outDep);
    virtual ~TaskMonitor();
    void notifyReady()override;
    void notifyFinished(Task* t)override;
    void notifyError(Task* t)override;
    void start();
  private:
    Executor& _executor;
    Task* _task;
    std::set<Dependency*> _inDeps;
    Dependency* _outDep;
    int _unreadyDependencies;
  };
  ActionQueue _queue;
  std::list<TaskMonitor*> _allTasks;
  std::list<Dependency*> _allDeps;
};

class RunTaskAction: public Action
{
public:
  RunTaskAction(Task* t);
  virtual ~RunTaskAction();
  void run()override;
private:
  Task* _task;
};

class DependencyReadyAction: public Action
{
public:
  DependencyReadyAction(Dependency* dependency);
  virtual ~DependencyReadyAction();
  void run()override;
private:
  Dependency* _dependency;
};

class NewTaskAction: public Action
{
public:
  NewTaskAction(Executor& executor,
                Task* t,
                const std::list<Dependency*>& inDeps,
                Dependency* outDep);
  virtual ~NewTaskAction();
  void run()override;
private:
  Executor& _executor;
  Task* _task;
  std::set<Dependency*> _inDeps;
  Dependency* _outDep;
};

class TaskFinishedAction : public Action
{
public:
  TaskFinishedAction(Executor& executor, Dependency* outDep);
  virtual ~TaskFinishedAction();
  void run()override;
private:
  Executor& _executor;
  Dependency* _outDep;
};

}
#endif
