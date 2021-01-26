#include "ActionQueue.hxx"
namespace bwe
{
void ActionQueue::addAction(Action* a)
{
  std::unique_lock<std::mutex> lock(_actions_mutex);
  _actions.push(a);
  _startCondition.notify_one();
}
  
void ActionQueue::start()
{
  {
    std::unique_lock<std::mutex> lock(_actions_mutex);
    _stop = false;
  }
  _actionsThread = std::async([this] {runActions();});
}

void ActionQueue::stop()
{
  {
    std::unique_lock<std::mutex> lock(_actions_mutex);
    _stop = true;
    _startCondition.notify_one();
  }
  _actionsThread.wait();
}

void ActionQueue::runActions()
{
  Action* current_action = nullptr;
  bool threadStop = false;
  while(!threadStop)
  {
    {
      std::unique_lock<std::mutex> lock(_actions_mutex);
      _startCondition.wait(lock, [this] {return !_actions.empty() || _stop;});
      if(_actions.empty())
        current_action = nullptr;
      else
      {
        current_action = _actions.front();
        _actions.pop();
      }
    }
    if(current_action != nullptr)
    {
      current_action->run();
      delete(current_action);
    }
    {
      std::unique_lock<std::mutex> lock(_actions_mutex);
      threadStop = _stop && _actions.empty(); // TODO: verify the number of running tasks.
    }
  }
}
} //namespace bwe
