#ifndef BWE_ACTION_H
#define BWE_ACTION_H

namespace bwe
{
class Action
{
public:
  virtual ~Action(){}
  virtual void run()=0;
};
}

#endif
