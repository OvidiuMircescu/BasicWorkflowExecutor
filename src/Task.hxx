#ifndef BWE_TASK_H
#define BWE_TASK_H
//#include <memory>
#include <set>

namespace bwe
{
class Task;
class TaskObserver
{
public:
  virtual ~TaskObserver(){}
  virtual void notifyFinished(Task*)=0;
  virtual void notifyError(Task*)=0;
};

class Task
{
public:
  virtual ~Task(){}
  virtual void run()=0;
  void addObserver(TaskObserver* ob); // to notify when finished
  void removeObserver(TaskObserver* ob); // to notify when finished

protected:
  void notifyFinished();
  void notifyError();

private:
  std::set<TaskObserver*> _observers;
};

//typedef std::shared_ptr<Task> TaskPtr;
}
#endif
