#include "Task.hxx"

namespace bwe
{
void Task::addObserver(TaskObserver* ob)
{
  _observers.insert(ob);
}

void Task::removeObserver(TaskObserver* ob)
{
  _observers.erase(ob);
}

void Task::notifyFinished()
{
  for(auto ob :_observers)
    ob->notifyFinished(this);
}

void Task::notifyError()
{
  for(auto ob :_observers)
    ob->notifyError(this);
}

}
