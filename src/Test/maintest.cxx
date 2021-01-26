#include "../Executor.hxx"
#include "../Task.hxx"
//#include "../Dependency.hxx"
#include <thread>
#include <future>
#include <cassert>
#include <iostream>

class MyTask: public bwe::Task
{
public:
  MyTask(const std::string& mes):_message(mes){}
  ~MyTask() { _f.wait();}
  void run()override
  {
    _f = std::async(std::launch::async, [this]
        {
          std::cout << _message << std::endl;
          std::this_thread::sleep_for(std::chrono::seconds(5));
          notifyFinished();
        });
  }
private:
  std::string _message;
  std::future<void> _f;
};

int main(int argc, char *argv[])
{
  bwe::Executor exe;
  MyTask mt1("t1"), mt2("t2"), mt3("t3"), mt4("t4");
  exe.start();
  bwe::Dependency* d1 = exe.registerTask(&mt1, std::list<bwe::Dependency*>());
  bwe::Dependency* d2 = exe.registerTask(&mt2, std::list<bwe::Dependency*>{d1});
  bwe::Dependency* d3 = exe.registerTask(&mt3, std::list<bwe::Dependency*>{d1});
  bwe::Dependency* d4 = exe.registerTask(&mt4, std::list<bwe::Dependency*>{d2, d3});
  exe.stop();
  return 0;
}
