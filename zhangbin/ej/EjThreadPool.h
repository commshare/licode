//
// Created by zhangbin on 2019/11/24.
//

#ifndef LICODE_EJTHREADPOOL_H
#define LICODE_EJTHREADPOOL_H

#include <thread/ThreadPool.h>
#include "EjType.h"

namespace EJ
{
class EjThreadPool {
public:
   //这个是啥
  //    static Nan::Persistent<v8::Function> constructor;
  void New(int num_workers)
  {
    this->num_workers_ = num_workers;
    this->me_.reset(new erizo::ThreadPool(num_workers));
  }
  /*
    * Starts all workers in the ThreadPool
    */
  bool Start()
  {
    this->me_->start();
  }
  /*
 * Closes the ThreadPool.
 * The object cannot be used after this call
 */
  bool Close()
  {
    this->me_->close();
  }

  void GetDurationDistribution()
  {
    this->me_->getDurationDistribution();
  }
  void ResetStats()
  {
    this->me_->resetStats();
  }
private:
  unsigned int num_workers_;
  std::unique_ptr<erizo::ThreadPool> me_;
};
typedef utils::Singleton<EjThreadPool> EjThreadPoolSingleton;
}



#endif //LICODE_EJTHREADPOOL_H
