#ifndef BWE_DEPENDENCY_H
#define BWE_DEPENDENCY_H
#include <set>
//#include <memory>
#include <condition_variable> // notifications
#include <mutex>

namespace bwe
{
class Executor;
class DependencyObserver
{
public:
  virtual ~DependencyObserver();
  virtual void notifyReady()=0;
};

class Dependency
{
public:
  Dependency(Executor& executor);
  virtual ~Dependency();
  void setReadyAction(); // use the executor => thread safe
  void wait(); // block the current thread until ready
  // not thread safe methods
  void setReady();
  void addObserver(DependencyObserver* obs);

private:
  Executor& _executor;
  bool _ready;
  std::set<DependencyObserver*> _observers;
  std::mutex _mutex;
  std::condition_variable _condition;
};

//typedef std::shared_ptr<Dependency> DependencyPtr;
}
#endif
