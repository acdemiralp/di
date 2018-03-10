#ifndef DI_SYSTEM_HPP_
#define DI_SYSTEM_HPP_

namespace di
{
class engine;

class system
{
public:
  virtual ~system() = default;

  virtual void initialize() { }
  virtual void pre_tick  () { }
  virtual void tick      () { }
  virtual void post_tick () { }
  virtual void terminate () { }

protected:
  friend class engine;

  engine* engine_ = nullptr;
};
}

#endif
