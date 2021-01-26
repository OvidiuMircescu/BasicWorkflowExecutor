#include "Executor.hxx"
#include "Task.hxx"
#include <set>

namespace bwe
{
Executor::~Executor()
{
  for(Executor::TaskMonitor* tm : _allTasks)
    delete tm;
  for(Dependency* dep : _allDeps)
    delete dep;
}

void Executor::start()
{
  _queue.start();
}

void Executor::stop()
{
  for(Dependency* dep : _allDeps)
    dep->wait();
  _queue.stop();
}

void Executor::launchTask(Task* t)
{
  Action* a = new RunTaskAction(t);
  _queue.addAction(a);
}

Dependency* Executor::registerTask(Task* t,
                                   const std::list<Dependency*>& inDeps)
{
  Dependency* outDep(new Dependency(*this));
  _allDeps.push_back(outDep);
  Action* a = new NewTaskAction(*this, t, inDeps, outDep);
  _queue.addAction(a);
  return outDep;
}

void Executor::newTask(Task* t,
                       const std::set<Dependency*>& inDeps,
                       Dependency* outDep)
{
  TaskMonitor* tm = new TaskMonitor(*this, t, inDeps, outDep);
  _allTasks.push_back(tm);
  tm->start();
}

void Executor::finishTask(Dependency* dep)
{
  dep->setReady();
}

void Executor::registerDependencyReady(Dependency* dep)
{
  Action* a = new DependencyReadyAction(dep);
  _queue.addAction(a);
}

void Executor::registerFinishedTask(Dependency* dep)
{
  Action* a = new TaskFinishedAction(*this, dep);
  _queue.addAction(a);
}

// class Executor::TaskMonitor
Executor::TaskMonitor::TaskMonitor(Executor& exec,
                                    Task* t,
                                    std::set<Dependency*> inDeps,
                                    Dependency* outDep)
: DependencyObserver()
, TaskObserver()
, _executor(exec)
, _task(t)
, _inDeps(inDeps)
, _outDep(outDep)
, _unreadyDependencies(_inDeps.size())
{
}

Executor::TaskMonitor::~TaskMonitor()
{
}

void Executor::TaskMonitor::start()
{
  _task->addObserver(this);
  if(_unreadyDependencies > 0)
    for(Dependency* dep : _inDeps)
      dep->addObserver(this);
  else
    notifyReady();
}

void Executor::TaskMonitor::notifyReady()
{
  --_unreadyDependencies;
  if(_unreadyDependencies <= 0)
    _executor.launchTask(_task);
}

void Executor::TaskMonitor::notifyFinished(Task* t)
{
  _executor.registerFinishedTask(_outDep);
}

void Executor::TaskMonitor::notifyError(Task* t)
{
  // TODO
}


// class RunTaskAction
RunTaskAction::RunTaskAction(Task* t)
: Action()
, _task(t)
{
}

RunTaskAction::~RunTaskAction()
{
}

void RunTaskAction::run()
{
  _task->run();
}

// class DependencyReadyAction
DependencyReadyAction::DependencyReadyAction(Dependency* dependency)
: Action()
, _dependency(dependency)
{
}

DependencyReadyAction::~DependencyReadyAction()
{
}

void DependencyReadyAction::run()
{
  _dependency->setReady();
}

// class NewTaskAction

NewTaskAction::NewTaskAction(Executor& executor,
                              Task* t,
                              const std::list<Dependency*>& inDeps,
                              Dependency* outDep)
: Action()
, _executor(executor)
, _task(t)
, _inDeps()
, _outDep(outDep)
{
  for(Dependency* dep : inDeps)
    _inDeps.insert(dep);
}

NewTaskAction::~NewTaskAction()
{
}

void NewTaskAction::run()
{
  _executor.newTask(_task, _inDeps, _outDep);
}

// class TaskFinishedAction
TaskFinishedAction::TaskFinishedAction(Executor& executor, Dependency* outDep)
: Action()
, _executor(executor)
, _outDep(outDep)
{
}

TaskFinishedAction::~TaskFinishedAction()
{
}

void TaskFinishedAction::run()
{
  _executor.finishTask(_outDep);
}

}
