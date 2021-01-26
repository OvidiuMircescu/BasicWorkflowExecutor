#include "Dependency.hxx"
#include "Executor.hxx"
namespace bwe
{
DependencyObserver::~DependencyObserver(){}

Dependency::Dependency(Executor& executor)
: _executor(executor)
, _ready(false)
, _observers()
, _mutex()
, _condition()
{
}

Dependency::~Dependency()
{
}

void Dependency::setReadyAction()
{
  _executor.registerDependencyReady(this);
}

void Dependency::setReady()
{
  std::unique_lock<std::mutex> lock(_mutex);
  if(!_ready)
  {
    _ready = true;
    for(DependencyObserver* obs: _observers)
      obs->notifyReady();
    _observers.clear();
  }
  _condition.notify_one();
}

void Dependency::addObserver(DependencyObserver* obs)
{
  if(_ready)
    obs->notifyReady();
  else
    _observers.insert(obs);
}

void Dependency::wait()
{
  std::unique_lock<std::mutex> lock(_mutex);
  _condition.wait(lock, [this] {return _ready;});
}
}
