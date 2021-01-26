#ifndef BWE_ACTION_QUEUE_H
#define BWE_ACTION_QUEUE_H
#include <list>
#include "Action.hxx"
#include <queue>
#include <future>
#include <condition_variable> // notifications
#include <mutex>

namespace bwe
{
class ActionQueue
{
public:
  void start(); // start the evaluation thread.
  void addAction(Action* a);
  void stop(); // empty the queue and close the thread.
private:
  std::queue<Action*> _actions;
  std::future<void> _actionsThread;
  std::mutex _actions_mutex;
  std::condition_variable _startCondition; // start tasks thread notification
  bool _stop = true;
  void runActions();
};
}
#endif
